/*
 * NotifyUserDMA.c
 *
 *  Created on: Jan 3, 2023
 *      Author: karl.yamashita
 */

#include "main.h"

/*
 * Description: This will add CR and LF to string before saving to tx buffer.
 * Input: Pointer to buffer, the string to add to tx buffer, and linefeed enable
 *
 */
void NotifyUserDMA(UART_DMA_TxQueueStruct *msg, char *str, bool lineFeed)
{
    uint8_t strMsg[UART_DMA_CHAR_SIZE] = {0};
    uint8_t *ptr = strMsg;
    int i = 0;

    strcpy((char*)strMsg, str);

    if(lineFeed == true)
    {
    	strcat((char*)strMsg, "\r\n");
    }

	memset(&msg->queue[msg->ptr.iIndexIN].data, 0 , sizeof(msg->queue[msg->ptr.iIndexIN].data));

    msg->queue[msg->ptr.iIndexIN].dataSize = strlen((char*)strMsg);
    while(*ptr != '\0')
    {
    	msg->queue[msg->ptr.iIndexIN].data[i++] = *ptr++;
    }

    DRV_RingBuffPtr__Input(&msg->ptr, UART_DMA_QUEUE_SIZE);
}
