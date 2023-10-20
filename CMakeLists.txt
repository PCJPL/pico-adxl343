cmake_minimum_required(VERSION 3.13)

project(pico-adxl343 C CXX ASM)

# Pico SDK dependencies
set(PICO_DEPENDENCIES pico_stdlib hardware_i2c)

set(ADXL_SOURCES src/c/adxl343.c)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND CXX IN_LIST CMAKE_ENABLE_COMPILE_LANGUAGES)
    list(APPEND ADXL_SOURCES src/cpp/adxl343_cpp.cpp)
endif()

add_library(adxl343 STATIC ${ADXL_SOURCES})

target_include_directories(adxl343 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
        $<INSTALL_INTERFACE:include>
)

target_include_directories(adxl343 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/c)

target_link_libraries(adxl343 ${PICO_DEPENDENCIES})

install(TARGETS adxl343
    EXPORT pico-adxl343-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT pico-adxl343-targets
    NAMESPACE pico-adxl343::
    DESTINATION lib/cmake/pico-adxl343
)
