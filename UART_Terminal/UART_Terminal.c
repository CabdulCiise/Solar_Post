/* Driver Library */
#include "driverlib.h"

/* Project Includes */
#include "UART_Terminal.h"

const eUSCI_UART_Config uartConfigA0 =
{
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,    // SMCLK Clock Source at 12000000 (divided down from MCLK)
     6,                                 // BRDIV (clock prescaler) = INT((12000000/115200=104.167/16)
     8,                                 // UCxBRF INT([(104.167/16)-INT(104.167/16)]x16))=(6.51-6)x16=8.16)
     0x11,                              // UCxBRS = 0x11 (fractional part of N from table on p.721 of tech ref)
     EUSCI_A_UART_NO_PARITY,            // No Parity
     EUSCI_A_UART_LSB_FIRST,            // LSB First
     EUSCI_A_UART_ONE_STOP_BIT,         // One stop bit
     EUSCI_A_UART_MODE,                 // UART mode
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION // Oversampling
};

/* EUSCI A0 ISR - gets data from PC terminal emulator, sends to ESP8266 connected to EUSCI A2 */
void EUSCIA0_ISR(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT)
    {
        uint8_t U0RXData = MAP_UART_receiveData(EUSCI_A0_BASE);
        MAP_UART_transmitData(EUSCI_A2_BASE, U0RXData);
    }
}

/* Initializes UART on module A0. This is to be used with the terminal.
 * Creates and enables interrupt to handle UART TX/RX. */
void Termianl_Init(void)
{
    /* Selecting P1.2 and P1.3 in UART mode. */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfigA0);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);

    /* Enable UART_A0_ISR */
    MAP_UART_registerInterrupt(EUSCI_A0_BASE, EUSCIA0_ISR);
    MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
}
