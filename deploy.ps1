param(
    [string]$Ip = "192.168.1.163",
    [string]$TitleId = "GPSX00001",
    [string]$EbootPath = "build\eboot.bin"
)

function Send-VitaCompanionCommand {
    param([string]$Cmd)
    Write-Host "Sending command to vitacompanion: $Cmd"
    try {
        $client = New-Object System.Net.Sockets.TcpClient($Ip, 1338)
        $stream = $client.GetStream()
        $writer = New-Object System.IO.StreamWriter($stream)
        $writer.WriteLine($Cmd)
        $writer.Flush()
        Start-Sleep -Seconds 1
        $client.Close()
    } catch {
        Write-Host "Failed to send command: $_"
    }
}

# 1. Fermer l'application
Send-VitaCompanionCommand "destroy"

# 2. Envoyer le nouvel eboot.bin via FTP
Write-Host "Uploading eboot.bin..."
curl.exe -T $EbootPath "ftp://${Ip}:1337/ux0:/app/${TitleId}/eboot.bin"

Write-Host "Uploading param.sfo..."
curl.exe -T "build\GPS_Explore.vpk_param.sfo" "ftp://${Ip}:1337/ux0:/app/${TitleId}/sce_sys/param.sfo"

# 3. Relancer l'application
Send-VitaCompanionCommand "launch $TitleId"
Write-Host "Deployment complete."
