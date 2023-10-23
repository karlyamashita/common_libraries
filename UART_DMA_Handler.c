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
	// We need to enable the DMA to receive twice the data size so that Half Callback is called instead of Complete Callback.   
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
 * Description: Returns 0 if no new message, 1 if there is message.
 *				msgToParse will point to the queue index
 */
int UART_DMA_MsgRdy(UART_DMA_QueueStruct *msg)
{
	if(msg->rx.ptr.cnt_Handle)
	{
		msg->rx.msgToParse = &msg->rx.queue[msg->rx.ptr.index_OUT];
		RingBuff_Ptr_Output(&msg->rx.ptr, msg->rx.queueSize);
		return 1;
	}

	return 0;
}

/*
* Description: Add data to TX buffer. Used for binary data.
				For Strings, you can call UART_DMA_NotifyUser, which will call this function.
*/
void UART_DMA_TX_AddDataToBuffer(UART_DMA_QueueStruct *msg, uint8_t *data, uint32_t size)
{
    UART_DMA_Data *ptr = &msg->tx.queue[msg->tx.ptr.index_IN];

    memcpy(ptr->data, data, size);
    ptr->size = size;
    RingBuff_Ptr_Input(&msg->tx.ptr, msg->tx.queueSize);
}

/*
 * Description: This must be called from a polling routine.
 * 				If HAL status is HAL_OK, we can increment the buffer pointer. 
 *				Else if HAL_BUSY, then we can attempt to send again when called from polling routine.
 *
 */
void UART_DMA_SendData(UART_DMA_QueueStruct * msg)
{
	if(msg->tx.ptr.cnt_Handle)
	{
		if(HAL_UART_Transmit_DMA(msg->huart, msg->tx.queue[msg->tx.ptr.index_OUT].data, msg->tx.queue[msg->tx.ptr.index_OUT].size) == HAL_OK)
		{
			RingBuff_Ptr_Output(&msg->tx.ptr, msg->tx.queueSize);
		}
	}
}

/*
* Description: Add string to TX structure. You can specifiy to add CR and LF to end of string.
*/
void UART_DMA_NotifyUser(UART_DMA_QueueStruct *msg, char *str, bool lineFeed)
{
	uint8_t strMsg[UART_DMA_DATA_SIZE] = {0};

    strcpy((char*)strMsg, str);
    
    if(lineFeed == true)
    {
    	strcat((char*)strMsg, "\r\n"); // add CR and LF to string
    }

    UART_DMA_TX_AddDataToBuffer(msg, strMsg, strlen((char*)strMsg));
}


/*
 - Below is an example of checking for a new message and have msgToParse point to the queue buffer index to be parsed.
 - User would call UART_CheckForNewMessage(&uart2Msg) from a polling routine.

void UART_CheckForNewMessage(UART_DMA_RxQueueStruct *msg)
{
	if(UART_DMA_MsgRdy(msg))
	{
		// user can parse msgToParse.
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


