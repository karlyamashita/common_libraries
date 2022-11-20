/*
 * UartCharBuffer.c
 *
 *  Created on: Dec 2, 2019
 *      Author: Karl
 *
 *      06-16-2021 Rev. 1.0.1 - Changed code to make more universal.
 *
 *      // Basic Info
 *      � This is a universal uart message buffer for ASCII characters. It has a ring buffer for individual characters and a ring buffer for messages.
 *      � Do not use this for 8bit data. Receiving 8bit data requires a different type of buffer and parser.
 *      � This file shouldn't need any modifications except for bug fixes.
 *
 *      // Receive info
 *      � Use AddUartCharBuffer() to add single characters to a character buffer. You would usually call this from a uart IRQ.
 *      � Call UartParseRxCharBuffer() from a polling routine.
 *         This will parse the character buffer, looking for a '\r' to then save the characters to a message buffer.
 *      � Use ParseUartRxMessageBuffer() as an example to parse the message buffer. Write you own code inside the function.
 *
 *      // Transmit info
 *      � Use UartAddTxMessageBuffer() to add a message to a Tx message buffer.
 *      � Call UartSendMessage() from a polling routine. This will send any messages in the transmit message buffer.
 *
 *
 *
 *      Note1: Adjust value in UartCharBuffer.h file accordingly.
        #define MAX_UART_RX_CHAR_BUFFER_SINGLE 1
        #define MAX_UART_RX_CHAR_BUFFER 192 // this should be double the size of data you are receiving up to '\r' character.
        #define MAX_UART_TX_CHAR_BUFFER 192
        #define MAX_UART_RX_MESSAGE_BUFFER 3
        #define MAX_UART_TX_MESSAGE_BUFFER 3

        Note2: These includes should be defined in main.h in these order
        #include "RingBuff.h"
        #include "UartIncludes.h" // see Note1
        #include "UartCharBuffer.h"
 */

#include "main.h"
#include "UartCharBuffer.h"


// the array for the UART IRQ data received.
uint8_t uartIRQ_ByteBuffer[MAX_UART_RX_IRQ_BYTE_LENGTH];

/*
 * Description: I have a new project where I have to receive data on two UART ports where before it was only one.
 * 				I am rewriting the code to accept from either port. So index 0 can be whatever port you assign to it,
 * 				and index 1 is another port. It is up to the user to know that index 0 or 1 is what port your are receiving from.
 * 				The data can be either ASCII or Binary on either index, but you can't have both ASCII and Binary on the same index.
 * 				User will need to know which Parsing routine to use for that index, UartParseRx_BINARY_Buffer() or UartParseRx_CHAR_Buffer()
 *
 */
// Declare the data buffer for two ports to accept data in ASCII or binary.
UartRxByteBufferStruct uartRxByteBuffer[NUMBER_OF_UART_PORTS] = {0};
RING_BUFF_INFO uartRxByteBufferPointer[NUMBER_OF_UART_PORTS] = {0};


// Rx message buffer
UartRxMsgBufferStruct uartRxMessageBuffer[MAX_UART_RX_MESSAGE_BUFFER];
RING_BUFF_INFO uartRxMsgBufferPointer;
// Tx message buffer
UartTxMsgBufferStruct uartTxMessageBuffer[MAX_UART_TX_MESSAGE_BUFFER];
RING_BUFF_INFO uartTxMsgBufferPointer;


/*
 * Description: Parse UART character buffer and save in message buffer. The string being received should have LF terminator
 *              This needs to be called from a polling routine.
 *
 * Input: Index pointer to which byte buffer to parse
 * Output: none
 */
void UartParseRx_CHAR_Buffer(uint8_t index)
{
    static uint32_t idxPtr = 0;
    uint32_t portNumber;

    switch(index)
    {
    case 0:
    	portNumber = UART_INDEX_PTR_0;
    	break;
    case 1:
    	portNumber = UART_INDEX_PTR_1;
    	break;
    	// add more if more than 2 UART ports
    default:
    	portNumber = UART_INDEX_PTR_0;
    	break;
    }

    if (uartRxByteBufferPointer[index].iCnt_Handle) {
        uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexIN].data[idxPtr++] = uartRxByteBuffer[index].data[uartRxByteBufferPointer[index].iIndexOUT]; // save character
        if(uartRxByteBuffer[uartRxMsgBufferPointer.iIndexIN].data[uartRxByteBufferPointer[index].iIndexOUT] == '\n'){ // check if line feed
            idxPtr = 0;  // reset pointer
            uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexIN].uartPort = portNumber;
            uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexIN].dataSize = idxPtr - 1;
            DRV_RingBuffPtr__Input(&uartRxMsgBufferPointer, MAX_UART_RX_MESSAGE_BUFFER); // new message complete
        }
        DRV_RingBuffPtr__Output(&uartRxByteBufferPointer[index], MAX_UART_RX_BYTE_BUFFER); // next character
    }
}

/*
 * Description: Parse UART Binary buffer and save in message buffer.
 *              This needs to be called from a polling routine.
 *
 * Input: Index pointer to which UART buffer to parse, the size of bytes to receive for a complete packet
 * Output: none
 */
void UartParseRx_BINARY_Buffer(uint8_t index, uint32_t packetSize)
{
	static uint32_t idxPtr = 0;
    uint32_t portNumber;

    switch(index)
    {
    case 0:
    	portNumber = UART_INDEX_PTR_0;
    	break;
    case 1:
    	portNumber = UART_INDEX_PTR_1;
    	break;
    	// add more if more than 2 UART ports
    default:
    	portNumber = UART_INDEX_PTR_0;
    	break;
    }

    if (uartRxByteBufferPointer[index].iCnt_Handle)
    {
		uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexIN].data[idxPtr++] = uartRxByteBuffer[index].data[uartRxByteBufferPointer[index].iIndexOUT]; // save character
		if(idxPtr > packetSize)
		{
			if(ValidateChkSum(uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexIN].data, packetSize)) // validate checksum
			{
				DRV_RingBuffPtr__Input(&uartRxMsgBufferPointer, MAX_UART_RX_MESSAGE_BUFFER); // new message complete
				uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexIN].uartPort = portNumber;
				uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexIN].dataSize = packetSize;
			}
		}
        DRV_RingBuffPtr__Output(&uartRxByteBufferPointer[index], MAX_UART_RX_BYTE_BUFFER); // next character
    }
}

/*
 * Description: Add char array to message buffer to be sent.
 *              Be sure to call UartSendMessage() in a polling routine
 *
 * Input: The data array
 * Output:
 *
 */
void UartAddTxMessageBuffer(UartTxMsgBufferStruct *uartBufferPointer_IN){
    uint8_t i = 0;
    uint8_t *pData = uartBufferPointer_IN->data;

    if(strlen((char*)pData) == 0){ // array is empty
        return;
    }

    memset(&uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexIN].data, 0, sizeof(uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexIN].data));

    uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexIN].uartPort = uartBufferPointer_IN->uartPort;
    while(*pData != '\0'){
        uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexIN].data[i++] = *pData;
        pData++;
    }

    DRV_RingBuffPtr__Input(&uartTxMsgBufferPointer, MAX_UART_TX_MESSAGE_BUFFER);
}


/*
 * Description: Call this from a polling routine.
 *              UartTxMessage() should be part of your code for your specific MCU.
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
    HAL_StatusTypeDef HAL_Status = HAL_OK;
    if (uartTxMsgBufferPointer.iCnt_Handle) {
        // TODO - pass argument which uart port to send on
        HAL_Status = UartTxMessage(&uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexOUT]);
        if (HAL_Status == HAL_OK) {
            DRV_RingBuffPtr__Output(&uartTxMsgBufferPointer, MAX_UART_TX_MESSAGE_BUFFER);
        }
        else
        {

        }
    }
}


/*
 * Description: Add one byte to the data buffer.
 *
 * Input: index to which data buffer to write to, the pointer to data array, the size/length of data
 * Output: No error or overflow
 */
int UartAddByteToBuffer(uint8_t index, uint8_t *data, uint32_t sizeOfData){
	int i;
    if(uartRxByteBufferPointer[index].iCnt_OverFlow){
        // byte buffer is full. You should monitor the return status during development and increase buffer size to if overflow;
        return UART_BUFFER_OVERFLOW;
    }

	for(i = 0; i < sizeOfData; i++)
	{
		uartRxByteBuffer[index].data[uartRxByteBufferPointer[index].iIndexIN] = *data;
		DRV_RingBuffPtr__Input(&uartRxByteBufferPointer[index], MAX_UART_RX_BYTE_BUFFER);
	}

    return NO_ERROR;
}

/*
 * Description: Helper to copy the uart port and data to a buffer structure
 *
 * Input: The Port number, the transmit data structure, the string to copy to array
 * Output: none
 *
 */
int UartCopyStrToCharBufferTxStruct(uint8_t uartPort, UartTxMsgBufferStruct *uartTx_OUT, char *str_IN){
    uint8_t *pData = uartTx_OUT->data; // pointer to data
    if(strlen(str_IN) == 0) return -1; // make sure there is some characters.
    memset(uartTx_OUT, 0, sizeof(*uartTx_OUT)); // clear variable
    uartTx_OUT->uartPort = uartPort; // save uart port
    memcpy(pData, (uint8_t*)str_IN, strlen(str_IN)); // copy str to data array.
    return 0;
}

/*
 * This is an example on how to parse a message from the message buffer. Copy this to a file and call this function from polling routine.
 *
 * You will need to define two extern variables where ever you have this function
 * � extern UartCharBufferRxStruct uartRxMessageBuffer[MAX_UART_RX_MESSAGE_BUFFER];
 * � extern RING_BUFF_INFO uartRxMsgBufferPointer;
 *
 */
/*
void ParseUartRxMessageBuffer(void){
    UartCharBufferTxStruct uartTx;
    uint8_t message[MAX_UART_RX_CHAR_BUFFER] = {0};
    char str[64] = {0};
    int errorCode = 0;

    if(uartRxMsgBufferPointer.iCnt_Handle){
        memcpy(&message, &uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexOUT].data, sizeof(message));

        if(strncmp((char*)message, "CMD0", 4) == 0){
            strcpy(str, "I received CMD0\r\n");
            UartCopyStrToCharBufferTxStruct(UART_PORT_1 , &uartTx, str);
            UartAddTxMessageBuffer(&uartTx);
            TimerCallbackEnable(SendUart_Message, DISABLE);
        }
        else if(strncmp((char*)message, "CMD1", 4) == 0){
            strcpy(str, "I received CMD1\r\n");
            UartCopyStrToCharBufferTxStruct(UART_PORT_1 , &uartTx, str);
            UartAddTxMessageBuffer(&uartTx);
            TimerCallbackEnable(SendUart_Message, ENABLE);
        }
        else
        {
            errorCode = UartCopyStrToCharBufferTxStruct(UART_PORT_1 , &uartTx, (char*)message);
            if(errorCode != 0){
                return;
            }
            UartAddTxMessageBuffer(&uartTx);
        }
        memset(&uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexOUT].data, 0, MAX_UART_RX_CHAR_BUFFER); // clear data
        DRV_RingBuffPtr__Output(&uartRxMsgBufferPointer, MAX_UART_RX_MESSAGE_BUFFER);
    }
}
*/


