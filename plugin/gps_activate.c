// GPS_Explore — Plugin d'ACTIVATION (Phase 3, piste 1, version propre)
//
// Au chargement du module LOCATION (avant que l'appli appelle sceLocationOpen) :
//   1. écrit les flags d'activation dans les données de liblocation :
//        seg1+0x30 (u32) = 2   (privilège)
//        seg1+0x34 (u8)  = 1   (flag testé par SceLibLocation_2311B24A)
//   2. appelle l'init SceLibLocationInternal_3500A98C(1, 0x10000) -> crée le contexte/session.
//
// Ensuite l'APPLI exécute son flux normal open -> sceLocationConfirm (dialogue) -> getLocation
// dans son propre contexte runtime (UI prête). L'app.db doit avoir la clé 2840145610 pour
// GPSX00001 (autorisation localisation).
//
// NE PAS charger en même temps que gps_spoofer (qui hooke open). Log : ux0:data/gps_activate.txt

#include <psp2/types.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/io/fcntl.h>
#include <taihen.h>

static tai_hook_ref_t ref_load;
static int done = 0;

#define INT_LIB   0x098b0c75u
#define INT_INIT  0x3500A98Cu

static unsigned slen(const char*s){unsigned n=0;while(s[n])n++;return n;}
static void zero(void*p,unsigned n){unsigned char*b=p;for(unsigned i=0;i<n;i++)b[i]=0;}
static int streq(const char*a,const char*b){while(*a&&*a==*b){a++;b++;}return *(const unsigned char*)a==*(const unsigned char*)b;}
static void wr(const char*s,unsigned n){SceUID fd=sceIoOpen("ux0:data/gps_activate.txt",SCE_O_WRONLY|SCE_O_CREAT|SCE_O_APPEND,0777);if(fd>=0){sceIoWrite(fd,s,n);sceIoClose(fd);}}
static void ps(const char*s){wr(s,slen(s));}
static void phex(uint32_t v){char b[11];b[0]='0';b[1]='x';const char*H="0123456789ABCDEF";for(int i=0;i<8;i++)b[2+i]=H[(v>>((7-i)*4))&0xF];b[10]=0;wr(b,10);}

typedef struct { uint16_t size,version,flags,num_funcs; uint32_t num_vars,num_unk,lib_nid;
                 const char*lib_name; const uint32_t*nid_table; void*const*entry_table; } sce_exports_t;
static void* resolve_export(uintptr_t start,uintptr_t end,uint32_t lib,uint32_t fnid){
    for(uintptr_t p=start;p+sizeof(sce_exports_t)<=end;){const sce_exports_t*e=(const sce_exports_t*)p;unsigned sz=e->size?e->size:0x20;
        if(e->lib_nid==lib&&e->nid_table&&e->entry_table)for(int i=0;i<e->num_funcs;i++)if(e->nid_table[i]==fnid)return (void*)e->entry_table[i];
        p+=sz;}
    return 0;
}
static unsigned char* liblocation_seg1(void){
    SceUID uids[192];SceSize num=192;SceUID mod=-1;
    if(sceKernelGetModuleList(0xFF,uids,&num)>=0)for(SceSize i=0;i<num;i++){
        SceKernelModuleInfo mi;zero(&mi,sizeof mi);mi.size=sizeof mi;
        if(sceKernelGetModuleInfo(uids[i],&mi)<0)continue;
        if(streq(mi.module_name,"SceLibLocation")){mod=uids[i];break;}}
    if(mod<0)return 0;
    SceKernelModuleInfo mi;zero(&mi,sizeof mi);mi.size=sizeof mi;
    if(sceKernelGetModuleInfo(mod,&mi)<0)return 0;
    return (unsigned char*)mi.segments[1].vaddr;
}

static void activate(void){
    ps("--- ACTIVATE ---\n");
    unsigned char*s1=liblocation_seg1();
    if(!s1){ps("[ACT] seg1 introuvable\n");return;}
    *(volatile uint32_t*)(s1+0x30)=2;
    *(volatile uint8_t*)(s1+0x34)=1;
    ps("[ACT] flags: priv=");phex(*(volatile uint32_t*)(s1+0x30));
    ps(" flag34=");phex(*(volatile uint8_t*)(s1+0x34));ps("\n");

    tai_module_info_t ti;ti.size=sizeof(ti);
    if(taiGetModuleInfo("SceLibLocationInternal",&ti)<0){
        sceKernelLoadStartModule("vs0:sys/external/liblocation_internal.suprx",0,(void*)0,0,(SceKernelLMOption*)0,(int*)0);
        if(taiGetModuleInfo("SceLibLocationInternal",&ti)<0){ps("[ACT] internal absent\n");return;}
    }
    void*fn=resolve_export(ti.exports_start,ti.exports_end,INT_LIB,INT_INIT);
    if(!fn){ps("[ACT] init non resolu\n");return;}
    typedef int(*init_t)(int,int);
    int r=((init_t)fn)(1,0x10000);
    ps("[ACT] 3500A98C -> ");phex((uint32_t)r);ps("\n");
    s1=liblocation_seg1();
    if(s1) ps(*(volatile uint32_t*)(s1+0x00)!=0 ? "[ACT] >>> contexte cree, appli peut ouvrir la localisation\n"
                                                : "[ACT] contexte NULL (init KO)\n");
}

static int patched_load(uint16_t id){
    struct _tai_hook_user*cur=(struct _tai_hook_user*)ref_load;
    struct _tai_hook_user*next=(struct _tai_hook_user*)cur->next;
    typedef int(*func_t)(uint16_t);
    func_t old_func=(func_t)((next==0)?cur->old:next->func);
    int res=old_func(id);
    if(id==SCE_SYSMODULE_LOCATION&&res>=0&&!done){done=1;activate();}
    return res;
}
int module_start(SceSize a,const void*b){(void)a;(void)b;
    taiHookFunctionImport(&ref_load,TAI_MAIN_MODULE,TAI_ANY_LIBRARY,0x79A0160A,patched_load);
    return SCE_KERNEL_START_SUCCESS;}
int module_stop(SceSize a,const void*b){(void)a;(void)b;return SCE_KERNEL_STOP_SUCCESS;}
int _start(SceSize a,const void*b) __attribute__((weak,alias("module_start")));
