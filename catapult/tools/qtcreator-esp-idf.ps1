# Launch Qt Creator with the ESP-IDF v5.5 environment loaded, so its CMake code
# model and any in-IDE builds use the ESP32-C6 toolchain (riscv32-esp-elf, the
# IDF cmake/ninja, openocd). This is the integration trick: Qt Creator must
# inherit IDF_PATH and the IDF tools on PATH.
#
# Run with:
#   powershell -ExecutionPolicy Bypass -File catapult\tools\qtcreator-esp-idf.ps1
#
# Machine-specific paths — adjust if you move ESP-IDF or Qt Creator.

$IDF_EXPORT = 'C:\Users\andri\esp\esp-idf\export.ps1'
$QTCREATOR  = 'C:\Qt\Tools\QtCreator\bin\qtcreator.exe'
$PROJECT    = Join-Path $PSScriptRoot '..\firmware\CMakeLists.txt'

. $IDF_EXPORT
Start-Process $QTCREATOR -ArgumentList (Resolve-Path $PROJECT).Path
