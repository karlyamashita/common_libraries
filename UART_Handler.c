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
extern uint8_t uartDataBuffer[MAX_UART_RX_CHAR_BUFFER_SINGLE];


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//if(huart->Instance == huart2.Instance)
	{

	}
	if(huart->Instance == huart2.Instance)
	{
		UartAddCharToBuffer(UART_PORT_2, uartDataBuffer);
		HAL_UART_Receive_IT(&huart2, uartDataBuffer, MAX_UART_RX_CHAR_BUFFER_SINGLE); //re-enable interrupt
	}
}


int UartTxMessage(UartCharBufferTxStruct * msg)
{
	HAL_StatusTypeDef hal_status;

	if(msg->uartPort == UART_PORT_2)
	{
		hal_status = HAL_UART_Transmit_IT(&huart2, msg->data, msg->dataLength);
		if(hal_status != HAL_OK)
		{
			return UART_TX_ERROR;
		}
	}

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

        status = UartAddCharToBuffer(uart_base, &_char); // save character to char buffer
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
int UartTxMessage(UartCharBufferTxStruct *uartBufferPointer)
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
    UartAddCharToBuffer(UART_PORT_1, chr);
}

/*
 * Description: Sends string
 *
 * Input: Character buffer structure. The structure holds the UART base and the char array.
 * Output: HAL status
 */
int UartTxMessage(UartCharBufferTxStruct *msg)
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
int UartTxMessage(UartCharBufferTxStruct *uartBufferPointer)
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


