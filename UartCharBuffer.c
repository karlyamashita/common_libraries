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
 *          This will parse the character buffer, looking for a '\r' to then save the characters to a message buffer.
 *      � Use ParseUartRxMessageBuffer() as an example to parse the message buffer. Write you own code inside the function.
 *
 *      // Transmit info
 *      � Use UartAddTxMessageBuffer() to add a message to a Tx message buffer.
 *      � Call UartSendMessage() from a polling routine. This will send any messages in the transmit message buffer.
 *
 *
 *
 *      Note1: Add these defines to new/existing UartIncludes.h file and adjust value accordingly.
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
        if(uartRxCharBuffer[uartRxCharBufferPointer.iIndexOUT] == '\n'){
            DRV_RingBuffPtr__Output(&uartRxCharBufferPointer, MAX_UART_RX_CHAR_BUFFER); // Don't care about new line so go to next character in buffer
            return;
        }
        uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexIN].data[idxPtr++] = uartRxCharBuffer[uartRxCharBufferPointer.iIndexOUT]; // save character
        if(uartRxCharBuffer[uartRxCharBufferPointer.iIndexOUT] == '\r'){ // check if carriage return
            idxPtr = 0;  // reset pointer
            DRV_RingBuffPtr__Input(&uartRxMsgBufferPointer, MAX_UART_RX_MESSAGE_BUFFER); // new message complete
        }
        DRV_RingBuffPtr__Output(&uartRxCharBufferPointer, MAX_UART_RX_CHAR_BUFFER); // next character
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
void UartAddTxMessageBuffer(UartCharBufferTxStruct *uartBufferPointer_IN){
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
    HAL_StatusTypeDef HAL_Status;
    if (uartTxMsgBufferPointer.iCnt_Handle) {
        // TODO - pass argument which uart port to send on
        HAL_Status = UartTxMessage(&uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexOUT]);
        if (HAL_Status == HAL_OK) {
            DRV_RingBuffPtr__Output(&uartTxMsgBufferPointer, MAX_UART_TX_MESSAGE_BUFFER);
        }
    }
}


/*
 * Description: add one character to the character buffer.
 *
 * Input: reference to char array
 * Output: HAL Status
 */
HAL_StatusTypeDef UartAddCharToBuffer(uint8_t uartPort, char *_char_IN){
    if(uartRxCharBufferPointer.iCnt_OverFlow){
        // character buffer is full, return and let UartParseRxCharBuffer() routine parse character buffer to free some space.
        return HAL_BUSY;
    }
    uartRxCharBuffer[uartRxCharBufferPointer.iIndexIN] = *_char_IN;
    DRV_RingBuffPtr__Input(&uartRxCharBufferPointer, MAX_UART_RX_CHAR_BUFFER);
    return HAL_OK;
}

/*
 * Description: Helper to copy the uart port and data to a buffer structure
 *
 * Input: The Port number, the transmit data structure, the string to copy to array
 * Ouput: none
 *
 */
int UartCopyStrToCharBufferTxStruct(uint8_t uartPort, UartCharBufferTxStruct *uartTx_OUT, char *str_IN){
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


