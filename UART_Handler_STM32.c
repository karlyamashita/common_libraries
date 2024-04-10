/*
 * UART_Handler.c
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */



#include "main.h"


//extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef hlpuart1;

// Init uart2 or lpuart1
UartBufferStruct uart1_msg =
{
	.huart = &hlpuart1,
	.rx.uartType = UART_ASCII,
	.rx.irqByte = 0
};


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
		msg->rx.HAL_Status = HAL_OK;
		UART_EnableRxInterrupt(msg);
	}
}

/*
 * Description: HAL UART callback.
 *
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart1_msg.huart)
	{
		UART_AddByteToBuffer(&uart1_msg);
		UART_EnableRxInterrupt(&uart1_msg);
	}
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart1_msg.huart)
	{
		UART_TxMessage_IT(&uart1_msg); // transmit more if queue is not empty
	}
}


/*
 * Description: Transmit any available messages. Call from main while loop
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
			RingBuff_Ptr_Output(&msg->tx.ptr, UART_TX_MESSAGE_QUEUE_SIZE);
		}
	}

	return status;
}

