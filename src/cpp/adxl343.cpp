extern "C" {
    #include "ADXL343.h"
}

#include "ADXL343.hpp"

ADXL343::ADXL343() {
    adxl343_initialize();
}

ADXL343::~ADXL343() {
    // Destructor implementation
}
