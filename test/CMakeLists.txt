cmake_minimum_required(VERSION 3.13)
project(pico-adxl343-tests)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/pico_sdk_import.cmake)

# Unity testing framework
set(UNITY_SOURCES unity/unity.c)
add_library(unity STATIC ${UNITY_SOURCES})
target_include_directories(unity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/unity)

# Test executables
add_executable(test_pico_adxl343 test_smoke.c)
target_include_directories(
    test_pico_adxl343
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/../include
)

target_link_libraries(
    test_pico_adxl343
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../build/libadxl343.a
    unity
)

add_test(test_smoke test_pico_adxl343)
