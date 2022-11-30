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
}

/*
 * Description: Enable UART Rx interrupt. Set flag if HAL_BUSY.
 * 				UART_ReceiveIT_ErrorHandler() should be called from main in polling routine to check for this flag status
 *
 */
void UART_Enable_ReceiveIT(void)
{
	if(HAL_UART_Receive_IT(&huart2, uartIRQ_ByteBuffer, MAX_UART_RX_IRQ_BYTE_LENGTH) != HAL_OK)
	{
		UART_RxEnErrorFlag = true;
	}
}

/*
 * Description: Transmit available message. If HAL_BUSY then return UART_TX_ERROR.
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

	return NO_ERROR;
}

/*
 * Description: Checks for HAL_UART_Receive_IT() return status. Retry and enable if error is true
 * 				This needs to be called from main polling routine.
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

#endif // end HAL_MODULE_ENABLED

#ifdef ccs // TI Code Composer Studio

bool echoMode = 0;

// this function should be registered when initializing the UART, UARTIntRegister(UART0_BASE, USART0_IRQHandler);
void USART0_IRQHandler(void)
{
    Uart_Receive(UART0_BASE);
}

void Uart_Receive(uint32_t uart_base)
{
    int status;
    uint8_t _char;
    uint32_t interruptStatus;

    interruptStatus = UARTIntStatus(uart_base, true);
    UARTIntClear(uart_base, interruptStatus);

    if (UARTCharsAvail(uart_base))
    {
        _char = UARTCharGet(uart_base); // get character
        if(echoMode == 1) {
            UARTCharPut(UART0_BASE, _char); // echo character
        }

        status = UART_AddCharToBuffer(uart_base, &_char); // save character to char buffer
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
int UART_TxMessage(UartCharBufferTxStruct *uartBufferPointer)
{
    uint32_t uart_base;
    uint8_t *pData = uartBufferPointer->data;
    uint8_t count = strlen((char*) pData);

    // we need to convert the generic port number to a uart base number. If we have many ports then consider using a table lookup
    switch(uartBufferPointer->uartPort){
        case UART_PORT_0:
            uart_base = UART0_BASE;
        break;
        case UART_PORT_1:
            uart_base = UART1_BASE;
            break;
        case UART_PORT_3:
            uart_base = UART3_BASE;
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
    UART_AddCharToBuffer(UART_PORT_1, chr);
}

/*
 * Description: Sends string
 *
 * Input: Character buffer structure. The structure holds the UART base and the char array.
 * Output: HAL status
 */
int UART_TxMessage(UartCharBufferTxStruct *msg)
{
    uint16_t count = 0;
    uint8_t i = 0;
    uint8_t uartPort;

    uartPort = msg->uartPort;
    count = msg->dataLength;

    while(count){
        if(!(U1STAHbits.UTXBF == 1))
        {
            if(uartPort == 1)
            {
                U1TXREG = msg->data[i];
            }
            else if(uartPort == 2)
            {
                U2TXREG = msg->data[i];
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

		status = UART_AddCharToBuffer(UART_PORT_0, (char*)buffer);
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
int UART_TxMessage(UartCharBufferTxStruct *uartBufferPointer)
{
	uint8_t *pData = uartBufferPointer->data;
	while (*pData != '\0') {
		switch (uartBufferPointer->uartPort) {
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


