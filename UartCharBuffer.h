/*
 * UartBuffer.h
 *
 *  Created on: Dec 2, 2019
 *      Author: Karl
 */

#ifndef UARTBUFFER_H_
#define UARTBUFFER_H_


#define MAX_UART_RX_CHAR_BUFFER_SINGLE 1 // this is for the uart IRQ array reference. For STM32 we are receiving one character at a time.
#define MAX_UART_RX_CHAR_BUFFER 192
#define MAX_UART_TX_CHAR_BUFFER 192
#define MAX_UART_RX_MESSAGE_BUFFER 3
#define MAX_UART_TX_MESSAGE_BUFFER 3


#ifndef ERRORSTATUS_H_
#define NO_ERROR 0
#define UART_BUSY 1
#endif



/*
 * Because different MCU's uses Uart port defines differently, we're going to use generic uart port names.
 * For your MCU you will convert the generic port names to the actual instance name when writing to the specific uart port.
 *
 */
typedef struct{
	uint8_t uartPort;
	uint8_t data[MAX_UART_RX_CHAR_BUFFER];
	uint32_t dataLength;
}UartCharBufferRxStruct;

typedef struct{
	uint8_t uartPort;
	uint8_t data[MAX_UART_TX_CHAR_BUFFER];
	uint32_t dataLength;
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
void UartParseRxByteBuffer(void);

void UartAddTxMessageBuffer(UartCharBufferTxStruct *uartBufferPointer);
void UartSendMessage(void);
int UartAddCharToBuffer(uint8_t uartPort, uint8_t *_char);
int UartCopyStrToCharBufferTxStruct(uint8_t uartPort, char *str_IN, UartCharBufferTxStruct *uartTx_OUT);
int UartCopyDataToDataBufferTxStruct(uint8_t uartPort, uint8_t *data_IN, uint32_t dataLen, UartCharBufferTxStruct *uartTx_OUT);
void UartClearRxCharBuffer(void);


#endif /* UARTBUFFER_H_ */
