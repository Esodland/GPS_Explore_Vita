/*
 * gps_assist.skprx — module noyau chargé à la demande par GPSX00001 (jamais sous *KERNEL).
 *
 * SceGps n'accepte _sceGpsIoctl que d'un programme système
 * (ksceSblACMgrIsSystemProgram, gps.elf 3.60 0x81003046). Ce module accroche
 * cet import dans SceGps et répond « système » uniquement pour le processus
 * dont le pid est passé au chargement. Les autres contrôles de SceGps
 * (machine d'état, type de commande) restent actifs.
 *
 * Aucune adresse ni NID ForKernel : seulement SceSblACMgrForDriver et
 * SceThreadmgrForDriver, stables entre 3.60 et 3.65.
 */
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/threadmgr.h>
#include <taihen.h>

#define SCE_SBL_ACMGR_FOR_DRIVER_NID 0x9AD8E213
#define IS_SYSTEM_PROGRAM_NID        0x8612B243

static tai_hook_ref_t is_system_ref;
static SceUID is_system_hook = -1;
static SceUID allowed_pid = -1;

static int is_system_program_hook(SceUID pid) {
    int ret = TAI_CONTINUE(int, is_system_ref, pid);
    /* SceGps passe 0 = processus courant. */
    if (ret == 0 && pid == 0 && allowed_pid > 0 && ksceKernelGetProcessId() == allowed_pid)
        return 1;
    return ret;
}

int _start(SceSize args, void *argp) __attribute__((weak, alias("module_start")));
int module_start(SceSize args, void *argp) {
    if (args < sizeof(SceUID) || !argp) return SCE_KERNEL_START_FAILED;
    allowed_pid = *(const SceUID *)argp;
    is_system_hook = taiHookFunctionImportForKernel(KERNEL_PID, &is_system_ref, "SceGps",
                                                    SCE_SBL_ACMGR_FOR_DRIVER_NID,
                                                    IS_SYSTEM_PROGRAM_NID,
                                                    is_system_program_hook);
    return is_system_hook < 0 ? SCE_KERNEL_START_FAILED : SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp) {
    (void)args;
    (void)argp;
    if (is_system_hook >= 0) taiHookReleaseForKernel(is_system_hook, is_system_ref);
    is_system_hook = -1;
    allowed_pid = -1;
    return SCE_KERNEL_STOP_SUCCESS;
}
