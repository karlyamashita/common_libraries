/*
  	  UartCharBuffer.c

   Created on: Dec 2, 2019
       Author: Karl

       06-16-2021 Rev. 1.0.1 - Changed code to make more universal.
       10-19-2022 Rev. 1.0.2 - Can now send bytes instead of ASCII only

       // Basic Info
       • This is a universal uart message buffer for ASCII characters.
       • It has a ring buffer for individual characters and a ring buffer for messages.
       • This file shouldn't need any modifications except for bug fixes.

       // Receive info
       • Use AddUartCharBuffer() to add single characters to a character buffer. You would usually call this from a uart IRQ.
       • Call UartParseRxCharBuffer() from a polling routine.
          This will parse the character buffer, looking for a '\r' to then save the characters to a message buffer.
       • Use ParseUartRxMessageBuffer() as an example to parse the message buffer. Write you own code inside the function.

       // Transmit info
       • Use UartAddTxMessageBuffer() to add a message to a Tx message buffer.
       • Call UartSendMessage() from a polling routine. This will send any messages in the transmit message buffer.



       Note1: Adjust value in UartCharBuffer.h file accordingly.
        #define MAX_UART_RX_CHAR_BUFFER_SINGLE 1
        #define MAX_UART_RX_CHAR_BUFFER 192 // this should be double the size of data you are receiving up to '\r' character.
        #define MAX_UART_TX_CHAR_BUFFER 192
        #define MAX_UART_RX_MESSAGE_BUFFER 3
        #define MAX_UART_TX_MESSAGE_BUFFER 3

        Note2: These includes should be defined in main.h in these order
        #include "RingBuff.h"
        #include "UartCharBuffer.h"
 */

#include "main.h"
#include "UartCharBuffer.h"

// this is the array that holds the data from the UART interrupt. For STM32 we only need one character
uint8_t uartDataBuffer[MAX_UART_RX_CHAR_BUFFER_SINGLE] = {0};

// Rx character buffer. The array for the Uart IRQ.
uint8_t uartRxCharBuffer[MAX_UART_RX_CHAR_BUFFER];
RING_BUFF_INFO uartRxCharBufferPointer;

// Rx message buffer
UartCharBufferRxStruct uartRxMessageBuffer[MAX_UART_RX_MESSAGE_BUFFER];
RING_BUFF_INFO uartRxMsgBufferPointer;
// Tx message buffer
UartCharBufferTxStruct uartTxMessageBuffer[MAX_UART_TX_MESSAGE_BUFFER];
RING_BUFF_INFO uartTxMsgBufferPointer;


/*
 * Description: Parse uart character buffer and save in message buffer.
 *              This needs to be called from a polling routine
 *
 * Input: none
 * Output: none
 */
void UartParseRxCharBuffer(void){
    static uint32_t idxPtr = 0;
    if (uartRxCharBufferPointer.iCnt_Handle) {
        uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexIN].data[idxPtr++] = uartRxCharBuffer[uartRxCharBufferPointer.iIndexOUT]; // save character
        if(uartRxCharBuffer[uartRxCharBufferPointer.iIndexOUT] == '\n'){ // check if carriage return
            idxPtr = 0;  // reset pointer
            DRV_RingBuffPtr__Input(&uartRxMsgBufferPointer, MAX_UART_RX_MESSAGE_BUFFER); // new message complete
        }
        DRV_RingBuffPtr__Output(&uartRxCharBufferPointer, MAX_UART_RX_CHAR_BUFFER); // next character
    }
}

/*
 * Description: Parse uart buffer for bytes and save in message buffer.
 * 				You'll need to know how many bytes to parse and/or up to the checksum.
 *              This needs to be called from a polling routine
 *
 * Input: none
 * Output: none
 */
void UartParseRxByteBuffer(void){
	// not implemented yet
}


/*
 * Description: Add byte or char array to message buffer to be sent.
 *              Be sure to call UartSendMessage() in a polling routine
 *
 * Input: The data array
 * Output:
 *
 */
void UartAddTxMessageBuffer(UartCharBufferTxStruct *uartBufferPointer_IN){
    uint8_t i = 0;
    uint8_t *pData = uartBufferPointer_IN->data;

    if(strlen((char*)pData) == 0){ // array is empty
        return;
    }

    memset(&uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexIN].data, 0, sizeof(uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexIN].data));

    uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexIN].uartPort = uartBufferPointer_IN->uartPort;
    uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexIN].dataLength = uartBufferPointer_IN->dataLength;

    for(i = 0; i < uartBufferPointer_IN->dataLength; i++)
    {
        uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexIN].data[i] = *pData;
        pData++;
    }

    DRV_RingBuffPtr__Input(&uartTxMsgBufferPointer, MAX_UART_TX_MESSAGE_BUFFER);
}


/*
 * Description: Call this from a polling routine.
 *              UartTxMessage() This is a generic function and should be part of your code for your specific MCU.
 *
 *              ********* Example function defined for STM32 *********
 *              HAL_StatusTypeDef UartTxMessage(UartCharBufferTxStruct *uartBufferPointer)
 *              {
					HAL_StatusTypeDef HAL_Status;
					UART_HandleTypeDef huart;
					uint8_t count = strlen((char*) uartBufferPointer->data);

					switch(uartBufferPointer->uartPort){
					case UART_PORT_1:
						huart = huart1;
						break;
					}

				#if defined HAL_DMA_MODULE_ENABLED
					HAL_Status = HAL_UART_Transmit_DMA(&huart, (uint8_t*) uartBufferPointer->data, count);
				#else
					HAL_Status = HAL_UART_Transmit_IT(&huart, (uint8_t*) uartBufferPointer->data, count);
				#endif
					return HAL_Status;
				}

                ********* Example function defined for TM4C123 ********
                HAL_StatusTypeDef UartTxMessage(UartCharBufferTxStruct *uartBufferPointer)
                {
					uint32_t uart_base;
					uint8_t *pData = uartBufferPointer->data;
					uint8_t count = strlen((char*) pData);

					switch(uartBufferPointer->uartPort){
					case 1:
						uart_base = UART1_BASE;
						break;
					case 3:
						uart_base = UART3_BASE;
						break;
					}
					while(count--){
						UARTCharPut(uart_base, *pData++);
					}
					return HAL_OK;
				}

                ********* Example function defined for MicroBlaze ********
                HAL_StatusTypeDef UartTxMessage(UartCharBufferTxStruct *uartBufferPointer)
                {
					uint8_t *pData = uartBufferPointer->data;
					while (*pData != '\0') {
						OutbyteUart1(*pData);
						pData++;
					}
					return HAL_OK;
				}

                static void OutbyteUart1(char c) {
                    XUartLite_SendByte(XPAR_AXI_UARTLITE_1_BASEADDR, c);
                }

 *
 * Input: none
 * Output: none
 *
 */
void UartSendMessage(void){
    int status;
    if (uartTxMsgBufferPointer.iCnt_Handle) {
    	// UartTxMessage() is a generic call. You'll have to create a function specific to your MCU
        status = UartTxMessage(&uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexOUT]);
        if (status == NO_ERROR) {
            DRV_RingBuffPtr__Output(&uartTxMsgBufferPointer, MAX_UART_TX_MESSAGE_BUFFER);
        }
    }
}


/*
 * Description: add one byte or character to the character buffer.
 *
 * Input: reference to char array. uartPort not currently used 
 * Return: Error status
 */
int UartAddCharToBuffer(uint8_t uartPort, uint8_t *_char_IN){
    if(uartRxCharBufferPointer.iCnt_OverFlow){
        // character buffer is full, return and let UartParseRxCharBuffer() routine parse character buffer to free some space.
        return UART_BUSY;
    }
    uartRxCharBuffer[uartRxCharBufferPointer.iIndexIN] = *_char_IN;
    DRV_RingBuffPtr__Input(&uartRxCharBufferPointer, MAX_UART_RX_CHAR_BUFFER);

    return NO_ERROR;
}

/*
 * Description: Helper to copy the uart port and string to a buffer structure
 *
 * Input: The Port number, the string to copy to array
 * Output: The UartCharBufferTxStruct transmit data structure
 *
 */
int UartCopyStrToCharBufferTxStruct(uint8_t uartPort, char *str_IN, UartCharBufferTxStruct *uartTx_OUT)
{
    uint8_t *pData = uartTx_OUT->data; // pointer to data buffer
    uint32_t strLength = strlen(str_IN);

    if(strlen(str_IN) == 0) return -1; // make sure there are some characters.
    memset(uartTx_OUT, 0, sizeof(*uartTx_OUT)); // clear variable
    uartTx_OUT->uartPort = uartPort; // save uart port
    uartTx_OUT->dataLength = strLength; // save the string length
    memcpy(pData, (uint8_t*)str_IN, strLength); // copy str to data array.

    return 0;
}

/*
 * Description: Helper to copy the uart port and data array to a buffer structure
 *
 * Input: The Port number, the data array to copy to data buffer, the data length
 * Output: The UartCharBufferTxStruct transmit data structure
 *
 */
int UartCopyDataToDataBufferTxStruct(uint8_t uartPort, uint8_t *data_IN, uint32_t dataLen, UartCharBufferTxStruct *uartTx_OUT)
{
	uint8_t *pData = uartTx_OUT->data; // pointer to data buffer

	memset(uartTx_OUT, 0, sizeof(*uartTx_OUT)); // clear variable
	uartTx_OUT->uartPort = uartPort; // save uart port
	uartTx_OUT->dataLength = dataLen;
	memcpy(pData, data_IN, dataLen); // copy data array to buffer array.

	return 0;
}


/*
 * This is an example on how to parse a message from the message buffer. Copy this to a file and call this function from polling routine.
 *
 * You will need to define two extern variables where ever you have this function
 * • extern UartCharBufferRxStruct uartRxMessageBuffer[MAX_UART_RX_MESSAGE_BUFFER];
 * • extern RING_BUFF_INFO uartRxMsgBufferPointer;
 *
 */
/*
void ParseUartRxMessageBuffer(void){
    uint8_t message[MAX_UART_RX_CHAR_BUFFER] = {0};
	uint8_t *ptr = message;

	if(uartRxMsgBufferPointer.iCnt_Handle)
	{
		//copy buffer contents to array
		memcpy(&message, &uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexOUT].data, sizeof(message));

		// Example, if message was "get mcu fw version"
		RemoveSpaces((char*)ptr);
		// now message is "getmcufwversion"

		// look for keyword "get"
		if(strncmp((char*)message, "get", strlen("get")) == 0)
		{
			ptr += strlen("get"); // removes "get"
			GetMessage(ptr); // pass message "mcufwversion"
		}
		// if message is "set led green: on"
		else if(strncmp((char*)message, "set", strlen("set")) == 0)
		{
			ptr += strlen("set"); // removes "set"
			SetMessage(ptr); // pass message "ledgreen:on"
		}

		DRV_RingBuffPtr__Output(&uartRxMsgBufferPointer, MAX_UART_RX_MESSAGE_BUFFER);
	}
}
*/


