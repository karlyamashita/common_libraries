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
 * Description: Enable rx interrupt. This only needs to be called once since DMA is enabled in circular mode.
 *
 */
void UART_DMA_EnableRxInterruptIdle(UART_DMA_Struct_t *msg)
{
	msg->rx.hal_status = HAL_UARTEx_ReceiveToIdle_DMA(msg->huart, msg->dma.dma_data, UART_DMA_BUFFER_SIZE);
}

/*
 * Description: In the rare case in UART_DMA_EnableRxInterruptIdle that HAL_UARTEx_ReceiveToIdle_DMA above returns HAL_BUSY,
 * 				you can call this function from PollingRoutine to try again to enable interrupt.
 */
void UART_DMA_CheckHAL_Status(UART_DMA_Struct_t *msg)
{
	if(msg->rx.hal_status != HAL_OK)
	{
		UART_DMA_EnableRxInterruptIdle(msg);
	}
}

/*
 * Description: Parses circular buffer and places bytes into queue until LF is reached.
 * 				Call from Polling Routine.
 */
void UART_DMA_ParseCircularBuffer(UART_DMA_Struct_t *msg)
{
	while(msg->dma.circularPtr.cnt_Handle)
	{
		msg->rx.msgQueue[msg->rx.rxQueuePtr.index_IN].data[msg->dma.queueBytePtr] = msg->dma.circularBuffer[msg->dma.circularPtr.index_OUT];
		RingBuff_Ptr_Output(&msg->dma.circularPtr, UART_DMA_CIRCULAR_SIZE);
		if(msg->rx.msgQueue[msg->rx.rxQueuePtr.index_IN].data[msg->dma.queueBytePtr] == '\n') // Line Feed
		{
			msg->dma.queueBytePtr += 1;
			msg->rx.msgQueue[msg->rx.rxQueuePtr.index_IN].data[msg->dma.queueBytePtr] = '\0'; // add null
			msg->rx.msgQueue[msg->rx.rxQueuePtr.index_IN].size = msg->dma.queueBytePtr;
			msg->dma.queueBytePtr = 0; // reset
			RingBuff_Ptr_Input(&msg->rx.rxQueuePtr, msg->rx.queueSize); // increment queue
		}
		else
		{
			msg->dma.queueBytePtr++;
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

	if(Size < msg->dma.dma_ptrLast)
	{
		size = (UART_DMA_BUFFER_SIZE - msg->dma.dma_ptrLast) + Size;
	}
	else
	{
		size = Size - msg->dma.dma_ptrLast;
	}

	msg->dma.dma_ptrLast = Size; // update the pointer

	return size;
}

/*
 * Description: Return 0 if no new message, 1 if there is message in msgOut
 */
int UART_DMA_RxMsgRdy(UART_DMA_Struct_t *msg)
{
	if(msg->rx.rxQueuePtr.cnt_Handle)
	{
		msg->rx.msgToParse = &msg->rx.msgQueue[msg->rx.rxQueuePtr.index_OUT];
		RingBuff_Ptr_Output(&msg->rx.rxQueuePtr, msg->rx.queueSize);
		return 1;
	}

	return 0;
}

/*
* Description: Add message to TX buffer
*/
void UART_DMA_TX_AddDataToBuffer(UART_DMA_Struct_t *msg, uint8_t *data, uint32_t size)
{
	UART_DMA_Data *ptr = &msg->tx.msgQueue[msg->tx.txQueuePtr.index_IN];

	memcpy(ptr->data, data, size);
	ptr->size = size;

    RingBuff_Ptr_Input(&msg->tx.txQueuePtr, msg->tx.queueSize);

    UART_DMA_SendMessage(msg); // Try to send message if !msg->tx.txPending
}

/*
 * Description: This will be called from UART_DMA_TX_AddDataToBuffer or from HAL_UART_TxCpltCallback
 *
 */
void UART_DMA_SendMessage(UART_DMA_Struct_t * msg)
{
	if(msg->tx.txQueuePtr.cnt_Handle)
	{
		if(!msg->tx.txPending) // If no message is being sent then send message in queue
		{
			if(HAL_UART_Transmit_DMA(msg->huart, msg->tx.msgQueue[msg->tx.txQueuePtr.index_OUT].data, msg->tx.msgQueue[msg->tx.txQueuePtr.index_OUT].size) == HAL_OK)
			{
				msg->tx.txPending = true;
				RingBuff_Ptr_Output(&msg->tx.txQueuePtr, msg->tx.queueSize);
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

void UART_DMA_PrintError(UART_DMA_Struct_t *msg, char *msg_copy, uint32_t error)
{
	char str[UART_DMA_QUEUE_DATA_SIZE] = {0};

	GetErrorString(error, str);

	strcat(msg_copy, "=");
	strcat(msg_copy, str);

	UART_DMA_NotifyUser(msg, msg_copy, strlen(msg_copy), true);
}

/*
 * Description: Returns the original message + string arguments
 * Input: DMA message data structure, the original string message, the string to add to the original message
 * Output: none
 * Return: none
 */
void UART_DMA_PrintReply(UART_DMA_Struct_t *msg, char *msg_copy, char *msg2)
{
	strcat(msg_copy, "=");
	strcat(msg_copy, msg2);

	UART_DMA_NotifyUser(msg, msg_copy, strlen(msg_copy), true);
}


/*
 - Below is an example of checking for a new message and have msgToParse as a pointer to the queue.
 - It is totally up to the user how to send messages to the STM32 and how to parse the messages.
 - User would call UART_CheckForNewMessage(&uartDMA_RXMsg) from a polling routine

 - When you want to transmit strings, you can use UART_DMA_NotifyUser.
 - When you want to transmit bytes, you can use UART_DMA_TX_AddDataToBuffer


1. For each uart instance, define these in main.h
#define UART2_DMA_RX_QUEUE_SIZE 10 // queue size
#define UART2_DMA_TX_QUEUE_SIZE 4

2. Create variable buffer for each UART port
UART_DMA_Data uart2_dmaDataRxQueue[UART2_DMA_RX_QUEUE_SIZE] = {0};
UART_DMA_Data uart2_dmaDataTxQueue[UART2_DMA_TX_QUEUE_SIZE] = {0};

3. Then create UART msg instance using the above parameters
UART_DMA_Struct_t uart2_msg =
{
	.huart = &huart2,
	.rx.queueSize = UART2_DMA_RX_QUEUE_SIZE,
	.rx.msgQueue = uart2_dmaDataRxQueue,
	.tx.queueSize = UART2_DMA_TX_QUEUE_SIZE,
	.tx.msgQueue = uart2_dmaDataTxQueue,
	.dma.dmaPtr.SkipOverFlow = true
};

// Add these, typically in your polling routine
void UART_CheckForNewMessage(UART_DMA_Struct_t *msg)
{
	if(UART_DMA_RxMsgRdy(msg))
	{
		// user can parse msg variable.
		if(strncmp((char*)msg->rx.msgToParse->data, "get version", strlen("get version")) == 0)
		{
			// call function to return version number
		}
		else if(strncmp((char*)msg->rx.msgToParse->data, "get status", strlen("get status")) == 0)
		{
			// call function to get status information
		}
	}
}

// Here are callbacks that should be placed in your polling routine or interrupt routine.
// This is for UART2. If you have more UART instances then test for those.

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
			uart2_msg.dma.circularBuffer[uart2_msg.dma.circularPtr.index_IN] = uart2_msg.dma.dma_data[uart2_msg.dma.dmaPtr.index_IN];
			RingBuff_Ptr_Input(&uart2_msg.dma.circularPtr, UART_DMA_CIRCULAR_SIZE);
			RingBuff_Ptr_Input(&uart2_msg.dma.dmaPtr, UART_DMA_BUFFER_SIZE);
		}
	}
	// repeat for other UART ports using (else if)
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart2_msg.huart)
	{
		uart2_msg.tx.txPending = false;
		UART_DMA_SendMessage(&uart2_msg);
	}
	// repeat for other UART ports using (else if)
}


// be sure to call this in the PollingRoutine function (same as main while loop).
// This will extract messages from the circular buffer a place into queue.
UART_DMA_ParseCircularBuffer(&uart2_msg);

*/


