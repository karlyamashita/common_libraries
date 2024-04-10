/*
 * UartBuffer.h
 *
 *  Created on: Dec 2, 2019
 *      Author: Karl
 */

#ifndef UARTBUFFER_H_
#define UARTBUFFER_H_

#include "main.h"


/* 	MAX_UART_RX_IRQ_BYTE_LENGTH is for the UART IRQ.
 *	If packets with MOD256 checksum, then change to received packet size.
 *
 */
#define UART_RX_IRQ_BYTE_SIZE 1 // Typically this is 1 byte for most uC.

#define UART_RX_BYTE_BUFFER_SIZE 512 // this holds all the IRQ data
#define UART_TX_BYTE_BUFFER_SIZE 256
#define UART_RX_MESSAGE_QUEUE_SIZE 8// buffer size of complete strings or packets.
#define UART_TX_MESSAGE_QUEUE_SIZE 8 // buffer size of complete strings or packets.

// end user defines


// buffer status
enum
{
	UART_BUFFER_EMPTY,
	UART_BUFFER_OVERFLOW
};

enum UART_TYPE
{
	UART_ASCII,
	UART_BINARY
};

typedef enum CheckSumType
{
	CHECKSUM_MOD256, // 8 bit
	CHECKSUM_16 // 16 bit
}CheckSumType;

typedef struct
{
	uint8_t data[UART_RX_BYTE_BUFFER_SIZE];
	uint32_t size;
}UartDataStruct;

typedef struct
{
#ifdef USE_HAL_DRIVER
	UART_HandleTypeDef *huart;
#else
    uint32_t instance; // Can be TI's or Xilinx UART base
#endif
    struct
	{
    	uint8_t irqByte; // UART IRQ needs to save Rx byte to this variable
		UartDataStruct queue[UART_RX_MESSAGE_QUEUE_SIZE];
		UartDataStruct *msgToParse;

    	uint8_t binaryBuffer[UART_RX_BYTE_BUFFER_SIZE]; // For binary data, bytes saved here as they come in from uartIRQ_ByteBuffer.
		uint32_t packetSize; // for binary packets

		RING_BUFF_STRUCT bytePtr; // pointer for byteBuffer
		RING_BUFF_STRUCT ptr; // pointer for queue
		uint8_t uartType; // UART_ASCII or UART_BINARY, default UART_ASCII
#ifdef USE_HAL_DRIVER
		HAL_StatusTypeDef HAL_Status;
#endif
	}rx;
	struct
	{
		UartDataStruct queue[UART_TX_MESSAGE_QUEUE_SIZE];
		UartDataStruct *msgToSend;
		RING_BUFF_STRUCT ptr; // pointer for queue
	}tx;
}UartBufferStruct;

// add to buffer
void UART_AddByteToBuffer(UartBufferStruct *msg);

// sorting
void UART_SortRx_BINARY_Buffer(UartBufferStruct *msg, CheckSumType checkSumType);
void UART_InitPacketSize(UartBufferStruct *msg, uint32_t size);
// check pending message
bool UART_RxMessagePending(UartBufferStruct *msg);

// Tx
void UART_TX_AddDataToBuffer(UartBufferStruct *msgOut, uint8_t *msgIN, uint32_t dataSize);


#endif /* UARTBUFFER_H_ */
