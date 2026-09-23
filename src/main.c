#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/sysmodule.h>
#include <psp2/location.h>
#include <psp2/ctrl.h>
#include <psp2/apputil.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <vita2d.h>
#include <malloc.h>

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

    int ret_loc = sceSysmoduleLoadModule(SCE_SYSMODULE_LOCATION);
    log_debug("sceSysmoduleLoadModule(LOCATION)", ret_loc);
    log_memory("after LOCATION activation");
    if (ret_loc >= 0) probe_methods();

    vita2d_init();
    vita2d_set_clear_color(RGBA8(20, 20, 30, 255));
    
    vita2d_pgf *font = vita2d_load_default_pgf();
    log_memory("after vita2d and font");

    SceLocationHandle handle = 0;
    int ret_open = -1;
    int last_error = 0;
    const char *error_stage = "open";
    int handle_open = 0;
    int gps_state = 0; // 0 = Attente, 1 = Demande permission, 2 = Actif, -1 = Erreur

    SceLocationLocationInfo location;
    memset(&location, 0, sizeof(location));

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
        prev_buttons = pad.buttons;

        vita2d_start_drawing();
        vita2d_clear_screen();

        if (gps_state == 0) {
            vita2d_pgf_draw_text(font, 20, 50, RGBA8(255, 255, 255, 255), 1.2f, "Go!Explore 2.0 (Vita Port)");
            vita2d_pgf_draw_text(font, 20, 100, RGBA8(200, 200, 200, 255), 1.0f, "Appuyez sur CROIX pour tester la geolocalisation");
            
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
            vita2d_pgf_draw_text(font, 20, 150, RGBA8(255, 255, 0, 255), 1.2f, "Veuillez accepter l'autorisation a l'ecran...");
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
                    gps_state = 2;
                } else {
                    last_error = ret ? ret : (int)SCE_LOCATION_INFO_DENIED_BY_USER;
                    error_stage = "consentement";
                    gps_state = -1;
                }
            }
        } else if (gps_state == -1) {
            char err_buf[128];
            snprintf(err_buf, sizeof(err_buf), "Erreur %s: 0x%08X", error_stage, (unsigned)last_error);
            vita2d_pgf_draw_text(font, 20, 50, RGBA8(255, 50, 50, 255), 1.2f, err_buf);
            const char *detail = "Consultez gps_debug.txt pour le diagnostic.";
            switch ((unsigned)last_error) {
                case 0x80024302: detail = "Memoire physique indisponible : pool a identifier."; break;
                case 0x80101244: detail = "Methode de localisation invalide."; break;
                case 0x8010124F: detail = "Session de localisation non activee."; break;
                case 0x80101249: detail = "Application non autorisee pour la localisation."; break;
                case 0x80101203: detail = "Autorisation de localisation non accordee."; break;
            }
            vita2d_pgf_draw_text(font, 20, 90, RGBA8(200, 200, 200, 255), 1.0f, detail);
            vita2d_pgf_draw_text(font, 20, 140, RGBA8(200, 200, 200, 255), 1.0f, "CROIX : revenir au test. START : quitter.");
            if (cross_pressed) gps_state = 0;
        } else if (gps_state == 2) {
            // 3. Récupérer les coordonnées actuelles
            ret = sceLocationGetLocation(handle, &location);

            // Log toutes les 60 frames (environ 1 sec)
            if (frame % 60 == 0) {
                FILE *log_file = fopen("ux0:/data/gps_log.txt", "a");
                if (log_file) {
                    fprintf(log_file, "[Frame %d] sceLocationGetLocation: 0x%08X (Lat: %f, Lon: %f)\n", frame, ret, location.latitude, location.longitude);
                    fclose(log_file);
                }
            }

            if (ret == 0) {
                char buf[128];
                
                snprintf(buf, sizeof(buf), "GPS Status: FIX OBTENU");
                vita2d_pgf_draw_text(font, 20, 50, RGBA8(0, 255, 0, 255), 1.2f, buf);

                snprintf(buf, sizeof(buf), "Latitude  : %f", location.latitude);
                vita2d_pgf_draw_text(font, 20, 100, RGBA8(255, 255, 255, 255), 1.0f, buf);

                snprintf(buf, sizeof(buf), "Longitude : %f", location.longitude);
                vita2d_pgf_draw_text(font, 20, 130, RGBA8(255, 255, 255, 255), 1.0f, buf);

                snprintf(buf, sizeof(buf), "Altitude  : %.1f m", location.altitude);
                vita2d_pgf_draw_text(font, 20, 160, RGBA8(255, 255, 255, 255), 1.0f, buf);

                snprintf(buf, sizeof(buf), "Vitesse   : %.1f km/h", location.speed * 3.6f);
                vita2d_pgf_draw_text(font, 20, 190, RGBA8(255, 255, 255, 255), 1.0f, buf);
            } else {
                char anim[4] = {0};
                int dots = (frame / 20) % 4; // Change tous les tiers de seconde (à 60fps)
                for (int i = 0; i < dots; i++) anim[i] = '.';
                anim[dots] = '\0';
                
                char search_text[128];
                snprintf(search_text, sizeof(search_text), "Recherche des satellites (Cold Start)%s", anim);
                vita2d_pgf_draw_text(font, 20, 50, RGBA8(255, 165, 0, 255), 1.2f, search_text);
                
                char err_buf[128];
                snprintf(err_buf, sizeof(err_buf), "Statut SceLocation : 0x%08X", ret);
                vita2d_pgf_draw_text(font, 20, 90, RGBA8(200, 200, 200, 255), 1.0f, err_buf);
                
                vita2d_pgf_draw_text(font, 20, 120, RGBA8(150, 150, 150, 255), 1.0f, "Assurez-vous d'etre en exterieur.");
                
                // Micro animation de radar simple
                int radar_radius = (frame % 60);
                vita2d_draw_fill_circle(480, 272, radar_radius, RGBA8(0, 255, 0, 100 - (radar_radius)));
            }
        }
        
        // Affichage de la version en bas à gauche
        char version_buf[64];
        snprintf(version_buf, sizeof(version_buf), "Go!Explore 2.0 - Diagnostic memoire 1");
        vita2d_pgf_draw_text(font, 10, 530, RGBA8(100, 100, 100, 255), 1.0f, version_buf);

        vita2d_end_drawing();
        vita2d_swap_buffers();
    }

    // Nettoyage
    if (handle_open) sceLocationClose(handle);
    sceSysmoduleUnloadModule(SCE_SYSMODULE_LOCATION);
    
    vita2d_free_pgf(font);
    vita2d_fini();
    
    if (ret_netctl_init >= 0) sceNetCtlTerm();
    if (ret_net_init >= 0) sceNetTerm();
    free(net_param.memory);
    sceAppUtilShutdown();
    sceKernelExitProcess(0);
    return 0;
}
