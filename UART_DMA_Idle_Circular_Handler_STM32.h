/*
 * UART_DMA_Handler.h
 *
 *  Created on: Jan 3, 2023
 *      Author: karl.yamashita
 */

#ifndef INC_UART_DMA_HANDLER_H_
#define INC_UART_DMA_HANDLER_H_

// USER DEFINES User can adjust these defines to fit their project requirements
#define UART_DMA_DATA_SIZE 128 // max packet/message size
#define UART_DMA_QUEUE_SIZE 8 // queue size
#define DMA_BUFFER_SIZE 32 // the DMA buffer size
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
	uint8_t data[UART_DMA_DATA_SIZE];
}__attribute__ ((aligned (32))) UART_DMA_Data; // this is used in queue structure

typedef struct
{
	UART_HandleTypeDef *huart;
	struct
	{
		// DMA data buffer
		uint8_t dma_data[DMA_BUFFER_SIZE];
		RING_BUFF_STRUCT dma_ptr; // be sure to skip overflow
		uint32_t dma_ptrLast; // keep track of DMA last Size

		// Circular buffer. DMA data gets copied to this circular buffer
		uint8_t circularBuffer[UART_DMA_DATA_SIZE];
		RING_BUFF_STRUCT circularPtr;

		// Queue buffer. The circular buffer is parsed and placed into queue
		UART_DMA_Data queue[UART_DMA_QUEUE_SIZE];
		UART_DMA_Data *msgToParse; // pointer to queue message
		RING_BUFF_STRUCT ptr;

		uint32_t packetSize; // for binary packets
		uint8_t uartType; // UART_ASCII or UART_BINARY, default UART_ASCII
		uint8_t checksumType; // For use with binary packets. 0 = MOD256, 1 = 16 bit (not implemented yet)

		uint32_t queueBytePtr;
		uint32_t queueSize; // init with UART_DMA_QUEUE_SIZE
		HAL_StatusTypeDef hal_status;
	}rx;
	struct
	{
		UART_DMA_Data queue[UART_DMA_QUEUE_SIZE];
		RING_BUFF_STRUCT ptr;
		uint32_t queueSize;
		bool txPending;
	}tx;
}__attribute__ ((aligned (32))) UART_DMA_QueueStruct;


void UART_DMA_Init(UART_DMA_QueueStruct *msg, UART_HandleTypeDef *huart);
void UART_DMA_EnableRxInterruptIdle(UART_DMA_QueueStruct *msg);
void UART_DMA_ParseCircularBuffer(UART_DMA_QueueStruct *msg);
uint32_t UART_DMA_GetSize(UART_DMA_QueueStruct *msg, uint32_t Size);
int UART_DMA_MsgRdy(UART_DMA_QueueStruct *msg);
void UART_DMA_NotifyUser(UART_DMA_QueueStruct *msg, char *str, uint32_t size, bool lineFeed);

void UART_DMA_TX_AddDataToBuffer(UART_DMA_QueueStruct *msg, uint8_t *data, uint32_t size);
void UART_DMA_SendMessage(UART_DMA_QueueStruct * msg);


#endif /* INC_UART_DMA_HANDLER_H_ */
