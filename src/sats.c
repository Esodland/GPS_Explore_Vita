#include "sats.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/kernel/threadmgr.h>

#define LOG_DIR "ux0:/liblocation"
#define TAIL_BYTES (24 * 1024)
#define POLL_US (2 * 1000 * 1000)

static SceKernelLwMutexWork lock;
static SceUID thid = -1;
static volatile int stop;
static SatInfo shared;
static char tail[TAIL_BYTES + 1];

/* Cycle GSV en cours d'accumulation, par constellation (GP = GPS, GL = GLONASS...). */
typedef struct {
    char talker[3];
    int in_view;
    int count;
    int snr[SATS_MAX];
    int committed_in_view;
    int committed_count;
    int committed_snr[SATS_MAX];
} GsvCycle;

/* Renvoie le champ n (0 = identifiant de trame) d'une phrase NMEA, sans le checksum. */
static const char *nmea_field(const char *line, int n, int *len) {
    const char *p = line;
    for (int i = 0; i < n; ++i) {
        p = strchr(p, ',');
        if (!p) return NULL;
        p++;
    }
    const char *end = p;
    while (*end && *end != ',' && *end != '*' && *end != '\r' && *end != '\n') end++;
    *len = (int)(end - p);
    return p;
}

static int nmea_int(const char *line, int n, int fallback) {
    int len;
    const char *f = nmea_field(line, n, &len);
    if (!f || len == 0) return fallback;
    return atoi(f);
}

static GsvCycle *cycle_for(GsvCycle *cycles, int *ncycles, const char *talker) {
    for (int i = 0; i < *ncycles; ++i)
        if (cycles[i].talker[0] == talker[0] && cycles[i].talker[1] == talker[1]) return &cycles[i];
    if (*ncycles >= 4) return NULL;
    GsvCycle *c = &cycles[(*ncycles)++];
    memset(c, 0, sizeof(*c));
    c->talker[0] = talker[0];
    c->talker[1] = talker[1];
    return c;
}

static void parse_tail(const char *buf, SatInfo *out) {
    GsvCycle cycles[4];
    int ncycles = 0;
    memset(out, 0, sizeof(*out));
    out->driver_sats = -1;

    for (const char *line = buf; line && *line;) {
        const char *next = strchr(line, '\n');
        /* Ligne du pilote : "[GPS] scePosGpsGetData Satellite Num [N]" */
        const char *sat_num = strstr(line, "Satellite Num [");
        if (sat_num && (!next || sat_num < next)) out->driver_sats = atoi(sat_num + 15);
        const char *nmea = strchr(line, '$');
        if (nmea && (!next || nmea < next) && strlen(nmea) > 6) {
            const char *type = nmea + 3;   /* $GPGSV : talker = nmea+1, type = nmea+3 */
            if (!strncmp(type, "GSV", 3)) {
                out->has_gsv = 1;
                GsvCycle *c = cycle_for(cycles, &ncycles, nmea + 1);
                if (c) {
                    int total = nmea_int(nmea, 1, 0), num = nmea_int(nmea, 2, 0);
                    if (num == 1) {
                        c->count = 0;
                        c->in_view = nmea_int(nmea, 3, 0);
                    }
                    for (int k = 0; k < 4 && c->count < SATS_MAX; ++k) {
                        int len;
                        const char *prn = nmea_field(nmea, 4 + 4 * k, &len);
                        if (!prn || len == 0) break;
                        c->snr[c->count++] = nmea_int(nmea, 7 + 4 * k, 0);
                    }
                    if (num == total && total > 0) {
                        c->committed_in_view = c->in_view;
                        c->committed_count = c->count;
                        memcpy(c->committed_snr, c->snr, sizeof(c->snr));
                    }
                    out->available = 1;
                }
            } else if (!strncmp(type, "GGA", 3)) {
                out->fix_quality = nmea_int(nmea, 6, 0);
                out->used = nmea_int(nmea, 7, 0);
                out->available = 1;
            } else if (!strncmp(type, "GSA", 3)) {
                out->fix_type = nmea_int(nmea, 2, 1);
                out->available = 1;
            }
            /* L'horodatage du journal Sony est en début de ligne : "hh:mm:ss.mmm,[nmea],..." */
            if (line[2] == ':' && line[5] == ':') {
                memcpy(out->last_time, line, 8);
                out->last_time[8] = '\0';
            }
        }
        line = next ? next + 1 : NULL;
    }

    for (int i = 0; i < ncycles; ++i) {
        out->in_view += cycles[i].committed_in_view;
        for (int k = 0; k < cycles[i].committed_count && out->count < SATS_MAX; ++k) {
            int snr = cycles[i].committed_snr[k];
            out->snr[out->count++] = snr;
            if (snr > 0) out->tracked++;
        }
    }
}

/* Le journal le plus récent : les noms contiennent la date, l'ordre alphabétique suffit. */
static int newest_log(char *path, int size) {
    SceUID dir = sceIoDopen(LOG_DIR);
    if (dir < 0) return dir;
    char best[256] = "";
    SceIoDirent ent;
    while (sceIoDread(dir, &ent) > 0) {
        if (!strncmp(ent.d_name, "liblocation_", 12) && strcmp(ent.d_name, best) > 0)
            snprintf(best, sizeof(best), "%s", ent.d_name);
    }
    sceIoDclose(dir);
    if (!best[0]) return -1;
    snprintf(path, size, LOG_DIR "/%s", best);
    return 0;
}

static int monitor_thread(SceSize args, void *argp) {
    (void)args; (void)argp;
    char path[300];
    while (!stop) {
        if (newest_log(path, sizeof(path)) == 0) {
            SceUID fd = sceIoOpen(path, SCE_O_RDONLY, 0);
            if (fd >= 0) {
                SceOff size = sceIoLseek(fd, 0, SCE_SEEK_END);
                SceOff start = size > TAIL_BYTES ? size - TAIL_BYTES : 0;
                sceIoLseek(fd, start, SCE_SEEK_SET);
                int n = sceIoRead(fd, tail, TAIL_BYTES);
                sceIoClose(fd);
                if (n > 0) {
                    tail[n] = '\0';
                    SatInfo info;
                    parse_tail(tail, &info);
                    sceKernelLockLwMutex(&lock, 1, NULL);
                    shared = info;
                    sceKernelUnlockLwMutex(&lock, 1);
                }
            }
        }
        sceKernelDelayThread(POLL_US);
    }
    return 0;
}

int sats_start(void) {
    memset(&shared, 0, sizeof(shared));
    int ret = sceKernelCreateLwMutex(&lock, "SatInfoLock", 0, 0, NULL);
    if (ret < 0) return ret;
    stop = 0;
    thid = sceKernelCreateThread("SatMonitor", monitor_thread, 0x10000100 + 20, 0x4000, 0, 0, NULL);
    if (thid < 0) return thid;
    return sceKernelStartThread(thid, 0, NULL);
}

void sats_stop(void) {
    if (thid < 0) return;
    stop = 1;
    SceUInt timeout = 5 * 1000 * 1000;
    if (sceKernelWaitThreadEnd(thid, NULL, &timeout) >= 0) {
        sceKernelDeleteThread(thid);
        sceKernelDeleteLwMutex(&lock);
    }
    thid = -1;
}

void sats_get(SatInfo *out) {
    if (thid < 0) {
        memset(out, 0, sizeof(*out));
        return;
    }
    sceKernelLockLwMutex(&lock, 1, NULL);
    *out = shared;
    sceKernelUnlockLwMutex(&lock, 1);
}
