#include <psp2/types.h>
#include <psp2/location.h>
#include <psp2/kernel/modulemgr.h>
#include <taihen.h>

static tai_hook_ref_t ref_hook_open;
static tai_hook_ref_t ref_hook_confirm;
static tai_hook_ref_t ref_hook_confirm_status;
static tai_hook_ref_t ref_hook_get_location;

#define FAKE_HANDLE 0x1337

SceInt32 patched_sceLocationOpen(SceLocationHandle *handle, SceLocationLocationMethod locateMethod, SceLocationHeadingMethod headingMethod) {
    // Force la réussite et assigne notre faux handle
    *handle = FAKE_HANDLE;
    return 0; 
}

SceInt32 patched_sceLocationConfirm(SceLocationHandle handle) {
    if (handle == FAKE_HANDLE) {
        return 0; // Success immediat, pas de popup
    }
    return 0x80101201; // Invalid handle
}

SceInt32 patched_sceLocationConfirmGetStatus(SceLocationHandle handle, SceLocationDialogStatus *status) {
    if (handle == FAKE_HANDLE) {
        *status = SCE_LOCATION_DIALOG_STATUS_FINISHED;
        return 0;
    }
    return 0x80101201;
}

SceInt32 patched_sceLocationGetLocation(SceLocationHandle handle, SceLocationLocationInfo *location) {
    if (handle == FAKE_HANDLE) {
        // Injection de nos fausses coordonnées (Tour Eiffel)
        location->latitude = 48.858370;
        location->longitude = 2.294481;
        location->altitude = 35.0;
        location->speed = 5.0; // En mouvement
        location->direction = 90.0;
        return 0; // Success
    }
    return 0x80101201;
}

int module_start(SceSize argc, const void *args) {
    taiHookFunctionImport(&ref_hook_open, 
                          TAI_MAIN_MODULE,
                          TAI_ANY_LIBRARY,
                          0xDD271661, 
                          patched_sceLocationOpen);

    taiHookFunctionImport(&ref_hook_confirm,
                          TAI_MAIN_MODULE,
                          TAI_ANY_LIBRARY,
                          0xC895E567, 
                          patched_sceLocationConfirm);

    taiHookFunctionImport(&ref_hook_confirm_status,
                          TAI_MAIN_MODULE,
                          TAI_ANY_LIBRARY,
                          0x730FF842, 
                          patched_sceLocationConfirmGetStatus);

    taiHookFunctionImport(&ref_hook_get_location,
                          TAI_MAIN_MODULE,
                          TAI_ANY_LIBRARY,
                          0x15BC27C8, 
                          patched_sceLocationGetLocation);
                          
    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
    return SCE_KERNEL_STOP_SUCCESS;
}

int _start(SceSize argc, const void *args) __attribute__ ((weak, alias ("module_start")));
