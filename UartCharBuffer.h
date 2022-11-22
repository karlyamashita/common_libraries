/*
 * UartBuffer.h
 *
 *  Created on: Dec 2, 2019
 *      Author: Karl
 */

#ifndef UARTBUFFER_H_
#define UARTBUFFER_H_

#include "main.h"

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
#define MAX_UART_RX_MESSAGE_BUFFER 2 // buffer size of complete strings or packets.
#define MAX_UART_TX_MESSAGE_BUFFER 2 // buffer size of complete strings or packets.

#define UART_PACKET_SIZE 16 // The number of bytes for complete packet + checksum.


// end user defines




/*
 * Description: Rx byte buffer to save for the Uart IRQ data received.
 *
 */
typedef struct
{
	uint8_t data[MAX_UART_RX_BYTE_BUFFER];
	RING_BUFF_INFO ringPtr;
}UartRxByteBufferStruct;


/*
 * Because different MCU's uses UART port defines differently, we're going to use generic UART port number.
 * For your MCU you will convert the generic port numbers to the actual instance name when writing to the specific UART port.
 *
 */
typedef struct
{
	struct
	{
		uint8_t uartPort;
		uint8_t data[MAX_UART_RX_BYTE_BUFFER];
		uint8_t dataSize; // used mainly for binary data
	}BufStruct[MAX_UART_RX_MESSAGE_BUFFER];
	struct
	{
		RING_BUFF_INFO ringPtr;
	}RingBuff;
}UartRxMsgBufferStruct;

typedef struct
{
	struct
	{
		uint8_t uartPort;
		uint8_t data[MAX_UART_TX_BYTE_BUFFER];
		uint8_t dataSize; // used mainly for binary data
	}BufStruct[MAX_UART_TX_MESSAGE_BUFFER];
	struct
	{
		RING_BUFF_INFO ringPtr;
	}RingBuff;
}UartTxMsgBufferStruct;


void UartSortRx_CHAR_Buffer(void);
void UartSortRx_BINARY_Buffer(void);

void UartAddMessageToTxBuffer(UartTxMsgBufferStruct *uartBufferPointer);
void UartSendMessage(void);

int UartAddCharToBuffer(uint8_t *_char, uint32_t sizeOfData);
int UartAddByteToBuffer(uint8_t *data, uint32_t sizeOfData);

int UartCopyStringToTxStruct(uint8_t uartPort, char *str_IN, UartTxMsgBufferStruct *uartTx_OUT);
int UartCopyBinaryDataToTxStruct(uint8_t uartPort, uint8_t *dataIN, uint32_t sizeOfData, UartTxMsgBufferStruct *uartTx_OUT);

void UartClearRxCharBuffer(void);

bool UartStringMessagePending(void);
void UartStringMessageClear(void);
uint32_t UartStringMessageGetLength(void);
void UartStringMessageIncPtr(void);
void UartStringMessageCopyNoCRLF(char *retStr);
void UartStringMessageCopy(char *retStr);

#endif /* UARTBUFFER_H_ */
