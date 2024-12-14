/*
 * UART_DMA_Handler.h
 *
 *  Created on: Jan 3, 2023
 *      Author: karl.yamashita
 */

#ifndef INC_UART_DMA_HANDLER_H_
#define INC_UART_DMA_HANDLER_H_

// USER DEFINES User can adjust these defines to fit their project requirements
#define UART_DMA_QUEUE_DATA_SIZE 128 // max packet/message size
#define UART_DMA_QUEUE_SIZE 16 // queue size
#define UART_DMA_CIRCULAR_SIZE 256 // circular buffer size
#define DMA_BUFFER_SIZE 16 // the DMA buffer size.
// END USER DEFINES
// **************************************************
// ********* Do not modify code below here **********
// **************************************************

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
	uint32_t size; // the amount of data in the data buffer
	uint8_t data[UART_DMA_QUEUE_DATA_SIZE];
}UART_DMA_Data; // this is used in queue structure

typedef struct
{
	RING_BUFF_STRUCT dmaPtr; // DMA data pointer.
	RING_BUFF_STRUCT circularPtr; // data pointer for the circular buffer
	RING_BUFF_STRUCT rxQueuePtr; // rx message queue pointer
	RING_BUFF_STRUCT txQueuePtr; // tx message queue pointer
	uint32_t queueBytePtr; // pointer to the queue when copying data from the Circular buffer to the msgQueue
	uint32_t dma_ptrLast; // copy of the last dmaPtr value. Helps calculate how many bytes to read with with DMA callback Size
}RingBuffer_t;

typedef struct
{
	uint8_t dma_data[DMA_BUFFER_SIZE]; // The DMA writes to this buffer
	uint8_t circularBuffer[UART_DMA_CIRCULAR_SIZE]; // The dma_data is copied to this circular buffer
	UART_DMA_Data msgQueue[UART_DMA_QUEUE_SIZE]; // The circular buffer contents are copied to this queue buffer
	UART_DMA_Data *msgToParse; // pointer to rx msgQueue
	uint32_t queueSize; // The msgQueue size
	HAL_StatusTypeDef hal_status;

	// TODO - write code to receive binary packets
	//uint32_t packetSize; // for binary packets
	//uint8_t uartType; // UART_ASCII or UART_BINARY, default UART_ASCII
	//uint8_t checksumType; // For use with binary packets. 0 = MOD256, 1 = 16 bit (not implemented yet)
}RxDataBuffer_t;

typedef struct
{
	UART_DMA_Data msgQueue[UART_DMA_QUEUE_SIZE]; // String packets are written to this queue to be transmitted
	uint32_t queueSize; // The msgQueue size
	bool txPending; // flag to indicate if HAL transmit is busy. HAL_UART_TxCpltCallback will clear this flag
}TxDataBuffer_t;

typedef struct
{
	UART_HandleTypeDef *huart;
	RingBuffer_t ringBuffer;
	RxDataBuffer_t rx;
	TxDataBuffer_t tx;
}__attribute__ ((aligned (32))) UART_DMA_Struct_t;


void UART_DMA_Init(UART_DMA_Struct_t *msg, UART_HandleTypeDef *huart);
void UART_DMA_EnableRxInterruptIdle(UART_DMA_Struct_t *msg);
void UART_DMA_ParseCircularBuffer(UART_DMA_Struct_t *msg);
uint32_t UART_DMA_GetSize(UART_DMA_Struct_t *msg, uint32_t Size);
int UART_DMA_RxMsgRdy(UART_DMA_Struct_t *msg);
void UART_DMA_NotifyUser(UART_DMA_Struct_t *msg, char *str, uint32_t size, bool lineFeed);

void UART_DMA_TX_AddDataToBuffer(UART_DMA_Struct_t *msg, uint8_t *data, uint32_t size);
void UART_DMA_SendMessage(UART_DMA_Struct_t * msg);


#endif /* INC_UART_DMA_HANDLER_H_ */
