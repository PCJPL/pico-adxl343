/*
 * pico-adxl34x
 * Version 0.0.1
 *
 * Description: Library for using the ADXL34x series of accelerometers with the
 * Raspberry Pi Pico SDK
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

#include "ADXL34x.h"

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef ADXL34X_DEBUG
#define debug_log(...) printf(__VA_ARGS__)
#else
#define debug_log(...)
#endif

// ADXL34x series acelerometers report a fixed Device ID.
const uint8_t ADXL34X_DEVICE_ID = 0xE5;
const uint8_t ADXL34X_SUCCESS = 1;
const int ADXL34X_ADC_RESOLUTION = 1023;

static const float GRAVITY = 9.81;

static adxl34x_result_t read_reg(adxl34x_sensor_t *sensor, uint8_t reg) {
  debug_log("Attempting to read register 0x%02X at address 0x%X\n", (unsigned int)reg,
	    sensor->addr);

  adxl34x_result_t result;

  // Send a 1 byte address of the register we want to read from.
  // We expect to get back the number of bytes written (1).
  // If not, it's an error code.
  int write_error = i2c_write_blocking(sensor->i2c, sensor->addr, &reg, 1, false);

  if (write_error != 1) {
    result.status = write_error;
    result.value = 0;
    return result;
  }

  // Then read the data from the register
  uint8_t data;
  int read_error = i2c_read_blocking(sensor->i2c, sensor->addr, &data, 1, false);

  if (read_error != 1) {
    result.status = read_error;
    result.value = 0;
    return result;
  }

  result.status = ADXL34X_SUCCESS;
  result.value = data;

  return result;
}

static adxl34x_result_t write_reg(adxl34x_sensor_t *sensor, uint8_t reg, uint8_t value) {
  debug_log("Attempting to write register 0x%02X at address 0x%X\n", (unsigned int)reg,
	    sensor->addr);

  uint8_t data[2] = {reg, value};

  adxl34x_result_t result;
  result.value = 0;

  int error = i2c_write_blocking(sensor->i2c, sensor->addr, data, 2, false);

  // We write two bytes, and expect 2 bytes back in return.
  if (error != 2) {
    result.status = error;
    return result;
  }

  result.status = ADXL34X_SUCCESS;

  return result;
}

void adxl34x_enable_i2c(uint8_t cs_pin) {
  // According to the data sheet (Pg. 13, "Serial Connections")
  // the chip select pin must be set to high to enable I2C communication.

  gpio_init(cs_pin);
  gpio_set_dir(cs_pin, GPIO_OUT);
  gpio_put(cs_pin, 1);
}

bool adxl34x_init(adxl34x_sensor_t *sensor, adxl34x_address_t addr, i2c_inst_t *i2c) {
  if (!sensor) {
    return false;
  }

  debug_log("Initializing ADXL34X sensor at address 0x%02X\n", addr);
  sensor->addr = addr;
  sensor->i2c = i2c;

  // Initialization code here...

  return true;
}

adxl34x_result_t adxl34x_enable_measurement(adxl34x_sensor_t *sensor) {
  // At startup, the sensor is in standby mode. To start taking measurements,
  // we have to set the MEASURE_BIT in the POWER_CTL register.
  uint8_t register_value = 0;

  // Set the AUTO_SLEEP and MEASURE bits
  register_value |= ADXL34X_AUTO_SLEEP_BIT | ADXL34X_MEASURE_BIT;

  return write_reg(sensor, ADXL34X_POWER_CTL_REGISTER, register_value);
}

adxl34x_result_t adxl34x_set_data_rate(adxl34x_sensor_t *sensor,
				       adxl34x_data_rate_t rate) {

  // Both power and data rate are controlled by setting the BW_RATE register.
  return write_reg(sensor, ADXL34X_BW_RATE_REGISTER, rate);
}

adxl34x_result_t adxl34x_get_device_id(adxl34x_sensor_t *sensor) {
  return read_reg(sensor, ADXL34X_DEVID_REGISTER);
}

adxl34x_result_t adxl34x_get_tap_threshold(adxl34x_sensor_t *sensor) {
  return read_reg(sensor, ADXL34X_THRESH_TAP_REGISTER);
}

adxl34x_accel_data_t adxl34x_get_raw_acceleration(adxl34x_sensor_t *sensor) {
  uint8_t data[6];

  adxl34x_accel_data_t accel_data;

  accel_data.x.raw = 0;
  accel_data.y.raw = 0;
  accel_data.z.raw = 0;

  /*
  Acceleration data for each of the three axes are stored across two 8-bit
  registers, for a total of six registers altogether:

  DATAX0 (0x32)
  DATAX1 (0x33)
  DATAY0 (0x34)
  DATAY1 (0x35)
  DATAZ0 (0x36)
  DATAZ1 (0x37)

  The first axis's register (i.e DATAX0) is the least significant byte (LSB),
  and the second axis's register (i.e DATAX1) is the most significant byte
  (MSB). The data is stored in twos complement.

  Starting at DATAX0 (0x32) we can sequentially read all 6 registers / bytes
  in one I2C transaction. That way the data doesn't change between reads.
  */

  const uint8_t reg = ADXL34X_DATAX0_REGISTER;

  int write_error = i2c_write_blocking(sensor->i2c, sensor->addr, &reg, 1, false);

  if (write_error != 1) {
    accel_data.status = write_error;
    return accel_data;
  }

  int read_error = i2c_read_blocking(sensor->i2c, sensor->addr, data, 6, false);

  if (read_error != 6) {
    accel_data.status = read_error;
    return accel_data;
  }

  accel_data.x.raw = (int16_t)(data[1] << 8 | data[0]);
  accel_data.y.raw = (int16_t)(data[3] << 8 | data[2]);
  accel_data.z.raw = (int16_t)(data[5] << 8 | data[4]);

  accel_data.status = ADXL34X_SUCCESS;
  return accel_data;
}

void adxl34x_convert_to_g(adxl34x_accel_data_t *data,
			  adxl34x_sensitivity_range_t sensitivity) {
}

adxl34x_result_t adxl34x_configure_data_format(adxl34x_sensor_t *sensor, bool full_res,
					       adxl34x_sensitivity_range_t range) {

  // Read the current value of the DATA_FORMAT register
  adxl34x_result_t reg = read_reg(sensor, ADXL34X_DATA_FORMAT_REGISTER);

  // Clear the FULL_RES and RANGE bits in the register value
  reg.value &= ~(ADXL34X_DATA_FORMAT_FULL_RES_BIT | ADXL34X_DATA_FORMAT_RANGE_BITMASK);

  // Set the FULL_RES bit if full_res is true
  if (full_res) {
    reg.value |= ADXL34X_DATA_FORMAT_FULL_RES_BIT;
  }

  // Set the RANGE bits
  reg.value |= range;

  // Write the modified value back to the DATA_FORMAT register
  return write_reg(sensor, ADXL34X_DATA_FORMAT_REGISTER, reg.value);
}
