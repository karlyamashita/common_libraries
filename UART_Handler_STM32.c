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
UartBufferStruct uart1 =
{
	.huart = &hlpuart1,
	.rx.uartType = UART_ASCII
};



/*
 * Description: Enables the HAL_UART_Receive_IT interrupt. Call before main while loop and in HAL_UART_RxCpltCallback
 */
void UART_EnableRxInterrupt(UartBufferStruct *msg)
{
	msg->rx.HAL_Status = HAL_UART_Receive_IT(msg->huart, msg->rx.irqByte, 1);
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
	if(huart == uart1.huart)
	{
		UART_AddByteToBuffer(&uart1);
		UART_EnableRxInterrupt(&uart1);
	}
}

/*
 * Description: STM32 IDLE callback
 *
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart == uart1.huart)
	{
		uart1.rx.queue[uart1.rx.ptr.index_IN].size = Size;
		RingBuff_Ptr_Input(&uart1.rx.ptr, UART_RX_MESSAGE_QUEUE_SIZE);
		UART_EnableRxInterrupt(&uart1);
	}
}

/*
 * Description: Transmit any available messages. Call from main while loop
 */
int UART_TxMessage_IT(UartBufferStruct *msg)
{
	int status = 0;

	if(UART_TxMessagePending(msg))
	{
		if(HAL_UART_Transmit_IT(msg->huart, msg->tx.msgToSend->data, msg->tx.msgToSend->size) == HAL_OK)
		{
			UART_TxMessagePendingDone(msg);
		}
	}

	return status;
}


/*
 * How to use:
 * 	You can use pointers to the buffers or you can use fixed size buffers. Use USE_BUFFER_POINTERS in UartCharBuffer.h file to enable/disable.
 *
 * 	Initializing buffers and passing the address to the pointers.
 * 	The optional Size is for ease of use when you know the data structure name but don't remember the define size names
 *

// define the UART2 Rx buffers
uint8_t uart2RxIrqByteBuffer[UART_RX_IRQ_BYTE_SIZE] = {0}; // irq array
uint8_t uart2RxByteBuffer[UART_RX_BYTE_BUFFER_SIZE] = {0}; // byte array
UartMsgQueueStruct uart2RxMsgQueue[UART_RX_MESSAGE_QUEUE_SIZE] = {0}; // message array
UartRxBufferStruct uart2Rx =
{
	.huart = &huart2,
	.uartIRQ_ByteBuffer = uart2RxIrqByteBuffer,
	.uartIRQ_ByteSize = UART_RX_IRQ_BYTE_SIZE, // optional
	.byteBuffer = uart2RxByteBuffer,
	.byteBufferSize = UART_RX_BYTE_BUFFER_SIZE, // optional
	.msgQueue = uart2RxMsgQueue,
	.msgQueueSize = UART_RX_MESSAGE_QUEUE_SIZE // optional
};

// define the UART2 Tx buffers
UartMsgQueueStruct uart2TxMsgQueue[UART_TX_MESSAGE_QUEUE_SIZE] = {0};
UartTxBufferStruct uart2Tx =
{
	.huart = &huart2,
	.msgQueue = uart2TxMsgQueue,
	.msgQueueSize = UART_TX_MESSAGE_QUEUE_SIZE // optional
};


* You can use the data structure name for Size or the define names like below

RingBuff_Ptr_Output(&msg->msgPtr, msg->msgQueueSize) (or) RingBuff_Ptr_Output(&msg->msgPtr, UART_TX_MESSAGE_QUEUE_SIZE);


*/
