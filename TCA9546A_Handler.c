/*
 * TCA9546A_Handler.c
 *
 *  Created on: Jun 21, 2024
 *      Author: karl.yamashita
 */


#include "main.h"


extern UART_DMA_QueueStruct uart2;

TCA9546A_HandlerTypeDef tca = {0};



int TCA9546A_GetChannel(TCA9546A_HandlerTypeDef *tca)
{
	int status = NO_ERROR;

	status = TCA9546A_Read(tca, TCA9546A_GetChannelCallback);
	if(status != NO_ERROR)
	{
		return status;
	}
	else
	{
		return NO_ACK; // callback will return channel
	}

	return status;
}



// CALLBACKS


void TCA9546A_GetChannelCallback(TCA9546A_HandlerTypeDef *tca)
{
	uint32_t channel = 0;
	char str[128] = {0};

	channel = tca->i2c_Instance->dataPtr[0];

	sprintf(str, "%s=%ld", tca->commandCopy, channel);

	UART_DMA_NotifyUser(&uart2, str, strlen(str), true);
}

