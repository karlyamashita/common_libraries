/*
 * UART_Handler.c
 *
 *  Created on: Oct 19, 2022
 *      Author: karl.yamashita
 */


#include "main.h"
#include "UART_Handler.h"


#ifdef HAL_UART_MODULE_ENABLED // STM32

extern UART_HandleTypeDef huart2; // update to which uart instance is used.


UartRxBufferStruct uart2_rxMsg = {0};
UartTxBufferStruct uart2_txMsg = {0};

UartMsgQueueStruct rxMsgQueue_2[UART_RX_MESSAGE_QUEUE_SIZE];
UartMsgQueueStruct txMsgQueue_2[UART_TX_MESSAGE_QUEUE_SIZE];

/*
 * Description: User needs to init each variable for rx and tx
 *
 *
 */
void UART_HandlerInitBuffer(void)
{
	UART_InitRxBuffer(&uart2_rxMsg, rxMsgQueue_2);
	UART_InitTxBuffer(&uart2_txMsg, txMsgQueue_2);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart2.Instance)
	{
		UART_AddCharToBuffer(&uart2_rxMsg);
		UART_Enable_ReceiveIT(&uart2_rxMsg);
	}
	// User will need to add more code if more UART ports are used.
}

/*
 * Description: Enables UART Rx interrupt. If HAL_BUSY then set error flag
 *
 */
void UART_Enable_ReceiveIT(UartRxBufferStruct *buffer)
{
	if(HAL_UART_Receive_IT(&huart2, buffer->BufStruct.uartIRQ_ByteBuffer, UART_RX_IRQ_BYTE_SIZE) != HAL_OK) //re-enable interrupt
	{
		buffer->BufStruct.UART_RxEnErrorFlag = true;
	}
}

/*
 * Description: This should be called from main loop. If error flag is set then try to enable UART Rx interrupt again.
 *
 */
void UART_ReceiveIT_ErrorHandler(UartRxBufferStruct *buffer)
{
	if(buffer->BufStruct.UART_RxEnErrorFlag)
	{
		buffer->BufStruct.UART_RxEnErrorFlag = false;
		UART_Enable_ReceiveIT(buffer);
	}
}



/*
 * Description: This is called from UART_SendMessage() in UartCharBuff.c
 * Input: Pointer to tx buffer and the UART port to transmit to
 *
 */
int UART_TxMessage(UartTxBufferStruct *buffer, uint8_t uartPort)
{
	HAL_StatusTypeDef hal_status;

	if(uartPort == UART_PORT_2)
	{
		hal_status = HAL_UART_Transmit_IT(&huart2, buffer->BufStruct.msgQueue[buffer->RingBuff.msgPtr.iIndexOUT].msgData, buffer->BufStruct.msgQueueSize[buffer->RingBuff.msgPtr.iIndexOUT].dataSize);
		if(hal_status != HAL_OK)
		{
			return UART_TX_ERROR;
		}
	}
	// User will need to add more code if more UART ports are used as well the reference to that huart.

	return NO_ERROR;
}


#endif // end HAL_MODULE_ENABLED

#ifdef ccs // TI Code Composer Studio

bool echoMode = 0;

UartRxBufferStruct uart0_rxMsg = {0};
UartTxBufferStruct uart0_txMsg = {0};

UartRxBufferStruct uart6_rxMsg = {0};
UartTxBufferStruct uart6_txMsg = {0};

UartMsgQueueStruct rxMsgQueue_0[UART_RX_MESSAGE_QUEUE_SIZE];
UartMsgQueueStruct txMsgQueue_0[UART_TX_MESSAGE_QUEUE_SIZE];

UartMsgQueueStruct rxMsgQueue_6[UART_RX_MESSAGE_QUEUE_SIZE];
UartMsgQueueStruct txMsgQueue_6[UART_TX_MESSAGE_QUEUE_SIZE];

void UART_HandlerInitBuffer(void)
{
	UART_InitRxBuffer(&uart0_rxMsg, rxMsgQueue_0);
	UART_InitTxBuffer(&uart0_txMsg, txMsgQueue_0);
	UART_InitRxBuffer(&uart6_rxMsg, rxMsgQueue_6);
	UART_InitTxBuffer(&uart6_txMsg, txMsgQueue_6);
}

// this function should be registered when initializing the UART, UARTIntRegister(UART0_BASE, USART0_IRQHandler);
void USART0_IRQHandler(void)
{
    Uart_Receive(UART0_BASE, &uart0_rxMsg);
}

void USART6_IRQHandler(void)
{
    Uart_Receive(UART6_BASE, &uart6_rxMsg);
}

void Uart_Receive(uint32_t uart_base, UartRxBufferStruct *buffer)
{
    int status;
    uint32_t interruptStatus;
    uint32_t data;

    interruptStatus = UARTIntStatus(uart_base, true);
    UARTIntClear(uart_base, interruptStatus);

	if (UARTCharsAvail(uart_base))
	{
		data = UARTCharGet(uart_base);
		UART_Add_IRQ_Byte(buffer, (uint8_t*)data, 1);
		if(echoMode == 1) {
			UARTCharPut(UART0_BASE, (unsigned char)buffer->BufStruct.uartIRQ_ByteBuffer[0]); // echo character
		}

		status = UART_AddCharToBuffer(buffer); // save character to char buffer
		if(status != NO_ERROR){
			// TODO - check HAL Status and act accordingly.
			// For now we're going to lose a character if buffer is full. Try increasing MAX_UART_RX_CHAR_BUFFER size .
		}
	}
}

/*
 * Description: Sends string
 *
 * Input: Character buffer structure. The structure holds the uart base and the char array.
 * Output: HAL status
 */
int UART_TxMessage(UartTxBufferStruct *buffer, uint8_t uartPort)
{
    uint32_t uart_base;
    uint8_t *pData = buffer->BufStruct.msgQueue[buffer->RingBuff.msgPtr.iIndexOUT].msgData;
    uint8_t count = buffer->BufStruct.msgQueueSize[buffer->RingBuff.msgPtr.iIndexOUT].dataSize;

    // we need to convert the generic port number to a uart base number. If we have many ports then consider using a table lookup
    switch(uartPort){
        case UART_PORT_0:
            uart_base = UART0_BASE;
        break;
        case UART_PORT_1:
            uart_base = UART1_BASE;
            break;
        case UART_PORT_3:
            uart_base = UART3_BASE;
            break;
        case UART_PORT_6:
            uart_base = UART6_BASE;
            break;
    }
    while(count--){
        UARTCharPut(uart_base, *pData++);
    }
    return NO_ERROR;
}

void SetEchoMode(bool mode)
{
    echoMode = mode;
}

bool GetEchoMode(void)
{
    return echoMode;
}


#endif // end css

#ifdef _XC_H

UartRxBufferStruct uart1_rxMsg;
UartTxBufferStruct uart1_txMsg;

UartMsgQueueStruct rxMsgQueue_1[UART_RX_MESSAGE_QUEUE_SIZE];
UartMsgQueueStruct txMsgQueue_1[UART_TX_MESSAGE_QUEUE_SIZE];

void UART_HandlerInitBuffer(void)
{
	UART_InitRxBuffer(&uart1_rxMsg, rxMsgQueue_1);
	UART_InitTxBuffer(&uart1_txMsg, txMsgQueue_1);
}

void UART1_Receive_CallBack(void)
{
	UART_Add_IRQ_Byte(&uart1_rxMsg, (uint8_t*)U1RXREG, 1);
    UART_AddCharToBuffer(&uart1_rxMsg);
}

/*
 * Description: Sends string
 *
 * Input: Character buffer structure. The structure holds the UART base and the char array.
 * Output: HAL status
 */
int UART_TxMessage(UartTxBufferStruct *buffer, uint8_t uartPort)
{
    uint16_t count = 0;
    uint8_t i = 0;

    count = buffer->BufStruct.msgQueueSize[buffer->RingBuff.msgPtr.iIndexOUT].dataSize;

    while(count){
        if(!(U1STAHbits.UTXBF == 1))
        {
            if(uartPort == 1)
            {
                U1TXREG = buffer->BufStruct.msgQueue[buffer->RingBuff.msgPtr.iIndexOUT].msgData[i];
            }
            else if(uartPort == 2)
            {
                U2TXREG = buffer->BufStruct.msgQueue[buffer->RingBuff.msgPtr.iIndexOUT].msgData[i];
            }
            count--;
            i++;
        }
    }
    return NO_ERROR;
}

#endif // end _XC_H

#ifdef XPARAMETERS_H // Xilinx

static void OutbyteUart1(char c);

/*
 * Description:
 *
 * Input:
 * Output:
 */
void Uart0ReceiveInterruptHandler(void *CallBackRef, unsigned int EventData)
{
	int status;

	if(!XUartLite_IsReceiveEmpty(XPAR_UARTLITE_0_BASEADDR)){
		XUartLite_Recv(&UartLite , uart1_rxMsg.BufStruct.uartIRQ_ByteBuffer, 1);

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


