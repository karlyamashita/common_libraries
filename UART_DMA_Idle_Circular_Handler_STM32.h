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
	uint32_t size;
	uint8_t data[UART_DMA_QUEUE_DATA_SIZE];
}__attribute__ ((aligned (32))) UART_DMA_Data; // this is used in queue structure

typedef struct
{
	RING_BUFF_STRUCT dmaPtr; // be sure to skip overflow
	RING_BUFF_STRUCT circularPtr;
	RING_BUFF_STRUCT rxQueuePtr;
	RING_BUFF_STRUCT txQueuePtr;
	uint32_t queueBytePtr;
	uint32_t dma_ptrLast;
}RingBuffer_t;

typedef struct
{
	uint8_t dma_data[DMA_BUFFER_SIZE];
	uint8_t circularBuffer[UART_DMA_CIRCULAR_SIZE];
	UART_DMA_Data msgQueue[UART_DMA_QUEUE_SIZE];
	UART_DMA_Data *msgToParse; // pointer to queue message
	uint32_t queueSize;
	HAL_StatusTypeDef hal_status;

	//uint32_t packetSize; // for binary packets
	//uint8_t uartType; // UART_ASCII or UART_BINARY, default UART_ASCII
	//uint8_t checksumType; // For use with binary packets. 0 = MOD256, 1 = 16 bit (not implemented yet)
}RxDataBuffer_t;

typedef struct
{
	UART_DMA_Data msgQueue[UART_DMA_QUEUE_SIZE];
	uint32_t queueSize;
	bool txPending;
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
int UART_DMA_MsgRdy(UART_DMA_Struct_t *msg);
void UART_DMA_NotifyUser(UART_DMA_Struct_t *msg, char *str, uint32_t size, bool lineFeed);

void UART_DMA_TX_AddDataToBuffer(UART_DMA_Struct_t *msg, uint8_t *data, uint32_t size);
void UART_DMA_SendMessage(UART_DMA_Struct_t * msg);


#endif /* INC_UART_DMA_HANDLER_H_ */
