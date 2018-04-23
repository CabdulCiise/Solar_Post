/* Standard Includes */
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* DriverLib Includes */
#include "driverlib.h"

/* Project Includes */
#include "ClockSystem/ClockSystem.h"
#include "BME280/bme280_support.h"
#include "esp8266_uart.h"
#include "esp8266.h"

#define HTML_SIZE 2048

char strHTML[HTML_SIZE];
uint8_t channel = 0;

void ESP8266_Init(void)
{
    // EN pin on ESP8266 pulled high
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0);
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0);

    // RST pin on ESP8266 pulled low to reset, pull high with 10K
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN1);
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1);

    EUSCIA2_Init();

    ESP8266_Reset();

    // setup server
    ESP8266_SetupAP();
}

void ESP8266_SetupAP(void)
{
    // pull ESP from reset
    ESP8266_Start();

    // wait for startup sequence
    __delay_cycles(1000000);

    // Set WiFi mode
    ESP8266_SendCommand("AT+CWMODE=1") ? printf("1\n") : printf("0\n");

    // Connect to personal hotspot
    ESP8266_SendCommand("AT+CWJAP=\"iPhone\",\"eskisehir01\"") ? printf("1\n") : printf("0\n");
}

uint8_t ESP8266_SendSensorData(void)
{
    // clear response buffer
    RX_Count = 0;
    memset(RX_Buffer, '\0', RX_BUFFER_SIZE);

    char ESP8266String[150] = ""; memset(ESP8266String, '\0', 150);
    char PostSensorData[150] = ""; memset(PostSensorData, '\0', 150);

    // connect to Google pushingbox API
    sprintf(ESP8266String, "AT+CIPSTART=\"TCP\",\"api.pushingbox.com\",80");

    if(!ESP8266_SendCommand(ESP8266String))
        return 0;

    __delay_cycles(1000000);

    sprintf(PostSensorData,"GET /pushingbox?devid=v044C7149636F3E6&temperature=%.1f&humidity=%.1f&pressure=%.2f "
        "HTTP/1.1\r\nHost: api.pushingbox.com\r\nUser-Agent: ESP8266/1.0\r\nConnection: close\r\n",
        BME280_GetTemperature(),
        BME280_GetHumidity(),
        BME280_GetPressure());

    // send api request for encrypting sensor data
    memset(ESP8266String, '\0', 150);
    sprintf(ESP8266String, "AT+CIPSEND=%d", strlen(PostSensorData)+2);

    if(!ESP8266_SendCommand(ESP8266String))
        return 0;

    __delay_cycles(1000000);

    if(!ESP8266_SendCommand(PostSensorData))
        return 0;

    __delay_cycles(100000);

    return 1;
}

uint8_t ESP8266_SendCommand(char* command)
{
    uint16_t success = 0, i = 0;

    // send command to ESP8266
    for(i = 0; i < strlen(command); i++)
    {
        MAP_UART_transmitData(EUSCI_A2_BASE, command[i]);
    }
    MAP_UART_transmitData(EUSCI_A2_BASE, '\r');
    MAP_UART_transmitData(EUSCI_A2_BASE, '\n');

    // wait for response by looking for possible responses
    while(strstr(RX_Buffer, "OK") == NULL &&
          strstr(RX_Buffer, "ERROR") == NULL &&
          strstr(RX_Buffer, "FAIL") == NULL);

    // received success response
    if(strstr(RX_Buffer, "OK") != NULL)
    {
        success = 1;
    }

    // clear response buffer
    RX_Count = 0;
    memset(RX_Buffer, '\0', RX_BUFFER_SIZE);

    return success;
}
