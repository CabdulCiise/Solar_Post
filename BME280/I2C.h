#ifndef BME_I2C_H
#define BME_I2C_H

/* Standard Includes */
#include "stdint.h"

#define BOSCH_SENSOR_ADDR 0x76

void I2C_Init(void);
int8_t I2C_WRITE_STRING(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t byteCount);
int8_t I2C_READ_STRING(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t byteCount);

#endif /* BME_I2C_H */
