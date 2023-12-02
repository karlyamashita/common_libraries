/*
 * UART_Handler.h
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */

#ifndef INC_UART_HANDLER_H_
#define INC_UART_HANDLER_H_


enum
{
    UART_TX_DONE,
    UART_TX_PENDING
};

void USART0_IRQHandler(void);
int UART_TxMessage_IT(UartBufferStruct *msg);

int UART_Parse(UartBufferStruct *msg);
int UART_TxMessage(UartBufferStruct *msg);


#endif /* INC_UART_HANDLER_H_ */
