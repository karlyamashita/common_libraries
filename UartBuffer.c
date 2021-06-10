/*
 * UartBuffer.c
 *
 *  Created on: Dec 2, 2019
 *      Author: Karl
 *
 *      This file currently does Tx/Rx with one uart (huart1). You can expand by adding new char/message buffers and checking for huartx.
 *
 */

#include "main.h"
#include "UartBuffer.h"

#include "RingBuff.h"

// extern are usually defined in main.c
extern UART_HandleTypeDef huart1; // Only two uarts being used. Will need to expand variables if more uarts are used.

uint8_t uartCharBuffer[1]; // only need 1 character to hold
// Rx huart1 character buffer.
uint8_t uartRxCharBuffer[MAX_UART_RX_CHAR_BUFFER];
RING_BUFF_INFO uartRxCharBufferPointer;

// huart1 message buffer
UartRxMsgTypeDef uartRxMessageBuffer[MAX_UART_RX_MESSAGE_BUFFER];
RING_BUFF_INFO uartRxMsgBufferPointer;
UartTxMsgTypeDef uartTxMessageBuffer[MAX_UART_TX_MESSAGE_BUFFER];
RING_BUFF_INFO uartTxMsgBufferPointer;


/*
 * Description: Enables interrupts for uart
 *
 * Input: The uart instance and pointer to data array
 * Output:
 */
void EnableUartRecInterrupt(UART_HandleTypeDef *huart, uint8_t *uartRxBuffer) {
	HAL_UART_Receive_IT(huart, uartRxBuffer, 1); // set to receive one character at a time.
}

/*
 * Description: Parse uart character buffer and save in message buffer. Be sure to call this from a polling routine
 * Input: none
 * Output: none
 */
void ParseUartRxCharBuffer(void){
	static uint32_t idxPtr = 0;
	if (uartRxCharBufferPointer.iCnt_Handle) {
		uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexIN].Data[idxPtr++] = uartRxCharBuffer[uartRxCharBufferPointer.iIndexOUT]; // save character
		if(uartRxCharBuffer[uartRxCharBufferPointer.iIndexOUT] == '\r'){ // check if carriage return
			idxPtr = 0;  // reset pointer
			DRV_RingBuffPtr__Input(&uartRxMsgBufferPointer, MAX_UART_RX_MESSAGE_BUFFER); // new message complete
		}
		DRV_RingBuffPtr__Output(&uartRxCharBufferPointer, MAX_UART_RX_CHAR_BUFFER); // next character
	}
}

/*
 * Description: Add array to buffer to be sent. Be sure to call SendUartMessage() in a polling routine
 *
 * Input: The data array
 * Output:
 *
 */
HAL_StatusTypeDef AddUartTxMessageBuffer(UART_HandleTypeDef *huart, uint8_t *msg){
	HAL_StatusTypeDef HAL_Status = HAL_OK;
	uint8_t i = 0;

	if(strlen((char*)msg) == 0){ // array is empty
		return HAL_ERROR;
	}

	memset(&uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexIN].Data, 0, sizeof(uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexIN].Data));

	while(*msg != '\0'){
		if(huart->Instance == huart1.Instance){
			uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexIN].Data[i++] = *msg;
		}
		msg++;
	}

	DRV_RingBuffPtr__Input(&uartTxMsgBufferPointer, MAX_UART_TX_MESSAGE_BUFFER);

	return HAL_Status;
}

/*
 * Description: Call this from a polling routine.
 *
 * Input:
 * Output:
 *
 */
void SendUartMessage(UART_HandleTypeDef *huart){
	HAL_StatusTypeDef HAL_Status;
	uint8_t count;
	if(huart->Instance == huart1.Instance){ // huart1
		if (uartTxMsgBufferPointer.iCnt_Handle) {
			count =	strlen((char*) uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexOUT].Data);
			HAL_Status = HAL_UART_Transmit_IT(huart, uartTxMessageBuffer[uartTxMsgBufferPointer.iIndexOUT].Data, count);
			if (HAL_Status == HAL_OK) {
				DRV_RingBuffPtr__Output(&uartTxMsgBufferPointer, MAX_UART_TX_MESSAGE_BUFFER);
			}
		}
	}
}

/*
 * Description: This overrides the HAL_UART_RxCpltCallback()
 *
 *
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == huart1.Instance){
		uartRxCharBuffer[uartRxCharBufferPointer.iIndexIN] = uartCharBuffer[0];
		DRV_RingBuffPtr__Input(&uartRxCharBufferPointer, MAX_UART_RX_CHAR_BUFFER);
	}

	EnableUartRecInterrupt(huart, uartCharBuffer);
}


/*
 * This is an example on how to parse a message from the message buffer. Copy this to your polling routine.
 *
 *
 */
/*
void ParseUartRxMessageBuffer(void){
	HAL_StatusTypeDef HAL_Status;
	uint8_t message[MAX_UART_RX_MESSAGE_BUFFER] = {0};
	uint8_t str[32] = {0};
	if(uartRxMsgBufferPointer.iCnt_Handle){
		memcpy(&message, &uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexOUT].Data, sizeof(message));
		memset(&uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexOUT].Data, 0, MAX_UART_RX_MESSAGE_BUFFER); // clear data

		if(strncmp(message, "CMD0", 4) == 0){
			sprintf((char*)str, "I received CMD0");
			HAL_Status = AddUartTxMessageBuffer(&huart1, str);
			if (HAL_Status != HAL_OK) {

			}
		}
		else if(strncmp(message, "CMD1:",5) == 0){
			sprintf((char*)str, "I received CMD1");
			HAL_Status = AddUartTxMessageBuffer(&huart1, str);
			if (HAL_Status != HAL_OK) {

			}
		}
		DRV_RingBuffPtr__Output(&uartRxMsgBufferPointer, MAX_UART_RX_MESSAGE_BUFFER);
	}
}
*/


