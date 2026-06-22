# flash.ps1 — flash + serial-monitor the catapult firmware on the ESP32-C6.
# Loads the proven ESP-IDF environment (export.ps1) and runs idf.py against the
# build Qt Creator made. idf.py auto-builds if the binary is stale.
#
# Run from cmd or PowerShell (from the repo root):
#   powershell -ExecutionPolicy Bypass -File catapult\tools\flash.ps1
#   powershell -ExecutionPolicy Bypass -File catapult\tools\flash.ps1 -Port COM5
#   powershell -ExecutionPolicy Bypass -File catapult\tools\flash.ps1 -Build ESP32_IDF-Release
#
# Exit the serial monitor with Ctrl + ]
param(
  [string]$Port  = "COM6",
  [string]$Build = "ESP32_IDF-Debug"
)
$ErrorActionPreference = "Stop"

Write-Host ">> flashing build/$Build to $Port (then monitor; Ctrl+] to quit)"
. 'C:\Users\andri\esp\esp-idf\export.ps1' | Out-Null
$env:IDF_CCACHE_ENABLE = '0'   # export.ps1 turns ccache on; keep it off (flaky on Windows)
idf.py -C 'E:\develop\ardu\catapult\firmware' -B "build/$Build" -p $Port flash monitor
