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


void TM4C_USART_IRQHandler(UartBufferStruct *msg);

int UART_TxMessage_IT(UartBufferStruct *msg);
int UART_TxMessage_DMA(UartBufferStruct *msg);

void SetEchoMode(bool mode);
bool GetEchoMode(void);

void UART_NotifyUser(UartBufferStruct *msg, char *str, uint32_t size, bool lineFeed);

void TM4C_UART_Rx_IRQ(UartBufferStruct *msg);
void TM4C_UART_Tx_IRQ(UartBufferStruct *msg);
void TM4C_UART_ErrorCallback(UartBufferStruct *msg);


#endif /* INC_UART_HANDLER_H_ */
