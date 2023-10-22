cmake_minimum_required(VERSION 3.13)

if(NOT DEFINED ENV{PICO_SDK_PATH})
    message(FATAL_ERROR "The PICO_SDK_PATH environment variable is not set. Please set it before building the examples.")
endif()

# Include the Pico SDK import file.
include(pico_sdk_import.cmake)

#include(pico_extras_import_optional.cmake)

# Set the project name and version.
project(pico-adxl34x-test VERSION 1.0)

# Initialize the Pico SDK.
pico_sdk_init()

# Enable debug logging in the ADXL343 library for debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DADXL34X_DEBUG)
endif()

# Add your main.c file.
add_executable(pico-adxl34x-test
    test.c
    unity.c
    #unity_integration.c
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(pico-adxl34x-test PRIVATE -Wall -Wextra)
endif()

# Link the Pico SDK libraries to your project.
target_link_libraries(pico-adxl34x-test 
    PRIVATE
    pico_stdlib
    hardware_i2c
)

target_sources(pico-adxl34x-test PRIVATE 
    ../src/ADXL34x.c
    ../src/ADXL34x.h
)

target_include_directories(pico-adxl34x-test PRIVATE 
    ../src
)

if (TARGET tinyusb_device)
    # enable usb output, disable uart output
    pico_enable_stdio_usb(pico-adxl34x-test 1)
    pico_enable_stdio_uart(pico-adxl34x-test 0)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()

# Create a .uf2 binary for dragging onto the Pico when it's in bootloader mode.
pico_add_extra_outputs(pico-adxl34x-test)

# Add any other necessary configurations or settings here.
