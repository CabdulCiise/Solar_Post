#ifndef ESP8266_ESP8266_UART_H_
#define ESP8266_ESP8266_UART_H_

#define RX_BUFFER_SIZE 1024

extern uint16_t RX_Count;
extern char RX_Buffer[RX_BUFFER_SIZE];

void EUSCIA2_Init(void);
void EUSCIA2_ISR(void);
void ESP8266_Reset(void);
void ESP8266_Start(void);

#endif /* ESP8266_ESP8266_UART_H_ */
