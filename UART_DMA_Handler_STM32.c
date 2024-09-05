/*
 * UART_DMA_Handler.c
 *
 *  Created on: Jan 3, 2023
 *      Author: karl.yamashita
 *
 *      FOR STM32 with HAL_UARTEx_ReceiveToIdle_DMA and HAL_UARTEx_RxEventCallback
 *
 */

#include "main.h"
#include "UART_DMA_Handler_STM32.h"


/*
 * Description: assign uart instance to data structure, if variable was not assigned during instantiation
 * 	example: UART_DMA_RxInit(&uartDMA_RXMsg, &huart2);
 *
 */
void UART_DMA_Init(UART_DMA_QueueStruct *msg, UART_HandleTypeDef *huart)
{
	msg->huart = huart;
}

/*
 * Description: Enable rx interrupt
 *
 */
void UART_DMA_EnableRxInterrupt(UART_DMA_QueueStruct *msg)
{
	msg->rx.hal_status = HAL_UARTEx_ReceiveToIdle_DMA(msg->huart, msg->rx.queue[msg->rx.ptr.index_IN].data, UART_DMA_DATA_SIZE * 2);
}

/*
 * Description: Call from polling routine
 *
 */
void UART_DMA_CheckRxInterruptErrorFlag(UART_DMA_QueueStruct *msg)
{
	if(msg->rx.hal_status != HAL_OK)
	{
		msg->rx.hal_status = HAL_OK;
		UART_DMA_EnableRxInterrupt(msg);
	}
}


/*
 * Description: Return 0 if no new message, 1 if there is message in msgOut
 */
int UART_DMA_MsgRdy(UART_DMA_QueueStruct *msg)
{
	if(msg->rx.ptr.cnt_Handle)
	{
		msg->rx.msgToParse = &msg->rx.queue[msg->rx.ptr.index_OUT];
		RingBuff_Ptr_Output(&msg->rx.ptr, UART_DMA_QUEUE_SIZE);
		return 1;
	}

	return 0;
}

/*
* Description: Add message to TX buffer
*/
void UART_DMA_TX_AddDataToBuffer(UART_DMA_QueueStruct *msg, uint8_t *data, uint32_t size)
{
	UART_DMA_Data *ptr = &msg->tx.queue[msg->tx.ptr.index_IN];

	memcpy(ptr->data, data, size);
	ptr->size = size;

    RingBuff_Ptr_Input(&msg->tx.ptr, UART_DMA_QUEUE_SIZE);

    UART_DMA_SendMessage(msg); // Try to send message if !msg->tx.txPending
}

/*
 * Description: This will be called from UART_DMA_TX_AddDataToBuffer or from HAL_UART_TxCpltCallback
 *
 */
void UART_DMA_SendMessage(UART_DMA_QueueStruct * msg)
{
	if(msg->tx.ptr.cnt_Handle)
	{
		//if(msg->huart->gState == HAL_UART_STATE_READY) // this hasn't been tested yet but could take place of txPending
		if(!msg->tx.txPending) // If no message is being sent then send message in queue
		{
			if(HAL_UART_Transmit_DMA(msg->huart, msg->tx.queue[msg->tx.ptr.index_OUT].data, msg->tx.queue[msg->tx.ptr.index_OUT].size) == HAL_OK)
			{
				msg->tx.txPending = true;
				RingBuff_Ptr_Output(&msg->tx.ptr, UART_DMA_QUEUE_SIZE);
			}
		}
	}
}

/*
* Description: Add string to TX structure
*/
void UART_DMA_NotifyUser(UART_DMA_QueueStruct *msg, char *str, uint32_t size, bool lineFeed)
{
	uint8_t strMsg[UART_DMA_DATA_SIZE] = {0};

    strcpy((char*)strMsg, str);
    
    if(lineFeed == true)
    {
    	strcat((char*)strMsg, "\r\n");
    	size += 2; // add 2 due to CR and LF
    }

    UART_DMA_TX_AddDataToBuffer(msg, strMsg, size); // add message to queue
}


/*
 - Below is an example of checking for a new message and have msgToParse as a pointer to the queue.
 - It is totally up to the user how to send messages to the STM32 and how to parse the messages.
 - User would call UART_CheckForNewMessage(&uartDMA_RXMsg) from a polling routine

 - When you want to transmit strings, you can use UART_DMA_NotifyUser.
 - When you want to transmit bytes, you can use UART_DMA_TX_AddDataToBuffer

void UART_CheckForNewMessage(UART_DMA_QueueStruct *msg)
{
	if(UART_DMA_MsgRdy(msg))
	{
		// user can parse msg variable.
		if(strncmp(msg->msgToParse->data, "get version", strlen("get version")) == 0)
		{
			// call function to return version number
		}
		else if(strncmp(msg->msgToParse->data, "get status", strlen("get status")) == 0)
		{
			// call function to get status information
		}
	}
}

// Here are callbacks that should be placed in your polling routine or interrupt routine.
 * This is for uart1. If you have more UART instances then test for those.

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart == uart1.huart)
	{
		RingBuff_Ptr_Input(&uart1.rx.ptr, uart1.rx.queueSize);
		UART_DMA_EnableRxInterrupt(&uart1);
	}
	else if(huart == uart2.huart)
	{
		RingBuff_Ptr_Input(&uart2.rx.ptr, uart2.rx.queueSize);
		UART_DMA_EnableRxInterrupt(&uart2);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart1.huart)
	{
		uart1.tx.txPending = false;
		UART_DMA_SendMessage(&uart1);
	}
	else if(huart == uart2.huart)
	{
		uart2.tx.txPending = false;
		UART_DMA_SendMessage(&uart2);
	}
}

// Be sure to initialize UART instance in polling routine
 *
UART_DMA_QueueStruct uart1 =
{
	.huart = &hlpuart1,
	.rx.queueSize = UART_DMA_QUEUE_SIZE,
	.tx.queueSize = UART_DMA_QUEUE_SIZE
};





 */


