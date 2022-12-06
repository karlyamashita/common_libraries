
#include "main.h"
#include "NotifyUser.h"


/*
 * This file also needs to include UartCharBuffer.h
 *
 *
 */


/*
 * Description: This will add CR and LF to string before saving to tx buffer.
 * 				Be sure the char array str length is large enough for the added characters.
 * Input: String or char array
 *
 *
 */
void NotifyUser(uint8_t uartPort, char *str, bool lineFeed)
{
    UartTxMsgBufferStruct uartTx;
    
    memset(&uartTx, 0 , sizeof(uartTx));
    
    char strMsg[MAX_UART_TX_BYTE_BUFFER] = {0};

    strcpy(strMsg, str);
    
    if(lineFeed == true)
    {
    	strcat(strMsg, "\r\n");
    }

    UartCopyStringToTxStruct(uartPort, strMsg, &uartTx);
    UartAddMessageToTxBuffer(&uartTx);
}

