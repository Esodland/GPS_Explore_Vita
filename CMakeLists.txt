cmake_minimum_required(VERSION 3.16)

# Utilise la toolchain croisée du VitaSDK (arm-vita-eabi-gcc)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Définis la variable d'environnement VITASDK vers ton SDK !")
  endif()
endif()

project(GPS_Explore)
include("${VITASDK}/share/vita.cmake" REQUIRED)

# Métadonnées de l'application (finissent dans param.sfo)
set(VITA_APP_NAME "Go!Explore 2.0")
set(VITA_TITLEID  "GPSX00001")   # On utilise l'ID d'Ecolibrium pour bypasser app.db
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

add_executable(${PROJECT_NAME}
  src/main.c
)

target_link_libraries(${PROJECT_NAME}
  vita2d
  SceDisplay_stub
  SceGxm_stub
  SceCtrl_stub
  SceSysmodule_stub
  SceLocation_stub
  SceAppUtil_stub
  SceNet_stub
  SceNetCtl_stub
  ScePgf_stub
  SceAppMgr_stub
  SceCommonDialog_stub
  freetype
  png
  jpeg
  z
  m
  c
  gcc
)

# ELF -> eboot.bin (format SELF signé "fake" accepté par HENkaku)
vita_create_self(eboot.bin ${PROJECT_NAME} UNSAFE "-a" "0x2800000000000018")

# Pour la géolocalisation et le réseau, il faut déclarer les privilèges dans le param.sfo de la PS Vita
set(VITA_MKSFOEX_FLAGS "-d ATTRIBUTE2=4112 -d ATTRIBUTE=1 -s CATEGORY=gd")

# eboot.bin + param.sfo -> .vpk installable
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
