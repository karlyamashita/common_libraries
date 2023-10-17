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

// User can use these default variables or rename them to their liking.
// If not using, be sure to change name in HAL_UARTEx_RxEventCallback below.
UART_DMA_QueueStruct uartDMA_Msg = {0};


/*
 * Description: Must be called prior to using UART DMA. This assigns the DMA structure with a UART Port
 * 	example: UART_DMA_RxInit(&uartDMA_RXMsg, &huart2);
 *
 */
void UART_DMA_RxInit(UART_DMA_QueueStruct *msg, UART_HandleTypeDef *huart)
{
	msg->rx.huart = huart;
}
// same as UART_DMA_RxInit above
void UART_DMA_TxInit(UART_DMA_QueueStruct *msg, UART_HandleTypeDef *huart)
{
	msg->tx.huart = huart;
}

/*
 * Description: Enable rx interrupt. Set flag if not succesful
 *
 */
void UART_DMA_EnableRxInterrupt(UART_DMA_QueueStruct *msg)
{
	if(HAL_UARTEx_ReceiveToIdle_DMA(msg->rx.huart, msg->rx.queue[msg->rx.ptr.index_IN].data, UART_DMA_CHAR_SIZE) != HAL_OK)
	{
		msg->rx.uart_dma_rxIntErrorFlag = true;
	}
}

/*
 * Description: Call from polling routine. Try to enable Rx interrupt again if there was
 *				an error on the previous attempt.
 *
 */
void UART_DMA_CheckRxInterruptErrorFlag(UART_DMA_QueueStruct *msg)
{
	if(msg->rx.uart_dma_rxIntErrorFlag)
	{
		msg->rx.uart_dma_rxIntErrorFlag = false;
		UART_DMA_EnableRxInterrupt(msg);
	}
}

/*
 * Description: Increment ring buffer pointer and enable DMA Rx interrupt
 *				for next packet of data
 *
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart == uartDMA_Msg.rx.huart)
	{
		uartDMA_Msg.rx.queue[uartDMA_Msg.rx.ptr.index_IN].size = Size;
		RingBuff_Ptr_Input(&uartDMA_Msg.rx.ptr, UART_DMA_QUEUE_SIZE);
		UART_DMA_EnableRxInterrupt(&uartDMA_Msg);
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
 *				Only increment ring buffer pointer if HAL status is HAL_OK.
 *
 */
void UART_DMA_SendMessage(UART_DMA_QueueStruct * msg)
{
	if(msg->tx.ptr.cnt_Handle)
	{
		if(HAL_UART_Transmit_DMA(msg->tx.huart, msg->tx.queue[msg->tx.ptr.index_OUT].data, msg->tx.queue[msg->tx.ptr.index_OUT].size) == HAL_OK)
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
 - Below is an example of checking for a new message and have msgNew point to it.
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


