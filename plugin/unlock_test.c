// GPS_Explore — Test deblocage session (Phase 3, piste 1)
//
// Sequence d'activation decouverte par RE :
//   - seg1+0x34 (u8) : flag teste par SceLibLocation_2311B24A ; s'il vaut 0,
//     l'init SceLibLocationInternal_3500A98C echoue a 0x8010124D (sub_81000478 l.594).
//   - seg1+0x30 (u32): global de privilege (0 => 0x8010124F cote API).
//
// On ecrit les deux flags dans les donnees de liblocation, puis on appelle l'init.
// Si le contexte (seg1+0x00) devient != 0, la session client est etablie.
//
// Log : ux0:data/gps_unlock.txt   (freestanding)

#include <psp2/types.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/io/fcntl.h>
#include <taihen.h>

static tai_hook_ref_t ref_load;
static int done = 0;

#define INT_LIB   0x098b0c75u
#define INT_INIT  0x3500A98Cu
#define LOC_LIB   0x924d86d7u   // SceLibLocation
#define NID_OPEN  0xDD271661u   // sceLocationOpen(handle*, method, hmethod)
#define NID_GETL  0x15BC27C8u   // sceLocationGetLocation(handle, info*)

static unsigned slen(const char*s){unsigned n=0;while(s[n])n++;return n;}
static void zero(void*p,unsigned n){unsigned char*b=p;for(unsigned i=0;i<n;i++)b[i]=0;}
static int streq(const char*a,const char*b){while(*a&&*a==*b){a++;b++;}return *(const unsigned char*)a==*(const unsigned char*)b;}
static void wr(const char*s,unsigned n){SceUID fd=sceIoOpen("ux0:data/gps_unlock.txt",SCE_O_WRONLY|SCE_O_CREAT|SCE_O_APPEND,0777);if(fd>=0){sceIoWrite(fd,s,n);sceIoClose(fd);}}
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
static void dump(const char*tag,unsigned char*s1){
    ps("[UNLK] ");ps(tag);
    ps(" ctx=");phex(*(volatile uint32_t*)(s1+0x00));
    ps(" priv=");phex(*(volatile uint32_t*)(s1+0x30));
    ps(" flag34=");phex(*(volatile uint8_t*)(s1+0x34));ps("\n");
}

static void run(void){
    ps("--- UNLOCK TEST START ---\n");
    unsigned char*s1=liblocation_seg1();
    if(!s1){ps("[UNLK] seg1 introuvable\n");ps("[UNLK] fin\n");return;}
    ps("[UNLK] seg1=");phex((uint32_t)(uintptr_t)s1);ps("\n");
    dump("AVANT:",s1);

    // Ecrire les 2 flags d'activation
    *(volatile uint32_t*)(s1+0x30)=2;
    *(volatile uint8_t*)(s1+0x34)=1;
    dump("FLAGS ECRITS:",s1);

    // Charger internal + resoudre l'init
    tai_module_info_t ti;ti.size=sizeof(ti);
    int r=taiGetModuleInfo("SceLibLocationInternal",&ti);
    if(r<0){
        SceUID mid=sceKernelLoadStartModule("vs0:sys/external/liblocation_internal.suprx",0,(void*)0,0,(SceKernelLMOption*)0,(int*)0);
        ps("[UNLK] LoadStart(internal)=");phex((uint32_t)mid);ps("\n");
        r=taiGetModuleInfo("SceLibLocationInternal",&ti);
    }
    ps("[UNLK] getModuleInfo(internal)=");phex((uint32_t)r);ps("\n");
    if(r<0){ps("[UNLK] internal absent — fin\n");return;}
    void*fn=resolve_export(ti.exports_start,ti.exports_end,INT_LIB,INT_INIT);
    ps("[UNLK] 3500A98C @ ");phex((uint32_t)(uintptr_t)fn);ps("\n");
    if(!fn){ps("[UNLK] init non resolu — fin\n");return;}

    typedef int(*init_t)(int,int);
    ps("[UNLK] appel 3500A98C(1,0x10000)...\n");
    int ret=((init_t)fn)(1,0x10000);
    ps("[UNLK] init retour=");phex((uint32_t)ret);ps("\n");

    s1=liblocation_seg1();  // recharger (au cas ou)
    if(s1) dump("APRES INIT:",s1);
    if(!(s1 && *(volatile uint32_t*)(s1+0x00)!=0)){
        ps("[UNLK] contexte toujours NULL (init n'a pas cree la session) — fin\n");
        return;
    }
    ps("[UNLK] >>> CONTEXTE CREE ! session client etablie.\n");

    // --- Etape suivante : vrai sceLocationOpen / sceLocationGetLocation ---
    tai_module_info_t lti; lti.size=sizeof(lti);
    if(taiGetModuleInfo("SceLibLocation",&lti)<0){ ps("[UNLK] getModuleInfo(SceLibLocation) KO\n"); return; }
    typedef int(*open_t)(int*,int,int);
    typedef int(*getl_t)(int,void*);
    open_t f_open=(open_t)resolve_export(lti.exports_start,lti.exports_end,LOC_LIB,NID_OPEN);
    getl_t f_getl=(getl_t)resolve_export(lti.exports_start,lti.exports_end,LOC_LIB,NID_GETL);
    ps("[UNLK] open @ ");phex((uint32_t)(uintptr_t)f_open);
    ps("  getLocation @ ");phex((uint32_t)(uintptr_t)f_getl);ps("\n");
    if(!f_open||!f_getl){ ps("[UNLK] exports open/getLocation non resolus — fin\n"); return; }

    // tester chaque methode ; on cherche un handle valide (fin de 0x8010124F)
    for(int method=1; method<=5; method++){
        int handle=0;
        ps("\n[UNLK] sceLocationOpen(&h, method=");phex((uint32_t)method);ps(", 0) ...\n");
        int ro=f_open(&handle,method,0);
        ps("[UNLK] open retour=");phex((uint32_t)ro);ps(" handle=");phex((uint32_t)handle);ps("\n");
        if(ro>=0 && handle!=0 && handle!=(int)0xFFFFFFFF){
            ps("[UNLK] >>> HANDLE VALIDE ! tentative getLocation...\n");
            static unsigned char loc[64]; for(int i=0;i<64;i++) loc[i]=0;
            int rg=f_getl(handle,loc);
            ps("[UNLK] getLocation retour=");phex((uint32_t)rg);ps("\n");
            ps("[UNLK] locinfo[0..47]=");
            const char*H="0123456789ABCDEF";
            for(int i=0;i<48;i++){char c[3];c[0]=H[(loc[i]>>4)&0xF];c[1]=H[loc[i]&0xF];c[2]=' ';wr(c,3);}
            ps("\n");
            break;
        }
    }
    ps("[UNLK] fin\n");
}

static int patched_load(uint16_t id){
    struct _tai_hook_user*cur=(struct _tai_hook_user*)ref_load;
    struct _tai_hook_user*next=(struct _tai_hook_user*)cur->next;
    typedef int(*func_t)(uint16_t);
    func_t old_func=(func_t)((next==0)?cur->old:next->func);
    int res=old_func(id);
    if(id==SCE_SYSMODULE_LOCATION&&res>=0&&!done){done=1;run();}
    return res;
}
int module_start(SceSize a,const void*b){(void)a;(void)b;
    taiHookFunctionImport(&ref_load,TAI_MAIN_MODULE,TAI_ANY_LIBRARY,0x79A0160A,patched_load);
    return SCE_KERNEL_START_SUCCESS;}
int module_stop(SceSize a,const void*b){(void)a;(void)b;return SCE_KERNEL_STOP_SUCCESS;}
int _start(SceSize a,const void*b) __attribute__((weak,alias("module_start")));
