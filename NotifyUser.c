
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
    UartCharBufferTxStruct uartTx = {0};
    
    if(lineFeed == true)
    {
    	strcat(str, "\r\n"); // add CR and LF
    }

    UartCopyStrToCharBufferTxStruct(uartPort, str, &uartTx);
    UartAddTxMessageBuffer(&uartTx);
}

