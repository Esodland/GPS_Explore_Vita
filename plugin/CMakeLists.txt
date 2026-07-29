cmake_minimum_required(VERSION 3.10)
project(gps_spoofer)
include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

# Ajout du fichier source
add_library(${PROJECT_NAME} SHARED main.c)

# On lie TaiHEN et les stubs de base
target_link_libraries(${PROJECT_NAME} taihen_stub SceSysmodule_stub)

# Création du .suprx (User Mode Plugin)
vita_create_suprx(${PROJECT_NAME}.suprx ${PROJECT_NAME})
