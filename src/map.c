#include "map.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/net/http.h>
#include <psp2/sysmodule.h>

/*
 * Tuiles raster 256x256 "Voyager" de CARTO (données OpenStreetMap), servies en
 * HTTP simple : pas de TLS, dont la pile et les certificats de la Vita sont datés.
 * L'attribution est affichée dans le panneau.
 */
#define TILE_SIZE 256
#define TILE_URL "http://%c.basemaps.cartocdn.com/rastertiles/voyager/%d/%d/%d.png"
#define TILE_MAX_BYTES (256 * 1024)
#define CACHE_SLOTS 24
#define RETRY_AFTER_FRAMES 300  /* ~5 s avant de retenter une tuile en échec */

enum {
    SLOT_EMPTY,
    SLOT_QUEUED,       /* à télécharger (thread réseau) */
    SLOT_DOWNLOADING,
    SLOT_DATA,         /* PNG reçu, à décoder (thread d'affichage) */
    SLOT_TEXTURE,
    SLOT_FAILED
};

typedef struct {
    int state;
    int z, x, y;
    unsigned char *data;
    unsigned size;
    vita2d_texture *tex;
    unsigned last_used;
    unsigned failed_at;
} TileSlot;

static TileSlot slots[CACHE_SLOTS];
static SceKernelLwMutexWork slots_lock;
static SceUID fetch_thid = -1;
static volatile int fetch_stop;
static int http_tmpl = -1;
static unsigned draw_counter;
static int ready;

/* Textures évincées : le GPU peut encore les lire pour les images en vol,
 * on ne les libère qu'après quelques images. */
#define RETIRED_MAX CACHE_SLOTS
#define RETIRE_DELAY_FRAMES 4
static struct { vita2d_texture *tex; unsigned frame; } retired[RETIRED_MAX];

static void retire_texture(vita2d_texture *tex) {
    for (int i = 0; i < RETIRED_MAX; ++i) {
        if (!retired[i].tex) {
            retired[i].tex = tex;
            retired[i].frame = draw_counter;
            return;
        }
    }
    /* File pleine (improbable) : attendre le GPU avant de libérer. */
    vita2d_wait_rendering_done();
    vita2d_free_texture(tex);
}

static void free_retired_textures(int force) {
    for (int i = 0; i < RETIRED_MAX; ++i) {
        if (retired[i].tex && (force || draw_counter - retired[i].frame >= RETIRE_DELAY_FRAMES)) {
            vita2d_free_texture(retired[i].tex);
            retired[i].tex = NULL;
        }
    }
}

static void log_map(const char *msg, int code) {
    FILE *f = fopen("ux0:/data/gps_debug.txt", "a");
    if (f) {
        fprintf(f, "[MAP] %s : 0x%08X\n", msg, (unsigned)code);
        fclose(f);
    }
}

/* Télécharge url dans un tampon malloc ; renvoie 0 ou un code d'erreur. */
static int http_get(const char *url, unsigned char **out, unsigned *out_size) {
    int ret;
    int conn = sceHttpCreateConnectionWithURL(http_tmpl, url, 0);
    if (conn < 0) return conn;
    int req = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_GET, url, 0);
    if (req < 0) {
        sceHttpDeleteConnection(conn);
        return req;
    }

    unsigned char *buf = NULL;
    unsigned size = 0;
    int status = 0;
    ret = sceHttpSendRequest(req, NULL, 0);
    if (ret >= 0) ret = sceHttpGetStatusCode(req, &status);
    if (ret >= 0 && status != 200) ret = -status;
    if (ret >= 0) {
        buf = malloc(TILE_MAX_BYTES);
        if (!buf) ret = -1;
    }
    while (ret >= 0) {
        if (size == TILE_MAX_BYTES) {
            ret = -2;
            break;
        }
        int n = sceHttpReadData(req, buf + size, TILE_MAX_BYTES - size);
        if (n < 0) ret = n;
        if (n <= 0) break;
        size += n;
    }

    sceHttpDeleteRequest(req);
    sceHttpDeleteConnection(conn);
    if (ret < 0 || size == 0) {
        free(buf);
        return ret < 0 ? ret : -3;
    }
    *out = buf;
    *out_size = size;
    return 0;
}

static int fetch_thread(SceSize args, void *argp) {
    (void)args; (void)argp;
    char url[160];
    unsigned request_count = 0;
    while (!fetch_stop) {
        TileSlot *slot = NULL;
        int z = 0, x = 0, y = 0;

        sceKernelLockLwMutex(&slots_lock, 1, NULL);
        /* La tuile la plus récemment demandée d'abord : c'est celle qu'on regarde. */
        for (int i = 0; i < CACHE_SLOTS; ++i) {
            if (slots[i].state == SLOT_QUEUED && (!slot || slots[i].last_used > slot->last_used))
                slot = &slots[i];
        }
        if (slot) {
            slot->state = SLOT_DOWNLOADING;
            z = slot->z; x = slot->x; y = slot->y;
        }
        sceKernelUnlockLwMutex(&slots_lock, 1);

        if (!slot) {
            sceKernelDelayThread(50 * 1000);
            continue;
        }

        snprintf(url, sizeof(url), TILE_URL, "abcd"[request_count++ % 4], z, x, y);
        unsigned char *data = NULL;
        unsigned size = 0;
        int ret = http_get(url, &data, &size);
        if (ret < 0) log_map("tile download", ret);

        sceKernelLockLwMutex(&slots_lock, 1, NULL);
        if (ret == 0) {
            slot->data = data;
            slot->size = size;
            slot->state = SLOT_DATA;
        } else {
            slot->state = SLOT_FAILED;
            slot->failed_at = draw_counter;
        }
        sceKernelUnlockLwMutex(&slots_lock, 1);
    }
    return 0;
}

int map_init(void) {
    int ret = sceSysmoduleLoadModule(SCE_SYSMODULE_HTTP);
    log_map("sceSysmoduleLoadModule(HTTP)", ret);
    if (ret < 0) return ret;
    ret = sceHttpInit(512 * 1024);
    log_map("sceHttpInit", ret);
    if (ret < 0) return ret;

    http_tmpl = sceHttpCreateTemplate("GoExploreVita/2.0 (PS Vita homebrew)", SCE_HTTP_VERSION_1_1, 1);
    log_map("sceHttpCreateTemplate", http_tmpl);
    if (http_tmpl < 0) return http_tmpl;
    sceHttpSetConnectTimeOut(http_tmpl, 10 * 1000 * 1000);
    sceHttpSetRecvTimeOut(http_tmpl, 10 * 1000 * 1000);
    sceHttpSetSendTimeOut(http_tmpl, 10 * 1000 * 1000);

    memset(slots, 0, sizeof(slots));
    ret = sceKernelCreateLwMutex(&slots_lock, "MapTileLock", 0, 0, NULL);
    if (ret < 0) return ret;
    fetch_stop = 0;
    fetch_thid = sceKernelCreateThread("MapTileFetch", fetch_thread, 0x10000100, 0x4000, 0, 0, NULL);
    if (fetch_thid < 0) return fetch_thid;
    ret = sceKernelStartThread(fetch_thid, 0, NULL);
    log_map("start tile thread", ret);
    if (ret < 0) return ret;
    ready = 1;
    return 0;
}

/* Trouve la tuile (z,x,y) dans le cache ou la met en file. Verrou tenu par l'appelant. */
static TileSlot *request_tile(int z, int x, int y) {
    TileSlot *victim = NULL;
    for (int i = 0; i < CACHE_SLOTS; ++i) {
        TileSlot *s = &slots[i];
        if (s->state != SLOT_EMPTY && s->z == z && s->x == x && s->y == y) {
            s->last_used = draw_counter;
            if (s->state == SLOT_FAILED && draw_counter - s->failed_at >= RETRY_AFTER_FRAMES)
                s->state = SLOT_QUEUED;
            return s;
        }
        /* On ne recycle jamais une tuile en cours de téléchargement ni une tuile de cette image. */
        if (s->state == SLOT_DOWNLOADING || (s->state != SLOT_EMPTY && s->last_used == draw_counter))
            continue;
        if (!victim || s->state == SLOT_EMPTY ||
            (victim->state != SLOT_EMPTY && s->last_used < victim->last_used))
            victim = s;
    }
    if (!victim) return NULL;

    if (victim->tex) {
        retire_texture(victim->tex);
        victim->tex = NULL;
    }
    free(victim->data);
    victim->data = NULL;
    victim->z = z; victim->x = x; victim->y = y;
    victim->state = SLOT_QUEUED;
    victim->last_used = draw_counter;
    return victim;
}

static void draw_panel_frame(vita2d_pgf *font, int x, int y, int w, int h) {
    unsigned border = RGBA8(90, 90, 110, 255);
    vita2d_draw_rectangle(x - 2, y - 2, w + 4, 2, border);
    vita2d_draw_rectangle(x - 2, y + h, w + 4, 2, border);
    vita2d_draw_rectangle(x - 2, y, 2, h, border);
    vita2d_draw_rectangle(x + w, y, 2, h, border);
    vita2d_draw_rectangle(x, y + h - 18, w, 18, RGBA8(255, 255, 255, 180));
    vita2d_pgf_draw_text(font, x + 4, y + h - 4, RGBA8(60, 60, 60, 255), 0.6f,
                         "(c) OpenStreetMap contributors (c) CARTO");
}

void map_draw_placeholder(vita2d_pgf *font, int x, int y, int w, int h, const char *text) {
    vita2d_draw_rectangle(x, y, w, h, RGBA8(35, 35, 50, 255));
    vita2d_pgf_draw_text(font, x + 16, y + h / 2, RGBA8(160, 160, 180, 255), 1.0f, text);
    draw_panel_frame(font, x, y, w, h);
}

void map_draw(vita2d_pgf *font, int x, int y, int w, int h,
              double lat, double lon, float accuracy, int zoom) {
    if (!ready) {
        map_draw_placeholder(font, x, y, w, h, "Carte indisponible (reseau)");
        return;
    }
    draw_counter++;
    free_retired_textures(0);

    /* Projection Web Mercator en pixels "monde" au niveau de zoom demandé. */
    double world = (double)TILE_SIZE * (double)(1 << zoom);
    double lat_rad = lat * M_PI / 180.0;
    double px = (lon + 180.0) / 360.0 * world;
    double py = (1.0 - asinh(tan(lat_rad)) / M_PI) / 2.0 * world;
    double left = px - w / 2.0;
    double top = py - h / 2.0;
    int max_tile = (1 << zoom) - 1;

    vita2d_draw_rectangle(x, y, w, h, RGBA8(225, 222, 215, 255));
    vita2d_enable_clipping();
    vita2d_set_clip_rectangle(x, y, x + w, y + h);

    int tx0 = (int)floor(left / TILE_SIZE), tx1 = (int)floor((left + w - 1) / TILE_SIZE);
    int ty0 = (int)floor(top / TILE_SIZE), ty1 = (int)floor((top + h - 1) / TILE_SIZE);
    int decoded = 0;

    sceKernelLockLwMutex(&slots_lock, 1, NULL);
    for (int ty = ty0; ty <= ty1; ++ty) {
        for (int tx = tx0; tx <= tx1; ++tx) {
            if (ty < 0 || ty > max_tile) continue;
            int wrapped_x = ((tx % (max_tile + 1)) + (max_tile + 1)) % (max_tile + 1);
            TileSlot *s = request_tile(zoom, wrapped_x, ty);
            if (!s) continue;
            /* Décodage PNG sur le thread d'affichage, une tuile par image pour rester fluide. */
            if (s->state == SLOT_DATA && !decoded) {
                s->tex = vita2d_load_PNG_buffer(s->data);
                free(s->data);
                s->data = NULL;
                s->state = s->tex ? SLOT_TEXTURE : SLOT_FAILED;
                if (!s->tex) log_map("tile decode", -1);
                decoded = 1;
            }
            if (s->state == SLOT_TEXTURE) {
                float sx = (float)(x + tx * TILE_SIZE - left);
                float sy = (float)(y + ty * TILE_SIZE - top);
                vita2d_draw_texture(s->tex, sx, sy);
            }
        }
    }
    sceKernelUnlockLwMutex(&slots_lock, 1);

    /* Position : cercle de précision puis point bleu au centre du panneau. */
    float cx = x + w / 2.0f, cy = y + h / 2.0f;
    if (accuracy > 0) {
        double meters_per_px = 156543.03392 * cos(lat_rad) / (double)(1 << zoom);
        float r = (float)(accuracy / meters_per_px);
        if (r > 6 && r < w)
            vita2d_draw_fill_circle(cx, cy, r, RGBA8(40, 120, 255, 60));
    }
    vita2d_draw_fill_circle(cx, cy, 9, RGBA8(255, 255, 255, 255));
    vita2d_draw_fill_circle(cx, cy, 6, RGBA8(30, 110, 255, 255));

    vita2d_disable_clipping();

    char zoom_buf[32];
    snprintf(zoom_buf, sizeof(zoom_buf), "Zoom %d  (L/R)", zoom);
    vita2d_draw_rectangle(x, y, 118, 20, RGBA8(255, 255, 255, 180));
    vita2d_pgf_draw_text(font, x + 4, y + 15, RGBA8(40, 40, 40, 255), 0.7f, zoom_buf);
    draw_panel_frame(font, x, y, w, h);
}

void map_fini(void) {
    if (fetch_thid >= 0) {
        fetch_stop = 1;
        SceUInt timeout = 15 * 1000 * 1000;
        if (sceKernelWaitThreadEnd(fetch_thid, NULL, &timeout) >= 0)
            sceKernelDeleteThread(fetch_thid);
        fetch_thid = -1;
    }
    if (ready) {
        vita2d_wait_rendering_done();
        free_retired_textures(1);
        for (int i = 0; i < CACHE_SLOTS; ++i) {
            if (slots[i].tex) vita2d_free_texture(slots[i].tex);
            free(slots[i].data);
        }
        sceKernelDeleteLwMutex(&slots_lock);
        ready = 0;
    }
    if (http_tmpl >= 0) sceHttpDeleteTemplate(http_tmpl);
    http_tmpl = -1;
    sceHttpTerm();
    sceSysmoduleUnloadModule(SCE_SYSMODULE_HTTP);
}
