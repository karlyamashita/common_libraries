/*
 * UartBuffer.h
 *
 *  Created on: Dec 2, 2019
 *      Author: Karl
 */

#ifndef UARTBUFFER_H_
#define UARTBUFFER_H_

#include "main.h"


// User defines start
//#define USE_BUFFER_POINTERS // comment in if using pointers to buffers. User needs to create buffers and call UART_RxBufferInit and UART_TxBufferInit

/* 	MAX_UART_RX_IRQ_BYTE_LENGTH is for the UART IRQ.
 *	If packets with MOD256 checksum, then change to received packet size.
 *
 */
#define UART_RX_IRQ_BYTE_SIZE 1 // Typically this is 1 byte for most uC.

#define UART_RX_BYTE_BUFFER_SIZE 128 // this holds all the IRQ data
#define UART_TX_BYTE_BUFFER_SIZE 128
#define UART_RX_MESSAGE_QUEUE_SIZE 8// buffer size of complete strings or packets.
#define UART_TX_MESSAGE_QUEUE_SIZE 8 // buffer size of complete strings or packets.

// end user defines


// buffer status
enum
{
	UART_BUFFER_EMPTY,
	UART_BUFFER_OVERFLOW
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

// receive
typedef struct
{
	struct
	{
#ifdef USE_BUFFER_POINTERS
		uint8_t *uartIRQ_ByteBuffer;
		uint8_t *byteBuffer;
		UartDataStruct *msgQueue;
#else
		uint8_t uartIRQ_ByteBuffer[UART_RX_IRQ_BYTE_SIZE]; // UART IRQ will save to this. Typically would be 1 byte in size
		uint8_t byteBuffer[UART_RX_BYTE_BUFFER_SIZE]; // bytes saved here as they come in from uartIRQ_ByteBuffer.
		UartDataStruct msgQueue[UART_RX_MESSAGE_QUEUE_SIZE];
#endif // USE_BUFFER_POINTERS
		UartDataStruct *msgToParse;
#ifdef HAL_MODULE_ENABLED // STM32
		UART_HandleTypeDef *huart;
		bool UART_RxEnErrorFlag; // used with STM32 with HAL functions.
#else
		uint32_t instance; // typically a UART Port number 0-n for identification purpose. CAN also be STM32 huart instance
#endif // HAL_MODULE_ENABLED
		uint32_t packetSize; // for binary packets
		uint32_t sortPtr; // for binary packet sorting

		RING_BUFF_STRUCT bytePtr; // pointer for byteBuffer
		RING_BUFF_STRUCT msgPtr; // pointer for msgQueue
		uint32_t uartIRQ_ByteSize;
		uint32_t byteBufferSize;
		uint32_t msgQueueSize;
	}rx;
	struct
	{
#ifdef USE_BUFFER_POINTERS
		UartDataStruct *msgQueue;
#else
		UartDataStruct msgQueue[UART_TX_MESSAGE_QUEUE_SIZE];
#endif
	//#ifdef TM4C12x
		bool msgToSend_Pending; // used for TM4C12x in UART_Handler.c
		uint8_t *msgToSend;
		uint32_t msgToSend_Size;
		uint32_t msgToSend_BytePtr; //
	//#endif // USE_BUFFER_POINTERS
#ifdef HAL_MODULE_ENABLED // STM32
		UART_HandleTypeDef *huart;
#else
		uint32_t instance; // typically a UART Port number 0-n for identification purpose. Can also be TI's or Xilinx UART base
#endif // HAL_MODULE_ENABLED
		RING_BUFF_STRUCT msgPtr; // pointer for msgBuffer and msgDataSize
		uint32_t msgQueueSize;
	}tx;
}UartBufferStruct;

// transmit
//typedef struct
//{
//#ifdef USE_BUFFER_POINTERS
//	UartDataStruct *msgQueue;
//#else
//	UartDataStruct msgQueue[UART_TX_MESSAGE_QUEUE_SIZE];
//#endif
////#ifdef TM4C12x
//	bool msgToSend_Pending; // used for TM4C12x in UART_Handler.c
//	uint8_t *msgToSend;
//	uint32_t msgToSend_Size;
//	uint32_t msgToSend_BytePtr; //
////#endif // USE_BUFFER_POINTERS
//#ifdef HAL_MODULE_ENABLED // STM32
//    UART_HandleTypeDef *huart;
//#else
//    uint32_t instance; // typically a UART Port number 0-n for identification purpose. Can also be TI's or Xilinx UART base
//#endif // HAL_MODULE_ENABLED
//	RING_BUFF_STRUCT msgPtr; // pointer for msgBuffer and msgDataSize
//	uint32_t msgQueueSize;
//}UartTxBufferStruct;

// add to buffer
void UART_Add_IRQ_Byte(UartBufferStruct *msg, uint8_t *char_in, uint32_t dataSize);
int UART_AddByteToBuffer(UartBufferStruct *msg, uint8_t *char_in, uint32_t dataSize);

// sorting
void UART_SortRx_CHAR_Buffer(UartBufferStruct *msg);
void UART_SortRx_BINARY_Buffer(UartBufferStruct *msg, CheckSumType checkSumType);
void UART_InitPacketSize(UartBufferStruct *msg, uint32_t size);
// check pending message
bool UART_RxMessagePending(UartBufferStruct *msg);

// When using STM32 HAL
#ifdef HAL_MODULE_ENABLED // STM32
bool UART_GetRxIntErrorFlag(UartBufferStruct *msg);
void UART_SetRxIntErrorFlag(UartBufferStruct *msg, bool status);
#endif // HAL_MODULE_ENABLED

// Tx
void UART_TX_AddDataToBuffer(UartBufferStruct *msgOut, uint8_t *msgIN, uint32_t dataSize);


#endif /* UARTBUFFER_H_ */
