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

#define UART_RX_BYTE_BUFFER_SIZE 128 // this holds all the IRQ data
#define UART_TX_BYTE_BUFFER_SIZE 128
#define UART_RX_MESSAGE_QUEUE_SIZE 8// buffer size of complete strings or packets.
#define UART_TX_MESSAGE_QUEUE_SIZE 8 // buffer size of complete strings or packets.


// end user defines



typedef struct
{
	uint8_t data[UART_RX_BYTE_BUFFER_SIZE];
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
    uint8_t uartIRQ_ByteBuffer[UART_RX_IRQ_BYTE_SIZE]; // UART IRQ will save to this. Typically would be 1 byte in size
    uint8_t byteBuffer[UART_RX_BYTE_BUFFER_SIZE]; // bytes saved here as they come in from uartIRQ_ByteBuffer.
    uint32_t packetSize;
    
    UartMsgQueueStruct msgQueue[UART_RX_MESSAGE_QUEUE_SIZE];
    UartMsgQueueSize msgQueueSize[UART_RX_MESSAGE_QUEUE_SIZE]; // the message length
    
    SortPtr sortPtr[UART_RX_MESSAGE_QUEUE_SIZE];
    bool UART_RxEnErrorFlag; // used with STM32 with HAL functions.

	RING_BUFF_INFO bytePtr; // pointer for byteBuffer
	RING_BUFF_INFO msgPtr; // pointer for msgQueue and msgQueueSize
}UartRxBufferStruct;

// transmit
typedef struct
{
    uint8_t uartPort[UART_TX_MESSAGE_QUEUE_SIZE];
    UartMsgQueueStruct msgQueue[UART_TX_MESSAGE_QUEUE_SIZE];
    UartMsgQueueSize msgQueueSize[UART_TX_MESSAGE_QUEUE_SIZE];

    RING_BUFF_INFO msgPtr; // pointer for msgBuffer and msgDataSize
}UartTxBufferStruct;


void UART_Add_IRQ_Byte(UartRxBufferStruct *msg, uint8_t *char_in, uint32_t dataSize);

void UART_SortRx_CHAR_Buffer(UartRxBufferStruct *msg);
void UART_SortRx_BINARY_Buffer(UartRxBufferStruct *msg);
void UART_InitPacketSize(UartRxBufferStruct *msg, uint32_t size);

void UART_TX_AddMessageToBuffer(UartTxBufferStruct *msgOut, uint8_t uartPort, uint8_t *msgIN, uint32_t dataSize);
uint32_t UART_TX_GetMessageSize(UartTxBufferStruct *msg);
void UART_SendMessage(UartTxBufferStruct *msg);

int UART_AddCharToBuffer(UartRxBufferStruct *msg);


// string
bool UART_RxStringMessagePending(UartRxBufferStruct *msg, UartMsgQueueStruct *msgOut);
void UART_RxStringMessageCopyNoCRLF(UartRxBufferStruct *msg, char *retStr);
void UART_RxStringMessageCopy(UartRxBufferStruct *msg, char *retStr);

#endif /* UARTBUFFER_H_ */
