/*
 * UartBuffer.h
 *
 *  Created on: Dec 2, 2019
 *      Author: Karl
 */

#ifndef UARTBUFFER_H_
#define UARTBUFFER_H_

#define MAX_UART_RX_CHAR_BUFFER 64
#define MAX_UART_TX_BUFFER 64
#define MAX_UART_RX_MESSAGE_BUFFER 8
#define MAX_UART_TX_MESSAGE_BUFFER 8


// Receive
typedef struct UartRxMsgTypeDef{
	UART_HandleTypeDef UART_RxHeaderTypeDef;
	uint8_t Data[MAX_UART_RX_CHAR_BUFFER];
}UartRxMsgTypeDef;

// Transmit
typedef struct UartTxMsgTypeDef{
	UART_HandleTypeDef UART_TxHeaderTypeDef;
	uint8_t Data[MAX_UART_TX_BUFFER];
}UartTxMsgTypeDef;


void EnableUartRecInterrupt(UART_HandleTypeDef *huart, uint8_t *uartRxBuffer);
void ParseUartRxCharBuffer(void);
HAL_StatusTypeDef AddUartTxMessageBuffer(UART_HandleTypeDef *huart, uint8_t *msg);
void SendUartMessage(UART_HandleTypeDef *huart);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* UARTBUFFER_H_ */
