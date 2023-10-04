/*
 * UART_Handler.h
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */

#ifndef INC_UART_HANDLER_H_
#define INC_UART_HANDLER_H_


#ifdef HAL_MODULE_ENABLED // STM32

void UART_EnableRxInterrupt(UartBufferStruct *msg);
void UART_CheckRxIntError(UartBufferStruct *msg);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
int UART_TxMessage_IT(UartBufferStruct *msg);

#endif

#ifdef ccs

enum
{
    UART_TX_DONE,
    UART_TX_PENDING
};

void USART0_IRQHandler(void);
void USART6_IRQHandler(void);
int UART_TxMessage_IT(UartTxBufferStruct *msg);

int UART_Parse(UartRxBufferStruct *msg);

#endif

#ifdef _XC_H // Microchip

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
int UART_TxMessage_IT(UartTxBufferStruct *msg);


#endif

#endif /* INC_UART_HANDLER_H_ */
