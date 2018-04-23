#ifndef BME280_BME280_SUPPORT_H_
#define BME280_BME280_SUPPORT_H_

void BME280_Init(void);
void Delay1ms(void);
float BME280_GetTemperature();
float BME280_GetPressure();
float BME280_GetHumidity();

#endif /* BME280_BME280_SUPPORT_H_ */
