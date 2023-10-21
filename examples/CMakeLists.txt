cmake_minimum_required(VERSION 3.13)

# Include the Pico SDK import file.
include(pico_sdk_import.cmake)

#include(pico_extras_import_optional.cmake)

# Set the project name and version.
project(pico-adxl34x-example VERSION 1.0)

# Initialize the Pico SDK.
pico_sdk_init()

# Add your main.c file.
add_executable(pico-adxl34x-example
    main.c
)

# Link the Pico SDK libraries to your project.
target_link_libraries(pico-adxl34x-example 
    pico_stdlib
    hardware_i2c
)

target_sources(pico-adxl34x-example PRIVATE 
    ../src/ADXL34x.c
)

target_include_directories(pico-adxl34x-example PRIVATE 
    ../src
)

if (TARGET tinyusb_device)
    # enable usb output, disable uart output
    pico_enable_stdio_usb(pico-adxl34x-example 1)
    pico_enable_stdio_uart(pico-adxl34x-example 0)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()

# Create a .uf2 binary for dragging onto the Pico when it's in bootloader mode.
pico_add_extra_outputs(pico-adxl34x-example)

# Add any other necessary configurations or settings here.
