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
/* 	MAX_UART_RX_IRQ_BYTE_LENGTH is for the UART IRQ.
 *	If packets with MOD256 checksum, then change to received packet size.
 *
 */

#define USE_BUFFER_POINTERS // comment in if using pointers to buffers. User needs to create buffers and call UART_RxBufferInit and UART_TxBufferInit

#define UART_RX_IRQ_BYTE_SIZE 1

#define UART_RX_BYTE_BUFFER_SIZE 128 // this holds all the IRQ data
#define UART_TX_BYTE_BUFFER_SIZE 128
#define UART_RX_MESSAGE_QUEUE_SIZE 8// buffer size of complete strings or packets.
#define UART_TX_MESSAGE_QUEUE_SIZE 8 // buffer size of complete strings or packets.


// end user defines

typedef enum CheckSumType
{
	CHECKSUM_MOD256, // 8 bit
	CHECKSUM_16 // 16 bit
}CheckSumType;

typedef struct
{
	uint8_t data[UART_RX_BYTE_BUFFER_SIZE];
	uint32_t size;
}UartMsgQueueStruct;

typedef struct
{
	uint32_t dataSize;
}UartMsgQueueSize;

typedef struct
{
    uint32_t ptr;
}SortPtr;

// receive
typedef struct
{
#ifdef USE_BUFFER_POINTERS
    uint8_t *uartIRQ_ByteBuffer;
    uint8_t *byteBuffer;
    UartMsgQueueStruct *msgQueue;
#else
    uint8_t uartIRQ_ByteBuffer[UART_RX_IRQ_BYTE_SIZE]; // UART IRQ will save to this. Typically would be 1 byte in size
    uint8_t byteBuffer[UART_RX_BYTE_BUFFER_SIZE]; // bytes saved here as they come in from uartIRQ_ByteBuffer.
    UartMsgQueueStruct msgQueue[UART_RX_MESSAGE_QUEUE_SIZE];
#endif
    uint32_t packetSize;
    uint32_t msgQueueSize[UART_RX_MESSAGE_QUEUE_SIZE]; // the message length
    uint32_t sortPtr;
    bool UART_RxEnErrorFlag; // used with STM32 with HAL functions.

    RING_BUFF_STRUCT bytePtr; // pointer for byteBuffer
    RING_BUFF_STRUCT msgPtr; // pointer for msgQueue and msgQueueSize
}UartRxBufferStruct;

// transmit
typedef struct
{
#ifdef USE_BUFFER_POINTERS
	UartMsgQueueStruct *msgQueue;
#else
	UartMsgQueueStruct msgQueue[UART_TX_MESSAGE_QUEUE_SIZE];
#endif
	uint8_t uartPort[UART_TX_MESSAGE_QUEUE_SIZE];
	uint32_t msgQueueSize[UART_TX_MESSAGE_QUEUE_SIZE];
	RING_BUFF_STRUCT msgPtr; // pointer for msgBuffer and msgDataSize
}UartTxBufferStruct;


// add to buffer
void UART_Add_IRQ_Byte(UartRxBufferStruct *msg, uint8_t *char_in, uint32_t dataSize);
int UART_AddByteToBuffer(UartRxBufferStruct *msg, uint8_t *char_in, uint32_t dataSize);

// sorting
void UART_SortRx_CHAR_Buffer(UartRxBufferStruct *msg);
void UART_SortRx_BINARY_Buffer(UartRxBufferStruct *msg, CheckSumType checkSumType);
void UART_InitPacketSize(UartRxBufferStruct *msg, uint32_t size);
// pending
bool UART_RxMessagePending(UartRxBufferStruct *msg, UartMsgQueueStruct *msgOut);
// initialize
void UART_RxBufferInit(UartRxBufferStruct *msg, uint8_t *irqData, uint8_t *buffer, UartMsgQueueStruct *queue);
void UART_TxBufferInit(UartTxBufferStruct *msg, UartMsgQueueStruct *queue);

// Tx
void UART_TX_AddDataToBuffer(UartTxBufferStruct *msgOut, uint8_t uartPort, uint8_t *msgIN, uint32_t dataSize);
void UART_SendMessage(UartTxBufferStruct *msg);




#endif /* UARTBUFFER_H_ */
