mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DPICO_BOARD=yourboardhere .. && make
picotool load pico-adxl34x-test.bin && picotool reboot
