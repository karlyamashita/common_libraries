/*
 * UART_Handler.h
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */

#ifndef INC_UART_HANDLER_H_
#define INC_UART_HANDLER_H_


// PORT number defines to assign to instance.
enum
{
	UART_PORT_0,
	UART_PORT_1,
	UART_PORT_2,
	UART_PORT_3,
	UART_PORT_4,
	UART_PORT_5,
	UART_PORT_6,
	UART_PORT_7,
	UART_PORT_8,
	UART_PORT_9,
	UART_PORT_10,
	UART_PORT_11,
	UART_PORT_12,
	UART_PORT_13,
	UART_PORT_14,
	UART_PORT_15
	// add more if needed.
};

void UART1_Receive_CallBack(void);
void UART2_Receive_CallBack(void);
void UART_TX_AddDataToBuffer(UartBufferStruct *msgOut, uint8_t *msgIN, uint32_t dataSize);
int UART_TxMessage(UartBufferStruct *msg);


#endif /* INC_UART_HANDLER_H_ */
