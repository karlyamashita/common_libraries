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
#include "UART_DMA_Handler.h"


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
	msg->rx.HAL_Status = HAL_UARTEx_ReceiveToIdle_DMA(msg->huart, msg->rx.queue[msg->rx.ptr.index_IN].data, UART_DMA_CHAR_SIZE * 2);
}

/*
 * Description: Call from polling routine
 *
 */
void UART_DMA_CheckRxInterruptErrorFlag(UART_DMA_QueueStruct *msg)
{
	if(msg->rx.HAL_Status != HAL_OK)
	{
		msg->rx.HAL_Status = HAL_OK;
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
void UART_DMA_TX_AddMessageToBuffer(UART_DMA_QueueStruct *msg, uint8_t *str, uint32_t size)
{
    uint8_t i = 0;
    uint8_t *pData = (uint8_t*)str;

    for(i = 0; i < size; i++)
    {
    	msg->tx.queue[msg->tx.ptr.index_IN].data[i] = *pData++;
    }
    msg->tx.queue[msg->tx.ptr.index_IN].size = size;
    RingBuff_Ptr_Input(&msg->tx.ptr, UART_DMA_QUEUE_SIZE);
}

/*
 * Description: This must be called from a polling routine.
 *
 */
void UART_DMA_SendMessage(UART_DMA_QueueStruct * msg)
{
	if(msg->tx.ptr.cnt_Handle)
	{
		if(HAL_UART_Transmit_DMA(msg->huart, msg->tx.queue[msg->tx.ptr.index_OUT].data, msg->tx.queue[msg->tx.ptr.index_OUT].size) == HAL_OK)
		{
			RingBuff_Ptr_Output(&msg->tx.ptr, UART_DMA_QUEUE_SIZE);
		}
	}
}

/*
* Description: Add string to TX structure
*/
void UART_DMA_NotifyUser(UART_DMA_QueueStruct *msg, char *str, bool lineFeed)
{
	uint8_t strMsg[UART_DMA_CHAR_SIZE] = {0};

    strcpy((char*)strMsg, str);
    
    if(lineFeed == true)
    {
    	strcat((char*)strMsg, "\r\n");
    }

    msg->tx.queue[msg->tx.ptr.index_IN].size = strlen((char*)strMsg);
    UART_DMA_TX_AddMessageToBuffer(msg, strMsg, strlen((char*)strMsg));
}


/*
 - Below is an example of checking for a new message and have it copied to msgNew variable.
 - It is totally up to the user how to send messages to the STM32 and how to parse the messages.
 - User would call UART_CheckForNewMessage(&uartDMA_RXMsg) from a polling routine

void UART_CheckForNewMessage(UART_DMA_RxQueueStruct *msg)
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

 */


