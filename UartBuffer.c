/*
 * UartBuffer.c
 *
 *  Created on: Dec 2, 2019
 *      Author: Karl
 */
#include "LSP.h"

#ifdef USE_UART_BUFFER
#include "main.h"
#include "UartBuffer.h"


#include "RingBuff.h"

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;


uint8_t uartBufferRx[64];

void EnableUartRecInterrupt(void) {
	HAL_UART_Receive_IT(&huart2, (uint8_t *)uartBufferRx, 64);
}




#endif // USE_UART_BUFFER
