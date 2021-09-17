/*
 * Uart_TM4C_IRQ_Handler.h
 *
 *  Created on: Jun 17, 2021
 *      Author: karl.yamashita
 */

#ifndef UART_TM4C_IRQ_HANDLER_H_
#define UART_TM4C_IRQ_HANDLER_H_

HAL_StatusTypeDef UartTxData(uint32_t uartBase, uint8_t *data, uint8_t size);
HAL_StatusTypeDef UartTxMessage(UartCharBufferTxStruct *uartBufferPointer);
void Uart_Receive(uint32_t uart_base);

#endif /* UART_TM4C_IRQ_HANDLER_H_ */
