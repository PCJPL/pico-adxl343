#ifndef adxl34x_H
#define adxl34x_H

#include <stdint.h>
#include <stdbool.h>

#include "hardware/i2c.h"

// These register names are taken verbatim from the datasheet.
//
// https://web.archive.org/web/20230305234556/http://www.analog.com:80/media/en/technical-documentation/data-sheets/adxl34x.pdf

typedef enum {
    ADXL34X_DEVID_REGISTER,
    // (Registers 0x01 through 0x1C are reserved)
    ADXL34X_THRESH_TAP_REGISTER = 0x1D,
    ADXL34X_OFSX_REGISTER,
    ADXL34X_OFSY_REGISTER,
    ADXL34X_OFSZ_REGISTER,
    ADXL34X_DUR_REGISTER,
    ADXL34X_LATENT_REGISTER,
    ADXL34X_WINDOW_REGISTER,
    ADXL34X_THRESH_ACT_REGISTER,
    ADXL34X_THRESH_INACT_REGISTER,
    ADXL34X_ACT_INACT_CTL_REGISTER,
    ADXL34X_THRESH_FF_REGISTER,
    ADXL34X_TIME_FF_REGISTER,
    ADXL34X_TAP_AXES_REGISTER,
    ADXL34X_ACT_TAP_STATUS_REGISTER,
    ADXL34X_BW_RATE_REGISTER,
    ADXL34X_POWER_CTL_REGISTER,
    ADXL34X_INT_ENABLE_REGISTER,
    ADXL34X_INT_MAP_REGISTER,
    ADXL34X_INT_SOURCE_REGISTER,
    ADXL34X_DATA_FORMAT_REGISTER,
    ADXL34X_DATAX0_REGISTER,
    ADXL34X_DATAX1_REGISTER,
    ADXL34X_DATAY0_REGISTER,
    ADXL34X_DATAY1_REGISTER,
    ADXL34X_DATAZ0_REGISTER,
    ADXL34X_DATAZ1_REGISTER,
    ADXL34X_FIFO_CTL_REGISTER,
    ADXL34X_FIFO_STATUS_REGISTER,

    // The ADXL344 has three additional registers not found on the ADXL343 or ADXL345.
    // They range from 0x3A through 0x3C
    ADXL34X_TAP_SIGN,
    ADXL34X_ORIENT_CONF,
    ADXL34X_ORIENT
} adxl34x_registers_t;

typedef enum {
    LINK_BIT       = 1 << 5,
    AUTO_SLEEP_BIT = 1 << 4,
    MEASURE_BIT    = 1 << 3,
    SLEEP_BIT      = 1 << 2,
    WAKEUP_BIT     = 1 << 1
} adxl34x_power_ctl_t;

typedef enum {
    ADXL34X_ADDRESS_DEFAULT = 0x1D,
    ADXL34X_ADDRESS_ALT     = 0x53
} adxl34x_address_t;

typedef struct {
    i2c_inst_t *i2c;
    uint8_t addr;
} adxl34x_sensor_t;

extern const uint8_t ADXL34X_DEVICE_ID;

bool adxl34x_init(adxl34x_sensor_t *sensor, adxl34x_address_t addr, i2c_inst_t *i2c);

/**
 * @brief Read the device ID from the ADXL34x sensor.
 *
 * This function reads and returns the device ID from the specified ADXL34x sensor.
 *
 * @param sensor Pointer to the ADXL34x sensor struct.
 *
 * @return The device ID of the ADXL34x sensor. It should always be 0xE5.
 */
uint8_t adxl34x_read_device_id(adxl34x_sensor_t *sensor);

#endif // adxl34x_H
