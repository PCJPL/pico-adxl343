#include "ADXL34x.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include <stdint.h>
#include <stdio.h>

void setup_i2c() {
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

void setup_picotool_info() {
  // useful information for picotool
  bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN,
			     GPIO_FUNC_I2C));
  bi_decl(bi_program_description("ADXL34x library example"));
}

int main() {
  stdio_init_all();

  // We do plenty of printing via serial to demonstrate the usage of the library.
  // Give the host a second to re-establish the serial connection with our Pico
  // before we start blasting stdout.

  sleep_ms(2000);

  printf("[ADXL343] Starting!\n");

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) ||                       \
    !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning This example code requires a board with I2C pins
  printf("Default I2C pins were not defined\n");
  return 0;
#else

  setup_picotool_info();
  setup_i2c();

  const uint64_t baudrate = 100000;
  i2c_init(i2c_default, baudrate);

  adxl34x_sensor_t sensor;

  // The address of the sensor depends on your hardware configuration.
  // If the ALT ADDRESS pin (pin 12) of the sensor is set to high, the sensors
  // address will be at 0x53 (ADXL34X_ADDR_ALT). If not,
  // it will be at 0x1D (ADXL343X_ADDRESS_DEFAULT).

  if (!adxl34x_init(&sensor, ADXL34X_ADDRESS_ALT, i2c_default)) {
    printf("Sensor can not be NULL\n");
    return 0;
  }

  printf("Sensor initialized with address 0x%02X\n", sensor.addr);

  // The Chip Select (CS) pin must be set to high to enable I2C communication.
  // This could be done in software if the pin is connected to the RP2040.
  // That is not the case on the board I'm developing on. So I can't test setting
  // the CS pin.

  // You will need to ensure the CS pin is tied to voltage. For more details refer
  // to page 16 of the datasheet:
  //
  // https://web.archive.org/web/20230305234556/http://www.analog.com:80/media/en/technical-documentation/data-sheets/adxl34x.pdf

  // TODO:
  // adxl34x_enable_i2c(cs_pin);

  // Read the contents of the DEVID register at 0x00. This is a read only register
  // that will always return a fixed value.

  adxl34x_result_t device_id = adxl34x_get_device_id(&sensor);

  if (device_id.status == ADXL34X_SUCCESS) {
    printf("ADXL343 Device ID: 0x%02X\n", (unsigned int)device_id.value);
  } else if (device_id.status == PICO_ERROR_GENERIC) {
    printf("Error: I2C device not present or address not acknowledged\n.");
  } else {
    printf("Error: An unknown error occured while reading the Device ID register.\n");
  }

  // A good sanity check is to ensure that the device ID is the expected value.
  // It's not supposed to change and should always be the same. Otherwise something
  // unusual is probably going on.
  if (device_id.value != ADXL34X_DEVICE_ID) {
    printf("The sensor is reporting the wrong device ID.\n");
  }

  adxl34x_result_t configure_data =
      adxl34x_configure_data_format(&sensor, true, ADXL34X_SENSITIVITY_RANGE_16G);

  if (configure_data.status != ADXL34X_SUCCESS) {
    printf("Failed to set the sensitivity range and resolution!\n");
  }

  adxl34x_result_t tap_threshold = adxl34x_get_tap_threshold(&sensor);
  printf("Tap threshold: 0x%02X\n", (unsigned int)tap_threshold.value);

  printf("Enabling measurement\n");

  adxl34x_result_t enable_measurement = adxl34x_enable_measurement(&sensor);
  if (enable_measurement.status != ADXL34X_SUCCESS) {
    printf("Failed to enable measurement!\n");
  }

  while (1) {
    adxl34x_accel_data_t accel = adxl34x_get_raw_acceleration(&sensor);

    if (accel.status != ADXL34X_SUCCESS) {
      printf("Failed to read acceleration!\n");
    } else {
      printf("X: %d Y: %d Z: %d\n", accel.x.raw, accel.y.raw, accel.z.raw);
    }

    // adxl34x_raw_accel_to_gravity(accel)

    sleep_ms(50);
  }

#endif
}
