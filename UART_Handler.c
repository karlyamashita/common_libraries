/*
 * UART_Handler.c
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */



#include "main.h"


#ifdef HAL_MODULE_ENABLED // STM32

//extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef hlpuart1;


// Init uart2 or lpuart1
UartBufferStruct uart1 =
{
	.huart = &hlpuart1,
	.rx.uartType = UART_BINARY
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

	if(msg->tx.ptr.cnt_Handle)
	{
		if(HAL_UART_Transmit_IT(msg->huart, msg->tx.queue[msg->tx.ptr.index_OUT].data, msg->tx.queue[msg->tx.ptr.index_OUT].size) == HAL_OK)
		{
			RingBuff_Ptr_Output(&msg->tx.ptr, UART_RX_MESSAGE_QUEUE_SIZE);
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

#endif

#ifdef ccs


/*
 * Description: Init uart0 buffers
 */
UartBufferStruct uart0 =
{
    .instance = UART0_BASE,
    .rx.msgQueueSize = UART_RX_MESSAGE_QUEUE_SIZE,
    .tx.msgQueueSize = UART_TX_MESSAGE_QUEUE_SIZE
};

static void UART_TransmitMessage(UartBufferStruct *msg);

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
// this function should be registered when initializing the UART, UARTIntRegister(UART0_BASE, USART0_IRQHandler);
void USART0_IRQHandler(void)
{
    uint32_t ui32Status;
    uint8_t data[1];

    //
    // Get the interrupt status.
    //
    ui32Status = UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    MAP_UARTIntClear(UART0_BASE, ui32Status);

    if((ui32Status & UART_INT_TX) == UART_INT_TX)
    {
        UART_TransmitMessage(&uart0);
    }
    else if((ui32Status & UART_INT_RX) == UART_INT_RX)
    {
        //
        // Save available character in buffer.
        //
        if(MAP_UARTCharsAvail(UART0_BASE))
        {
            data[0] = (uint8_t)UARTCharGet(UART0_BASE);
            UART_IncrementCharPointer(&uart0, data, 1);
        }
    }
    else
    {
        // some other interrupt
        ToggleLedGrn();
    }
}


/*
 * Description: This will enable TX interrupt start the first byte transmit and continue interrupts until done.
 *              Must call from polling routine
 * Input: the UART data structure
 *
 */
int UART_TxMessage_IT(UartBufferStruct *msg)
{
    int status = 0;

    if(msg->tx.msgToSend_Pending == true) return UART_TX_PENDING;

    if(msg->tx.queuePtr.cnt_Handle)
    {
        msg->tx.msgToSend = &msg->tx.queue[msg->tx.queuePtr.index_OUT]; // set msgToSend to buffer index
        RingBuff_Ptr_Output(&msg->tx.queuePtr, msg->tx.msgQueueSize); // increment buffer index

        if(msg->tx.msgToSend->size > 1)
        {
            msg->tx.msgToSend_BytePtr = 0;
            msg->tx.msgToSend_Pending = true;
            UARTIntEnable(msg->instance, UART_INT_TX); // enable tx interrupts
            UART_TransmitMessage(msg); // start the transmit
        }
        else if(msg->tx.msgToSend->size == 1) // we can poll since this only 1 byte
        {
            UART_TxMessage(msg); // non interrupt send
        }
        else // the queue has no bytes to send.
        {
            return 1; // return non zero is an error
        }
    }

    return status;
}

/*
 * Description:
 */
static void UART_TransmitMessage(UartBufferStruct *msg)
{
    if(msg->tx.msgToSend_BytePtr < msg->tx.msgToSend->size)
    {
        if(UARTCharPutNonBlocking(msg->instance, msg->tx.msgToSend->data[msg->tx.msgToSend_BytePtr]) == true)
        {
            if(++msg->tx.msgToSend_BytePtr == msg->tx.msgToSend->size)
            {
                msg->tx.msgToSend_Pending = false; // clear for next message to be sent
                UARTIntDisable(msg->instance, UART_INT_TX); // done transmitting bytes, so disable the Tx interrupt
            }
        }
    }
}

/*
 * Description: UART Blocking method
 *
 */
int UART_TxMessage(UartBufferStruct *msg)
{
    int i = 0;

    if(msg->tx.queuePtr.cnt_Handle)
    {
        msg->tx.msgToSend = &msg->tx.queue[msg->tx.queuePtr.index_OUT];

        RingBuff_Ptr_Output(&msg->tx.queuePtr, msg->tx.msgQueueSize);

        for(i = 0; i < msg->tx.msgToSend->size; i++)
        {
            UARTCharPut(msg->instance, msg->tx.msgToSend->data[i]);
        }
    }

    return 0;
}


/*
 * Description: Check for a new messsage then Find instance, then call appropriate parser
 */
int UART_Parse(UartBufferStruct *msg)
{
    int status = 0;

    if(UART_RxMessagePending(msg))
    {
        switch (msg->instance)
        {
        case UART0_BASE:
            UART0_Parse(msg);
            break;
        }
    }

    return status;
}


/*
 * How to use:
 * 	You can use pointers to the buffers or you can use fixed size buffers. Use USE_BUFFER_POINTERS in UartCharBuffer.h file to enable/disable.
 *
 * 	Initializing buffers and passing the address to the pointers.
 *

// define the UART0 Rx buffers
uint8_t uart0RxIrqByteBuffer[UART_RX_IRQ_BYTE_SIZE] = {0}; // irq array
uint8_t uart0RxByteBuffer[UART_RX_BYTE_BUFFER_SIZE] = {0}; // byte array
UartMsgQueueStruct uart0RxMsgQueue[UART_RX_MESSAGE_QUEUE_SIZE] = {0}; // message array
UartRxBufferStruct uart0Rx =
{
	.instance = UART0_BASE,
	.uartIRQ_ByteBuffer = uart0RxIrqByteBuffer,
	.uartIRQ_ByteSize = UART_RX_IRQ_BYTE_SIZE, // optional
	.byteBuffer = uart0RxByteBuffer,
	.byteBufferSize = UART_RX_BYTE_BUFFER_SIZE, // optional
	.msgQueue = uart0RxMsgQueue
	.msgQueueSize = UART_RX_MESSAGE_QUEUE_SIZE // optional
};

// define the UART0 Tx buffers
UartMsgQueueStruct uart0TxMsgQueue[UART_TX_MESSAGE_QUEUE_SIZE] = {0};
UartTxBufferStruct uart0Tx =
{
	.instance = UART0_BASE,
	.msgQueue = uart0TxMsgQueue
	.msgQueueSize = UART_TX_MESSAGE_QUEUE_SIZE // optional
};

*/

#endif


#ifdef _XC_H // Microchip

UartRxBufferStruct uart1_rxMsg;
UartTxBufferStruct uart1_txMsg;
UartRxBufferStruct uart2_rxMsg;
UartTxBufferStruct uart2_txMsg;


/*
 *  Microchip default callback
 *
 */
void UART1_Receive_CallBack(void)
{
	static uint8_t data[1];

	data[0] = (uint8_t)U1RXREG;
	UART_Add_IRQ_Byte(&uart1_rxMsg, data, 1);
	UART_IncrementCharPointer(&uart1_rxMsg);
}

void UART2_Receive_CallBack(void)
{
	static uint8_t data[1];

	data[0] = (uint8_t)U2RXREG;
	UART_Add_IRQ_Byte(&uart2_rxMsg, data, 1);
	UART_IncrementCharPointer(&uart2_rxMsg);
}


/*
 * Description: Sends string
 *
 * Input: Character buffer structure. The structure holds the UART base and the char array.
 * Output: HAL status
 */
int UART_TxMessage_IT(UartTxBufferStruct *msg)
{
    uint16_t count = 0;
    uint8_t i = 0;
    uint8_t uartPort;

    count = UART_TX_GetMessageSize(msg);
    uartPort = msg->uartPort[msg->queuePtr.iIndexOUT];

    while(count)
    {
        if(uartPort == 1)
        {
            if(!(U1STAHbits.UTXBF == 1))
            {
                U1TXREG = msg->queue[msg->queuePtr.iIndexOUT].data[i];
                count--;
                i++;
            }
        }
        else if(uartPort == 2)
        {
            if(!(U2STAHbits.UTXBF == 1))
            {
                U2TXREG = msg->queue[msg->queuePtr.iIndexOUT].data[i];
                count--;
                i++;
            }
        }
    }
    return NO_ERROR;
}

#endif // end _XC_H

#ifdef XPARAMETERS_H // Xilinx

// NOTE: UartCharBuffer.c has been modified quite a lot since this Xilinx driver has been written, so this driver may need updating

static void OutbyteUart1(char c);

UartRxBufferStruct uart0_rxMsg = {0};
UartTxBufferStruct uart0_txMsg = {0};

UartMsgQueueStruct rxMsgQueue_0[UART_RX_MESSAGE_QUEUE_SIZE];
UartMsgQueueStruct txMsgQueue_0[UART_TX_MESSAGE_QUEUE_SIZE];

void UART_HandlerInitBuffer(void)
{
	memset(&uart1_rxMsg, 0, sizeof(uart1_rxMsg));
    memset(&uart_txMsg, 0, sizeof(uart_txMsg));

	UART_InitRxBuffer(&uart1_rxMsg, rxMsgQueue_1);
	UART_InitTxBuffer(&uart_txMsg, txMsgQueue_1);
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
void Uart0ReceiveInterruptHandler(void *CallBackRef, unsigned int EventData)
{
	int status;
	uint32_t data;

	if(!XUartLite_IsReceiveEmpty(XPAR_UARTLITE_0_BASEADDR)){
		XUartLite_Recv(&UartLite , data, 1);

		UART_Add_IRQ_Byte(buffer, (uint8_t*)data, 1);

		status = UartAddCharToBuffer(&uart1_rxMsg);
		if(status != NO_ERROR){
			// TODO - check HAL Status and act accordingly. If buffer overflows then try to increase character buffer size.
		}
	}
}

/*
 * Description: Transmit string array from buffer
 *
 * Input: the data structure with parameters and data
 * Output: HAL status
 *
 */
int UartTxMessage(UartTxBufferStruct *buffer, uint8_t uartPort)
{
	uint8_t *pData = buffer->BufStruct.rxMsgQueue[buffer->RingBuff.queuePtr.iIndexOUT].msgData;
	while (*pData != '\0') {
		switch (uartPort) {
		case UART_PORT_0:
			OutbyteUart1(*pData);
			break;
		case UART_PORT_1:
			OutbyteUart1(*pData);
			break;
		default:
			return ERROR; // unknown port
			break;
		}
		pData++;
	}
	return NO_ERROR;
}

/*
 * Description: Transmit one byte at a time on Uart1
 *				Same as Xilinx outbyte.c print routine
 * Input: the character to send
 *
 */
static void OutbyteUart1(char c)
{
	 XUartLite_SendByte(XPAR_AXI_UARTLITE_0_BASEADDR, c);
}

#endif //XPARAMETERS_H
