@echo off
set VITASDK=C:\Users\Julien\PSVITA\vitasdk
set PATH=%VITASDK%\bin;C:\Users\Julien\PSVITA\tools\cmake\bin;C:\Users\Julien\PSVITA\tools;%PATH%

mkdir build 2>nul
cd build
C:\Users\Julien\PSVITA\tools\cmake\bin\cmake.exe .. -GNinja -DCMAKE_TOOLCHAIN_FILE="%VITASDK%\share\vita.toolchain.cmake"
C:\Users\Julien\PSVITA\tools\ninja.exe

echo.
echo Envoi du plugin vers ur0:/tai/gps_spoofer.suprx ...
curl.exe -T gps_spoofer.suprx ftp://192.168.1.163:1337/ur0:/tai/gps_spoofer.suprx
echo.
echo === INSTRUCTIONS TAIHEN ===
echo Ajoutez cette ligne dans votre ur0:tai/config.txt sous le titre *GPSX00001 (ou *ALL) :
echo ur0:tai/gps_spoofer.suprx
echo Puis redemarrez la Vita ou rechargez le fichier config dans les parametres HENkaku.
pause
