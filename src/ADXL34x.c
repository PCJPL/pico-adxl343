/*
 * pico-adxl34x
 * Version 0.0.1
 *
 * Description: Library for using the ADXL34x series of accelerometers with the Raspberry Pi Pico SDK  
 *
 * Author: Joshua Murphy 
 * Email: hello@joshmurphy.ca 
 *
 * License: MIT License (https://opensource.org/licenses/MIT)
 *
 * Copyright (c) 2023 Joshua Murphy 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS," WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>
#include <stdbool.h>

#include "ADXL34x.h"

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

// ADXLx series acelerometers report a fixed Device ID.
const uint8_t ADXL34X_DEVICE_ID = 0xE5;

static uint8_t i2c_read8(i2c_inst_t *i2c_instance, uint8_t i2c_address, uint8_t reg) {
    uint8_t data;
    
    // Send the register address we want to read from
    i2c_write_blocking(i2c_instance, i2c_address, &reg, 1, false);
    
    // Read the data from the register
    i2c_read_blocking(i2c_instance, i2c_address, &data, 1, false);

    return data;
}

static uint8_t read_reg(adxl34x_sensor_t *sensor, uint8_t reg) {
    return i2c_read8(sensor->i2c, sensor->addr, reg);
}

static uint8_t write_reg(adxl34x_sensor_t *sensor, uint8_t register_address, uint8_t value) {
    uint8_t data[2];
    data[0] = register_address;
    data[1] = value;

    return i2c_write_blocking(sensor->i2c, sensor->addr, data, 2, false);
}

uint8_t adxl34x_read_device_id(adxl34x_sensor_t *sensor) {
    return read_reg(sensor, ADXL34X_DEVID_REGISTER);
}

void adxl34x_enable_measurement(adxl34x_sensor_t *sensor) {
    // At startup, the sensor is in standby mode. To start taking measurements,
    // we have to set the MEASURE_BIT in the POWER_CTL register.
    uint8_t register_value = 0;

    // Set the AUTO_SLEEP and MEASURE bits
    register_value |= AUTO_SLEEP_BIT | MEASURE_BIT;
    
    uint8_t result = write_reg(sensor, ADXL34X_POWER_CTL_REGISTER, register_value); 
    // Check the return code for errors
}

static void adxl34x_enable_i2c(uint8_t cs_pin) {
    // According to the data sheet (Pg. 13, "Serial Connections")
    // the chip select pin must be set to high to enable I2C communication.

    gpio_init(cs_pin); 
    gpio_set_dir(cs_pin, GPIO_OUT); 
    gpio_put(cs_pin, 1);
}

bool adxl34x_init(adxl34x_sensor_t *sensor, adxl34x_address_t addr, i2c_inst_t *i2c) {
    if(!sensor) {
	return false;
    }

    sensor->addr = addr;
    sensor->i2c = i2c;
    
    //adxl34x_enable_measurement();

    // Initialization code here...

    return true;
}
