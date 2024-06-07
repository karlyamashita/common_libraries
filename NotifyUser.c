
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
void NotifyUser(UartBufferStruct *msg, char *str, uint32_t strlen, bool lineFeed)
{
    uint8_t strMsg[UART_TX_DATA_SIZE] = {0};
    uint32_t size = strlen;

    memcpy(&strMsg, str, size);
    
    if(lineFeed == true)
    {
    	strcat((char*)strMsg, "\r\n");
    	size += 2;
    }

    UART_TX_AddDataToBuffer(msg, strMsg, size);
}

// need to include ErrorStatus.c/h
void NotifyUser_Error(UartBufferStruct *msg, char *msgCpy, uint32_t error)
{
	char str[64] = {0};

	GetErrorString(error, str); // from ErrorStatus.c

	strcat(msgCpy, "=");
	strcat(msgCpy, str);

	NotifyUser(msg, msgCpy, strlen(msgCpy), true);
}

void NotifyUser_PrintReply(UartBufferStruct *msg, char *msgCpy, char *msg2)
{
	strcat(msgCpy, "=");
	strcat(msgCpy, msg2);
	NotifyUser(msg, msgCpy, strlen(msgCpy), true);
}

