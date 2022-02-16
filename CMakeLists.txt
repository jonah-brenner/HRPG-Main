cmake_minimum_required(VERSION 3.12)

# set(PICO_SDK_PATH "/home/ben/Desktop/HURC/pico/pico-sdk")

set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(mcp3008_serial C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(pong_table
    mcp3008_serial.c
)

pico_set_program_name(mcp3008_serial "mcp3008_serial")
pico_set_program_version(pmcp3008_serial "0.1")

# Add the necessary libraries to the build
target_link_libraries(mcp3008_serial
    pico_stdlib
)

# enable usb output, disable uart output
pico_enable_stdio_usb(mcp3008_serial 1)
pico_enable_stdio_uart(mcp3008_serial 0)

pico_add_extra_outputs(mcp3008_serial)