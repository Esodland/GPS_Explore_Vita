#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <psp2/kernel/processmgr.h>
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

int main(int argc, char *argv[]) {
    // Clear log at startup
    remove("ux0:/data/gps_debug.txt");
    FILE* f = fopen("ux0:/data/gps_debug.txt", "w");
    if(f) { fprintf(f, "--- NEW RUN START ---\n"); fclose(f); }

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

    // Initialiser le réseau
    int ret_net = sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
    log_debug("sceSysmoduleLoadModule(NET)", ret_net);
    
    SceNetInitParam net_param;
    memset(&net_param, 0, sizeof(net_param));
    net_param.memory = malloc(1024 * 1024);
    net_param.size = 1024 * 1024;
    net_param.flags = 0;
    int ret_net_init = sceNetInit(&net_param);
    log_debug("sceNetInit", ret_net_init);

    // Initialiser NetCtl
    int ret_netctl_init = sceNetCtlInit();
    log_debug("sceNetCtlInit", ret_netctl_init);

    int ret_loc = sceSysmoduleLoadModule(SCE_SYSMODULE_LOCATION);
    log_debug("sceSysmoduleLoadModule(LOCATION)", ret_loc);

    vita2d_init();
    vita2d_set_clear_color(RGBA8(20, 20, 30, 255));
    
    vita2d_pgf *font = vita2d_load_default_pgf();

    SceLocationHandle handle = 0;
    int ret_open = -1;
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
                
                int ret_open = -1;
                SceLocationHandle best_handle = 0;

                for (int i = 0; i <= 10; i++) {
                    for (int j = 0; j <= 2; j++) {
                        handle = 0;
                        ret_open = sceLocationOpen(&handle, i, j);
                        log_debug("sceLocationOpen(method, hmethod)", ret_open);
                        
                        if (handle != 0 && handle != 0xFFFFFFFF) {
                            best_handle = handle;
                        }
                        
                        if (ret_open >= 0) break;
                    }
                    if (ret_open >= 0) break;
                }
                
                if (best_handle != 0 && best_handle != 0xFFFFFFFF) {
                    handle = best_handle;
                    log_debug("Calling Confirm with handle", handle);
                    ret = sceLocationConfirm(handle);
                    log_debug("sceLocationConfirm", ret);
                    gps_state = 1;
                } else {
                    log_debug("No valid handle returned. Forcing Confirm(1)...", 0);
                    handle = 1;
                    ret = sceLocationConfirm(handle);
                    log_debug("sceLocationConfirm(1)", ret);
                    if (ret >= 0 || ret == 0x80101280 || ret == 0x8010124F) {
                        gps_state = 1;
                    } else {
                        gps_state = -1;
                        log_debug("All Location methods failed", ret_open);
                    }
                }
            }
        } else if (gps_state == 1) {
            SceLocationDialogStatus status = SCE_LOCATION_DIALOG_STATUS_IDLE;
            sceLocationConfirmGetStatus(handle, &status);
            
            vita2d_pgf_draw_text(font, 20, 150, RGBA8(255, 255, 0, 255), 1.2f, "Veuillez accepter l'autorisation a l'ecran...");
            
            if (status == SCE_LOCATION_DIALOG_STATUS_FINISHED) {
                gps_state = 2; // On passe en actif !
            }
        } else if (gps_state == -1) {
            char err_buf[128];
            snprintf(err_buf, sizeof(err_buf), "Erreur sceLocationOpen: 0x%08X", ret_open);
            vita2d_pgf_draw_text(font, 20, 50, RGBA8(255, 50, 50, 255), 1.2f, err_buf);
            
            if (ret_open == 0x80101244) {
                vita2d_pgf_draw_text(font, 20, 90, RGBA8(255, 150, 50, 255), 1.0f, "Code 1244 : Le module GPS materiel est indisponible.");
                vita2d_pgf_draw_text(font, 20, 120, RGBA8(200, 200, 200, 255), 1.0f, "Assurez-vous qu'une carte SIM valide est inseree.");
            } else if (ret_open == 0x8010124F) {
                vita2d_pgf_draw_text(font, 20, 90, RGBA8(255, 150, 50, 255), 1.0f, "Code 124F : Le service de localisation (Skyhook) est hors-ligne.");
                vita2d_pgf_draw_text(font, 20, 120, RGBA8(200, 200, 200, 255), 1.0f, "Sony a desactive ces serveurs. Le Wi-Fi Location est mort.");
            } else {
                vita2d_pgf_draw_text(font, 20, 90, RGBA8(200, 200, 200, 255), 1.0f, "Verifiez vos parametres de confidentialite Location.");
            }
            
            vita2d_pgf_draw_text(font, 20, 160, RGBA8(100, 100, 100, 255), 0.9f, "Meme l'application Photos officielle ne peut plus");
            vita2d_pgf_draw_text(font, 20, 185, RGBA8(100, 100, 100, 255), 0.9f, "acceder au GPS sur cette console aujourd'hui.");
        } else if (gps_state == 2) {
            // 3. Récupérer les coordonnées actuelles
            int ret = sceLocationGetLocation(handle, &location);

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
        snprintf(version_buf, sizeof(version_buf), "Go!Explore 2.0 - Final Build");
        vita2d_pgf_draw_text(font, 10, 530, RGBA8(100, 100, 100, 255), 1.0f, version_buf);

        vita2d_end_drawing();
        vita2d_swap_buffers();
    }

    // Nettoyage
    sceLocationClose(handle);
    sceSysmoduleUnloadModule(SCE_SYSMODULE_LOCATION);
    
    vita2d_free_pgf(font);
    vita2d_fini();
    
    sceKernelExitProcess(0);
    return 0;
}
