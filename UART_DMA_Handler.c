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

// User can use these default variables or rename them to their liking
UART_DMA_RxQueueStruct uartDMA_RXMsg = {0};
UART_DMA_TxQueueStruct uartDMA_TXMsg = {0};

/*
 * Description: Must be called prior to using UART DMA. This assigns the DMA structure with a UART Port
 * 	example: UART_DMA_RxInit(&uartDMA_RXMsg, &huart2);
 *
 */
void UART_DMA_RxInit(UART_DMA_RxQueueStruct *msg, UART_HandleTypeDef *huart)
{
	msg->huart = huart;
}
// same as UART_DMA_RxInit above
void UART_DMA_TxInit(UART_DMA_TxQueueStruct *msg, UART_HandleTypeDef *huart)
{
	msg->huart = huart;
}

/*
 * Description: Enable rx interrupt
 *
 */
void UART_DMA_EnableRxInterrupt(UART_DMA_RxQueueStruct *msg)
{
	if(HAL_UARTEx_ReceiveToIdle_DMA(msg->huart, msg->queue[msg->ptr.iIndexIN].data, UART_DMA_CHAR_SIZE) != HAL_OK)
	{
		msg->uart_dma_rxIntErrorFlag = true;
	}
}

/*
 * Description: Call from polling routine
 *
 */
void UART_DMA_CheckRxInterruptErrorFlag(UART_DMA_RxQueueStruct *msg)
{
	if(msg->uart_dma_rxIntErrorFlag)
	{
		msg->uart_dma_rxIntErrorFlag = false;
		UART_DMA_EnableRxInterrupt(msg);
	}
}

/*
 * Description: Increment pointer
 *
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == uartDMA_RXMsg.huart->Instance)
	{
		uartDMA_RXMsg.queue[uartDMA_RXMsg.ptr.iIndexIN].dataSize = Size;
		DRV_RingBuffPtr__Input(&uartDMA_RXMsg.ptr, UART_DMA_QUEUE_SIZE);
		UART_DMA_EnableRxInterrupt(&uartDMA_RXMsg);
	}
}

/*
 * Description: Return 0 if no new message, 1 if there is message in msgOut
 */
int UART_DMA_MsgRdy(UART_DMA_RxQueueStruct *msg, UART_MsgStruct *msgOut)
{
	uint32_t i;
	if(msg->ptr.iCnt_Handle)
	{
		msgOut->size = msg->queue[msg->ptr.iIndexOUT].dataSize;
		for(i = 0; i < msgOut->size; i++)
		{
			msgOut->data[i] = msg->queue[msg->ptr.iIndexOUT].data[i];
			msg->queue[msg->ptr.iIndexOUT].data[i] = 0;
		}
		DRV_RingBuffPtr__Output(&msg->ptr, UART_DMA_QUEUE_SIZE);
		return 1;
	}

	return 0;
}

/*
* Description: Add message to TX buffer
*/
void UART_DMA_TX_AddMessageToBuffer(UART_DMA_TxQueueStruct *msg, uint8_t uartPort, char *str, uint32_t size)
{

}

/*
 * Description: This must be called from a polling routine.
 *
 */
void UART_DMA_SendMessage(UART_DMA_TxQueueStruct * msg)
{
	if(msg->ptr.iCnt_Handle)
	{
		if(HAL_UART_Transmit_IT(msg->huart, msg->queue[msg->ptr.iIndexOUT].data, msg->queue[msg->ptr.iIndexOUT].dataSize) == HAL_OK)
		{
			DRV_RingBuffPtr__Output(&msg->ptr, UART_DMA_QUEUE_SIZE);
		}
	}
}

/*
* Description: Add string to TX structure
*/
void UART_DMA_NotifyUser(UART_DMA_TxQueueStruct *msg, char *str, bool lineFeed, uint8_t uartPort)
{
	uint8_t strMsg[UART_TX_BYTE_BUFFER_SIZE] = {0};

    strcpy((char*)strMsg, str);
    
    if(lineFeed == true)
    {
    	strcat((char*)strMsg, "\r\n");
    }

    msg->msgQueueSize[msg->msgPtr.iIndexIN].dataSize = strlen((char*)strMsg);
    UART_DMA_TX_AddMessageToBuffer(msg, uartPort, strMsg, strlen((char*)strMsg));
}

/*
 - Below is an example of checking for a new message and have it copied to msgNew variable.
 - It is totally up to the user how to send messages to the STM32 and how to parse the messages.
 - User would call UART_CheckForNewMessage(&uartDMA_RXMsg) from a polling routine

void UART_CheckForNewMessage(UART_DMA_RxQueueStruct *msg)
{
	UART_MsgStruct msgNew = {0};
	char *ptr = (char*)msgNew.data;

	if(UART_DMA_MsgRdy(msg, &msgNew))
	{
		// user can parse msg variable.
		if(strncmp(ptr, "get version", strlen("get version")) == 0)
		{
			// call function to return version number
		}
		else if(strncmp(ptr, "get status", strlen("get status")) == 0)
		{
			// call function to get status information
		}
	}
}

 */


