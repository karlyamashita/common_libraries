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

#define UART_RX_DATA_SIZE 128 // this holds all the IRQ data
#define UART_TX_DATA_SIZE 128
#define UART_RX_QUEUE_SIZE 8// buffer size of complete strings or packets.
#define UART_TX_QUEUE_SIZE 8 // buffer size of complete strings or packets.

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
    uint32_t sys_ctl_peripheral;
    uint32_t baudRate;
    uint32_t sysClock;
}UART_Config_t;

typedef struct
{
	uint8_t data[UART_RX_DATA_SIZE];
	uint32_t size;
}UartDataStruct;

typedef struct __UartBufferStruct
{
#ifdef USE_HAL_DRIVER
	UART_HandleTypeDef *huart;
#else
    uint32_t uart_base;
    void (*RxIRQ)(struct  __UartBufferStruct *msg); /*!< Function pointer on Rx IRQ handler */
    void (*TxIRQ)(struct __UartBufferStruct *msg); /*!< Function pointer on Tx IRQ handler */
    void (*ErrorIRQ)(struct __UartBufferStruct *msg); /*!< Function pointer on Error IRQ handler */
    uint32_t errorCode;
#endif
    struct
	{
    	uint8_t irqByte; // UART IRQ needs to save Rx byte to this variable
		UartDataStruct queue[UART_RX_QUEUE_SIZE];
		uint32_t queueSize;
		uint32_t bytePtrSize; // also used in binaryQueue
		UartDataStruct *msgToParse;

    	uint8_t binaryBuffer[UART_RX_DATA_SIZE]; // For binary data, bytes saved here as they come in from uartIRQ_ByteBuffer.
		uint32_t packetSize; // for binary packets

		RING_BUFF_STRUCT bytePtr; // pointer for byteBuffer
		RING_BUFF_STRUCT ptr; // pointer for queue
		uint8_t uartType; // UART_ASCII or UART_BINARY, default UART_ASCII
		uint8_t checksumType; // For use with binary packets. 0 = MOD256, 1 = 16 bit (not implemented yet)
#ifdef USE_HAL_DRIVER
		HAL_StatusTypeDef HAL_Status;
#endif
	}rx;
	struct
	{
		UartDataStruct queue[UART_TX_QUEUE_SIZE];
		uint32_t queueSize;
		UartDataStruct *msgToSend;
		uint32_t msgToSend_BytePtr;
		bool msgToSend_Pending;
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
