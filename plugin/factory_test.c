// GPS_Explore — Test piste "factory" (Phase 3, option B)
//
// Objectif : déterminer si le serveur IPMI "SceLocationLoc" accepte une connexion
// depuis notre processus homebrew, via le module liblocation_factory.
//
// L'export SceLibLocationFactory_9E020010(a1,a2,a3) : a1 = commande (valides {4,5,13,14,16,19}),
// toute commande valide déclenche sub_8100007A -> SceLibKernel_C458D60A("SceLocationLoc")
// -> SceIpmi_4E255C31 (création/connexion du client IPMI).
//   - retour 0x80101242 => échec de connexion (ACL refuse notre processus) = même mur.
//   - autre chose        => la connexion passe (voie factory exploitable).
//
// On hooke aussi l'appel SceIpmi_4E255C31 fait par le module factory pour capturer
// le code de retour EXACT de la connexion, indépendamment de ce qui suit.
//
// Log : ux0:data/gps_factory.txt
// Freestanding (pas de libc/newlib) : sceIo* + mini-formateur.

#include <psp2/types.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/io/fcntl.h>
#include <taihen.h>

static tai_hook_ref_t ref_load;
static tai_hook_ref_t ref_ipmi;
static tai_hook_ref_t ref_invoke;
static int done = 0;

#define FACNID_LIB   0x6f397272u   // SceLibLocationFactory (library nid)
#define FACNID_FUNC  0x9E020010u   // export à appeler
#define IPMI_LIB     0xf4e34edbu   // SceIpmi
#define IPMI_CREATE  0x4E255C31u   // SceIpmi_4E255C31 (create/connect client)
#define IPMI_INVOKE  0xB282B430u   // SceIpmi_B282B430 (invoke sync method)

static unsigned char buf_a2[512];
static unsigned char buf_a3[512];

// ---- helpers freestanding ---------------------------------------------------
static unsigned slen(const char *s){ unsigned n=0; while(s[n]) n++; return n; }
static void wr(const char *s, unsigned n){
    SceUID fd = sceIoOpen("ux0:data/gps_factory.txt",
                          SCE_O_WRONLY|SCE_O_CREAT|SCE_O_APPEND, 0777);
    if(fd>=0){ sceIoWrite(fd,s,n); sceIoClose(fd); }
}
static void ps(const char *s){ wr(s, slen(s)); }
static void phex(uint32_t v){
    char b[11]; b[0]='0'; b[1]='x'; const char *H="0123456789ABCDEF";
    for(int i=0;i<8;i++) b[2+i]=H[(v>>((7-i)*4))&0xF]; b[10]=0; wr(b,10);
}
static void pbytes(const unsigned char *p, int n){
    const char *H="0123456789ABCDEF";
    for(int i=0;i<n;i++){ char c[3]; c[0]=H[(p[i]>>4)&0xF]; c[1]=H[p[i]&0xF]; c[2]=' '; wr(c,3); }
    ps("\n");
}
static int has(const char *hay, const char *needle){
    for(;*hay;hay++){ const char*h=hay,*n=needle; while(*n&&*h==*n){h++;n++;} if(!*n) return 1; }
    return 0;
}

// ---- hook sur la création du client IPMI par le module factory ---------------
static int patched_ipmi_create(void *arg1){
    int r = TAI_CONTINUE(int, ref_ipmi, arg1);
    ps("[FAC] SceIpmi_4E255C31 (connexion SceLocationLoc) -> "); phex((uint32_t)r); ps("\n");
    return r;
}

static int patched_ipmi_invoke(int a1, int a2, int a3, int a4){
    int r = TAI_CONTINUE(int, ref_invoke, a1, a2, a3, a4);
    ps("[FAC] B282B430(a1="); phex((uint32_t)a1);
    ps(" a2="); phex((uint32_t)a2);
    ps(" a3="); phex((uint32_t)a3);
    ps(" a4="); phex((uint32_t)a4);
    ps(") -> "); phex((uint32_t)r); ps("\n");
    return r;
}

// ---- résolution de l'export via la table d'exports (taiGetModuleInfo) --------
typedef struct {
    uint16_t size; uint16_t version; uint16_t flags; uint16_t num_funcs;
    uint32_t num_vars; uint32_t num_unk; uint32_t lib_nid;
    const char *lib_name; const uint32_t *nid_table; void * const *entry_table;
} sce_exports_t;

static void *resolve_export(uintptr_t start, uintptr_t end, uint32_t lib, uint32_t fnid){
    uintptr_t p = start;
    while(p + sizeof(sce_exports_t) <= end){
        const sce_exports_t *e = (const sce_exports_t *)p;
        unsigned sz = e->size ? e->size : 0x20;
        if(e->lib_nid == lib && e->nid_table && e->entry_table){
            for(int i=0;i<e->num_funcs;i++){
                if(e->nid_table[i] == fnid) return (void *)e->entry_table[i];
            }
        }
        p += sz;
    }
    return 0;
}

// ---- test -------------------------------------------------------------------
static void run_factory_test(void){
    ps("--- FACTORY TEST START ---\n");

    // 1) charger le module factory dans notre processus
    SceUID mid = sceKernelLoadStartModule("vs0:sys/external/liblocation_factory.suprx",
                                          0, (void*)0, 0, (SceKernelLMOption*)0, (int*)0);
    ps("[FAC] LoadStartModule(liblocation_factory) -> "); phex((uint32_t)mid); ps("\n");

    // 2) infos module + exports
    tai_module_info_t ti; ti.size = sizeof(ti);
    int r = taiGetModuleInfo("SceLibLocationFactory", &ti);
    ps("[FAC] taiGetModuleInfo(SceLibLocationFactory) -> "); phex((uint32_t)r); ps("\n");
    if(r < 0){
        // découvrir le vrai nom : lister les modules
        SceUID uids[192]; SceSize num = 192;
        if(sceKernelGetModuleList(0xFF, uids, &num) >= 0){
            for(SceSize i=0;i<num;i++){
                SceKernelModuleInfo mi; for(unsigned k=0;k<sizeof mi;k++)((char*)&mi)[k]=0; mi.size=sizeof mi;
                if(sceKernelGetModuleInfo(uids[i],&mi)<0) continue;
                if(has(mi.module_name,"actor")||has(mi.module_name,"ocation")){
                    ps("[FAC] module present: '"); ps(mi.module_name); ps("'\n");
                }
            }
        }
        ps("[FAC] module factory introuvable via taiGetModuleInfo — abandon appel export.\n");
        ps("[FAC] --- fin ---\n");
        return;
    }

    // 3) hooks IPMI (create + invoke) appelés PAR le module factory
    SceUID h1 = taiHookFunctionImport(&ref_ipmi, "SceLibLocationFactory",
                                      IPMI_LIB, IPMI_CREATE, patched_ipmi_create);
    SceUID h2 = taiHookFunctionImport(&ref_invoke, "SceLibLocationFactory",
                                      IPMI_LIB, IPMI_INVOKE, patched_ipmi_invoke);
    ps("[FAC] hook create -> "); phex((uint32_t)h1);
    ps("  hook invoke -> "); phex((uint32_t)h2); ps("\n");

    // 4) résoudre l'export 9E020010
    void *fn = resolve_export(ti.exports_start, ti.exports_end, FACNID_LIB, FACNID_FUNC);
    ps("[FAC] export 9E020010 @ "); phex((uint32_t)(uintptr_t)fn); ps("\n");
    if(!fn){ ps("[FAC] export non résolu — abandon.\n"); ps("[FAC] --- fin ---\n"); return; }

    typedef int (*fac_t)(int, void*, void*);
    fac_t call = (fac_t)fn;

    // 5) tester toutes les commandes valides {4,5,13,14,16,19}
    static const int cmds[] = {4,5,13,14,16,19};
    for(unsigned c=0;c<sizeof(cmds)/sizeof(cmds[0]);c++){
        int cmd = cmds[c];
        for(int i=0;i<512;i++){ buf_a2[i]=0; buf_a3[i]=0; }
        ps("\n[FAC] === appel 9E020010(cmd="); phex((uint32_t)cmd); ps(", buf, buf) ===\n");
        int ret = call(cmd, buf_a2, buf_a3);
        ps("[FAC] retour export = "); phex((uint32_t)ret); ps("\n");
        ps("[FAC] buf_a2[0..63] = "); pbytes(buf_a2, 64);
        ps("[FAC] buf_a3[0..63] = "); pbytes(buf_a3, 64);
    }
    ps("[FAC] --- fin ---\n");
}

static int patched_load(uint16_t id){
    struct _tai_hook_user *cur=(struct _tai_hook_user*)ref_load;
    struct _tai_hook_user *next=(struct _tai_hook_user*)cur->next;
    typedef int(*func_t)(uint16_t);
    func_t old_func=(func_t)((next==0)?cur->old:next->func);
    int res=old_func(id);
    if(id==SCE_SYSMODULE_LOCATION && res>=0 && !done){
        done=1;
        run_factory_test();
    }
    return res;
}

int module_start(SceSize argc, const void *args){
    (void)argc;(void)args;
    taiHookFunctionImport(&ref_load, TAI_MAIN_MODULE, TAI_ANY_LIBRARY,
                          0x79A0160A /* sceSysmoduleLoadModule */, patched_load);
    return SCE_KERNEL_START_SUCCESS;
}
int module_stop(SceSize argc, const void *args){ (void)argc;(void)args; return SCE_KERNEL_STOP_SUCCESS; }
int _start(SceSize argc, const void *args) __attribute__((weak, alias("module_start")));
