/*
 * UART_DMA_Handler.h
 *
 *  Created on: Jan 3, 2023
 *      Author: karl.yamashita
 */

#ifndef INC_UART_DMA_HANDLER_H_
#define INC_UART_DMA_HANDLER_H_

// USER DEFINES User can adjust these defines to fit their project requirements
#define UART_DMA_DATA_SIZE 128
#define UART_DMA_QUEUE_SIZE 8
// END USER DEFINES
// **************************************************
// ********* Do not modify code below here **********
// **************************************************
typedef struct
{
	uint32_t size; // move to top to byte align
	uint8_t data[UART_DMA_DATA_SIZE];
}UART_DMA_Data; // this is used in queue structure

typedef struct
{
	UART_HandleTypeDef *huart;
	struct
	{
		UART_DMA_Data queue[UART_DMA_QUEUE_SIZE];
		UART_DMA_Data *msgToParse;
		RING_BUFF_STRUCT ptr;
		uint32_t queueSize;
		HAL_StatusTypeDef hal_status;
	}rx;
	struct
	{
		UART_DMA_Data queue[UART_DMA_QUEUE_SIZE];
		RING_BUFF_STRUCT ptr;
		uint32_t queueSize;
		bool txPending;
	}tx;
}UART_DMA_QueueStruct;


void UART_DMA_Init(UART_DMA_QueueStruct *msg, UART_HandleTypeDef *huart);
void UART_DMA_EnableRxInterrupt(UART_DMA_QueueStruct *msg);
void UART_DMA_CheckRxInterruptErrorFlag(UART_DMA_QueueStruct *msg);
int UART_DMA_MsgRdy(UART_DMA_QueueStruct *msg);
void UART_DMA_NotifyUser(UART_DMA_QueueStruct *msg, char *str, uint32_t size, bool lineFeed);

void UART_DMA_TX_AddMessageToBuffer(UART_DMA_QueueStruct *msg, uint8_t *data, uint32_t size);
void UART_DMA_SendMessage(UART_DMA_QueueStruct * msg);


#endif /* INC_UART_DMA_HANDLER_H_ */
