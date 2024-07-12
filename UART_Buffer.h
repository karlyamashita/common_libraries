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

#define UART_RX_DATA_SIZE 192 // this holds all the IRQ data
#define UART_TX_DATA_SIZE 192
#define UART_RX_QUEUE_SIZE 8// buffer size of complete strings or packets.
#define UART_TX_QUEUE_SIZE 8 // buffer size of complete strings or packets.

// end user defines

typedef void (*UART_Callback)(void);


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
	uint8_t data[UART_RX_DATA_SIZE];
	uint32_t size;
}UartDataStruct;

typedef struct __UartBufferStruct
{
#ifdef USE_HAL_DRIVER
	UART_HandleTypeDef *huart;
#else
    uint32_t uart_base; // Can be TI's or Xilinx UART base
    uint32_t errorStatus;
    void (*ErrorIRQ)(struct __UartBufferStruct *msg); // pointer to function to call
#endif
    struct
	{
    	uint8_t irqByte; // UART IRQ needs to save Rx byte to this variable
		UartDataStruct queue[UART_RX_QUEUE_SIZE]; // the queue that holds messages
		uint32_t queueSize; // The queue size for the queue. Assign UART_RX_QUEUE_SIZE to this
		UartDataStruct *msgToParse; // pointer to rx queue
		void (*RxIRQ)(struct __UartBufferStruct *msg); // pointer to function to call

    	uint8_t binaryBuffer[UART_RX_DATA_SIZE]; // For binary data, bytes saved here as they come in from uartIRQ_ByteBuffer.
		uint32_t packetSize; // for binary packets

		RING_BUFF_STRUCT bytePtr; // pointer for byteBuffer
		RING_BUFF_STRUCT ptr; // pointer for queue
        uint32_t dataSize; // the data size for bytePtr. Assign UART_RX_DATA_SIZE to this
		uint8_t uartType; // UART_ASCII or UART_BINARY, default UART_ASCII
#ifdef USE_HAL_DRIVER
		HAL_StatusTypeDef HAL_Status;
#endif
	}rx;
	struct
	{
		UartDataStruct queue[UART_TX_QUEUE_SIZE]; // the queue that holds messages
		uint32_t queueSize; // The queue size for the queue. Assign UART_TX_QUEUE_SIZE to this
		UartDataStruct *msgToSend; // pointer to tx queue
		void (*TxIRQ)(struct __UartBufferStruct *msg); // pointer to function to call
		RING_BUFF_STRUCT ptr; // pointer for queue
#ifdef css
		volatile uint32_t msgToSend_BytePtr;
		volatile bool msgToSend_Pending;
#endif
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
int UART_TX_AddDataToBuffer(UartBufferStruct *msgOut, uint8_t *msgIN, uint32_t dataSize);


#endif /* UARTBUFFER_H_ */
