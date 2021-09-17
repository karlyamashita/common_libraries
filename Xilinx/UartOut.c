/*
 * UartOut.c
 *
 *  Created on: Jun 2, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "UartOut.h"

static void OutbyteUart1(char c);

/*
 * Description: Transmit string array
 *
 *
 */
HAL_StatusTypeDef UartTxMessage(UartCharBufferTxStruct *uartBufferPointer){
	uint8_t *pData = uartBufferPointer->data;
	while (*pData != '\0') {
		switch (uartBufferPointer->uartPort) {
		case UART_PORT_0:
			OutbyteUart1(*pData);
			break;
		case UART_PORT_1:
			OutbyteUart1(*pData);
			break;
		case UART_PORT_2:
			// n/a
			break;
		}
		pData++;
	}
	return HAL_OK;
}

/*
 * Description: transmit
 *
 *
 */
void PrintUart1(const char8 *ptr)
{
  while (*ptr != (char8)0) {
    OutbyteUart1 (*ptr);
	ptr++;
  }
}

/*
 * Description: Prints string with carriage return and line feed.
 *
 *
 */
void PrintLine(const char8 *ptr){
	print(ptr);
	print("\r\n");
}

/*
 * Description: Transmit one byte at a time on Uart1
 *
 * Input: The
 *
 */
static void OutbyteUart1(char c) {
	 XUartLite_SendByte(XPAR_AXI_UARTLITE_0_BASEADDR, c);
}

