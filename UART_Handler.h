/*
 * UART_Handler.h
 *
 *  Created on: Oct 19, 2022
 *      Author: karl.yamashita
 */

#ifndef INC_UART_HANDLER_H_
#define INC_UART_HANDLER_H_

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
};

#ifdef HAL_UART_MODULE_ENABLED // STM32
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void UART_Enable_ReceiveIT(void);
void UART_ReceiveIT_ErrorHandler(void);
int UART_TxMessage(UartTxMsgBufferStruct * msg, uint32_t ringPtr);
#endif

#ifdef _XC_H
void UART1_Receive_CallBack(void);
int UART_TxMessage(UartTxMsgBufferStruct *msg, uint32_t ringPtr);
int UartTxMessageDMA(UartTxMsgBufferStruct *msg, uint32_t ringPtr);
#endif

#ifdef ccs // TI Code Composer Studio
void USART0_IRQHandler(void);
void USART6_IRQHandler(void);
void Uart_Receive(uint32_t uart_base);
int UART_TxMessage(UartTxMsgBufferStruct * msg, uint32_t ringPtr);
void SetEchoMode(bool mode);
bool GetEchoMode(void);

#endif

#ifdef XPARAMETERS_H // Xilinx
void Uart0ReceiveInterruptHandler(void *CallBackRef, unsigned int EventData);
int UART_TxMessage(UartTxMsgBufferStruct *msg, uint32_t ringPtr);
static void OutbyteUart1(char c) ;
#endif

#endif /* INC_UART_HANDLER_H_ */
