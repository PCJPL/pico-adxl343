#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "unity.h"
#include "ADXL34x.h"

adxl34x_sensor_t sensor;

void setUp(void) {
  adxl34x_init(&sensor, ADXL34X_ADDRESS_ALT, i2c_default);
}

void tearDown(void) {
  memset(&sensor, 0, sizeof(adxl34x_sensor_t));
}

void test_smoke(void) {
  adxl34x_sensor_t sensor_init;
  TEST_ASSERT_TRUE(adxl34x_init(&sensor_init, ADXL34X_ADDRESS_ALT, i2c_default));
}

void test_get_device_id_success(void) {
  adxl34x_sensor_t sensor_init;
  adxl34x_init(&sensor_init, ADXL34X_ADDRESS_ALT, i2c_default);

  adxl34x_result_t device_id = adxl34x_get_device_id(&sensor);
  TEST_ASSERT_TRUE(device_id.status == ADXL34X_SUCCESS);
}

void test_get_device_id_value(void) {
  adxl34x_sensor_t sensor_init;
  adxl34x_init(&sensor_init, ADXL34X_ADDRESS_ALT, i2c_default);

  adxl34x_result_t device_id = adxl34x_get_device_id(&sensor);
  TEST_ASSERT_TRUE(device_id.value == 0xE5);
}
  

int main(void) {
  stdio_init_all();
  sleep_ms(2000);

  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

  const uint64_t baudrate = 100000;
  i2c_init(i2c_default, baudrate);
  UNITY_BEGIN();

  RUN_TEST(test_smoke);
  RUN_TEST(test_get_device_id_success);
  RUN_TEST(test_get_device_id_value);

  return UNITY_END();
}
