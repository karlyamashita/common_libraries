/*
 * UART_Handler.h
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */

#ifndef INC_UART_HANDLER_H_
#define INC_UART_HANDLER_H_


void UART_EnableRxInterrupt(UartBufferStruct *msg);
void UART_CheckRxIntError(UartBufferStruct *msg);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
int UART_TxMessage_IT(UartBufferStruct *msg);


#endif /* INC_UART_HANDLER_H_ */
