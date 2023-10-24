/*
 * UART_DMA_Handler.h
 *
 *  Created on: Jan 3, 2023
 *      Author: karl.yamashita
 */

#ifndef INC_UART_DMA_HANDLER_H_
#define INC_UART_DMA_HANDLER_H_


#define UART_DMA_DATA_SIZE 128
#define UART_DMA_QUEUE_SIZE 4

typedef struct
{
	uint8_t data[UART_DMA_DATA_SIZE];
	uint32_t size;
}UART_DMA_Data;

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
		UART_DMA_Data queue[UART_DMA_QUEUE_SIZE * 3];
		RING_BUFF_STRUCT ptr;
		uint32_t queueSize;
	}tx;
}UART_DMA_QueueStruct;


void UART_DMA_Init(UART_DMA_QueueStruct *msg, UART_HandleTypeDef *huart);
void UART_DMA_EnableRxInterrupt(UART_DMA_QueueStruct *msg);
void UART_DMA_CheckRxInterruptErrorFlag(UART_DMA_QueueStruct *msg);
int UART_DMA_MsgRdy(UART_DMA_QueueStruct *msg);

void UART_DMA_TX_AddDataToBuffer(UART_DMA_QueueStruct *msg, uint8_t *data, uint32_t size);
void UART_DMA_SendData(UART_DMA_QueueStruct * msg);
void UART_DMA_NotifyUser(UART_DMA_QueueStruct *msg, char *str, bool lineFeed);

#endif /* INC_UART_DMA_HANDLER_H_ */
