/*
 * UART_Handler.c
 *
 *  Created on: Oct 19, 2022
 *      Author: karl.yamashita
 */


#include "main.h"
#include "UART_Handler.h"


#ifdef HAL_UART_MODULE_ENABLED // STM32

/*
 * PLEASE NOTE!!!!
 * This handler for STM32 is being replaced by UART_DMA_Handler.c
 * That file uses HAL_UARTEx_ReceiveToIdle_DMA and HAL_UARTEx_RxEventCallback. The DMA uses the idle bit so
 * any size packet or a string without a delimiter is saved to a ring buffer. This is far more efficient
 * than interrupting on each character.
 *
 */


#endif // end HAL_MODULE_ENABLED

#ifdef ccs // TI Code Composer Studio

bool echoMode = 0;

UartRxBufferStruct uart0_rxMsg = {0};
UartTxBufferStruct uart0_txMsg = {0};

UartRxBufferStruct uart6_rxMsg = {0};
UartTxBufferStruct uart6_txMsg = {0};


// this function should be registered when initializing the UART, UARTIntRegister(UART0_BASE, USART0_IRQHandler);
void USART0_IRQHandler(void)
{
    Uart_Receive(UART0_BASE, &uart0_rxMsg);
}

void USART6_IRQHandler(void)
{
    Uart_Receive(UART6_BASE, &uart6_rxMsg);
}

void Uart_Receive(uint32_t uart_base, UartRxBufferStruct *msg)
{
	int status;
	uint32_t interruptStatus;
	uint8_t data;

	interruptStatus = UARTIntStatus(uart_base, true);
	UARTIntClear(uart_base, interruptStatus);

	if (UARTCharsAvail(uart_base))
	{
		data = (uint8_t)UARTCharGet(uart_base);
		UART_Add_IRQ_Byte(msg, &data, 1);
		if(echoMode == 1) {
			UARTCharPut(UART0_BASE, (unsigned char)msg->uartIRQ_ByteBuffer[0]); // echo character
		}

		status = UART_AddCharToBuffer(msg); // save character to char buffer
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
int UART_TxMessage(UartTxBufferStruct *msg, uint8_t uartPort)
{
    uint32_t uart_base;
    uint8_t *pData = msg->msgQueue[msg->msgPtr.iIndexOUT].data;
    uint32_t count = UART_TX_GetMessageSize(msg);

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
int UART1_GetMessage(UartRxBufferStruct *msg, UartMsgQueueStruct *msgOut)
{    
    if(UART_RxStringMessagePending(msg, msgOut))
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

static void OutbyteUart1(char c);

UartRxBufferStruct uart0_rxMsg = {0};
UartTxBufferStruct uart0_txMsg = {0};

UartMsgQueueStruct rxMsgQueue_0[UART_RX_MESSAGE_QUEUE_SIZE];
UartMsgQueueStruct txMsgQueue_0[UART_TX_MESSAGE_QUEUE_SIZE];

void UART_HandlerInitBuffer(void)
{
	memset(&uart1_rxMsg, 0, sizeof(uart1_rxMsg));
    memset(&uart1_txMsg, 0, sizeof(uart1_txMsg));

	UART_InitRxBuffer(&uart1_rxMsg, rxMsgQueue_1);
	UART_InitTxBuffer(&uart1_txMsg, txMsgQueue_1);
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


