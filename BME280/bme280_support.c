/* Standard Includes */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* Driver Library */
#include "driverlib.h"

/* Project Includes */
#include "BME280/bme280.h"
#include "BME280/bme280_support.h"
#include "BME280/I2C.h"
#include "ClockSystem/ClockSystem.h"

/* Global Variables */
struct bme280_dev dev;
struct bme280_data comp_data = {0};

float BME280_GetTemperature()
{
    bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);
    return (float)(1.8 * comp_data.temperature + 32);
}

float BME280_GetHumidity()
{

    bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);
    return (float)comp_data.humidity;
}

float BME280_GetPressure()
{
    bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);
    return (float)(0.00029529980 * comp_data.pressure + 0.64);
}

void Delay1ms(void)
{
    _delay_cycles(48000000 / 1000);
}

void stream_sensor_data_normal_mode(struct bme280_dev *dev)
{
    uint8_t settings_sel;

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_1X;
    dev->settings.osr_t = BME280_OVERSAMPLING_1X;
    dev->settings.filter = BME280_FILTER_COEFF_16;
    dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

    settings_sel = BME280_OSR_PRESS_SEL;
    settings_sel |= BME280_OSR_TEMP_SEL;
    settings_sel |= BME280_OSR_HUM_SEL;
    settings_sel |= BME280_STANDBY_SEL;
    settings_sel |= BME280_FILTER_SEL;
    bme280_set_sensor_settings(settings_sel, dev);
    bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);
}

void BME280_Init(void)
{
    I2C_Init();

    dev.dev_id = BME280_I2C_ADDR_PRIM;
    dev.intf = BME280_I2C_INTF;
    dev.read = I2C_READ_STRING;
    dev.write = I2C_WRITE_STRING;
    dev.delay_ms = (void *)Delay1ms;

    bme280_init(&dev);
    stream_sensor_data_normal_mode(&dev);
}


