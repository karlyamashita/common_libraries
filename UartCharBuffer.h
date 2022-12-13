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
#define UART_RX_IRQ_BYTE_SIZE 1

#define UART_RX_BYTE_BUFFER_SIZE 192 // this holds all the IRQ data
#define UART_TX_BYTE_BUFFER_SIZE 192
#define UART_RX_MESSAGE_QUEUE_SIZE 8 // buffer size of complete strings or packets.
#define UART_TX_MESSAGE_QUEUE_SIZE 8 // buffer size of complete strings or packets.

#define UART_RX_PACKET_SIZE 16 // The number of bytes for complete packet + checksum.


// end user defines



typedef struct
{
	uint8_t msgData[UART_RX_BYTE_BUFFER_SIZE];
}UartMsgQueueStruct;

typedef struct
{
	uint32_t dataSize;
}UartMsgQueueSize;

typedef struct
{
    uint16_t ptr;
}SortPtr;

// receive
typedef struct
{
	struct
	{
		uint8_t uartIRQ_ByteBuffer[UART_RX_IRQ_BYTE_SIZE]; // UART IRQ will save to this. Typically would be 1 byte in size
		uint8_t byteBuffer[UART_RX_BYTE_BUFFER_SIZE]; // bytes saved here as they come in from uartIRQ_ByteBuffer.
		UartMsgQueueStruct *msgQueue;
		UartMsgQueueSize msgQueueSize[UART_RX_MESSAGE_QUEUE_SIZE]; // the message length
		SortPtr sortPtr[UART_RX_MESSAGE_QUEUE_SIZE];
		bool UART_RxEnErrorFlag; // used with STM32 with HAL functions.
	}BufStruct;
	struct
	{
		RING_BUFF_INFO bytePtr; // pointer for byteBuffer
		RING_BUFF_INFO msgPtr; // pointer for msgQueue and msgDataSize
	}RingBuff;
}UartRxBufferStruct;

// transmit
typedef struct
{
	struct
	{
		UartMsgQueueStruct *msgQueue;
		UartMsgQueueSize msgQueueSize[UART_TX_MESSAGE_QUEUE_SIZE];
	}BufStruct;
	struct
	{
		RING_BUFF_INFO msgPtr; // pointer for msgBuffer and msgDataSize
	}RingBuff;
}UartTxBufferStruct;



void UART_InitRxBuffer(UartRxBufferStruct *buffer, UartMsgQueueStruct *msgBuffer);
void UART_InitTxBuffer(UartTxBufferStruct *buffer, UartMsgQueueStruct *msgBuffer);

void UART_Add_IRQ_Byte(UartRxBufferStruct *buffer, uint8_t *char_in, uint32_t dataSize);

void UART_SortRx_CHAR_Buffer(UartRxBufferStruct *buffer);
void UART_SortRx_BINARY_Buffer(UartRxBufferStruct *buffer);

void UART_TX_AddMessageToBuffer(UartTxBufferStruct *buffer, uint8_t *msg, uint32_t dataSize);
uint32_t UART_TX_GetMessageSize(UartTxBufferStruct *buffer);
void UART_SendMessage(UartTxBufferStruct *buffer, uint8_t uartPort);

int UART_AddCharToBuffer(UartRxBufferStruct *buffer);


// string
bool UART_RxStringMessagePending(UartRxBufferStruct *buffer);
void UART_RxStringMessageClear(UartRxBufferStruct *buffer);
void UART_RxStringMessageIncPtr(UartRxBufferStruct *buffer);
void UART_RxStringMessageCopyNoCRLF(UartRxBufferStruct *buffer, char *retStr);
void UART_RxStringMessageCopy(UartRxBufferStruct *buffer, char *retStr);

//binary
bool UART_RxPacketPending(UartRxBufferStruct *buffer);
void UART_RxPacketClear(UartRxBufferStruct *buffer, uint32_t packetSize);
void UART_RxPacketIncPtr(UartRxBufferStruct *buffer);
void UART_RxPacketCopy(UartRxBufferStruct *buffer, uint32_t packetSize, uint8_t *retStr);

#endif /* UARTBUFFER_H_ */
