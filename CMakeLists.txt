cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_PATH "/home/ben/Desktop/HURC/pico/pico-sdk")

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pong_table C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(pong_table
    pong_table.c
    patterns.c
)

pico_set_program_name(pong_table "basic_LCD")
pico_set_program_version(pong_table "0.1")

# Generate PIO header for program and initialization function inclusion
pico_generate_pio_header(pong_table ${CMAKE_CURRENT_LIST_DIR}/pong_table.pio)

# Add the necessary libraries to the build
target_link_libraries(pong_table
    pico_stdlib
    pico_malloc
    pico_mem_ops
    pico_runtime
    hardware_pio
    pico_bootrom
)

# enable usb output, disable uart output
pico_enable_stdio_usb(pong_table 1)
pico_enable_stdio_uart(pong_table 0)

pico_add_extra_outputs(pong_table)