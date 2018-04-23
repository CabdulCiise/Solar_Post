#ifndef ESP8266_ESP8266_H_
#define ESP8266_ESP8266_H_

void ESP8266_Init(void);
uint8_t ESP8266_SendCommand(char* command);

void ESP8266_SetupAP(void);
uint8_t ESP8266_SendSensorData(void);

#endif /* ESP8266_ESP8266_H_ */
