$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
$sdk = Join-Path (Split-Path $projectRoot -Parent) 'vitasdk'
$output = Join-Path $projectRoot 'build'
New-Item -ItemType Directory -Force -Path $output | Out-Null
$name = 'gps_assist'
& "$sdk/bin/arm-vita-eabi-gcc.exe" -std=gnu17 -Wall -Wextra -Wshadow -O2 -ffreestanding -fno-builtin -nostdlib -nostartfiles `
    '-Wl,-q' "$PSScriptRoot/gps_assist_kernel.c" -o "$output/$name.elf" `
    -ltaihenForKernel_stub -lSceThreadmgrForDriver_stub -lgcc
if ($LASTEXITCODE -ne 0) { throw 'Kernel module compilation failed' }
& "$sdk/bin/vita-elf-create.exe" -e "$PSScriptRoot/gps_assist_kernel.yml" "$output/$name.elf" "$output/$name.velf"
if ($LASTEXITCODE -ne 0) { throw 'Kernel module ELF conversion failed' }
& "$sdk/bin/vita-make-fself.exe" -c "$output/$name.velf" "$output/$name.skprx"
if ($LASTEXITCODE -ne 0) { throw 'Kernel module SELF creation failed' }
Write-Output "$output/$name.skprx"
