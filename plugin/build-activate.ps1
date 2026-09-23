param([switch]$TraceIpmi)
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
$sdk = Join-Path (Split-Path $projectRoot -Parent) 'vitasdk'
$output = Join-Path $projectRoot 'build'
New-Item -ItemType Directory -Force -Path $output | Out-Null
$name = if ($TraceIpmi) { 'gps_activate_trace' } else { 'gps_activate' }
$compileArgs = @('-Wall', '-Wextra', '-Wshadow', '-O2', '-ffreestanding', '-fno-builtin',
    '-nostdlib', '-Wl,-q', '-Wl,-e,module_start')
if ($TraceIpmi) {
    Write-Warning 'Experimental trace: the console reported an error after the first hardware run. Do not deploy as the default plugin.'
    $compileArgs += '-DGPS_TRACE_IPMI'
}
& "$sdk/bin/arm-vita-eabi-gcc.exe" @compileArgs "$PSScriptRoot/gps_activate.c" -o "$output/$name.elf" -ltaihen_stub -lSceSysmodule_stub -lSceIofilemgr_stub -lSceLibKernel_stub -lSceKernelModulemgr_stub -lgcc
if ($LASTEXITCODE -ne 0) { throw 'Plugin compilation failed' }
& "$sdk/bin/vita-elf-create.exe" "$output/$name.elf" "$output/$name.velf"
if ($LASTEXITCODE -ne 0) { throw 'Plugin ELF conversion failed' }
& "$sdk/bin/vita-make-fself.exe" -c "$output/$name.velf" "$output/$name.suprx"
if ($LASTEXITCODE -ne 0) { throw 'Plugin SELF creation failed' }
Write-Output "$output/$name.suprx"
