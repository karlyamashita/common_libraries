/*
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Karl Yamashita
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */

/*
 * PollingRoutine.c
 *
 *  Created on: Oct 24, 2023
 *      Author: karl.yamashita
 *
 *
 *      Template for projects.
 *
 *      The object of this PollingRoutine.c/h files is to not have to write code in main.c which already has a lot of generated code.
 *      It is cumbersome having to scroll through all the generated code for your own code and having to find a USER CODE section so your code is not erased when CubeMX re-generates code.
 *      
 *      Direction: Call PollingInit before the main while loop. Call PollingRoutine from within the main while loop
 * 
 *      Example;
        // USER CODE BEGIN WHILE
        PollingInit();
        while (1)
        {
            PollingRoutine();
            // USER CODE END WHILE

            // USER CODE BEGIN 3
        }
        // USER CODE END 3

 */


#include "main.h"


/*
const char version[] = "v1.0.1";

#define UART2_DMA_RX_QUEUE_SIZE 10 // queue size
#define UART2_DMA_TX_QUEUE_SIZE 4
UART_DMA_Data uart2_dmaDataRxQueue[UART2_DMA_RX_QUEUE_SIZE] = {0};
UART_DMA_Data uart2_dmaDataTxQueue[UART2_DMA_TX_QUEUE_SIZE] = {0};

UART_DMA_Struct_t uart2_msg =
{
	.huart = &huart2,
	.rx.queueSize = UART2_DMA_RX_QUEUE_SIZE,
	.rx.msgQueue = uart2_dmaDataRxQueue,
	.tx.queueSize = UART2_DMA_TX_QUEUE_SIZE,
	.tx.msgQueue = uart2_dmaDataTxQueue,
	.dma.dmaPtr.SkipOverFlow = true
};
*/


void PollingInit(void)
{

}

void PollingRoutine(void)
{

	//UART_DMA_ParseCircularBuffer(&uart2_msg);
}

/*
void UART_ParseCommands(UART_DMA_Struct_t *msg)
{
	if(UART_DMA_RxMsgRdy(msg))
	{
		// user can parse msg variable.
		if(strncmp((char*)msg->rx.msgToParse->data, "get version", strlen("get version")) == 0)
		{
			// call function to return version number
		}
		else if(strncmp((char*)msg->rx.msgToParse->data, "get status", strlen("get status")) == 0)
		{
			// call function to get status information
		}
	}
}

void STM32_Ready(UART_DMA_Struct_t *msg)
{
	char str[UART_DMA_QUEUE_DATA_SIZE] = {0};

	sprintf(str, "Theia V2 Ready - %s", version);

	UART_DMA_NotifyUser(msg, str, strlen(str), true);
}

int Version(char *msg, char *retStr)
{
	sprintf(retStr, "%s", version);

	return NO_ERROR;
}

*/
