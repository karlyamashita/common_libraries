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


/*
 * Description: assign uart instance to data structure, if variable was not assigned during instantiation
 * 	example: UART_DMA_RxInit(&uartDMA_RXMsg, &huart2);
 *
 */
void UART_DMA_Init(UART_DMA_Struct_t *msg, UART_HandleTypeDef *huart)
{
	msg->huart = huart;
}

/*
 * Description: Enable rx interrupt
 *
 */
void UART_DMA_EnableRxInterruptIdle(UART_DMA_Struct_t *msg)
{
	msg->rx.hal_status = HAL_UARTEx_ReceiveToIdle_DMA(msg->huart, msg->rx.dma_data, DMA_BUFFER_SIZE);
}

/*
 * Description: Parses circular buffer and places bytes into queue until LF is reached.
 * 				Call from Polling Routine.
 */
void UART_DMA_ParseCircularBuffer(UART_DMA_Struct_t *msg)
{
	while(msg->ringBuffer.circularPtr.cnt_Handle)
	{
		msg->rx.msgQueue[msg->ringBuffer.rxQueuePtr.index_IN].data[msg->ringBuffer.queueBytePtr] = msg->rx.circularBuffer[msg->ringBuffer.circularPtr.index_OUT];
		RingBuff_Ptr_Output(&msg->ringBuffer.circularPtr, UART_DMA_CIRCULAR_SIZE);
		if(msg->rx.msgQueue[msg->ringBuffer.rxQueuePtr.index_IN].data[msg->ringBuffer.queueBytePtr] == '\n') // Line Feed
		{
			msg->ringBuffer.queueBytePtr += 1;
			msg->rx.msgQueue[msg->ringBuffer.rxQueuePtr.index_IN].data[msg->ringBuffer.queueBytePtr] = '\0'; // add null
			msg->rx.msgQueue[msg->ringBuffer.rxQueuePtr.index_IN].size = msg->ringBuffer.queueBytePtr;
			msg->ringBuffer.queueBytePtr = 0; // reset
			RingBuff_Ptr_Input(&msg->ringBuffer.rxQueuePtr, msg->rx.queueSize); // increment queue
		}
		else
		{
			msg->ringBuffer.queueBytePtr++;
		}
	}
}

/*
 * Description: Calculate the amount of data available in DMA data buffer to copy
 * 				Size is the HAL pointer to where the last byte of data is in the data array
 * Return: size, the amount of data to copy
 */
uint32_t UART_DMA_GetSize(UART_DMA_Struct_t *msg, uint32_t Size)
{
	uint32_t size = 0;

	if(Size < msg->ringBuffer.dma_ptrLast)
	{
		size = (DMA_BUFFER_SIZE - msg->ringBuffer.dma_ptrLast) + Size;
	}
	else
	{
		size = Size - msg->ringBuffer.dma_ptrLast;
	}

	msg->ringBuffer.dma_ptrLast = Size; // update the pointer

	return size;
}

/*
 * Description: Return 0 if no new message, 1 if there is message in msgOut
 */
int UART_DMA_RxMsgRdy(UART_DMA_Struct_t *msg)
{
	if(msg->ringBuffer.rxQueuePtr.cnt_Handle)
	{
		msg->rx.msgToParse = &msg->rx.msgQueue[msg->ringBuffer.rxQueuePtr.index_OUT];
		RingBuff_Ptr_Output(&msg->ringBuffer.rxQueuePtr, msg->rx.queueSize);
		return 1;
	}

	return 0;
}

/*
* Description: Add message to TX buffer
*/
void UART_DMA_TX_AddDataToBuffer(UART_DMA_Struct_t *msg, uint8_t *data, uint32_t size)
{
	UART_DMA_Data *ptr = &msg->tx.msgQueue[msg->ringBuffer.txQueuePtr.index_IN];

	memcpy(ptr->data, data, size);
	ptr->size = size;

    RingBuff_Ptr_Input(&msg->ringBuffer.txQueuePtr, msg->tx.queueSize);

    UART_DMA_SendMessage(msg); // Try to send message if !msg->tx.txPending
}

/*
 * Description: This will be called from UART_DMA_TX_AddDataToBuffer or from HAL_UART_TxCpltCallback
 *
 */
void UART_DMA_SendMessage(UART_DMA_Struct_t * msg)
{
	if(msg->ringBuffer.txQueuePtr.cnt_Handle)
	{
		if(!msg->tx.txPending) // If no message is being sent then send message in queue
		{
			if(HAL_UART_Transmit_DMA(msg->huart, msg->tx.msgQueue[msg->ringBuffer.txQueuePtr.index_OUT].data, msg->tx.msgQueue[msg->ringBuffer.txQueuePtr.index_OUT].size) == HAL_OK)
			{
				msg->tx.txPending = true;
				RingBuff_Ptr_Output(&msg->ringBuffer.txQueuePtr, msg->tx.queueSize);
			}
		}
	}
}

/*
* Description: Add string to TX structure
*/
void UART_DMA_NotifyUser(UART_DMA_Struct_t *msg, char *str, uint32_t size, bool lineFeed)
{
	uint8_t strMsg[UART_DMA_QUEUE_DATA_SIZE] = {0};

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
 * This is for UART2. If you have more UART instances then test for those.

//
// Description: Copy the DMA buffer data to circular buffer. The circular buffer will be parsed in polling routine.
//
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	int i = 0;
	uint32_t size = 0;

	if(huart == uart2_msg.huart)
	{
		size = UART_DMA_GetSize(&uart2_msg, Size);

		for(i = 0; i < size; i++)
		{
			uart2_msg.rx.circularBuffer[uart2_msg.ringBuffer.circularPtr.index_IN] = uart2_msg.rx.dma_data[uart2_msg.ringBuffer.dmaPtr.index_IN];
			RingBuff_Ptr_Input(&uart2_msg.ringBuffer.circularPtr, UART_DMA_CIRCULAR_SIZE);
			RingBuff_Ptr_Input(&uart2_msg.ringBuffer.dmaPtr, DMA_BUFFER_SIZE);
		}
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart1_msg.huart)
	{
		uart1_msg.tx.txPending = false;
		UART_DMA_SendMessage(&uart1_msg);
	}
	else if(huart == uart2_msg.huart)
	{
		uart2_msg.tx.txPending = false;
		UART_DMA_SendMessage(&uart2_msg);
	}
}

// Be sure to initialize UART instance in polling routine
 *
UART_DMA_Struct_t uart2_msg =
{
	.huart = &huart2,
	.rx.queueSize = UART_DMA_QUEUE_SIZE,
	.tx.queueSize = UART_DMA_QUEUE_SIZE,
	.ringBuffer.dmaPtr.SkipOverFlow = true // ignore overflow
};


 */


