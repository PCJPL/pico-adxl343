# pico-adxl343
Library for using the ADXL343 accelerometer with the Raspberry Pi Pico SDK

# Usage

Download `pico_adxl343.cmake` and add it to your project.

Update your projects `CMakeLists.txt` to include the line:

```cmake 
include(pico_adxl343.cmake)
```

And lastly, don't forget to link to the library in your `CMakeLists.txt`: 

```cmake
target_link_libraries(my_cool_pico_project adxl343)
```
