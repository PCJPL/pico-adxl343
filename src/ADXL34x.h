#ifndef ADXL34X_H
#define ADXL34X_H

#include "hardware/i2c.h"

#include <stdbool.h>
#include <stdint.h>

/**
 * \defgroup ADXL34X ADXL34X
 * @{
 */

extern const uint8_t ADXL34X_DEVICE_ID;
extern const uint8_t ADXL34X_SUCCESS;

/** @brief The maximum value that the ADC (Analog-to-Digital Converter) of the sensor
 *  can represent. All of our accelerometer readings are scaled using this value.
 */
extern const int ADXL34X_ADC_RESOLUTION;

typedef enum {
  ADXL34X_MODEL_ADXL343,
  ADXL34X_MODEL_ADXL344,
  ADXL34X_MODEL_ADXL345
} adxl34x_sensor_model_t;

// These register names are taken verbatim from the datasheet.
//

/**
 * @brief The register map for an ADXL34x series sensor.
 *
 * A thorough read-through of
 * the manufacturer's \datasheet is crucial in order to understand the specifics of their
 * usage and behaviour.
 *
 * @note For general use cases, you won't need to worry about these as the library
 * provides higher-level functions to handle dealing with registers.
 * However, if you are using more advanced functionality of the sensor that isn't yet
 * implemented in the library, you can interact with the registers yourself with the help
 * of this data type.
 *
 * **/
typedef enum {
  /**
   * The \c DEVID register at \c 0x00
   * @note Instead of reading this data directly, you can use
   * @ref adxl34x_get_device_id
   **/
  ADXL34X_DEVID_REGISTER = 0x00,
  // (Registers 0x01 through \c 0x1C are reserved)
  /** The \c THRESH_TAP register at \c 0x1D **/
  ADXL34X_THRESH_TAP_REGISTER = 0x1D,
  /** The \c OFSX register at \c 0x1E **/
  ADXL34X_OFSX_REGISTER = 0x1E,
  /** The \c OFSY register at \c 0x1F **/
  ADXL34X_OFSY_REGISTER = 0x1F,
  /** The \c OFSZ register at \c 0x20 **/
  ADXL34X_OFSZ_REGISTER,
  /** The \c DUR register at \c 0x21 **/
  ADXL34X_DUR_REGISTER,
  /** The \c LATENT register at \c 0x22 **/
  ADXL34X_LATENT_REGISTER,
  /** The \c WINDOW register at \c 0x23 **/
  ADXL34X_WINDOW_REGISTER,
  /** The \c THRESH_ACT register at \c 0x24 **/
  ADXL34X_THRESH_ACT_REGISTER,
  /** The \c TIME_INACT register at \c 0x25 **/
  ADXL34X_TIME_INACT_REGISTER,
  /** The \c THRESH_INACT register at \c 0x26 **/
  ADXL34X_THRESH_INACT_REGISTER,
  /** The \c ACT_INACT_CTL register at \c 0x27 **/
  ADXL34X_ACT_INACT_CTL_REGISTER,
  /** The \c THRESH_FF register at \c 0x28 **/
  ADXL34X_THRESH_FF_REGISTER,
  /** The \c TIME_FF register at \c 0x29 **/
  ADXL34X_TIME_FF_REGISTER,
  /** The \c TAP_AXES register at \c 0x2A **/
  ADXL34X_TAP_AXES_REGISTER,
  /** The \c TAP_STATUS register at \c 0x2B **/
  ADXL34X_ACT_TAP_STATUS_REGISTER,
  /** The \c BW_RATE register at \c 0x2C **/
  ADXL34X_BW_RATE_REGISTER,
  /** The \c POWER_CTL register at \c 0x2D **/
  ADXL34X_POWER_CTL_REGISTER,
  /** The \c INT_ENABLE register at \c 0x2E **/
  ADXL34X_INT_ENABLE_REGISTER,
  /** The \c INT_MAP register at \c 0x2F **/
  ADXL34X_INT_MAP_REGISTER,
  /**
   * The \c INT_SOURCE register at \c 0x30
   * @warning There are currently no library functions to support the functionality
   * this register provides.
   **/
  ADXL34X_INT_SOURCE_REGISTER,
  /**
   * The \c DATA_FORMAT register at \c 0x31
   * @warning There are currently no library functions to support the functionality
   * this register provides.
   **/
  ADXL34X_DATA_FORMAT_REGISTER,
  /**
   * The \c DATAX0 register at \c 0x32
   * @note Instead of reading this data directly, you can use
   * @ref adxl34x_get_raw_acceleration
   **/
  ADXL34X_DATAX0_REGISTER,
  /**
   * The \c DATAX1 register at \c 0x33
   * @note Instead of reading this data directly, you can use
   * @ref adxl34x_get_raw_acceleration
   **/
  ADXL34X_DATAX1_REGISTER,
  /**
   * The \c DATAY0 register at \c 0x34
   * @note Instead of reading this data directly, you can use
   * @ref adxl34x_get_raw_acceleration
   **/
  ADXL34X_DATAY0_REGISTER,
  /**
   * The \c DATAY1 register at \c 0x35
   * @note Instead of reading this data directly, you can use
   * @ref adxl34x_get_raw_acceleration
   **/
  ADXL34X_DATAY1_REGISTER,
  /**
   * The \c DATAZ0 register at \c 0x36
   * @note Instead of reading this data directly, you can use
   * @ref adxl34x_get_raw_acceleration
   **/
  ADXL34X_DATAZ0_REGISTER,
  /**
   * The \c DATAZ1 register at \c 0x37
   * @note Instead of reading this data directly, you can use
   * @ref adxl34x_get_raw_acceleration
   **/
  ADXL34X_DATAZ1_REGISTER,
  /** The \c FIFO_CTL register at \c 0x38 **/
  ADXL34X_FIFO_CTL_REGISTER,
  /** The \c FIFO_STATUS register at \c 0x39 **/
  ADXL34X_FIFO_STATUS_REGISTER,

  /**
   * The \c FIFO_STATUS register at \c 0x3A
   * @note This is an additional register only present on the ADXL344.
   */
  ADXL34X_TAP_SIGN,
  /**
   * The \c FIFO_STATUS register at \c 0x3B
   * @note This is an additional register only present on the ADXL344.
   */
  ADXL34X_ORIENT_CONF,
  /**
   * The \c FIFO_STATUS register at \c 0x3C
   * @note This is an additional register only present on the ADXL344.
   */
  ADXL34X_ORIENT
} adxl34x_registers_t;

/**
 * Representation of the bits for the \c POWER_CTL register.
 */
typedef enum {
  ADXL34X_LINK_BIT = 1 << 5,
  ADXL34X_AUTO_SLEEP_BIT = 1 << 4,
  ADXL34X_MEASURE_BIT = 1 << 3,
  ADXL34X_SLEEP_BIT = 1 << 2,
  ADXL34X_WAKEUP_BIT = 1 << 1
} adxl34x_power_ctl_reg_t;

/**
 * Representation of the bits for the \c DATA_FORMAT register.
 */
typedef enum {
  /** The \c SELF_TEST bit at position D7
   *  @warning There are currently no library functions to support the functionality
   *  this register bit provides.
   */
  ADXL34X_DATA_FORMAT_SELF_TEST_BIT = 1 << 7,
  /** The \c SPI bit at position D6
   *  @warning There are currently no library functions to support the functionality
   *  this register bit provides.
   */
  ADXL34X_DATA_FORMAT_SPI_BIT = 1 << 6,
  /** The \c INT_INVERT bit at position D5
   *  @warning There are currently no library functions to support the functionality
   *  this register bit provides.
   */
  ADXL34X_DATA_FORMAT_INT_INVERT_BIT = 1 << 5,
  /** The \c FULL_RES bit at position D3
   *  @warning There are currently no library functions to support the functionality
   *  this register bit provides.
   */
  ADXL34X_DATA_FORMAT_FULL_RES_BIT = 1 << 3,
  /** The \c JUSTIFY bit at position D2
   *  @warning There are currently no library functions to support the functionality
   *  this register bit provides.
   */
  ADXL34X_DATA_FORMAT_JUSTIFY_BIT = 1 << 2,
  /** Bitmask for the \c RANGE bits at position D1 and D0
   *  @warning There are currently no library functions to support the functionality
   *  this register bit provides.
   */
  ADXL34X_DATA_FORMAT_RANGE_BITMASK = 0x03
} adxl34x_data_format_reg_t;

typedef enum {
  ADXL34X_ADDRESS_DEFAULT = 0x1D,
  ADXL34X_ADDRESS_ALT = 0x53
} adxl34x_address_t;

typedef enum {
  ADXL34X_SENSITIVITY_RANGE_2G = 0x00,
  ADXL34X_SENSITIVITY_RANGE_4G = 0x01,
  ADXL34X_SENSITIVITY_RANGE_8G = 0x02,
  ADXL34X_SENSITIVITY_RANGE_16G = 0x03,
} adxl34x_sensitivity_range_t;

typedef struct {
  i2c_inst_t *i2c;
  uint8_t addr;
  adxl34x_sensitivity_range_t range;
} adxl34x_sensor_t;

typedef struct {
  union {
    int16_t raw;
    float value;
  } x;
  union {
    int16_t raw;
    float value;
  } y;
  union {
    int16_t raw;
    float value;
  } z;

  int status;
} adxl34x_accel_data_t;

typedef struct {
  uint8_t value;
  int status;
} adxl34x_result_t;

typedef enum {
  ADXL34X_NORMAL_POWER_MODE,
  // Not yet implemented
  // ADXL34X_LOW_POWER_MODE,
  // ADXL34X_AUTO_SLEEP_MODE,
  // ADLX34X_STANDBY_MODE
} adxl34x_power_mode_t;

/**
 * @brief Possible data rates for an ADXL34x.
 *
 * The data rate controls how many samples (or measurements) the sensor will take of
 * accelerometer data every second. The data rate is expressed in Hertz (Hz). A data rate
 * of 100Hz means the sensor takes a measurement 100 times a second.
 *
 * For a table of data rates, their bandwidth, and their binary representation,
 * see Table 7 and 8 in the \datasheet.
 */

typedef enum {
  /** Sets the data rate to 0.10Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_0_10_HZ,
  /** Sets the data rate to 0.20Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_0_20_HZ,
  /** Sets the data rate to 0.39Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_0_39_HZ,
  /** Sets the data rate to 0.78Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_0_78_HZ,
  /** Sets the data rate to 1.56Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_1_56_HZ,
  /** Sets the data rate to 3.13Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_3_13_HZ,
  /** Sets the data rate to 6.25Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_6_25_HZ,
  /** Sets the data rate to 12.5Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_12_5_HZ,
  /** Sets the data rate to 25Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_25_HZ,
  /** Sets the data rate to 50Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_50_HZ,
  /** Sets the data rate to 100Hz. This is the default data rate used by the sensor. */
  ADXL34X_DATA_RATE_100_HZ,
  /** Sets the data rate to 200Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_200_HZ,
  /** Sets the data rate to 400Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_400_HZ,
  /** Sets the data rate to 800Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_800_HZ,
  /** Sets the data rate to 1600Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_1600_HZ,
  /** Sets the data rate to 3200Hz.
   * @warning Not yet implemented
   */
  ADXL34X_DATA_RATE_3200_HZ,
} adxl34x_data_rate_t;

extern const uint8_t ADXL34X_DEVICE_ID;

bool adxl34x_init(adxl34x_sensor_t *sensor, adxl34x_address_t addr, i2c_inst_t *i2c);

adxl34x_result_t adxl34x_enable_measurement(adxl34x_sensor_t *sensor);

adxl34x_result_t adxl34x_set_power_and_data_rate(adxl34x_sensor_t *sensor,
						 adxl34x_power_mode_t mode,
						 adxl34x_data_rate_t rate);

/**
 * Get the device ID from the ADXL34x sensor.
 *
 * This function reads and returns the device ID from the specified ADXL34x sensor.
 *
 * @param sensor Pointer to the ADXL34x sensor struct.
 *
 * @return adxl34x_result_t The device ID of the ADXL34x sensor and a status code.
 *
 * @note The \c DEVID register (which stores the device ID) is read-only, and contains a
 * fixed value of \c 0xE5.
 */

adxl34x_result_t adxl34x_get_device_id(adxl34x_sensor_t *sensor);

adxl34x_result_t adxl34x_get_tap_threshold(adxl34x_sensor_t *sensor);

/**
 * Gets the raw, unconverted acceleration data from all 3 axes from an ADXL34x sensor.
 *
 * This function uses a multi-byte read to sequentially read the registers containing the
 * acceleration data. @note The data returned by this function is not in a useful
 * measurement such a \f$m/s^2\f$ or \f$g\f$
 *
 * @param sensor Pointer to the ADXL34x sensor struct.
 *
 * @return adxl34x_accel_data_t The device ID of the ADXL34x sensor and a status code.
 */

adxl34x_accel_data_t adxl34x_get_raw_acceleration(adxl34x_sensor_t *sensor);

/**
 * @brief This function converts raw accelerometer data to gravitational units \f$(g)\f$.
 *
 *
 * @param data Pointer to the accelerometer data structure.
 * @param sensitivity The sensitivity setting of the accelerometer.
 *
 * @note Here's the math behind it:
 * The conversion is based on the configured sensitivity setting of the accelerometer.
 * The scale factor for ±2g is derived as follows. For a 10-bit ADC, the maximum raw value
 * is:
 * @f[ \text{maximum raw value} = 2^{10} - 1 = 1023 @f]
 * Thus, the scale factor for ±2g is:
 * @f[ \text{scale factor for ±2g} = \frac{\text{total range in g's}}{\text{maximum raw
 * value}} = \frac{4.0}{1023} @f]
 *
 */
void adxl34x_convert_to_g(adxl34x_accel_data_t *data,
			  adxl34x_sensitivity_range_t sensitivity);

adxl34x_result_t adxl34x_configure_data_format(adxl34x_sensor_t *sensor, bool full_res,
					       adxl34x_sensitivity_range_t range);

/** @} */ // end of ADXL34X group
#endif	  // adxl34x_H
