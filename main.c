/* DriverLib Includes */
#include "driverlib.h"

/* Project Includes */
#include "BME280/bme280_support.h"
#include "ClockSystem/ClockSystem.h"
#include "UART_Terminal/UART_Terminal.h"
#include "ESP8266/esp8266.h"

/* Function Prototypes */
void Init_System(void);
void LED_Init(void);

void main(void)
{
    uint8_t success = 0;

    Init_System();

    /* Cycle through looking for network and page requests
     * once found send webpage and repeat */
    while(1)
    {
        do
        {
            success = ESP8266_SendSensorData();
            __delay_cycles(100000000);
        } while(!success);

        //MAP_PCM_gotoLPM3();             // got to sleep
    }
}

/* Do all required startup initializations */
void Init_System(void)
{
    MAP_WDT_A_holdTimer();          // halting the Watchdog
    LED_Init();                     // drive LEDs low

    Setup_Clocks();                 // setting MCLK and SMCLK

    BME280_Init();                  // setup BME sensor

    Termianl_Init();                // UART setup for terminal
    ESP8266_Init();                 // setup for ESP8266 module and internet access

    Timer32_Init();
}

/* Turn all MSP432 LEDs off */
void LED_Init(void)
{
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
}
