#include <stdio.h>
#include <stdint.h>

#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"

#include "ADXL34x.h"

void setup_i2c() {
    const uint64_t baudrate = 100000;
    i2c_init(i2c_default, baudrate);

    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

void setup_picotool_info() {
    // useful information for picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
    bi_decl(bi_program_description("ADXL34x library example"));
}

int main() {
    /**
    stdio_init_all();

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    #warning i2c / bmp280_i2c example requires a board with I2C pins
        puts("Default I2C pins were not defined");
    return 0;
#else

    setup_picotool_info();
    setup_i2c();

    adxl34x_sensor_t sensor;

    // If ALT ADDRESS is set high
    adxl34x_init(&sensor, ADXL34X_ADDRESS_DEFAULT, i2c_default);
    
    // If ALT ADDRESS is grounded
    //adxl34x_init(&sensor, ADXL34X_ADDRESS_ALT, i2c_default);
    
    // You must set the chip select pin to high to enable I2C,
    // unless you've wired it to already be high anyways.
    //adxl34x_enable_i2c(cs_pin);

    uint8_t device_id = adxl34x_read_device_id(&sensor); 

    while (1) {
        printf("Device ID is: %02X\n", device_id);
    }
    **/
    //const uint LED_PIN = 26;
    const uint LED_PIN = 38;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);
    }
    return 0;

//#endif
}

