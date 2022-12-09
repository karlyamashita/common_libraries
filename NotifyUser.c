
#include "main.h"
#include "NotifyUser.h"


/*
 * This file also needs to include UartCharBuffer.h
 *
 *
 */


/*
 * Description: This will add CR and LF to string before saving to tx buffer.
 * Input: Pointer to buffer, the string to add to tx buffer, and linefeed enable
 *
 */
void NotifyUser(UartTxBufferStruct *buffer, char *str, bool lineFeed)
{
    uint8_t strMsg[UART_TX_BYTE_BUFFER_SIZE] = {0};

    strcpy((char*)strMsg, str);
    
    if(lineFeed == true)
    {
    	strcat((char*)strMsg, "\r\n");
    }

    buffer->BufStruct.msgQueueSize[buffer->RingBuff.msgPtr.iIndexIN].dataSize = strlen((char*)strMsg);
    UART_TX_AddMessageToBuffer(buffer, strMsg, strlen((char*)strMsg));
}

