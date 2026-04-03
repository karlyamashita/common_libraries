/*
 * UART_DMA_Idle_Drv_STM32CubeMX2.c
 *
 *  Created on: Apr 2, 2026
 *      Author: karl.yamashita
 *
 *
 *  Description: This driver is updated for STM32CubeMX2 HAL driver.
 *
 *  		Some things that have changed from original HAL driver:
 *  			UART_HandleTypeDef > hal_uart_handle_t
 *  			HAL_StatusTypeDef > hal_status_t
 *  			HAL_UARTEx_RxEventCallback > HAL_UART_RxCpltCallback or HAL_UART_RxHalfCpltCallback
 *
 *  			hal_uart_handle_t is static so need to call mx_usart2_uart_gethandle() to get UART instance. See below example on how to create instance.
 *
 *
 */


#include "main.h"


/*
 * Description: Enable rx interrupt. This only needs to be called once since DMA is enabled in circular mode.
 *
 */
void UART_ReceiveIdleEnable_DMA(UART_DMA_Struct_t *msg)
{
	msg->rx.hal_status = HAL_UART_ReceiveToIdle_DMA((hal_uart_handle_t *)msg->huart, (uint8_t*)msg->dma.dma_data, UART_DMA_BUFFER_SIZE);
	if(msg->rx.hal_status != HAL_OK)
	{
		// error
		Nop();
	}
}

/*
 * Description: In the rare case in UART_DMA_EnableRxInterruptIdle that HAL_UARTEx_ReceiveToIdle_DMA above returns HAL_BUSY,
 * 				you can call this function from PollingRoutine to try again to enable interrupt.
 */
void UART_DMA_CheckHAL_Status(UART_DMA_Struct_t *msg)
{
	if(msg->rx.hal_status != HAL_OK)
	{
		UART_ReceiveIdleEnable_DMA(msg);
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
		RingBuff_Ptr_Output_V(&msg->dma.circularPtr, UART_DMA_CIRCULAR_SIZE);
		if(msg->rx.msgQueue[msg->rx.rxQueuePtr.index_IN].data[msg->dma.queueBytePtr] == '\n') // Line Feed
		{
			msg->dma.queueBytePtr += 1;
			msg->rx.msgQueue[msg->rx.rxQueuePtr.index_IN].data[msg->dma.queueBytePtr] = '\0'; // add null
			msg->rx.msgQueue[msg->rx.rxQueuePtr.index_IN].size = msg->dma.queueBytePtr;
			msg->dma.queueBytePtr = 0; // reset

			RingBuff_Ptr_Input_V(&msg->rx.rxQueuePtr, msg->rx.queueSize); // increment queue
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
		RingBuff_Ptr_Output_V(&msg->rx.rxQueuePtr, msg->rx.queueSize);
		return 1;
	}

	return 0;
}

/*
* Description: Add message to TX buffer
*/
void UART_DMA_TX_AddDataToBuffer(UART_DMA_Struct_t *msg, uint8_t *data, uint32_t size)
{
	volatile UART_DMA_Data *ptr = &msg->tx.msgQueue[msg->tx.txQueuePtr.index_IN];
	uint32_t i = 0;

	for(i = 0; i < size; i++)
	{
		ptr->data[i] = data[i];
	}
	ptr->size = size;

	RingBuff_Ptr_Input_V(&msg->tx.txQueuePtr, msg->tx.queueSize);

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
			if(HAL_UART_Transmit_DMA(msg->huart,
				(uint8_t *)msg->tx.msgQueue[msg->tx.txQueuePtr.index_OUT].data,
				msg->tx.msgQueue[msg->tx.txQueuePtr.index_OUT].size) == HAL_OK)
			{
				msg->tx.txPending = true;
				RingBuff_Ptr_Output_V(&msg->tx.txQueuePtr, msg->tx.queueSize);
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
 * Description: Copies DMA buffer data to Circular buffer based on size.
 * 				Keeps track of circularPtr and dmaPtr.
 * 				Should be called from HAL_UART_RxCpltCallback and HAL_UART_RxHalfCpltCallback.
 * Input: The UART DMA data struct, the size of data to copy
 * Output: none
 * Return: none
 *
 */
void UART_RX_Callback(UART_DMA_Struct_t *msg, uint32_t size)
{
	int i = 0;

	for(i = 0; i < size; i++)
	{
		msg->dma.circularBuffer[msg->dma.circularPtr.index_IN] = msg->dma.dma_data[msg->dma.dmaPtr.index_IN];
		RingBuff_Ptr_Input_V(&msg->dma.circularPtr, UART_DMA_CIRCULAR_SIZE);
		RingBuff_Ptr_Input_V(&msg->dma.dmaPtr, UART_DMA_BUFFER_SIZE);
	}
}


/*

********************************************************************************
*  User should add these functions to project, Example; in IRQ_Handler.c file  *
********************************************************************************

void HAL_UART_RxCpltCallback(hal_uart_handle_t *huart, uint32_t size_byte, hal_uart_rx_event_types_t rx_event)
{
	uint32_t size = 0;

	if(huart == uart2_msg.huart)
	{
		size = UART_DMA_GetSize(&uart2_msg, size_byte);
		UART_RX_Callback(&uart2_msg, size);
	}
	// else other UARTx
}

void HAL_UART_RxHalfCpltCallback(hal_uart_handle_t *huart)
{
	uint32_t size = 0;

	if(huart == uart2_msg.huart)
	{
		size = UART_DMA_GetSize(&uart2_msg, (UART_DMA_BUFFER_SIZE >> 1));
		UART_RX_Callback(&uart2_msg, size);
	}
	// else other UARTx
}

void HAL_UART_TxCpltCallback(hal_uart_handle_t *huart)
{
	if(huart == uart2_msg.huart)
	{
		uart2_msg.tx.txPending = false;
		UART_DMA_SendMessage(&uart2_msg);
	}
}

************************************************************************************
*    User will need to create instance, Example; in UART_Handler.c file            *
************************************************************************************

// UART_Handler is specific to project with UART2 instance.
#define UART2_DMA_RX_QUEUE_SIZE 10 // queue size
#define UART2_DMA_TX_QUEUE_SIZE 4

// queue for Rx and Tx messages
UART_DMA_Data uart2_dmaDataRxQueue[UART2_DMA_RX_QUEUE_SIZE] = {0};
UART_DMA_Data uart2_dmaDataTxQueue[UART2_DMA_TX_QUEUE_SIZE] = {0};

// Create uart2 instance, Example; uart2_msg.
// It's also up to user how they want to name their instance and defines.
UART_DMA_Struct_t uart2_msg =
{
	.rx.queueSize = UART2_DMA_RX_QUEUE_SIZE,
	.rx.msgQueue = uart2_dmaDataRxQueue,
	.tx.queueSize = UART2_DMA_TX_QUEUE_SIZE,
	.tx.msgQueue = uart2_dmaDataTxQueue,
	.dma.dmaPtr.SkipOverFlow = true
};

// Get UART2 handler created by CubeMX2, then enable HAL_UART_ReceiveToIdle_DMA
void UART_Init(void)
{
	uart2_msg.huart = mx_usart2_uart_gethandle();
	UART_ReceiveIdleEnable_DMA(&uart2_msg);
}




 */
