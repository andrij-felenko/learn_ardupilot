# --- ESP-IDF environment bootstrap (machine-specific) -------------------------
# Lets this project configure/build in ANY IDE or shell WITHOUT first sourcing
# export.ps1. We set the same variables export.ps1 sets, from inside CMake,
# BEFORE ESP-IDF's project.cmake runs. set(ENV{}) changes this cmake process'
# environment, and every child process it spawns (compiler checks, ninja,
# python) inherits it. If you move or upgrade ESP-IDF, fix the two roots below.

set(_IDF   "C:/Users/andri/esp/esp-idf")
set(_TOOLS "C:/Users/andri/.espressif")

set(ENV{IDF_PATH}            "${_IDF}")
set(ENV{IDF_TOOLS_PATH}      "${_TOOLS}")
set(ENV{IDF_PYTHON_ENV_PATH} "${_TOOLS}/python_env/idf5.5_py3.12_env")
set(ENV{ESP_IDF_VERSION}     "5.5")
set(ENV{ESP_ROM_ELF_DIR}     "${_TOOLS}/tools/esp-rom-elfs/20241011/")
set(ENV{OPENOCD_SCRIPTS}     "${_TOOLS}/tools/openocd-esp32/v0.12.0-esp32-20251215/openocd-esp32/share/openocd/scripts")
set(ENV{IDF_CCACHE_ENABLE}   "0")   # ccache off: it's called by bare name (not on
                                    # the build-step PATH) and crashes on Windows (0xc0000374)
set(ENV{IDF_TARGET}          "esp32c6")

# Pin the ESP-IDF venv Python. Otherwise CMake's find_package(Python) grabs the
# system Python (which lacks idf_component_manager and other ESP-IDF deps). This
# is what idf.py passes via -DPYTHON=...
set(_PYEXE "${_TOOLS}/python_env/idf5.5_py3.12_env/Scripts/python.exe")
set(ENV{PYTHON} "${_PYEXE}")
set(PYTHON           "${_PYEXE}" CACHE FILEPATH "ESP-IDF Python interpreter" FORCE)
set(Python3_EXECUTABLE "${_PYEXE}" CACHE FILEPATH "" FORCE)

# Prepend ESP-IDF tool dirs to PATH so the riscv compiler, ninja, python, etc.
# are found during configuration and the compiler check.
set(_PREPEND
  "${_TOOLS}/tools/riscv32-esp-elf/esp-14.2.0_20260121/riscv32-esp-elf/bin"
  "${_TOOLS}/tools/riscv32-esp-elf-gdb/16.3_20250913/riscv32-esp-elf-gdb/bin"
  "${_TOOLS}/tools/ninja/1.12.1"
  "${_TOOLS}/tools/cmake/3.30.2/bin"
  "${_TOOLS}/tools/openocd-esp32/v0.12.0-esp32-20251215/openocd-esp32/bin"
  "${_TOOLS}/tools/idf-exe/1.0.3"
  "${_TOOLS}/tools/ccache/4.12.1/ccache-4.12.1-windows-x86_64"
  "${_TOOLS}/python_env/idf5.5_py3.12_env/Scripts"
  "${_IDF}/components/espcoredump"
  "${_IDF}/components/partition_table"
  "${_IDF}/components/app_update"
  "${_IDF}/tools"
)
string(JOIN ";" _PREPEND_STR ${_PREPEND})
set(ENV{PATH} "${_PREPEND_STR};$ENV{PATH}")
