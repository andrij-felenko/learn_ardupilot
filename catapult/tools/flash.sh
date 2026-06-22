#!/usr/bin/env bash
# flash.sh — flash + serial-monitor the catapult firmware on the ESP32-C6.
#
# Runs from Git Bash, but delegates the heavy lifting to PowerShell's export.ps1
# (the ESP-IDF environment that is proven to work on Windows; ESP-IDF's bash
# export is unreliable under Git Bash). idf.py auto-builds if the binary is stale.
#
# Usage:
#   bash tools/flash.sh                          # COM6, the Debug build Qt Creator made
#   bash tools/flash.sh COM5                     # different port
#   bash tools/flash.sh COM6 ESP32_IDF-Release   # different build config
#
# Exit the serial monitor with Ctrl + ]
set -euo pipefail

PORT="${1:-COM6}"
BUILD="${2:-ESP32_IDF-Debug}"

IDF_EXPORT='C:\Users\andri\esp\esp-idf\export.ps1'
PROJECT='E:\develop\ardu\catapult\firmware'

echo ">> flashing build/$BUILD to $PORT (then monitor; Ctrl+] to quit)"
exec powershell -ExecutionPolicy Bypass -NoProfile -Command \
  ". '$IDF_EXPORT' | Out-Null; idf.py -C '$PROJECT' -B 'build/$BUILD' -p $PORT flash monitor"
