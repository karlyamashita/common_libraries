/*
 * UART_Handler.c
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */



#include "main.h"

extern UartBufferStruct uart2;

/*
 * Description: Enables the HAL_UART_Receive_IT interrupt. Call before main while loop and in HAL_UART_RxCpltCallback
 */
void UART_EnableRxInterrupt(UartBufferStruct *msg)
{
	msg->rx.HAL_Status = HAL_UART_Receive_IT(msg->huart, &msg->rx.irqByte, 1);
}

/*
 * Description: If error flag then call UART_EnableRxInterrupt. Call from main while loop.
 *
 */
void UART_CheckRxIntError(UartBufferStruct *msg)
{
	if(msg->rx.HAL_Status != HAL_OK)
	{
		UART_EnableRxInterrupt(msg);
	}
}

/*
 * Description: Transmit any available messages.
 */
int UART_TxMessage_IT(UartBufferStruct *msg)
{
	int status = 0;
	UartDataStruct *ptr;

	if(msg->tx.ptr.cnt_Handle)
	{
		ptr = &msg->tx.queue[msg->tx.ptr.index_OUT];

		if(HAL_UART_Transmit_IT(msg->huart, ptr->data, ptr->size) == HAL_OK)
		{
			RingBuff_Ptr_Output(&msg->tx.ptr, UART_TX_QUEUE_SIZE);
		}
	}

	return status;
}


// Example callback that need to be created, usually in pollingroutine

/*

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart2.huart)
	{
		UART_AddByteToBuffer(&uart2);
		UART_EnableRxInterrupt(&uart2);
	}
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart2.huart)
	{
		UART_TxMessage_IT(&uart2); // transmit more if queue is not empty
	}
}

// example create uart variable
UartBufferStruct uart2 =
{
	.huart = &huart2,
	.rx.queueSize = UART_RX_MESSAGE_QUEUE_SIZE,
	.tx.queueSize = UART_TX_MESSAGE_QUEUE_SIZE,
	.rx.uartType = UART_ASCII,
	.rx.irqByte = 0
};


 */
