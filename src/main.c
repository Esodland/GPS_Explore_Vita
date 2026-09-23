#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/sysmodule.h>
#include <psp2/location.h>
#include <psp2/ctrl.h>
#include <psp2/apputil.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/registrymgr.h>
#include <psp2/io/stat.h>
#include <math.h>
#include <malloc.h>

#include "gfx.h"
#include "map.h"
#include "sats.h"
#include "text.h"

/* Panneau carte, côté droit de l'écran 960x544 */
#define MAP_X 500
#define MAP_Y 24
#define MAP_W 440
#define MAP_H 480

void log_debug(const char* msg, int code) {
    FILE* f = fopen("ux0:/data/gps_debug.txt", "a");
    if (f) {
        fprintf(f, "[DEBUG] %s : 0x%08X\n", msg, code);
        fclose(f);
    }
}

static void log_memory(const char *stage) {
    SceKernelFreeMemorySizeInfo info = { .size = sizeof(info) };
    int result = sceKernelGetFreeMemorySize(&info);
    FILE *f = fopen("ux0:/data/gps_debug.txt", "a");
    if (f) {
        fprintf(f, "[MEM] %s ret=0x%08X user=%d cdram=%d phycont=%d bytes\n",
                stage, (unsigned)result, info.size_user, info.size_cdram, info.size_phycont);
        fclose(f);
    }
}

static int valid_handle(SceLocationHandle handle) {
    return handle != 0 && handle != 0xFFFFFFFFu;
}

static int measured_open(SceLocationHandle *handle, int method) {
    *handle = 0;
    log_debug("open method", method);
    log_memory("before open");
    int result = sceLocationOpen(handle, method, SCE_LOCATION_HMETHOD_NONE);
    log_memory("after open");
    log_debug("sceLocationOpen", result);
    log_debug("returned handle", *handle);
    return result;
}

/* Automatic, bounded probe: no confirm/getLocation and no invented handle. */
static void probe_methods(void) {
    const int methods[] = {0, 5, 2, 1, 3, 4};
    for (unsigned i = 0; i < sizeof(methods) / sizeof(methods[0]); ++i) {
        SceLocationHandle probe = 0;
        int result = measured_open(&probe, methods[i]);
        if (result == 0 && valid_handle(probe)) {
            log_debug("probe close", sceLocationClose(probe));
            log_memory("after probe close");
        }
    }
}

/*
 * sceLocationGetLocation bloque jusqu'au fix : on l'appelle depuis un thread
 * dédié, la boucle d'affichage ne lit que le dernier résultat publié.
 */
typedef struct {
    SceKernelLwMutexWork lock;
    SceLocationHandle handle;
    volatile int stop;
    int calls;      /* appels terminés */
    int fixes;      /* appels réussis */
    int last_ret;
    SceLocationLocationInfo last_fix;
    SceUInt64 started_us;       /* début de la recherche */
    SceUInt64 last_fix_us;      /* heure du dernier fix réussi, 0 si aucun */
    unsigned last_call_ms;      /* durée du dernier appel (GetLocation attend en interne) */
} LocationWorker;

/* Libellé lisible pour les retours "informatifs" de sceLocationGetLocation. */
static const char *location_status_text(int ret) {
    switch ((unsigned)ret) {
        case 0: return "OK";
        case 0x80101200: return "position indeterminee (satellites insuffisants)";
        case 0x80101201: return "informations insuffisantes";
        case 0x80101202: return "recherche annulee";
        case 0x80101203: return "refuse par l'utilisateur";
        default: return "erreur";
    }
}

static int location_thread(SceSize args, void *argp) {
    (void)args;
    LocationWorker *w = *(LocationWorker **)argp;
    while (!w->stop) {
        SceLocationLocationInfo info;
        memset(&info, 0, sizeof(info));
        SceUInt64 call_start = sceKernelGetProcessTimeWide();
        int result = sceLocationGetLocation(w->handle, &info);
        SceUInt64 now = sceKernelGetProcessTimeWide();
        if (w->stop) break;

        sceKernelLockLwMutex(&w->lock, 1, NULL);
        w->calls++;
        w->last_ret = result;
        w->last_call_ms = (unsigned)((now - call_start) / 1000);
        if (result == 0) {
            w->fixes++;
            w->last_fix = info;
            w->last_fix_us = now;
        }
        int calls = w->calls;
        unsigned elapsed_s = (unsigned)((now - w->started_us) / 1000000);
        unsigned call_ms = w->last_call_ms;
        sceKernelUnlockLwMutex(&w->lock, 1);

        FILE *log_file = fopen("ux0:/data/gps_log.txt", "a");
        if (log_file) {
            fprintf(log_file, "[Appel %d t+%us duree %u ms] sceLocationGetLocation: 0x%08X (Lat: %f, Lon: %f, Precision: %.1f m)\n",
                    calls, elapsed_s, call_ms, (unsigned)result, info.latitude, info.longitude, info.accuracy);
            fclose(log_file);
        }
        sceKernelDelayThread(1000 * 1000);
    }
    return 0;
}

static SceUID start_location_worker(LocationWorker *w, SceLocationHandle handle) {
    memset(w, 0, sizeof(*w));
    w->handle = handle;
    w->started_us = sceKernelGetProcessTimeWide();
    int ret = sceKernelCreateLwMutex(&w->lock, "GpsFixLock", 0, 0, NULL);
    if (ret < 0) return ret;
    SceUID thid = sceKernelCreateThread("GpsFixThread", location_thread, 0x10000100, 0x4000, 0, 0, NULL);
    if (thid < 0) {
        sceKernelDeleteLwMutex(&w->lock);
        return thid;
    }
    ret = sceKernelStartThread(thid, sizeof(w), &w);
    if (ret < 0) {
        sceKernelDeleteThread(thid);
        sceKernelDeleteLwMutex(&w->lock);
        return ret;
    }
    return thid;
}

static void stop_location_worker(LocationWorker *w, SceUID thid) {
    w->stop = 1;
    /* Débloque un GetLocation en attente de fix ; échoue sans effet si aucun appel n'est en cours. */
    log_debug("cancel get location", sceLocationCancelGetLocation(w->handle));
    SceUInt timeout = 5 * 1000 * 1000;
    int ret = sceKernelWaitThreadEnd(thid, NULL, &timeout);
    log_debug("wait location thread", ret);
    if (ret >= 0) {
        sceKernelDeleteThread(thid);
        sceKernelDeleteLwMutex(&w->lock);
    }
}

/* Satellites lus dans les trames NMEA du journal Sony : compteurs et barres de signal. */
static void draw_sats(float x, float y) {
    SatInfo sat;
    sats_get(&sat);
    char buf[96];
    if (!sat.available) {
        text_draw(x, y, RGBA8(150, 150, 150, 255), 0.8f, "Satellites : pas de trame NMEA");
        text_draw(x, y + 20, RGBA8(120, 120, 120, 255), 0.7f, "(journal Sony inactif ou GPS eteint)");
        return;
    }
    /* Un vrai fix exige des satellites utilisés : sans eux, la position NMEA est une
     * position par défaut (centre du pays), même si GGA/GSA annoncent une qualité. */
    int real_fix = sat.used > 0 && sat.fix_quality > 0;
    if (sat.has_gsv)
        snprintf(buf, sizeof(buf), "Satellites : %d visibles, %d captes, %d utilises",
                 sat.in_view, sat.tracked, sat.used);
    else if (sat.driver_sats >= 0)
        snprintf(buf, sizeof(buf), "Satellites : %d captes, %d utilises", sat.driver_sats, sat.used);
    else
        snprintf(buf, sizeof(buf), "Satellites : %d utilises", sat.used);
    text_draw(x, y, real_fix ? RGBA8(255, 255, 255, 255) : RGBA8(255, 120, 90, 255), 0.8f, buf);
    const char *fix = !real_fix ? "pas de fix (position par defaut)"
                    : sat.fix_type >= 3 ? "fix 3D" : "fix 2D";
    snprintf(buf, sizeof(buf), "Etat : %s   (trame %s)", fix, sat.last_time);
    text_draw(x, y + 20, RGBA8(180, 180, 180, 255), 0.7f, buf);
    if (!sat.has_gsv) {
        text_draw(x, y + 40, RGBA8(120, 120, 120, 255), 0.7f, "(la puce ne publie pas la liste des satellites)");
        return;
    }

    /* Barres : hauteur = SNR (0 à 50 dB-Hz). Vert >= 30, orange >= 20, rouge sinon. */
    float bar_w = 12, gap = 4, base = y + 90;
    for (int i = 0; i < sat.count && i < 24; ++i) {
        int snr = sat.snr[i];
        float h = snr > 50 ? 60 : snr * 60.0f / 50.0f;
        unsigned color = snr >= 30 ? RGBA8(60, 200, 90, 255) : snr >= 20 ? RGBA8(240, 170, 40, 255)
                       : snr > 0 ? RGBA8(220, 70, 60, 255) : RGBA8(80, 80, 90, 255);
        float bx = x + i * (bar_w + gap);
        gfx_rect(bx, base - 60, bar_w, 60, RGBA8(40, 40, 55, 255));
        if (h < 2) h = 2;
        gfx_rect(bx, base - h, bar_w, h, color);
    }
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    // Clear log at startup
    remove("ux0:/data/gps_debug.txt");
    FILE* f = fopen("ux0:/data/gps_debug.txt", "w");
    if(f) { fprintf(f, "--- GPS MEMORY DIAGNOSTIC 1 ---\n"); fclose(f); }
    log_memory("entry");

    int ret;
    
    // 0. Initialiser les modules de base
    ret = sceSysmoduleLoadModule(SCE_SYSMODULE_APPUTIL);
    log_debug("sceSysmoduleLoadModule(APPUTIL)", ret);
    
    SceAppUtilInitParam initParam;
    SceAppUtilBootParam bootParam;
    memset(&initParam, 0, sizeof(SceAppUtilInitParam));
    memset(&bootParam, 0, sizeof(SceAppUtilBootParam));
    ret = sceAppUtilInit(&initParam, &bootParam);
    log_debug("sceAppUtilInit", ret);
    log_memory("after APPUTIL");

    // Initialiser le réseau
    int ret_net = sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
    log_debug("sceSysmoduleLoadModule(NET)", ret_net);
    
    SceNetInitParam net_param;
    memset(&net_param, 0, sizeof(net_param));
    net_param.memory = malloc(1024 * 1024);
    net_param.size = 1024 * 1024;
    net_param.flags = 0;
    int ret_net_init = net_param.memory ? sceNetInit(&net_param) : -1;
    log_debug("sceNetInit", ret_net_init);

    // Initialiser NetCtl
    int ret_netctl_init = sceNetCtlInit();
    log_debug("sceNetCtlInit", ret_netctl_init);
    log_memory("before LOCATION activation");

    /* Journal interne de Sony (SceShell) : activé si le fichier drapeau existe.
     * Le serveur lit la clé à son démarrage : effet après un redémarrage de la console. */
    SceIoStat flag_stat;
    if (sceIoGetstat("ux0:/data/gps_sony_log.flag", &flag_stat) >= 0) {
        int previous = -1;
        log_debug("regmgr get enable_log", sceRegMgrGetKeyInt("/CONFIG/LOCATION", "enable_log", &previous));
        log_debug("enable_log previous", previous);
        /* Masque : 1 Skyhook, 2 pilote GPS, 4 NMEA, 8 localisation, 0x20+ divers. */
        log_debug("regmgr set enable_log=0xEF", sceRegMgrSetKeyInt("/CONFIG/LOCATION", "enable_log", 0xEF));
    }

    int ret_loc = sceSysmoduleLoadModule(SCE_SYSMODULE_LOCATION);
    log_debug("sceSysmoduleLoadModule(LOCATION)", ret_loc);
    log_memory("after LOCATION activation");
    if (ret_loc >= 0) probe_methods();

    int ret_gfx = gfx_init();
    log_debug("gfx_init (vitaGL)", ret_gfx);
    if (ret_gfx < 0) sceKernelExitProcess(0);
    log_debug("text_init", text_init());
    log_memory("after vitaGL and font");

    int ret_map = map_init();
    log_debug("map_init", ret_map);
    log_memory("after map init");
    log_debug("sats_start", sats_start());
    int map_zoom = MAP_ZOOM_DEFAULT;
    int map_3d = 1;
    int map_fullscreen = 0;
    float cam_heading = 0.0f;
    float cam_pitch = MAP_PITCH_3D;
    int manual_heading_until = 0;   /* frame jusqu'à laquelle le cap manuel prime sur le cap GPS */

    SceLocationHandle handle = 0;
    int ret_open = -1;
    int last_error = 0;
    const char *error_stage = "open";
    int handle_open = 0;
    int gps_state = 0; // 0 = Attente, 1 = Demande permission, 2 = Actif, -1 = Erreur

    static LocationWorker worker;
    SceUID worker_thid = -1;
    int search_start_frame = 0;

    int frame = 0;
    int prev_buttons = 0;

    // Configurer l'échantillonnage de la manette
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

    while (1) {
        frame++;
        
        SceCtrlData pad;
        sceCtrlPeekBufferPositive(0, &pad, 1);
        
        // Quitter l'application si on appuie sur START
        if (pad.buttons & SCE_CTRL_START) {
            break;
        }
        
        // Détecter un appui (front montant) sur CROIX
        int cross_pressed = (pad.buttons & SCE_CTRL_CROSS) && !(prev_buttons & SCE_CTRL_CROSS);
        if ((pad.buttons & SCE_CTRL_RTRIGGER) && !(prev_buttons & SCE_CTRL_RTRIGGER) && map_zoom < MAP_ZOOM_MAX)
            map_zoom++;
        if ((pad.buttons & SCE_CTRL_LTRIGGER) && !(prev_buttons & SCE_CTRL_LTRIGGER) && map_zoom > MAP_ZOOM_MIN)
            map_zoom--;
        if ((pad.buttons & SCE_CTRL_SQUARE) && !(prev_buttons & SCE_CTRL_SQUARE)) {
            map_3d = !map_3d;
            cam_pitch = map_3d ? MAP_PITCH_3D : MAP_PITCH_2D;
            if (!map_3d) cam_heading = 0.0f;   /* vue de dessus : nord en haut */
        }
        if ((pad.buttons & SCE_CTRL_TRIANGLE) && !(prev_buttons & SCE_CTRL_TRIANGLE))
            map_fullscreen = !map_fullscreen;
        prev_buttons = pad.buttons;

        /* Stick droit : rotation (X) et inclinaison (Y) de la caméra, avec zone morte. */
        int stick_x = pad.rx - 128, stick_y = pad.ry - 128;
        if (abs(stick_x) > 30) {
            cam_heading = fmodf(cam_heading + stick_x / 128.0f * 2.0f + 360.0f, 360.0f);
            manual_heading_until = frame + 5 * 60;
        }
        if (map_3d && abs(stick_y) > 30) {
            cam_pitch -= stick_y / 128.0f * 1.0f;
            if (cam_pitch < MAP_PITCH_MIN) cam_pitch = MAP_PITCH_MIN;
            if (cam_pitch > 80.0f) cam_pitch = 80.0f;
        }

        gfx_frame_begin(RGBA8(20, 20, 30, 255));
        gfx_begin_2d();

        if (gps_state == 0) {
            text_draw(20, 50, RGBA8(255, 255, 255, 255), 1.2f, "Go!Explore 2.0 (Vita Port)");
            text_draw(20, 100, RGBA8(200, 200, 200, 255), 1.0f, "Appuyez sur CROIX pour tester la geolocalisation");
            
            if (cross_pressed) {
                log_debug("User pressed CROSS", 0);
                
                const int methods[] = {5, 2, 1, 3, 4};
                if (handle_open) {
                    log_debug("close before retry", sceLocationClose(handle));
                    handle_open = 0;
                }
                for (unsigned mi = 0; mi < sizeof(methods) / sizeof(methods[0]); ++mi) {
                    ret_open = measured_open(&handle, methods[mi]);
                    if (ret_open == 0 && valid_handle(handle)) {
                        handle_open = 1;
                        break;
                    }
                }
                last_error = ret_open;
                error_stage = "open";
                gps_state = -1;
                if (handle_open) {
                    ret = sceLocationConfirm(handle);
                    log_debug("sceLocationConfirm", ret);
                    log_memory("after confirm");
                    last_error = ret;
                    error_stage = "confirm";
                    if (ret == 0) gps_state = 1;
                }
            }
        } else if (gps_state == 1) {
            SceLocationDialogStatus status = SCE_LOCATION_DIALOG_STATUS_IDLE;
            ret = sceLocationConfirmGetStatus(handle, &status);
            text_draw(20, 150, RGBA8(255, 255, 0, 255), 1.2f, "Veuillez accepter l'autorisation a l'ecran...");
            if (ret != 0) {
                last_error = ret;
                error_stage = "confirm status";
                log_debug(error_stage, ret);
                gps_state = -1;
            } else if (status == SCE_LOCATION_DIALOG_STATUS_FINISHED) {
                SceLocationDialogResult result = SCE_LOCATION_DIALOG_RESULT_NONE;
                ret = sceLocationConfirmGetResult(handle, &result);
                log_debug("confirm result call", ret);
                log_debug("confirm result", result);
                if (ret == 0 && result == SCE_LOCATION_DIALOG_RESULT_ENABLE) {
                    worker_thid = start_location_worker(&worker, handle);
                    log_debug("start location thread", worker_thid);
                    if (worker_thid >= 0) {
                        search_start_frame = frame;
                        gps_state = 2;
                    } else {
                        last_error = worker_thid;
                        error_stage = "thread GPS";
                        gps_state = -1;
                    }
                } else {
                    last_error = ret ? ret : (int)SCE_LOCATION_INFO_DENIED_BY_USER;
                    error_stage = "consentement";
                    gps_state = -1;
                }
            }
        } else if (gps_state == -1) {
            char err_buf[128];
            snprintf(err_buf, sizeof(err_buf), "Erreur %s: 0x%08X", error_stage, (unsigned)last_error);
            text_draw(20, 50, RGBA8(255, 50, 50, 255), 1.2f, err_buf);
            const char *detail = "Consultez gps_debug.txt pour le diagnostic.";
            switch ((unsigned)last_error) {
                case 0x80024302: detail = "SceShell sans memoire : retirer un plugin *main."; break;
                case 0x80101244: detail = "Methode de localisation invalide."; break;
                case 0x8010124F: detail = "Session de localisation non activee."; break;
                case 0x80101249: detail = "Application non autorisee pour la localisation."; break;
                case 0x80101203: detail = "Autorisation de localisation non accordee."; break;
            }
            text_draw(20, 90, RGBA8(200, 200, 200, 255), 1.0f, detail);
            text_draw(20, 140, RGBA8(200, 200, 200, 255), 1.0f, "CROIX : revenir au test. START : quitter.");
            if (cross_pressed) gps_state = 0;
        } else if (gps_state == 2) {
            // 3. Lire le dernier résultat publié par le thread GPS (jamais bloquant)
            sceKernelLockLwMutex(&worker.lock, 1, NULL);
            int calls = worker.calls;
            int fixes = worker.fixes;
            int last_ret = worker.last_ret;
            SceLocationLocationInfo location = worker.last_fix;
            SceUInt64 last_fix_us = worker.last_fix_us;
            unsigned last_call_ms = worker.last_call_ms;
            sceKernelUnlockLwMutex(&worker.lock, 1);
            SceUInt64 now_us = sceKernelGetProcessTimeWide();

            if (fixes > 0) {
                char buf[128];

                snprintf(buf, sizeof(buf), "GPS Status: FIX OBTENU (%d)", fixes);
                text_draw(20, 50, RGBA8(0, 255, 0, 255), 1.2f, buf);

                snprintf(buf, sizeof(buf), "Latitude  : %f", location.latitude);
                text_draw(20, 100, RGBA8(255, 255, 255, 255), 1.0f, buf);

                snprintf(buf, sizeof(buf), "Longitude : %f", location.longitude);
                text_draw(20, 130, RGBA8(255, 255, 255, 255), 1.0f, buf);

                snprintf(buf, sizeof(buf), "Altitude  : %.1f m", location.altitude);
                text_draw(20, 160, RGBA8(255, 255, 255, 255), 1.0f, buf);

                snprintf(buf, sizeof(buf), "Vitesse   : %.1f km/h", location.speed * 3.6f);
                text_draw(20, 190, RGBA8(255, 255, 255, 255), 1.0f, buf);

                snprintf(buf, sizeof(buf), "Precision : %.1f m", location.accuracy);
                text_draw(20, 220, RGBA8(255, 255, 255, 255), 1.0f, buf);

                if (last_ret != 0) {
                    unsigned lost_s = (unsigned)((now_us - last_fix_us) / 1000000);
                    text_draw(20, 260, RGBA8(255, 165, 0, 255), 1.0f, "Signal GPS perdu");
                    snprintf(buf, sizeof(buf), "Derniere position conservee (il y a %u s)", lost_s);
                    text_draw(20, 285, RGBA8(255, 165, 0, 255), 0.9f, buf);
                    text_draw(20, 308, RGBA8(200, 150, 80, 255), 0.8f, location_status_text(last_ret));
                }

                /* En mouvement, la caméra suit le cap GPS (sauf rotation manuelle récente). */
                int moving = location.speed > 1.5f && location.direction >= 0.0f &&
                             location.direction != (float)SCE_LOCATION_DATA_INVALID;
                if (moving && map_3d && frame > manual_heading_until) {
                    float diff = fmodf(location.direction - cam_heading + 540.0f, 360.0f) - 180.0f;
                    cam_heading = fmodf(cam_heading + diff * 0.08f + 360.0f, 360.0f);
                }

                MapView view = {
                    MAP_X, MAP_Y, MAP_W, MAP_H,
                    location.latitude, location.longitude, location.accuracy, map_zoom,
                    cam_heading, map_3d ? cam_pitch : MAP_PITCH_2D,
                    moving, moving ? location.direction : 0.0f
                };
                if (map_fullscreen) {
                    view.x = 0; view.y = 0; view.w = SCREEN_W; view.h = SCREEN_H;
                }
                map_draw(&view);
                if (!map_fullscreen) {
                    gfx_begin_2d();
                    draw_sats(20, 345);
                }

                if (map_fullscreen) {
                    if (last_ret != 0)
                        snprintf(buf, sizeof(buf), "Signal GPS perdu - derniere position (%u s)",
                                 (unsigned)((now_us - last_fix_us) / 1000000));
                    else
                        snprintf(buf, sizeof(buf), "%.1f km/h   precision %.0f m   fix %d",
                                 location.speed * 3.6f, location.accuracy, fixes);
                    gfx_rect(SCREEN_W / 2 - 170, 6, 340, 26, RGBA8(20, 20, 30, 190));
                    text_draw(SCREEN_W / 2 - 160, 25, RGBA8(255, 255, 255, 255), 0.9f, buf);
                }
            } else {
                char anim[4] = {0};
                int dots = (frame / 20) % 4; // Change tous les tiers de seconde (à 60fps)
                for (int i = 0; i < dots; i++) anim[i] = '.';
                anim[dots] = '\0';
                
                char search_text[128];
                snprintf(search_text, sizeof(search_text), "Recherche des satellites (Cold Start)%s", anim);
                text_draw(20, 50, RGBA8(255, 165, 0, 255), 1.2f, search_text);
                
                char err_buf[128];
                snprintf(err_buf, sizeof(err_buf), "Recherche depuis %d s", (frame - search_start_frame) / 60);
                text_draw(20, 90, RGBA8(200, 200, 200, 255), 1.0f, err_buf);
                if (calls > 0) {
                    snprintf(err_buf, sizeof(err_buf), "%d tentatives : %s", calls, location_status_text(last_ret));
                    text_draw(20, 150, RGBA8(200, 150, 80, 255), 0.8f, err_buf);
                    snprintf(err_buf, sizeof(err_buf), "Code 0x%08X, %u ms par tentative", (unsigned)last_ret, last_call_ms);
                    text_draw(20, 172, RGBA8(150, 150, 150, 255), 0.8f, err_buf);
                }
                
                text_draw(20, 120, RGBA8(150, 150, 150, 255), 1.0f, "Assurez-vous d'etre en exterieur.");
                draw_sats(20, 215);
                
                // Micro animation de radar simple, centrée sur le panneau carte
                int radar_radius = (frame % 60);
                map_draw_placeholder(MAP_X, MAP_Y, MAP_W, MAP_H, "Carte : en attente du premier fix");
                gfx_circle(MAP_X + MAP_W / 2, MAP_Y + MAP_H / 2 + 40, radar_radius,
                           RGBA8(0, 255, 0, 100 - (radar_radius)));
            }
        }
        if (gps_state != 2)
            map_draw_placeholder(MAP_X, MAP_Y, MAP_W, MAP_H, "Carte : en attente du premier fix");
        
        // Aide et version en bas à gauche (masquées en plein écran)
        if (!(gps_state == 2 && map_fullscreen)) {
            gfx_begin_2d();
            text_draw(10, 505, RGBA8(130, 130, 150, 255), 0.75f, "L/R zoom   Stick droit : camera");
            text_draw(10, 522, RGBA8(130, 130, 150, 255), 0.75f, "CARRE 2D/3D   TRIANGLE plein ecran");
            text_draw(10, 539, RGBA8(100, 100, 100, 255), 0.75f, "Go!Explore 2.0 - vue 3D");
        }

        gfx_frame_end();
    }

    // Nettoyage : arrêter le thread avant de fermer le handle qu'il utilise
    if (worker_thid >= 0) stop_location_worker(&worker, worker_thid);
    if (handle_open) sceLocationClose(handle);
    sceSysmoduleUnloadModule(SCE_SYSMODULE_LOCATION);

    sats_stop();
    if (ret_map >= 0) map_fini();
    text_fini();
    gfx_fini();
    
    if (ret_netctl_init >= 0) sceNetCtlTerm();
    if (ret_net_init >= 0) sceNetTerm();
    free(net_param.memory);
    sceAppUtilShutdown();
    sceKernelExitProcess(0);
    return 0;
}
