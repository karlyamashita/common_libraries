/*
 * UART_Handler.h
 *
 *  Created on: Oct 19, 2022
 *      Author: karl.yamashita
 */

#ifndef INC_UART_HANDLER_H_
#define INC_UART_HANDLER_H_

#ifdef HAL_UART_MODULE_ENABLED // STM32
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
int UartTxMessage(UartCharBufferTxStruct * msg);
#endif

#ifdef _XC_H
void UART1_Receive_CallBack(void);
int UartTxMessage(UartCharBufferTxStruct *msg);
#endif

#ifdef ccs // TI Code Composer Studio
void USART0_IRQHandler(void);
void Uart_Receive(uint32_t uart_base);
int UartTxMessage(UartCharBufferTxStruct * msg);
void SetEchoMode(bool mode);
bool GetEchoMode(void);
#endif

#ifdef XPARAMETERS_H // Xilinx
void Uart0ReceiveInterruptHandler(void *CallBackRef, unsigned int EventData);
int UartTxMessage(UartCharBufferTxStruct *uartBufferPointer);
static void OutbyteUart1(char c) ;
#endif

#endif /* INC_UART_HANDLER_H_ */
