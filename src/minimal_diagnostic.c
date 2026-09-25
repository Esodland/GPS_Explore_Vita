/* GPS-only diagnostic: no renderer, HTTP, method sweep or plugin changes. */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <psp2/apputil.h>
#include <psp2/ctrl.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/location.h>
#include <psp2/rtc.h>
#include <psp2/sysmodule.h>
#ifdef GPS_ASSIST
#include "gps_assist.h"
#define ASSIST_AT_SECONDS 5
#endif

#define DIAG_LOG "ux0:/data/gps_minimal_diag.txt"
#define SONY_DIR "ux0:/liblocation"
#define TAIL_SIZE (16 * 1024)
#define MAX_SECONDS (10 * 60)

static volatile int stop_worker;
static SceLocationHandle gps_handle;
static char tail[TAIL_SIZE + 1];

static void log_event(const char *fmt, ...) {
    SceDateTime date;
    memset(&date, 0, sizeof(date));
    sceRtcGetCurrentClockLocalTime(&date);
    FILE *file = fopen(DIAG_LOG, "a");
    if (!file) return;
    fprintf(file, "%04u-%02u-%02u %02u:%02u:%02u.%03u ",
            date.year, date.month, date.day, date.hour, date.minute,
            date.second, date.microsecond / 1000);
    va_list args;
    va_start(args, fmt);
    vfprintf(file, fmt, args);
    va_end(args);
    fputc('\n', file);
    fclose(file);
}

static int location_worker(SceSize args, void *argp) {
    (void)args;
    (void)argp;
    unsigned call = 0;
    while (!stop_worker) {
        SceLocationLocationInfo info;
        SceRtcTick before = {0}, after = {0};
        memset(&info, 0, sizeof(info));
        sceRtcGetCurrentTick(&before);
        int result = sceLocationGetLocation(gps_handle, &info);
        sceRtcGetCurrentTick(&after);
        if (stop_worker) break;
        ++call;
        if (result == 0) {
            log_event("API call=%u ret=0x%08X wait_ms=%llu lat=%.7f lon=%.7f accuracy_m=%.1f data_tick=%llu",
                      call, (unsigned)result,
                      (unsigned long long)((after.tick - before.tick) / 1000),
                      info.latitude, info.longitude, info.accuracy,
                      (unsigned long long)info.timestamp.tick);
        } else {
            log_event("API call=%u ret=0x%08X wait_ms=%llu",
                      call, (unsigned)result,
                      (unsigned long long)((after.tick - before.tick) / 1000));
        }
        sceKernelDelayThread(1000 * 1000);
    }
    return 0;
}

static int newest_sony_log(char *path, size_t size) {
    SceUID dir = sceIoDopen(SONY_DIR);
    if (dir < 0) return dir;
    char newest[256] = "";
    SceIoDirent entry;
    memset(&entry, 0, sizeof(entry));
    while (sceIoDread(dir, &entry) > 0) {
        if (!strncmp(entry.d_name, "liblocation_", 12) &&
            strstr(entry.d_name, ".log") && strcmp(entry.d_name, newest) > 0)
            snprintf(newest, sizeof(newest), "%s", entry.d_name);
        memset(&entry, 0, sizeof(entry));
    }
    sceIoDclose(dir);
    if (!newest[0]) return -1;
    snprintf(path, size, SONY_DIR "/%s", newest);
    return 0;
}

static int nmea_number(const char *sentence, int field) {
    const char *p = sentence;
    for (int i = 0; i < field; ++i) {
        p = strchr(p, ',');
        if (!p) return -1;
        ++p;
    }
    if (*p < '0' || *p > '9') return -1;
    return atoi(p);
}

static void nmea_time(const char *sentence, char out[16]) {
    const char *p = strchr(sentence, ',');
    if (!p) return;
    ++p;
    size_t n = strcspn(p, ",*\r\n");
    if (n > 15) n = 15;
    memcpy(out, p, n);
    out[n] = 0;
}

/* Observe only bytes added after launch, so an old log cannot masquerade as live GPS. */
static void poll_sony_log(char last_path[300], SceOff *last_size, int *seen_data) {
    char path[300];
    if (newest_sony_log(path, sizeof(path)) < 0) return;
    SceUID fd = sceIoOpen(path, SCE_O_RDONLY, 0);
    if (fd < 0) return;
    SceOff size = sceIoLseek(fd, 0, SCE_SEEK_END);
    if (size < 0) { sceIoClose(fd); return; }
    if (strcmp(path, last_path) != 0 || size < *last_size) {
        snprintf(last_path, 300, "%s", path);
        *last_size = 0;
        log_event("SONY_LOG file=%s", path);
    }
    if (size == *last_size) { sceIoClose(fd); return; }
    SceOff start = *last_size;
    if (size - start > TAIL_SIZE) start = size - TAIL_SIZE;
    sceIoLseek(fd, start, SCE_SEEK_SET);
    int read_size = sceIoRead(fd, tail, TAIL_SIZE);
    sceIoClose(fd);
    *last_size = size;
    if (read_size <= 0) return;
    tail[read_size] = 0;
    char *line = tail;
    if (start > 0 && start != *last_size) {
        line = strchr(tail, '\n');
        if (!line) return;
        ++line;
    }
    int found_gga = 0, quality = -1, used = -1, driver = -1;
    char utc[16] = "", chip_date[11] = "";
    for (; line && *line;) {
        char *next = strchr(line, '\n');
        if (!next) break; /* Ignore an incomplete final line. */
        *next = 0;
        char *sat = strstr(line, "Satellite Num [");
        if (sat) driver = atoi(sat + strlen("Satellite Num ["));
        /* Date de la puce : 2007/xx/xx tant que le compteur de semaines est faux. */
        char *date = strstr(line, "datetime[");
        if (date && strlen(date) >= 19) {
            memcpy(chip_date, date + strlen("datetime["), 10);
            chip_date[10] = 0;
        }
        char *nmea = strchr(line, '$');
        if (nmea && strlen(nmea) > 7 && !strncmp(nmea + 3, "GGA,", 4)) {
            found_gga = 1;
            quality = nmea_number(nmea, 6);
            used = nmea_number(nmea, 7);
            nmea_time(nmea, utc);
        }
        line = next + 1;
    }
    if (found_gga || driver >= 0) {
        *seen_data = 1;
        log_event("NMEA utc=%s quality=%d sats_used=%d driver_sats=%d chip_date=%s",
                  utc[0] ? utc : "?", quality, used, driver, chip_date[0] ? chip_date : "?");
    }
}

int main(void) {
#ifdef GPS_ASSIST
    log_event("BEGIN minimal GPS diagnostic + assist; GPS method=5; max=%d seconds; START=stop", MAX_SECONDS);
#else
    log_event("BEGIN minimal GPS diagnostic; GPS method=5; max=%d seconds; START=stop", MAX_SECONDS);
#endif
    SceAppUtilInitParam init = {0};
    SceAppUtilBootParam boot = {0};
    int apputil_module = sceSysmoduleLoadModule(SCE_SYSMODULE_APPUTIL);
    int apputil = sceAppUtilInit(&init, &boot);
    int location_module = sceSysmoduleLoadModule(SCE_SYSMODULE_LOCATION);
    log_event("INIT apputil_module=0x%08X apputil=0x%08X location_module=0x%08X",
              (unsigned)apputil_module, (unsigned)apputil, (unsigned)location_module);
    if (location_module < 0) return 1;

    /* Establish the log baseline before starting GPS. */
    char last_path[300] = "";
    SceOff last_size = 0;
    int seen_data = 0;
    char baseline[300];
    if (newest_sony_log(baseline, sizeof(baseline)) == 0) {
        SceUID fd = sceIoOpen(baseline, SCE_O_RDONLY, 0);
        if (fd >= 0) {
            last_size = sceIoLseek(fd, 0, SCE_SEEK_END);
            sceIoClose(fd);
            snprintf(last_path, sizeof(last_path), "%s", baseline);
            log_event("SONY_LOG baseline=%s bytes=%lld", baseline, (long long)last_size);
        }
    }

    gps_handle = 0;
    int result = sceLocationOpen(&gps_handle, SCE_LOCATION_LMETHOD_GPS,
                                 SCE_LOCATION_HMETHOD_NONE);
    log_event("OPEN method=5 ret=0x%08X handle=0x%08X", (unsigned)result, (unsigned)gps_handle);
    if (result < 0 || gps_handle == 0 || gps_handle == 0xFFFFFFFFu) goto done;
    result = sceLocationConfirm(gps_handle);
    log_event("CONFIRM ret=0x%08X", (unsigned)result);
    if (result < 0) goto close_handle;

    int permission = 0;
    for (int i = 0; i < 120 && !permission; ++i) {
        SceLocationDialogStatus status = SCE_LOCATION_DIALOG_STATUS_IDLE;
        result = sceLocationConfirmGetStatus(gps_handle, &status);
        if (result < 0) { log_event("CONFIRM_STATUS ret=0x%08X", (unsigned)result); break; }
        if (status == SCE_LOCATION_DIALOG_STATUS_FINISHED) {
            SceLocationDialogResult answer = SCE_LOCATION_DIALOG_RESULT_NONE;
            result = sceLocationConfirmGetResult(gps_handle, &answer);
            log_event("CONFIRM_RESULT ret=0x%08X answer=%d", (unsigned)result, (int)answer);
            permission = result == 0 && answer == SCE_LOCATION_DIALOG_RESULT_ENABLE;
            break;
        }
        sceKernelDelayThread(500 * 1000);
    }
    if (!permission) { log_event("STOP no location permission"); goto close_handle; }

    stop_worker = 0;
    SceUID thread = sceKernelCreateThread("GpsOnlyGetLocation", location_worker,
                                         0x10000100, 0x4000, 0, 0, NULL);
    if (thread < 0) { log_event("THREAD_CREATE ret=0x%08X", (unsigned)thread); goto close_handle; }
    result = sceKernelStartThread(thread, 0, NULL);
    if (result < 0) {
        log_event("THREAD_START ret=0x%08X", (unsigned)result);
        sceKernelDeleteThread(thread);
        goto close_handle;
    }
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_DIGITAL);
    for (int elapsed = 0; elapsed < MAX_SECONDS; ++elapsed) {
        sceKernelPowerTick(SCE_KERNEL_POWER_TICK_DISABLE_AUTO_SUSPEND);
        poll_sony_log(last_path, &last_size, &seen_data);
        SceCtrlData pad = {0};
        sceCtrlPeekBufferPositive(0, &pad, 1);
        if (pad.buttons & SCE_CTRL_START) { log_event("STOP requested by START"); break; }
#ifdef GPS_ASSIST
        /* GPS déjà démarré : la machine d'état de SceGps accepte alors les ioctl. */
        if (elapsed == ASSIST_AT_SECONDS && gps_assist_start(log_event) == 0) {
            gps_assist_inject_time();
            gps_assist_inject_xtra();
        }
#endif
        if (elapsed && elapsed % 30 == 0) log_event("HEARTBEAT elapsed_s=%d sony_data=%d", elapsed, seen_data);
        sceKernelDelayThread(1000 * 1000);
    }
    stop_worker = 1;
    result = sceLocationCancelGetLocation(gps_handle);
    log_event("CANCEL ret=0x%08X", (unsigned)result);
    SceUInt timeout = 5 * 1000 * 1000;
    result = sceKernelWaitThreadEnd(thread, NULL, &timeout);
    log_event("THREAD_END ret=0x%08X", (unsigned)result);
    if (result >= 0) sceKernelDeleteThread(thread);
    if (!seen_data) log_event("NMEA unavailable: check /CONFIG/LOCATION/enable_log and Sony log creation");
#ifdef GPS_ASSIST
    gps_assist_stop();
#endif

close_handle:
    result = sceLocationClose(gps_handle);
    log_event("CLOSE ret=0x%08X", (unsigned)result);
done:
    log_event("END minimal GPS diagnostic");
    sceSysmoduleUnloadModule(SCE_SYSMODULE_LOCATION);
    if (apputil >= 0) sceAppUtilShutdown();
    if (apputil_module >= 0) sceSysmoduleUnloadModule(SCE_SYSMODULE_APPUTIL);
    return 0;
}
