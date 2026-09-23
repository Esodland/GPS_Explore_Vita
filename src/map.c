#include "map.h"
#include "gfx.h"
#include "text.h"

#include <math.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/net/http.h>
#include <psp2/sysmodule.h>
#include <vitaGL.h>

/*
 * Tuiles raster 256x256 "Voyager" de CARTO (données OpenStreetMap), servies en
 * HTTP simple : pas de TLS, dont la pile et les certificats de la Vita sont datés.
 * L'attribution est affichée dans le panneau.
 */
#define TILE_SIZE 256
#define TILE_URL "http://%c.basemaps.cartocdn.com/rastertiles/voyager/%d/%d/%d.png"
#define TILE_MAX_BYTES (256 * 1024)
#define CACHE_SLOTS 64
#define RETRY_AFTER_FRAMES 300    /* ~5 s avant de retenter une tuile en échec */
#define UPLOADS_PER_FRAME 2       /* envois de textures au GPU par image, pour rester fluide */
#define FOV_Y 45.0f

#define GROUND_COLOR RGBA8(236, 233, 226, 255)
#define SKY_TOP RGBA8(120, 170, 225, 255)
#define SKY_HORIZON RGBA8(214, 228, 240, 255)

enum {
    SLOT_EMPTY,
    SLOT_QUEUED,       /* à télécharger (thread réseau) */
    SLOT_DOWNLOADING,
    SLOT_PIXELS,       /* PNG téléchargé et décodé en RGBA, à envoyer au GPU */
    SLOT_TEXTURE,
    SLOT_FAILED
};

typedef struct {
    int state;
    int z, x, y;
    unsigned char *pixels;   /* RGBA 256x256 */
    GLuint tex;
    unsigned last_used;
    unsigned failed_at;
    int priority;            /* distance au centre de la vue, plus petit = plus urgent */
} TileSlot;

static TileSlot slots[CACHE_SLOTS];
static SceKernelLwMutexWork slots_lock;
static SceUID fetch_thid = -1;
static volatile int fetch_stop;
static int http_tmpl = -1;
static unsigned draw_counter;
static int ready;

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

/* PNG -> RGBA 256x256 (API simplifiée de libpng). */
static unsigned char *decode_png(const unsigned char *data, unsigned size) {
    png_image img;
    memset(&img, 0, sizeof(img));
    img.version = PNG_IMAGE_VERSION;
    if (!png_image_begin_read_from_memory(&img, data, size)) return NULL;
    img.format = PNG_FORMAT_RGBA;
    if (img.width != TILE_SIZE || img.height != TILE_SIZE) {
        png_image_free(&img);
        return NULL;
    }
    unsigned char *pixels = malloc(PNG_IMAGE_SIZE(img));
    if (pixels && !png_image_finish_read(&img, NULL, pixels, 0, NULL)) {
        free(pixels);
        pixels = NULL;
    }
    png_image_free(&img);
    return pixels;
}

static int fetch_thread(SceSize args, void *argp) {
    (void)args; (void)argp;
    char url[160];
    unsigned request_count = 0;
    while (!fetch_stop) {
        TileSlot *slot = NULL;
        int z = 0, x = 0, y = 0;

        sceKernelLockLwMutex(&slots_lock, 1, NULL);
        /* Tuiles de l'image courante d'abord, puis les plus proches du centre. */
        for (int i = 0; i < CACHE_SLOTS; ++i) {
            TileSlot *s = &slots[i];
            if (s->state != SLOT_QUEUED) continue;
            if (!slot || s->last_used > slot->last_used ||
                (s->last_used == slot->last_used && s->priority < slot->priority))
                slot = s;
        }
        if (slot) {
            slot->state = SLOT_DOWNLOADING;
            z = slot->z; x = slot->x; y = slot->y;
        }
        sceKernelUnlockLwMutex(&slots_lock, 1);

        if (!slot) {
            sceKernelDelayThread(30 * 1000);
            continue;
        }

        snprintf(url, sizeof(url), TILE_URL, "abcd"[request_count++ % 4], z, x, y);
        unsigned char *data = NULL;
        unsigned size = 0;
        unsigned char *pixels = NULL;
        int ret = http_get(url, &data, &size);
        if (ret == 0) {
            pixels = decode_png(data, size);
            free(data);
            if (!pixels) ret = -4;
        }
        if (ret < 0) log_map("tile", ret);

        sceKernelLockLwMutex(&slots_lock, 1, NULL);
        if (ret == 0) {
            slot->pixels = pixels;
            slot->state = SLOT_PIXELS;
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
    fetch_thid = sceKernelCreateThread("MapTileFetch", fetch_thread, 0x10000100, 0x8000, 0, 0, NULL);
    if (fetch_thid < 0) return fetch_thid;
    ret = sceKernelStartThread(fetch_thid, 0, NULL);
    log_map("start tile thread", ret);
    if (ret < 0) return ret;
    ready = 1;
    return 0;
}

/* Trouve la tuile (z,x,y) dans le cache ou la met en file. Verrou tenu par l'appelant. */
static TileSlot *request_tile(int z, int x, int y, int priority) {
    TileSlot *victim = NULL;
    for (int i = 0; i < CACHE_SLOTS; ++i) {
        TileSlot *s = &slots[i];
        if (s->state != SLOT_EMPTY && s->z == z && s->x == x && s->y == y) {
            s->last_used = draw_counter;
            s->priority = priority;
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

    /* vitaGL diffère lui-même la libération tant que le GPU utilise la texture. */
    if (victim->tex) glDeleteTextures(1, &victim->tex);
    victim->tex = 0;
    free(victim->pixels);
    victim->pixels = NULL;
    victim->z = z; victim->x = x; victim->y = y;
    victim->state = SLOT_QUEUED;
    victim->last_used = draw_counter;
    victim->priority = priority;
    return victim;
}

static void upload_tile(TileSlot *s) {
    glGenTextures(1, &s->tex);
    glBindTexture(GL_TEXTURE_2D, s->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TILE_SIZE, TILE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, s->pixels);
    /* Mipmaps : évite le scintillement des tuiles lointaines en vue inclinée. */
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    free(s->pixels);
    s->pixels = NULL;
    s->state = SLOT_TEXTURE;
}

static void draw_panel_frame(int x, int y, int w, int h) {
    unsigned border = RGBA8(90, 90, 110, 255);
    gfx_rect(x - 2, y - 2, w + 4, 2, border);
    gfx_rect(x - 2, y + h, w + 4, 2, border);
    gfx_rect(x - 2, y, 2, h, border);
    gfx_rect(x + w, y, 2, h, border);
    gfx_rect(x, y + h - 18, w, 18, RGBA8(255, 255, 255, 180));
    text_draw(x + 4, y + h - 4, RGBA8(60, 60, 60, 255), 0.6f, "(c) OpenStreetMap contributors (c) CARTO");
}

void map_draw_placeholder(int x, int y, int w, int h, const char *text) {
    gfx_begin_2d();
    gfx_rect(x, y, w, h, RGBA8(35, 35, 50, 255));
    text_draw(x + 16, y + h / 2, RGBA8(160, 160, 180, 255), 1.0f, text);
    draw_panel_frame(x, y, w, h);
}

/* Disque plat sur le sol (repère local de la carte). */
static void ground_disc(float cx, float cy, float z, float r, unsigned color) {
    gfx_set_color(color);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(cx, cy, z);
    for (int i = 0; i <= 40; ++i) {
        float a = (float)i / 40 * 2.0f * (float)M_PI;
        glVertex3f(cx + cosf(a) * r, cy + sinf(a) * r, z);
    }
    glEnd();
}

/* Flèche de navigation posée au sol, pointant vers le cap (degrés, 0 = nord). */
static void ground_arrow(float heading, float size, float z, unsigned color) {
    float a = heading * (float)M_PI / 180.0f;
    float fx = sinf(a), fy = cosf(a);      /* avant */
    float rx = cosf(a), ry = -sinf(a);     /* droite */
    float tip_x = fx * size, tip_y = fy * size;
    float back = -0.6f * size, notch = -0.25f * size, half = 0.7f * size;
    gfx_set_color(color);
    glBegin(GL_TRIANGLES);
    glVertex3f(tip_x, tip_y, z);
    glVertex3f(fx * back + rx * half, fy * back + ry * half, z);
    glVertex3f(fx * notch, fy * notch, z);
    glVertex3f(tip_x, tip_y, z);
    glVertex3f(fx * notch, fy * notch, z);
    glVertex3f(fx * back - rx * half, fy * back - ry * half, z);
    glEnd();
}

void map_draw(const MapView *v) {
    if (!ready) {
        map_draw_placeholder(v->x, v->y, v->w, v->h, "Carte indisponible (reseau)");
        return;
    }
    draw_counter++;

    int zoom = v->zoom;
    float pitch = v->pitch < MAP_PITCH_MIN ? MAP_PITCH_MIN : (v->pitch > MAP_PITCH_2D ? MAP_PITCH_2D : v->pitch);
    int tilted = pitch < 80.0f;

    /* Projection Web Mercator en pixels "monde" au niveau de zoom demandé. */
    double world = (double)TILE_SIZE * (double)(1 << zoom);
    double lat_rad = v->lat * M_PI / 180.0;
    double px = (v->lon + 180.0) / 360.0 * world;
    double py = (1.0 - asinh(tan(lat_rad)) / M_PI) / 2.0 * world;
    int max_tile = (1 << zoom) - 1;

    /* Ciel (visible au-dessus de l'horizon en vue inclinée). */
    gfx_begin_2d();
    gfx_rect_vgradient(v->x, v->y, v->w, v->h, SKY_TOP, SKY_HORIZON);

    /* Caméra : distance telle qu'en vue de dessus 1 pixel de tuile = 1 pixel écran. */
    float dist = v->h / (2.0f * tanf(FOV_Y * 0.5f * (float)M_PI / 180.0f));
    float h_rad = v->heading * (float)M_PI / 180.0f;
    float p_rad = pitch * (float)M_PI / 180.0f;
    float fwd_x = sinf(h_rad), fwd_y = cosf(h_rad);

    glViewport(v->x, SCREEN_H - v->y - v->h, v->w, v->h);
    glEnable(GL_SCISSOR_TEST);
    glScissor(v->x, SCREEN_H - v->y - v->h, v->w, v->h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV_Y, (float)v->w / v->h, dist * 0.05f, dist * 8.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-fwd_x * dist * cosf(p_rad), -fwd_y * dist * cosf(p_rad), dist * sinf(p_rad),
              0, 0, 0,
              0, 0, 1);

    /* Brouillard vers la couleur de l'horizon : fond les bords de la zone chargée. */
    if (tilted) {
        float fog_color[4] = { 214 / 255.0f, 228 / 255.0f, 240 / 255.0f, 1.0f };
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogfv(GL_FOG_COLOR, fog_color);
        glFogf(GL_FOG_START, dist * 1.3f);
        glFogf(GL_FOG_END, dist * 2.6f);
    }

    /* Zone de tuiles : autour du centre en vue de dessus, décalée vers l'avant en 3D. */
    int radius = tilted ? 3 : 2;
    double focus_x = px + (tilted ? fwd_x * TILE_SIZE * 1.5 : 0);
    double focus_y = py - (tilted ? fwd_y * TILE_SIZE * 1.5 : 0);
    int ctx = (int)floor(focus_x / TILE_SIZE), cty = (int)floor(focus_y / TILE_SIZE);

    /* Sol uni sous les tuiles manquantes. */
    float extent = (radius + 2) * TILE_SIZE;
    float fcx = (float)(focus_x - px), fcy = (float)-(focus_y - py);
    glDisable(GL_TEXTURE_2D);
    gfx_set_color(GROUND_COLOR);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(fcx - extent, fcy + extent, 0);
    glVertex3f(fcx + extent, fcy + extent, 0);
    glVertex3f(fcx - extent, fcy - extent, 0);
    glVertex3f(fcx + extent, fcy - extent, 0);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glColor4ub(255, 255, 255, 255);
    int uploads = 0;
    sceKernelLockLwMutex(&slots_lock, 1, NULL);
    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            int tx = ctx + dx, ty = cty + dy;
            if (ty < 0 || ty > max_tile) continue;
            int wrapped_x = ((tx % (max_tile + 1)) + (max_tile + 1)) % (max_tile + 1);
            TileSlot *s = request_tile(zoom, wrapped_x, ty, dx * dx + dy * dy);
            if (!s) continue;
            if (s->state == SLOT_PIXELS && uploads < UPLOADS_PER_FRAME) {
                upload_tile(s);
                uploads++;
            }
            if (s->state != SLOT_TEXTURE) continue;

            /* Repère local : x vers l'est, y vers le nord, origine = position. */
            float x0 = (float)(tx * TILE_SIZE - px), x1 = x0 + TILE_SIZE;
            float y0 = (float)-(ty * TILE_SIZE - py), y1 = y0 - TILE_SIZE;
            glBindTexture(GL_TEXTURE_2D, s->tex);
            glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(0, 0); glVertex3f(x0, y0, 0);
            glTexCoord2f(1, 0); glVertex3f(x1, y0, 0);
            glTexCoord2f(0, 1); glVertex3f(x0, y1, 0);
            glTexCoord2f(1, 1); glVertex3f(x1, y1, 0);
            glEnd();
        }
    }
    sceKernelUnlockLwMutex(&slots_lock, 1);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);

    /* Position : cercle de précision, puis flèche de cap ou point. */
    if (v->accuracy > 0) {
        double meters_per_px = 156543.03392 * cos(lat_rad) / (double)(1 << zoom);
        float r = (float)(v->accuracy / meters_per_px);
        if (r > 6 && r < 3 * TILE_SIZE)
            ground_disc(0, 0, 0.1f, r, RGBA8(40, 120, 255, 60));
    }
    if (v->show_arrow) {
        ground_arrow(v->arrow_heading, 20, 0.2f, RGBA8(255, 255, 255, 255));
        ground_arrow(v->arrow_heading, 15, 0.3f, RGBA8(30, 110, 255, 255));
    } else {
        ground_disc(0, 0, 0.2f, 10, RGBA8(255, 255, 255, 255));
        ground_disc(0, 0, 0.3f, 7, RGBA8(30, 110, 255, 255));
    }

    /* Surcouches 2D : zoom, boussole, attribution, cadre. */
    gfx_begin_2d();
    char buf[48];
    snprintf(buf, sizeof(buf), "Zoom %d  %s", zoom, tilted ? "3D" : "2D");
    gfx_rect(v->x, v->y, 110, 20, RGBA8(255, 255, 255, 180));
    text_draw(v->x + 4, v->y + 15, RGBA8(40, 40, 40, 255), 0.7f, buf);

    float cx = v->x + v->w - 26.0f, cy = v->y + 26.0f;
    float na = -v->heading * (float)M_PI / 180.0f;   /* direction du nord à l'écran */
    float nx = sinf(na), ny = -cosf(na);
    gfx_circle(cx, cy, 18, RGBA8(255, 255, 255, 200));
    glBegin(GL_TRIANGLES);
    gfx_set_color(RGBA8(220, 40, 40, 255));
    glVertex3f(cx + nx * 14, cy + ny * 14, 0);
    glVertex3f(cx - ny * 5, cy + nx * 5, 0);
    glVertex3f(cx + ny * 5, cy - nx * 5, 0);
    gfx_set_color(RGBA8(90, 90, 90, 255));
    glVertex3f(cx - nx * 14, cy - ny * 14, 0);
    glVertex3f(cx - ny * 5, cy + nx * 5, 0);
    glVertex3f(cx + ny * 5, cy - nx * 5, 0);
    glEnd();

    draw_panel_frame(v->x, v->y, v->w, v->h);
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
        for (int i = 0; i < CACHE_SLOTS; ++i) {
            if (slots[i].tex) glDeleteTextures(1, &slots[i].tex);
            free(slots[i].pixels);
        }
        sceKernelDeleteLwMutex(&slots_lock);
        ready = 0;
    }
    if (http_tmpl >= 0) sceHttpDeleteTemplate(http_tmpl);
    http_tmpl = -1;
    sceHttpTerm();
    sceSysmoduleUnloadModule(SCE_SYSMODULE_HTTP);
}
