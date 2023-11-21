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
void NotifyUserDMA(UART_DMA_QueueStruct *msg, char *str, bool lineFeed)
{
    uint8_t strMsg[UART_DMA_DATA_SIZE] = {0};
    uint8_t *ptr = strMsg;
    int i = 0;

    strcpy((char*)strMsg, str);

    if(lineFeed == true)
    {
    	strcat((char*)strMsg, "\r\n");
    }

	memset(&msg->tx.queue[msg->tx.ptr.index_IN].data, 0 , sizeof(msg->tx.queue[msg->tx.ptr.index_IN].data));

    msg->tx.queue[msg->tx.ptr.index_IN].size = strlen((char*)strMsg);
    while(*ptr != '\0')
    {
    	msg->tx.queue[msg->tx.ptr.index_IN].data[i++] = *ptr++;
    }

    RingBuff_Ptr_Input(&msg->tx.ptr, UART_DMA_QUEUE_SIZE);
}
