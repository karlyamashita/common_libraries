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
extern uint8_t uartIRQ_ByteBuffer[MAX_UART_RX_IRQ_BYTE_LENGTH];

static bool UART_RxEnErrorFlag = false;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart2.Instance)
	{
		UART_AddCharToBuffer(uartIRQ_ByteBuffer, MAX_UART_RX_IRQ_BYTE_LENGTH);
		UART_Enable_ReceiveIT();
	}
	// User will need to add more code if more UART ports are used.
}

/*
 * Description: Enables UART Rx interrupt. If HAL_BUSY then set error flag
 *
 */
void UART_Enable_ReceiveIT(void)
{
	if(HAL_UART_Receive_IT(&huart2, uartIRQ_ByteBuffer, MAX_UART_RX_IRQ_BYTE_LENGTH) != HAL_OK) //re-enable interrupt
	{
		UART_RxEnErrorFlag = true;
	}
}

/*
 * Description: This should be called from main loop. If error flag is set then try to enable UART Rx interrupt again.
 *
 */
void UART_ReceiveIT_ErrorHandler(void)
{
	if(UART_RxEnErrorFlag)
	{
		UART_RxEnErrorFlag = false;
		UART_Enable_ReceiveIT();
	}
}

/*
 * Description: This is called from UART_SendMessage() in UartCharBuff.c
 *
 */
int UART_TxMessage(UartTxMsgBufferStruct * msg, uint32_t ringPtr)
{
	HAL_StatusTypeDef hal_status;

	if(msg->BufStruct[ringPtr].uartPort == UART_PORT_2)
	{
		hal_status = HAL_UART_Transmit_IT(&huart2, msg->BufStruct[ringPtr].data, msg->BufStruct[ringPtr].dataSize);
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

// this function should be registered when initializing the UART, UARTIntRegister(UART0_BASE, USART0_IRQHandler);
void USART0_IRQHandler(void)
{
    Uart_Receive(UART0_BASE);
}

void USART6_IRQHandler(void)
{
    Uart_Receive(UART6_BASE);
}

void Uart_Receive(uint32_t uart_base)
{
    int status;
    uint8_t _char[1];
    uint32_t interruptStatus;

    interruptStatus = UARTIntStatus(uart_base, true);
    UARTIntClear(uart_base, interruptStatus);

    if(uart_base == UART0_BASE)
    {
        if (UARTCharsAvail(uart_base))
        {
            _char[0] = UARTCharGet(uart_base); // get character
            if(echoMode == 1) {
                UARTCharPut(UART0_BASE, (unsigned char*)_char); // echo character
            }

            status = UART_AddCharToBuffer(_char, MAX_UART_RX_IRQ_BYTE_LENGTH); // save character to char buffer
            if(status != NO_ERROR){
                // TODO - check HAL Status and act accordingly.
                // For now we're going to lose a character if buffer is full. Try increasing MAX_UART_RX_CHAR_BUFFER size .
            }
        }
    }
    else if(uart_base == UART6_BASE)
    {
        if (UARTCharsAvail(uart_base))
        {
            _char[0] = UARTCharGet(uart_base); // get character

            status = UART_AddCharToBuffer(_char, MAX_UART_RX_IRQ_BYTE_LENGTH); // save character to char buffer
            if(status != NO_ERROR){
                // TODO - check HAL Status and act accordingly.
                // For now we're going to lose a character if buffer is full. Try increasing MAX_UART_RX_CHAR_BUFFER size .
            }
        }
    }
}

/*
 * Description: Sends string
 *
 * Input: Character buffer structure. The structure holds the uart base and the char array.
 * Output: HAL status
 */
int UART_TxMessage(UartTxMsgBufferStruct *msg, uint32_t ringPtr)
{
    uint32_t uart_base;
    uint8_t *pData = msg->BufStruct[ringPtr].data;
    uint8_t count = strlen((char*) pData);

    // we need to convert the generic port number to a uart base number. If we have many ports then consider using a table lookup
    switch(msg->BufStruct[ringPtr].uartPort){
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
void UART1_Receive_CallBack(void)
{
    uint8_t chr[1] = {0};

    chr[0] = U1RXREG;
    UART_AddCharToBuffer(chr, MAX_UART_RX_IRQ_BYTE_LENGTH);
}

/*
 * Description: Sends string
 *
 * Input: Character buffer structure. The structure holds the UART base and the char array.
 * Output: HAL status
 */
int UART_TxMessage(UartTxMsgBufferStruct *msg, uint32_t ringPtr)
{
    uint16_t count = 0;
    uint8_t i = 0;
    uint8_t uartPort;

    uartPort = msg->BufStruct[ringPtr].uartPort;
    count = msg->BufStruct[ringPtr].dataSize;

    while(count){
        if(!(U1STAHbits.UTXBF == 1))
        {
            if(uartPort == 1)
            {
                U1TXREG = msg->BufStruct[ringPtr].data[i];
            }
            else if(uartPort == 2)
            {
                U2TXREG = msg->BufStruct[ringPtr].data[i];
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
	uint8_t buffer[32] = {0};

	if(!XUartLite_IsReceiveEmpty(XPAR_UARTLITE_0_BASEADDR)){
		XUartLite_Recv(&UartLite , buffer, 1);

		status = UartAddCharToBuffer(UART_PORT_0, (char*)buffer);
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
int UartTxMessage(UartCharBufferTxStruct *msg, uint32_t ringPtr)
{
	uint8_t *pData = msg->BufStruct[0].data;
	while (*pData != '\0') {
		switch (msg->BufStruct[0].uartPort) {
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


