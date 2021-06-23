/*
 * UartBuffer.h
 *
 *  Created on: Dec 2, 2019
 *      Author: Karl
 */

#ifndef UARTBUFFER_H_
#define UARTBUFFER_H_

#include "main.h"

/*
 * Because different MCU's uses Uart port defines differently, we're going to use generic uart port names.
 * For your MCU you will convert the generic port names to the actual instance name when writing to the specific uart port.
 *
 */
typedef struct{
	uint8_t uartPort;
	uint8_t data[MAX_UART_RX_CHAR_BUFFER];
}UartCharBufferRxStruct;

typedef struct{
	uint8_t uartPort;
	uint8_t data[MAX_UART_TX_CHAR_BUFFER];
}UartCharBufferTxStruct;

#define UART_PORT_0 0
#define UART_PORT_1 1
#define UART_PORT_2 2
#define UART_PORT_3 3
#define UART_PORT_4 4
#define UART_PORT_5 5
#define UART_PORT_6 6
#define UART_PORT_7 7

void UartParseRxCharBuffer(void);
void UartAddTxMessageBuffer(UartCharBufferTxStruct *uartBufferPointer);
void UartSendMessage(void);
HAL_StatusTypeDef UartAddCharToBuffer(uint8_t uartPort, char *_char);
int UartCopyStrToCharBufferTxStruct(uint8_t uartPort, UartCharBufferTxStruct *uartTx_OUT, char *str);
void UartClearRxCharBuffer(void);


#endif /* UARTBUFFER_H_ */
