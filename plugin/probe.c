// GPS_Explore — Probe (Phase 2, diagnostic ONLY, no injection, no crash risk)
//
// But: confirmer sur le VRAI materiel (PCH-1100 3G) l'analyse statique du RE :
//   - liblocation (module "SceLibLocation") : base runtime du segment 1 (donnees)
//   - global de privilege a seg1+0x30 (VADDR statique 0x81004030) : attendu 0
//   - pointeur de contexte a seg1+0x00 (VADDR statique 0x81004000)  : attendu NULL
//
// Se declenche quand l'appli charge le module LOCATION (au demarrage, main.c:57).
// N'INJECTE RIEN, ne hooke pas sceLocationOpen : lit et logge seulement.
// Freestanding : aucune dependance libc/newlib (pas de snprintf), uniquement sceIo*.
// Log : ux0:data/gps_probe.txt

#include <psp2/types.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/io/fcntl.h>
#include <taihen.h>

static tai_hook_ref_t ref_load;
static int done = 0;

// ---- helpers freestanding ---------------------------------------------------
static unsigned slen(const char *s) { unsigned n = 0; while (s[n]) n++; return n; }

static void zero(void *p, unsigned n) {
    unsigned char *b = (unsigned char *)p;
    for (unsigned i = 0; i < n; i++) b[i] = 0;
}

static int streq(const char *a, const char *b) {
    while (*a && (*a == *b)) { a++; b++; }
    return (*(const unsigned char *)a == *(const unsigned char *)b);
}

static int has(const char *hay, const char *needle) {
    for (; *hay; hay++) {
        const char *h = hay, *n = needle;
        while (*n && *h == *n) { h++; n++; }
        if (!*n) return 1;
    }
    return 0;
}

static void wr(const char *s, unsigned n) {
    SceUID fd = sceIoOpen("ux0:data/gps_probe.txt",
                          SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 0777);
    if (fd >= 0) { sceIoWrite(fd, s, n); sceIoClose(fd); }
}
static void ps(const char *s) { wr(s, slen(s)); }

static void phex(uint32_t v) {
    char b[11];
    b[0] = '0'; b[1] = 'x';
    const char *H = "0123456789ABCDEF";
    for (int i = 0; i < 8; i++) b[2 + i] = H[(v >> ((7 - i) * 4)) & 0xF];
    b[10] = 0;
    wr(b, 10);
}

static void pint(int v) {
    char b[12]; int i = 12; unsigned u = (v < 0) ? (unsigned)(-v) : (unsigned)v;
    if (u == 0) { ps("0"); return; }
    while (u) { b[--i] = '0' + (u % 10); u /= 10; }
    if (v < 0) b[--i] = '-';
    wr(b + i, 12 - i);
}

// ---- probe ------------------------------------------------------------------
static void probe_location(void) {
    SceUID uids[192];
    SceSize num = sizeof(uids) / sizeof(uids[0]);
    int rl = sceKernelGetModuleList(0xFF, uids, &num);
    ps("[PROBE] getModuleList="); phex((uint32_t)rl);
    ps(" num="); pint((int)num); ps("\n");

    SceUID loc_modid = -1;
    if (rl >= 0) {
        for (SceSize i = 0; i < num; i++) {
            SceKernelModuleInfo mi; zero(&mi, sizeof mi); mi.size = sizeof(mi);
            if (sceKernelGetModuleInfo(uids[i], &mi) < 0) continue;
            if (has(mi.module_name, "ocation") || has(mi.module_name, "Loc")) {
                ps("[PROBE] module uid="); phex((uint32_t)uids[i]);
                ps(" name='"); ps(mi.module_name); ps("'\n");
                if (streq(mi.module_name, "SceLibLocation")) loc_modid = uids[i];
                if (loc_modid < 0 && has(mi.module_name, "ocation")
                    && !has(mi.module_name, "Internal")
                    && !has(mi.module_name, "Factory")
                    && !has(mi.module_name, "Extension"))
                    loc_modid = uids[i];
            }
        }
    }

    if (loc_modid < 0) {
        tai_module_info_t ti; ti.size = sizeof(ti);
        int r = taiGetModuleInfo("SceLibLocation", &ti);
        ps("[PROBE] taiGetModuleInfo(SceLibLocation)="); phex((uint32_t)r); ps("\n");
        if (r >= 0) loc_modid = ti.modid;
    }
    if (loc_modid < 0) { ps("[PROBE] module location introuvable — abandon.\n"); return; }

    SceKernelModuleInfo mi; zero(&mi, sizeof mi); mi.size = sizeof(mi);
    int r2 = sceKernelGetModuleInfo(loc_modid, &mi);
    ps("[PROBE] cible uid="); phex((uint32_t)loc_modid);
    ps(" name='"); ps((r2 >= 0) ? mi.module_name : "?"); ps("' getInfo="); phex((uint32_t)r2); ps("\n");
    if (r2 < 0) return;

    for (int s = 0; s < 4; s++) {
        ps("[PROBE] seg["); pint(s); ps("] vaddr=");
        phex((uint32_t)(uintptr_t)mi.segments[s].vaddr);
        ps(" memsz="); phex((uint32_t)mi.segments[s].memsz);
        ps(" filesz="); phex((uint32_t)mi.segments[s].filesz); ps("\n");
    }

    unsigned char *seg1 = (unsigned char *)mi.segments[1].vaddr;
    if (seg1 && mi.segments[1].memsz >= 0x34) {
        uint32_t ctx  = *(volatile uint32_t *)(seg1 + 0x00);
        uint32_t priv = *(volatile uint32_t *)(seg1 + 0x30);
        ps("[PROBE] >>> context(seg1+0x00)="); phex(ctx);
        ps("  priv_global(seg1+0x30)="); phex(priv); ps("\n");
        ps(priv == 0
           ? "[PROBE] priv==0 -> confirme : init privilegie PAS execute (blocage 0x8010124F attendu)\n"
           : "[PROBE] priv!=0 -> init a tourne (inattendu pour un homebrew)\n");
        ps(ctx == 0
           ? "[PROBE] context==NULL -> injecter priv=2 seul crasherait ; init IPMI cote noyau requis\n"
           : "[PROBE] context!=NULL -> contexte alloue, injection priv=2 peut suffire\n");
    } else {
        ps("[PROBE] segment 1 invalide ou trop petit\n");
    }
    ps("[PROBE] --- fin ---\n");
}

static int patched_load(uint16_t id) {
    struct _tai_hook_user *cur = (struct _tai_hook_user *)ref_load;
    struct _tai_hook_user *next = (struct _tai_hook_user *)cur->next;
    typedef int (*func_t)(uint16_t);
    func_t old_func = (func_t)((next == NULL) ? cur->old : next->func);

    int res = old_func(id);

    if (id == SCE_SYSMODULE_LOCATION && res >= 0 && !done) {
        done = 1;
        ps("--- PROBE RUN START ---\n");
        probe_location();
    }
    return res;
}

int module_start(SceSize argc, const void *args) {
    (void)argc; (void)args;
    taiHookFunctionImport(&ref_load, TAI_MAIN_MODULE, TAI_ANY_LIBRARY,
                          0x79A0160A /* sceSysmoduleLoadModule */, patched_load);
    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
    (void)argc; (void)args;
    return SCE_KERNEL_STOP_SUCCESS;
}

int _start(SceSize argc, const void *args) __attribute__((weak, alias("module_start")));
