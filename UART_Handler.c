/*
 * UART_Handler.c
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */



#include "main.h"


#ifdef HAL_MODULE_ENABLED // STM32

extern UART_HandleTypeDef huart2;

#ifdef USE_BUFFER_POINTERS
// define the UART2 Rx buffers
uint8_t uart2RxIrqByteBuffer[UART_RX_IRQ_BYTE_SIZE] = {0}; // irq array
uint8_t uart2RxByteBuffer[UART_RX_BYTE_BUFFER_SIZE] = {0}; // byte array
UartMsgQueueStruct uart2RxMsgQueue[UART_RX_MESSAGE_QUEUE_SIZE] = {0}; // message array
UartRxBufferStruct uart2Rx =
{	// init and point to buffers
	.huart = &huart2,
	.uartIRQ_ByteBuffer = uart2RxIrqByteBuffer,
	.uartIRQ_ByteSize = UART_RX_IRQ_BYTE_SIZE,
	.byteBuffer = uart2RxByteBuffer,
	.byteBufferSize = UART_RX_BYTE_BUFFER_SIZE,
	.msgQueue = uart2RxMsgQueue,
	.msgQueueSize = UART_RX_MESSAGE_QUEUE_SIZE
};

// define the UART2 Tx buffers
UartMsgQueueStruct uart2TxMsgQueue[UART_TX_MESSAGE_QUEUE_SIZE] = {0};
UartTxBufferStruct uart2Tx =
{	// init and point to buffers
	.huart = &huart2,
	.msgQueue = uart2TxMsgQueue,
	.msgQueueSize = UART_TX_MESSAGE_QUEUE_SIZE
};

#else

UartRxBufferStruct uart2Rx =
{
	.huart = &huart2,
	.uartIRQ_ByteSize = UART_RX_IRQ_BYTE_SIZE,
	.byteBufferSize = UART_RX_BYTE_BUFFER_SIZE,
	.msgQueueSize = UART_RX_MESSAGE_QUEUE_SIZE
};
UartTxBufferStruct uart2Tx =
{
	.huart = &huart2,
	.msgQueueSize = UART_TX_MESSAGE_QUEUE_SIZE
};
#endif

/*
 * Description: Enables the HAL_UART_Receive_IT interrupt. Call before main while loop and in HAL_UART_RxCpltCallback
 */
void UART_EnableRxInterrupt(UartRxBufferStruct *msg)
{
	if(HAL_UART_Receive_IT(msg->huart, msg->uartIRQ_ByteBuffer, msg->uartIRQ_ByteSize) != HAL_OK)
	{
	//	UART_SetRxIntErrorFlag(msg, true);
	}
}

/*
 * Description: If error flag then call UART_EnableRxInterrupt. Call from main while loop.
 *
 */
void UART_CheckRxIntError(UartRxBufferStruct *msg)
{
	if(UART_GetRxIntErrorFlag(msg))
	{
		UART_SetRxIntErrorFlag(msg, false);
		UART_EnableRxInterrupt(msg);
	}
}

/*
 * Description: HAL UART callback.
 *
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == uart2Rx.huart->Instance)
	{
		UART_AddByteToBuffer(&uart2Rx, uart2Rx.uartIRQ_ByteBuffer, uart2Rx.uartIRQ_ByteSize);
		UART_EnableRxInterrupt(&uart2Rx);
	}
}

/*
 * Description: Transmit any available messages. Call from main while loop
 */
int UART_TxMessage(UartTxBufferStruct *msg)
{
	int status = NO_ERROR;

	if(msg->msgPtr.cnt_Handle)
	{
		if(HAL_UART_Transmit_IT(msg->huart, msg->msgQueue[msg->msgPtr.index_OUT].data, msg->msgQueue[msg->msgPtr.index_OUT].size) == HAL_OK)
		{
			RingBuff_Ptr_Output(&msg->msgPtr, msg->msgQueueSize);
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
UartRxBufferStruct uart0_Rx =
{
 .instance = UART0_BASE,
 .msgPtr = {0},
 .byteBuffer = {0}
};

UartTxBufferStruct uart0_Tx =
{
 .instance = UART0_BASE,
 .msgPtr = {0}
};

uint8_t data[1];

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void USART0_IRQHandler(void)
{
    uint32_t ui32Status;

    //
    // Get the interrupt status.
    //
    ui32Status = UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    MAP_UARTIntClear(UART0_BASE, ui32Status);


    if(ui32Status & UART_INT_TX)
    {
        UART0_TransmitMessage(&uart0_Tx);
    }
    else
    {
        //
        // Save available character in buffer.
        //
        if(MAP_UARTCharsAvail(UART0_BASE))
        {
            data[0] = (uint8_t)UARTCharGet(UART0_BASE);
            UART_AddByteToBuffer(&uart0_Rx, data, 1);
        }
    }
}


/*
 * Description: Called from UartCharBuffer.c
 *              This will enable TX interrupt. The UART interrupt Handler which will call UART0_TransmitMessage
 */
int UART_TxMessage(UartTxBufferStruct *msg)
{
    int status = 0;

    if(msg->txPending == true) return UART_TX_PENDING;
    msg->txPending = true;

    msg->msgToSend = msg->msgQueue[msg->msgPtr.index_OUT].data;
    msg->msgToSendSize = msg->msgQueue[msg->msgPtr.index_OUT].size;
    msg->msgToSendPtr = 0; // reset

    UARTIntEnable(msg->instance, UART_INT_TX);

    return status;
}

/*
 * Description: called from uart interrupt
 */
void UART0_TransmitMessage(UartTxBufferStruct *msg)
{
    if(msg->msgToSendPtr < msg->msgToSendSize)
    {
        if(UARTCharPutNonBlocking(UART0_BASE, msg->msgToSend[msg->msgToSendPtr]) == true)
        {
            if(++msg->msgToSendPtr == msg->msgToSendSize)
            {
                msg->txPending = false; // clear for next message to be sent
                UARTIntDisable(msg->instance, UART_INT_TX);
            }
        }
    }
}

/*
 * Description: Check for a new messsage then Find instance, then call appropriate parser
 */
int UART_Parse(UartRxBufferStruct *msg)
{
    int status = 0;

    if(UART_RxMessagePending(msg))
    {
        switch (msg->instance)
        {
        case UART0_BASE:
            UART0_Parser(msg);
            break;
        case UART1_BASE:

            break;
        case UART2_BASE:

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
	.byteBuffer = uart0RxByteBuffer,
	.msgQueue = uart0RxMsgQueue
};

// define the UART0 Tx buffers
UartMsgQueueStruct uart0TxMsgQueue[UART_TX_MESSAGE_QUEUE_SIZE] = {0};
UartTxBufferStruct uart0Tx =
{
	.instance = UART0_BASE,
	.msgQueue = uart0TxMsgQueue
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
	UART_AddCharToBuffer(&uart1_rxMsg);
}

void UART2_Receive_CallBack(void)
{
	static uint8_t data[1];

	data[0] = (uint8_t)U2RXREG;
	UART_Add_IRQ_Byte(&uart2_rxMsg, data, 1);
	UART_AddCharToBuffer(&uart2_rxMsg);
}

/*
 * Description: return 1 if there is a message, 0 if no message.
 */
int UART1_GetMessage(UartRxBufferStruct *msg)
{
    if(UART_RxMessagePending(msg))
    {
        return 1;
    }

    return 0;
}

/*
 * Description: Sends string
 *
 * Input: Character buffer structure. The structure holds the UART base and the char array.
 * Output: HAL status
 */
int UART_TxMessage(UartTxBufferStruct *msg)
{
    uint16_t count = 0;
    uint8_t i = 0;
    uint8_t uartPort;

    count = UART_TX_GetMessageSize(msg);
    uartPort = msg->uartPort[msg->msgPtr.iIndexOUT];

    while(count)
    {
        if(uartPort == 1)
        {
            if(!(U1STAHbits.UTXBF == 1))
            {
                U1TXREG = msg->msgQueue[msg->msgPtr.iIndexOUT].data[i];
                count--;
                i++;
            }
        }
        else if(uartPort == 2)
        {
            if(!(U2STAHbits.UTXBF == 1))
            {
                U2TXREG = msg->msgQueue[msg->msgPtr.iIndexOUT].data[i];
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
	uint8_t *pData = buffer->BufStruct.rxMsgQueue[buffer->RingBuff.msgPtr.iIndexOUT].msgData;
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
