
#include "main.h"
#include "NotifyUser.h"


/*
 * This file also needs to include UartCharBuffer.h
 *
 *
 */


/*
 * Description: This will add CR and LF to string before saving to tx buffer.
 * Input: Pointer to buffer, the string to add to tx buffer, the size of string, and CR/LF (carriage return/linefeed enable)
 *
 */
void NotifyUser(UartTxBufferStruct *msg, char *str, uint32_t size, bool lineFeed)
{
    uint8_t strMsg[UART_TX_BYTE_BUFFER_SIZE] = {0};

    //strcpy((char*)strMsg, str);
    memcpy(&strMsg, str, size);
    
    if(lineFeed == true)
    {
    	strcat((char*)strMsg, "\r\n");
    	 size += 1;
    }

    msg->msgQueue[msg->msgPtr.index_IN].size = size;
    UART_TX_AddDataToBuffer(msg, strMsg, size);
}

