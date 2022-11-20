/*
 * UartBuffer.h
 *
 *  Created on: Dec 2, 2019
 *      Author: Karl
 */

#ifndef UARTBUFFER_H_
#define UARTBUFFER_H_

#include "main.h"

enum
{
	UART_PORT_0,
	UART_PORT_1,
	UART_PORT_2,
	UART_PORT_3,
	UART_PORT_4,
	UART_PORT_5,
	UART_PORT_6,
	UART_PORT_7
};


// buffer status
enum
{
	UART_BUFFER_EMPTY,
	UART_BUFFER_OVERFLOW
};


// User defines start
/* 	MAX_UART_RX_IRQ_BYTE_LENGTH is for the UART IRQ. For strings that vary in length, this should be 1.
 *	If packets with MOD256 checksum, then change to received packet size.
 *	Currently this does not support varied size binary data with checksum, just fixed size only. In a future revision this will.
 *
 */
#define MAX_UART_RX_IRQ_BYTE_LENGTH 1

#define MAX_UART_RX_BYTE_BUFFER 192 // this holds all the IRQ data
#define MAX_UART_TX_BYTE_BUFFER 192
#define MAX_UART_RX_MESSAGE_BUFFER 3 // buffer size of complete strings or packets.
#define MAX_UART_TX_MESSAGE_BUFFER 3 // buffer size of complete strings or packets.

#define NUMBER_OF_UART_PORTS 2 // This is for how many UART ports to receive from.
#define UART_INDEX_PTR_0 UART_PORT_2 // assign index pointer 0 the UART port used
#define UART_INDEX_PTR_1 UART_PORT_6 // assign index pointer 1 the UART port used
// #define UART_INDEX_PTR_3 UART_PORT_3 // add more if needed

// end user defines




/*
 * Description: Rx byte buffer to save for the Uart IRQ data received.
 *
 */
typedef struct
{
	uint8_t data[MAX_UART_RX_IRQ_BYTE_LENGTH];
}UartRxByteBufferStruct;


/*
 * Because different MCU's uses UART port defines differently, we're going to use generic UART port number.
 * For your MCU you will convert the generic port numbers to the actual instance name when writing to the specific UART port.
 *
 */
typedef struct
{
	uint8_t uartPort;
	uint8_t data[MAX_UART_RX_BYTE_BUFFER];
	uint8_t dataSize; // used mainly for binary data
}UartRxMsgBufferStruct;

typedef struct
{
	uint8_t uartPort;
<<<<<<< HEAD
	uint8_t data[MAX_UART_TX_BYTE_BUFFER];
	uint8_t dataSize; // used mainly for binary data
}UartTxMsgBufferStruct;


void UartParseRx_CHAR_Buffer(uint8_t index);
void UartParseRx_BINARY_Buffer(uint8_t index, uint32_t packetSize);

void UartAddMessageToTxMessageBuffer(UartTxMsgBufferStruct *uartBufferPointer);
void UartSendMessage(void);

int UartAddByteToBuffer(uint8_t uartPort, uint8_t *_char, uint32_t sizeOfData);

int UartCopyStringToTxStruct(uint8_t uartPort, char *str_IN, UartTxMsgBufferStruct *uartTx_OUT);
int UartCopyBinaryDataToTxStruct(uint8_t uartPort, uint8_t *dataIN, uint32_t sizeOfData, UartTxMsgBufferStruct *uartTx_OUT);

void UartClearRxCharBuffer(void);

=======
	uint8_t data[MAX_UART_TX_CHAR_BUFFER];
	uint32_t dataLength;
}UartCharBufferTxStruct;

void UartParseRxCharBuffer(void);
void UartParseRxByteBuffer(void);

void UartAddTxMessageBuffer(UartCharBufferTxStruct *uartBufferPointer);
void UartSendMessage(void);
int UartAddCharToBuffer(uint8_t uartPort, uint8_t *_char);
int UartCopyStrToCharBufferTxStruct(uint8_t uartPort, char *str_IN, UartCharBufferTxStruct *uartTx_OUT);
int UartCopyDataToDataBufferTxStruct(uint8_t uartPort, uint8_t *data_IN, uint32_t dataLen, UartCharBufferTxStruct *uartTx_OUT);
void UartRxBuffer_DRV_RingBuffPtr__Clean(void);
>>>>>>> 2deaf86b4f3d091b55c9460ed19d632064830b35

#endif /* UARTBUFFER_H_ */
