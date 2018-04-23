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

uint16_t RX_Count;
char RX_Buffer[RX_BUFFER_SIZE];

const eUSCI_UART_Config uartConfigA2 =
{
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,    // SMCLK Clock Source at 12000000 (divided down from MCLK)
     6,                                 // BRDIV (clock pre-scaler) = INT((12000000/115200=104.167/16)
     8,                                 // UCxBRF INT([(104.167/16)-INT(104.167/16)]x16))=(6.51-6)x16=8.16)
     0x11,                              // UCxBRS = 0x11 (fractional part of N from table on p.721 of tech ref)
     EUSCI_A_UART_NO_PARITY,            // No Parity
     EUSCI_A_UART_LSB_FIRST,            // LSB First
     EUSCI_A_UART_ONE_STOP_BIT,         // One stop bit
     EUSCI_A_UART_MODE,                 // UART mode
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION // Over sampling
};

/* Initializes UART on module A2. This is to be used with the ESP8266.
 * Creates and enables interrupt to handle UART TX/RX. */
void EUSCIA2_Init(void)
{
    // Selecting P3.2(RX) and P3.3(TX) in UART mode. ESP8266 UART
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 |
       GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    MAP_UART_initModule(EUSCI_A2_BASE, &uartConfigA2);      // Configuring UART Module
    MAP_UART_enableModule(EUSCI_A2_BASE);                 // Enable UART module

    MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_UART_registerInterrupt(EUSCI_A2_BASE, EUSCIA2_ISR);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA2);

    // initialize buffer
    RX_Count = 0;
    memset(RX_Buffer, '\0', RX_BUFFER_SIZE);
}

/* hold ESP on reset */
void ESP8266_Reset(void)
{
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
}


/* raise ESP from reset */
void ESP8266_Start(void)
{
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1);
}

/* EUSCI A2 UART ISR - gets data from ESP8266 on UART2, sends to terminal emulator on UART0 */
void EUSCIA2_ISR(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A2_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT)
    {
        uint8_t U2RXData = MAP_UART_receiveData(EUSCI_A2_BASE);
        RX_Buffer[RX_Count++] = U2RXData;
        MAP_UART_transmitData(EUSCI_A0_BASE, U2RXData); // send byte out UART0 port
    }
}
