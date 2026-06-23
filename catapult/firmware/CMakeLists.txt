# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# Bootstrap the ESP-IDF environment (IDF_PATH, toolchain on PATH, IDF_TARGET)
# so this project configures in any IDE/shell without pre-sourcing export.ps1.
# OPTIONAL: silently skipped on a machine where the env is already set up.
include(${CMAKE_CURRENT_LIST_DIR}/idf_env.cmake OPTIONAL)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(catapult)
