// GPS_Explore — Test init direct (Phase 3, suite de l'option B)
//
// Le test factory a prouvé que la connexion IPMI a "SceLocationLoc" est AUTORISEE
// pour notre homebrew. Le chemin normal echoue seulement parce que personne
// n'appelle l'init privilegie SceLibLocationInternal_3500A98C(a1<=8, 0x10000),
// qui : alloue le contexte, se connecte au serveur, et met le global (seg1+0x30)=2.
//
// Ici on APPELLE nous-memes cet init, puis on relit le global/contexte de
// liblocation. S'ils passent a 2 / non-NULL, l'init a reussi -> l'API normale
// (sceLocationOpen/GetLocation) devrait alors fonctionner.
//
// Log : ux0:data/gps_init.txt
// Freestanding.

#include <psp2/types.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/io/fcntl.h>
#include <taihen.h>

static tai_hook_ref_t ref_load;
static int done = 0;

#define INT_LIB   0x098b0c75u   // SceLibLocationInternal (library nid)
#define INT_INIT  0x3500A98Cu   // SceLibLocationInternal_3500A98C

// ---- helpers freestanding ---------------------------------------------------
static unsigned slen(const char *s){ unsigned n=0; while(s[n]) n++; return n; }
static void zero(void*p,unsigned n){ unsigned char*b=p; for(unsigned i=0;i<n;i++) b[i]=0; }
static int streq(const char*a,const char*b){ while(*a&&*a==*b){a++;b++;} return *(const unsigned char*)a==*(const unsigned char*)b; }
static int has(const char*h,const char*n){ for(;*h;h++){const char*x=h,*y=n; while(*y&&*x==*y){x++;y++;} if(!*y)return 1;} return 0; }
static void wr(const char*s,unsigned n){ SceUID fd=sceIoOpen("ux0:data/gps_init.txt",SCE_O_WRONLY|SCE_O_CREAT|SCE_O_APPEND,0777); if(fd>=0){sceIoWrite(fd,s,n);sceIoClose(fd);} }
static void ps(const char*s){ wr(s,slen(s)); }
static void phex(uint32_t v){ char b[11];b[0]='0';b[1]='x';const char*H="0123456789ABCDEF"; for(int i=0;i<8;i++)b[2+i]=H[(v>>((7-i)*4))&0xF];b[10]=0; wr(b,10); }

// ---- export table walk ------------------------------------------------------
typedef struct { uint16_t size,version,flags,num_funcs; uint32_t num_vars,num_unk,lib_nid;
                 const char*lib_name; const uint32_t*nid_table; void*const*entry_table; } sce_exports_t;
static void* resolve_export(uintptr_t start,uintptr_t end,uint32_t lib,uint32_t fnid){
    for(uintptr_t p=start; p+sizeof(sce_exports_t)<=end; ){
        const sce_exports_t*e=(const sce_exports_t*)p; unsigned sz=e->size?e->size:0x20;
        if(e->lib_nid==lib && e->nid_table && e->entry_table)
            for(int i=0;i<e->num_funcs;i++) if(e->nid_table[i]==fnid) return (void*)e->entry_table[i];
        p+=sz;
    }
    return 0;
}

// ---- trouver le segment 1 de liblocation et lire global/contexte ------------
static unsigned char* find_liblocation_seg1(void){
    SceUID uids[192]; SceSize num=192; SceUID mod=-1;
    if(sceKernelGetModuleList(0xFF,uids,&num)>=0)
        for(SceSize i=0;i<num;i++){
            SceKernelModuleInfo mi; zero(&mi,sizeof mi); mi.size=sizeof mi;
            if(sceKernelGetModuleInfo(uids[i],&mi)<0) continue;
            if(streq(mi.module_name,"SceLibLocation")){ mod=uids[i]; break; }
        }
    if(mod<0) return 0;
    SceKernelModuleInfo mi; zero(&mi,sizeof mi); mi.size=sizeof mi;
    if(sceKernelGetModuleInfo(mod,&mi)<0) return 0;
    return (unsigned char*)mi.segments[1].vaddr;
}
static void dump_state(const char*tag){
    unsigned char*s1=find_liblocation_seg1();
    if(!s1){ ps("[INIT] seg1 liblocation introuvable\n"); return; }
    uint32_t ctx=*(volatile uint32_t*)(s1+0x00), pr=*(volatile uint32_t*)(s1+0x30);
    ps("[INIT] "); ps(tag); ps(" context="); phex(ctx); ps(" priv="); phex(pr); ps("\n");
}

static void run_init_test(void){
    ps("--- INIT TEST START ---\n");
    dump_state("AVANT :");

    // charger liblocation_internal si besoin (normalement deja charge avec LOCATION)
    tai_module_info_t ti; ti.size=sizeof(ti);
    int r=taiGetModuleInfo("SceLibLocationInternal",&ti);
    ps("[INIT] taiGetModuleInfo(SceLibLocationInternal)="); phex((uint32_t)r); ps("\n");
    if(r<0){
        SceUID mid=sceKernelLoadStartModule("vs0:sys/external/liblocation_internal.suprx",0,(void*)0,0,(SceKernelLMOption*)0,(int*)0);
        ps("[INIT] LoadStart(internal)="); phex((uint32_t)mid); ps("\n");
        r=taiGetModuleInfo("SceLibLocationInternal",&ti);
        ps("[INIT] retry getModuleInfo="); phex((uint32_t)r); ps("\n");
        if(r<0){ ps("[INIT] internal introuvable — abandon.\n"); ps("[INIT] --- fin ---\n"); return; }
    }

    void*fn=resolve_export(ti.exports_start,ti.exports_end,INT_LIB,INT_INIT);
    ps("[INIT] export 3500A98C @ "); phex((uint32_t)(uintptr_t)fn); ps("\n");
    if(!fn){ ps("[INIT] export non resolu — abandon.\n"); ps("[INIT] --- fin ---\n"); return; }

    typedef int(*init_t)(int,int);
    init_t call=(init_t)fn;

    // essayer a1 = 1, puis 0, 2 tant que le global reste 0
    static const int modes[]={1,0,2};
    for(unsigned m=0;m<sizeof(modes)/sizeof(modes[0]);m++){
        ps("\n[INIT] appel 3500A98C(a1="); phex((uint32_t)modes[m]); ps(", 0x10000) ...\n");
        int ret=call(modes[m],0x10000);
        ps("[INIT] retour = "); phex((uint32_t)ret); ps("\n");
        dump_state("APRES :");
        unsigned char*s1=find_liblocation_seg1();
        if(s1 && *(volatile uint32_t*)(s1+0x30)!=0){ ps("[INIT] >>> global != 0 : INIT A PRIS EFFET !\n"); break; }
    }
    ps("[INIT] --- fin ---\n");
}

static int patched_load(uint16_t id){
    struct _tai_hook_user*cur=(struct _tai_hook_user*)ref_load;
    struct _tai_hook_user*next=(struct _tai_hook_user*)cur->next;
    typedef int(*func_t)(uint16_t);
    func_t old_func=(func_t)((next==0)?cur->old:next->func);
    int res=old_func(id);
    if(id==SCE_SYSMODULE_LOCATION && res>=0 && !done){ done=1; run_init_test(); }
    return res;
}
int module_start(SceSize argc,const void*args){ (void)argc;(void)args;
    taiHookFunctionImport(&ref_load,TAI_MAIN_MODULE,TAI_ANY_LIBRARY,0x79A0160A,patched_load);
    return SCE_KERNEL_START_SUCCESS; }
int module_stop(SceSize argc,const void*args){ (void)argc;(void)args; return SCE_KERNEL_STOP_SUCCESS; }
int _start(SceSize argc,const void*args) __attribute__((weak,alias("module_start")));
