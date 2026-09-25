/*
 * Aide à la puce GPS : injection de l'heure et des données XTRA, comme le
 * module COM de shell.self (3.60) le ferait s'il était démarré. Formats
 * relevés dans shell.self : TimeSet 0x81197c38, SET_XTRA 0x81197d9a,
 * lecture XTRA 0x81198212. Voir SceLocation_Reverse_Engineering.md.
 */
#include <stdio.h>
#include <string.h>
#include <psp2/gps.h>
#include <psp2/io/fcntl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/rtc.h>
#include <taihen.h>
#include "gps_assist.h"

#define GPS_ASSIST_MODULE    "ux0:app/GPSX00001/gps_assist.skprx"
#define GPS_ASSIST_XTRA_PATH "ux0:data/gps_assist/xtra.bin"

#define GPS_IOCTL_GET_XTRA_INFO 0xAB001000u /* 0x404 octets */
#define GPS_IOCTL_GET_1001      0xAB001001u /* 4 octets, lu par Sony avant TimeSet */
#define GPS_IOCTL_SET_XTRA      0x5B001000u /* 0x5E4 octets par bloc */
#define GPS_IOCTL_TIME_SET      0x5B001001u /* 16 octets */

#define XTRA_CHUNK_MAX  0x5DC
#define XTRA_URL_COUNT  3
#define XTRA_URL_SIZE   0x100
/* Sony envoie 0 ; l'horloge de la console n'est juste qu'à la seconde près. */
#define TIME_UNC_MSEC   1000
#define UNIX_EPOCH_TICK 62135596800000000ULL /* µs entre 0001-01-01 et 1970-01-01 */

typedef struct {
    int64_t time_msec;    /* ms UTC depuis 1970 */
    int32_t time_unc_msec;
    uint8_t time_base;    /* 1 chez Sony */
    uint8_t force;        /* 0 chez Sony */
    uint8_t pad[2];
} GpsTimeSet;

typedef struct {
    uint16_t max_size;
    uint8_t url_count;
    uint8_t reserved;
    char urls[XTRA_URL_COUNT][XTRA_URL_SIZE];
    uint8_t tail[0x404 - 4 - XTRA_URL_COUNT * XTRA_URL_SIZE];
} GpsXtraInfo;

typedef struct {
    uint8_t part;         /* 0, 1, 2… */
    uint8_t pad;
    uint16_t reserved;
    uint16_t total_size;
    uint16_t chunk_size;
    uint8_t data[XTRA_CHUNK_MAX];
} GpsXtraChunk;

_Static_assert(sizeof(GpsTimeSet) == 16, "TimeSet: 16 octets");
_Static_assert(sizeof(GpsXtraInfo) == 0x404, "XTRA info: 0x404 octets");
_Static_assert(sizeof(GpsXtraChunk) == 0x5E4, "SET_XTRA: 0x5E4 octets");

static gps_assist_log_fn log_fn;
static SceUID module_id = -1;
static uint16_t xtra_max_size;
static GpsXtraChunk chunk;
static uint8_t xtra[0xFFFF];

static int gps_ioctl(SceUInt32 cmd, void *arg, SceSize size) {
    /* Le noyau lit la taille dans le premier mot (≤ 0x640) puis recopie arg en retour. */
    SceSize io[2] = {size, 0};
    return _sceGpsIoctl(cmd, arg, size, io);
}

int gps_assist_start(gps_assist_log_fn log) {
    log_fn = log;
    SceUID pid = sceKernelGetProcessId();
    module_id = taiLoadStartKernelModule(GPS_ASSIST_MODULE, sizeof(pid), &pid, 0);
    log_fn("ASSIST module=%s ret=0x%08X pid=0x%08X", GPS_ASSIST_MODULE,
           (unsigned)module_id, (unsigned)pid);
    if (module_id < 0) return module_id;

    uint32_t value = 0;
    int ret = gps_ioctl(GPS_IOCTL_GET_1001, &value, sizeof(value));
    log_fn("ASSIST GET 0xAB001001 ret=0x%08X value=0x%08X", (unsigned)ret, (unsigned)value);

    static GpsXtraInfo info;
    memset(&info, 0, sizeof(info));
    ret = gps_ioctl(GPS_IOCTL_GET_XTRA_INFO, &info, sizeof(info));
    xtra_max_size = ret == 0 ? info.max_size : 0;
    log_fn("ASSIST GET_XTRA_INFO ret=0x%08X max_size=%u urls=%u", (unsigned)ret,
           (unsigned)info.max_size, (unsigned)info.url_count);
    for (unsigned i = 0; ret == 0 && i < info.url_count && i < XTRA_URL_COUNT; ++i) {
        info.urls[i][XTRA_URL_SIZE - 1] = 0;
        log_fn("ASSIST XTRA_URL[%u]=%s", i, info.urls[i]);
    }
    return ret;
}

int gps_assist_inject_time(void) {
    SceRtcTick tick = {0};
    const char *source = "retained_network";
    int ret = sceRtcGetCurrentRetainedNetworkTick(&tick);
    if (ret < 0 || tick.tick <= UNIX_EPOCH_TICK) {
        source = "system";
        ret = sceRtcGetCurrentTick(&tick);
    }
    if (ret < 0 || tick.tick <= UNIX_EPOCH_TICK) {
        log_fn("ASSIST TIME no valid clock ret=0x%08X", (unsigned)ret);
        return ret < 0 ? ret : -1;
    }
    GpsTimeSet arg;
    memset(&arg, 0, sizeof(arg));
    arg.time_msec = (int64_t)((tick.tick - UNIX_EPOCH_TICK) / 1000);
    arg.time_unc_msec = TIME_UNC_MSEC;
    arg.time_base = 1;
    ret = gps_ioctl(GPS_IOCTL_TIME_SET, &arg, sizeof(arg));
    log_fn("ASSIST TimeSet ret=0x%08X source=%s time_msec=%lld unc_ms=%d", (unsigned)ret,
           source, (long long)arg.time_msec, (int)arg.time_unc_msec);
    return ret;
}

int gps_assist_inject_xtra(void) {
    SceUID fd = sceIoOpen(GPS_ASSIST_XTRA_PATH, SCE_O_RDONLY, 0);
    if (fd < 0) {
        log_fn("ASSIST XTRA open %s ret=0x%08X", GPS_ASSIST_XTRA_PATH, (unsigned)fd);
        return fd;
    }
    int size = sceIoRead(fd, xtra, sizeof(xtra));
    int extra = sceIoRead(fd, &chunk, 1);
    sceIoClose(fd);
    if (size <= 0 || extra != 0 || (xtra_max_size && size > xtra_max_size)) {
        log_fn("ASSIST XTRA refused size=%d too_big=%d max=%u", size, extra != 0,
               (unsigned)xtra_max_size);
        return -1;
    }
    log_fn("ASSIST XTRA file size=%d", size);

    /* Sony attend une notification de la puce entre deux blocs ; on ne la voit
       pas d'ici, d'où une pause fixe et l'arrêt au premier refus. */
    int ret = 0;
    unsigned part = 0;
    for (int offset = 0; offset < size; offset += chunk.chunk_size, ++part) {
        memset(&chunk, 0, sizeof(chunk));
        chunk.part = (uint8_t)part;
        chunk.total_size = (uint16_t)size;
        chunk.chunk_size = (uint16_t)(size - offset > XTRA_CHUNK_MAX ? XTRA_CHUNK_MAX : size - offset);
        memcpy(chunk.data, xtra + offset, chunk.chunk_size);
        ret = gps_ioctl(GPS_IOCTL_SET_XTRA, &chunk, sizeof(chunk));
        log_fn("ASSIST SET_XTRA part=%u offset=%d len=%u ret=0x%08X", part, offset,
               (unsigned)chunk.chunk_size, (unsigned)ret);
        if (ret != 0) break;
        sceKernelDelayThread(300 * 1000);
    }
    return ret;
}

void gps_assist_stop(void) {
    if (module_id < 0) return;
    int status = 0;
    int ret = taiStopUnloadKernelModule(module_id, 0, NULL, 0, NULL, &status);
    log_fn("ASSIST unload ret=0x%08X status=0x%08X", (unsigned)ret, (unsigned)status);
    module_id = -1;
}
