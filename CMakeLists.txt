cmake_minimum_required(VERSION 3.12)

# set(PICO_SDK_PATH "/home/ben/Desktop/HURC/pico/pico-sdk")

set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(HRPG-PICO C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_subdirectory("mcp3008_serial")