#include <psp2/types.h>
#include <psp2/location.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/io/fcntl.h>
#include <taihen.h>
#include <stdio.h>

static tai_hook_ref_t ref_hook_load;

static tai_hook_ref_t ref_hook_open;
static tai_hook_ref_t ref_hook_confirm;
static tai_hook_ref_t ref_hook_confirm_status;
static tai_hook_ref_t ref_hook_get_location;

#define FAKE_HANDLE 0x1337

SceInt32 patched_sceLocationOpen(SceLocationHandle *handle, SceLocationLocationMethod locateMethod, SceLocationHeadingMethod headingMethod) {
    *handle = FAKE_HANDLE;
    return 0; 
}

SceInt32 patched_sceLocationConfirm(SceLocationHandle handle) {
    if (handle == FAKE_HANDLE) {
        return 0;
    }
    return 0x80101201; 
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
        location->latitude = 48.858370;
        location->longitude = 2.294481;
        location->altitude = 35.0;
        location->speed = 5.0; 
        location->direction = 90.0;
        return 0; 
    }
    return 0x80101201;
}

static int hooks_installed = 0;

int patched_sceSysmoduleLoadModule(uint16_t id) {
    struct _tai_hook_user *cur = (struct _tai_hook_user *)ref_hook_load;
    struct _tai_hook_user *next = (struct _tai_hook_user *)cur->next;
    typedef int (* func_t)(uint16_t);
    func_t old_func = (func_t)((next == NULL) ? cur->old : next->func);
    
    int res = old_func(id);

    if (id == SCE_SYSMODULE_LOCATION && res >= 0 && !hooks_installed) {
        hooks_installed = 1;
        
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
    }

    return res;
}

int module_start(SceSize argc, const void *args) {
    taiHookFunctionImport(&ref_hook_load, 
                          TAI_MAIN_MODULE,
                          TAI_ANY_LIBRARY,
                          0x79A0160A, 
                          patched_sceSysmoduleLoadModule);
                          
    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
    return SCE_KERNEL_STOP_SUCCESS;
}

int _start(SceSize argc, const void *args) __attribute__ ((weak, alias ("module_start")));
