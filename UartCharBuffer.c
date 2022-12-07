/*
 * UartCharBuffer.c
 *
 *  Created on: Dec 2, 2019
 *      Author: Karl
 *
 *      06-16-2021 Rev. 1.0.1 - Changed code to make more universal.
 *      10-19-2022 Rev. 1.0.2 - Can now send bytes instead of ASCII only
 *      11-30-2022 Rev. 1.0.3 - Renamed Functions. Created API calls so user doesn't need to deal with actual ring buffers and pointers,
 *      							at least for the most part.
 *      					  - Now have separate ASCII and BINARY buffer.
 *      							Typically for two UARTS, one for commands and the other for telemetry data.
 *
 *		// Notes for Basic Info, Receive info and Transmit info have not been updated yet.
 *      // Basic Info
 *      - This is a universal uart message buffer for ASCII characters. It has a ring buffer for individual characters and a ring buffer for messages.
 *      - Do not use this for 8bit data. Receiving 8bit data requires a different type of buffer and parser.
 *      - This file shouldn't need any modifications except for bug fixes.
 *
 *      // Receive info
 *      - Use AddUartCharBuffer() to add single characters to a character buffer. You would usually call this from a uart IRQ.
 *      - Call UartParseRxCharBuffer() from a polling routine.
 *         This will parse the character buffer, looking for a '\r' to then save the characters to a message buffer.
 *      - Use ParseUartRxMessageBuffer() as an example to parse the message buffer. Write you own code inside the function.
 *
 *      // Transmit info
 *      - Use UartAddTxMessageBuffer() to add a message to a Tx message buffer.
 *      - Call UartSendMessage() from a polling routine. This will send any messages in the transmit message buffer.



        Note1: Adjust value in UartCharBuffer.h file accordingly.
        #define MAX_UART_RX_IRQ_BYTE_LENGTH 1

        #define MAX_UART_RX_CHAR_BUFFER 192 // this should be double the size of data you are receiving up to '\r' character.
        #define MAX_UART_TX_CHAR_BUFFER 192
        #define MAX_UART_RX_MESSAGE_BUFFER 3
        #define MAX_UART_TX_MESSAGE_BUFFER 3

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

UartRxByteBufferStruct uartRx_ASCII_Buf;
UartRxByteBufferStruct uartRx_BINARY_Buf;


// Rx string buffer
UartRxMsgBufferStruct uartRx_STR_Buf;
// Tx string buffer
UartTxMsgBufferStruct uartTx_STR_Buf;


// Rx binary buffer
UartRxMsgBufferStruct uartRx_BINARY_Packet;
// Tx binary buffer
UartTxMsgBufferStruct uartTx_BINARY_Packet;


static uint32_t UART_StringMessageGetLength(void);
static uint32_t UART_BinaryPacketGetLength(void);


/*
 * Description: Sort UART character buffer and save string in message buffer. The string being received should have LF terminator
 *              This needs to be called from a polling routine.
 *
 * Input: Index pointer to which byte buffer to parse
 * Output: none
 */
void UART_SortRx_CHAR_Buffer(void)
{
    static uint32_t idxPtr = 0;
    uint32_t portNumber = 0;

    if (uartRx_ASCII_Buf.ringPtr.iCnt_Handle)
    {
    	uartRx_STR_Buf.BufStruct[uartRx_STR_Buf.RingBuff.ringPtr.iIndexIN].data[idxPtr++] = uartRx_ASCII_Buf.data[uartRx_ASCII_Buf.ringPtr.iIndexOUT];
        if(uartRx_ASCII_Buf.data[uartRx_ASCII_Buf.ringPtr.iIndexOUT] == '\n'){ // check if line feed
            idxPtr = 0;  // reset pointer
            uartRx_STR_Buf.BufStruct[uartRx_STR_Buf.RingBuff.ringPtr.iIndexIN].uartPort = portNumber;
            uartRx_STR_Buf.BufStruct[uartRx_STR_Buf.RingBuff.ringPtr.iIndexIN].dataSize = idxPtr - 1;
            DRV_RingBuffPtr__Input(&uartRx_STR_Buf.RingBuff.ringPtr, MAX_UART_RX_MESSAGE_BUFFER); // new message complete
        }
        DRV_RingBuffPtr__Output(&uartRx_ASCII_Buf.ringPtr, MAX_UART_RX_BYTE_BUFFER); // next character
    }
}

/*
 * Description: Sort UART Binary buffer and save binary packet in message buffer.
 *              This needs to be called from a polling routine.
 *
 * Input: Index pointer to which UART buffer to parse, the size of bytes to receive for a complete packet
 * Output: none
 */
void UART_SortRx_BINARY_Buffer(void)
{
    uint32_t i = 0;
    uint32_t portNumber = 0; // TODO - need to determine port.
    uint8_t checkSum = 0; // MOD256 checksum
    uint8_t tempTelemetry[MAX_UART_RX_BYTE_BUFFER];
    uint32_t pointer = 0;

    if (uartRx_BINARY_Buf.ringPtr.iCnt_Handle >= UART_PACKET_SIZE)
    {
        // copy the bytes to a temporary array
        for(i = 0; i < UART_PACKET_SIZE; i++)
        {
            pointer = uartRx_BINARY_Buf.ringPtr.iIndexOUT + i;
            if(pointer >= MAX_UART_RX_BYTE_BUFFER)// past max range
            {
                pointer -= MAX_UART_RX_BYTE_BUFFER;
            }

            tempTelemetry[i] = uartRx_BINARY_Buf.data[pointer];
        }
        
        // calculate checksum
        for(i = 0; i < (UART_PACKET_SIZE - 1); i++)
        {
            checkSum += tempTelemetry[i];
        }
        
        // verify checksum
        if(checkSum == tempTelemetry[UART_PACKET_SIZE - 1]) // compare checksum to last index
        {
            // we have a crc match so save the packet to the Rx packet buffer
            for(i = 0; i < UART_PACKET_SIZE; i++)
            {
                uartRx_BINARY_Packet.BufStruct[uartRx_BINARY_Packet.RingBuff.ringPtr.iIndexIN].data[i] = uartRx_BINARY_Buf.data[uartRx_BINARY_Buf.ringPtr.iIndexOUT];
                DRV_RingBuffPtr__Output(&uartRx_BINARY_Buf.ringPtr, MAX_UART_RX_BYTE_BUFFER );
            }
            checkSum = 0; // reset checksum
            uartRx_BINARY_Packet.BufStruct[uartRx_BINARY_Packet.RingBuff.ringPtr.iIndexIN].uartPort = portNumber;
            DRV_RingBuffPtr__Input(&uartRx_BINARY_Packet.RingBuff.ringPtr, MAX_UART_RX_MESSAGE_BUFFER); // increment rx packet pointer
        }
        else
        {
            DRV_RingBuffPtr__Output(&uartRx_BINARY_Buf.ringPtr, MAX_UART_RX_BYTE_BUFFER ); // increment rx byte pointer
        }
    }
}

/*
 * Description: Example code using the API calls below.
 * 					This should be created in user file and called in a main loop polling routine
 *

void UART_CheckForNewMessage(void)
{
	char msg[MAX_UART_RX_BYTE_BUFFER] = {0};

	if(UART_StringMessagePending())
	{
		UART_RxStringMessageCopyNoCRLF(msg); // copy current ring buffer message to msg
		UART_StringMessageClear(); // clears the current ring buffer
		UART_StringMessageIncPtr(); // increment the ring buffer pointer.

		// user can parse msg variable.

	}
}
 */


/*
 * Description: Checks for pending string message in message ring buffer.
 * Input: none
 * Output: none
 *
 * Return: true if message available, else returns false
 *
 */
bool UART_RxStringMessagePending(void)
{
    if(uartRx_STR_Buf.RingBuff.ringPtr.iCnt_Handle != 0)
    {
        return true;
    }
    return false;
}

/*
 * Description: clears the current ring buffer array
 */
void UART_RxStringMessageClear(void)
{
    memset(&uartRx_STR_Buf.BufStruct[uartRx_STR_Buf.RingBuff.ringPtr.iIndexOUT].data, 0, MAX_UART_RX_BYTE_BUFFER); // clear data  
}

/*
 * Description: Increments ring pointer. This should be called in main loop
 */
void UART_RxStringMessageIncPtr(void)
{
    DRV_RingBuffPtr__Output(&uartRx_STR_Buf.RingBuff.ringPtr, MAX_UART_RX_MESSAGE_BUFFER);
}

/*
 * Description: Copies message from ring buffer to retStr NOT including the CR and LF characters.
 */
void UART_RxStringMessageCopyNoCRLF(char *retStr)
{
    memcpy(retStr, (char*)&uartRx_STR_Buf.BufStruct[uartRx_STR_Buf.RingBuff.ringPtr.iIndexOUT].data, UART_StringMessageGetLength() - 2); // all except CR LF
}

/*
 * Description: Copies message from ring buffer to retStr including the CR and LF characters.
 */
void UART_RxStringMessageCopy(char *retStr)
{
    memcpy(retStr, &uartRx_STR_Buf.BufStruct[uartRx_STR_Buf.RingBuff.ringPtr.iIndexOUT].data, UART_StringMessageGetLength());
}


/*
 * TODO - API calls for Binary packets
 */
/*
 * Description: Checks for pending binary packet in message ring buffer.
 * Input: none
 * Output: none
 *
 * Return: true if packet available, else returns false
 *
 */
bool UART_RxBinaryPacketPending(void)
{
    if(uartRx_BINARY_Packet.RingBuff.ringPtr.iCnt_Handle != 0)
    {
        return true;
    }
    return false;
}

/*
 * Description: Increments ring pointer. This should be called in main loop
 */
void UART_RxBinaryPacketIncPtr(void)
{
    DRV_RingBuffPtr__Output(&uartRx_STR_Buf.RingBuff.ringPtr, MAX_UART_RX_MESSAGE_BUFFER);
}


/*
 * Description: Copies message from ring buffer to retStr including the CR and LF characters.
 */
void UART_RxBinaryPacketCopy(uint8_t *retData)
{
	uint32_t dataSize = uartRx_STR_Buf.BufStruct[uartRx_STR_Buf.RingBuff.ringPtr.iIndexOUT].dataSize;

    memcpy(retData, &uartRx_STR_Buf.BufStruct[uartRx_STR_Buf.RingBuff.ringPtr.iIndexOUT].data, dataSize);
}


/*
 * Description: Add message to message buffer to be sent. Use either UartCopyStringToTxStruct() or UartCopyBinaryDataToTxStruct() to build message first
 *              Be sure to call UartSendMessage() in a polling routine
 *
 * Input: The message structure
 * 6844
 * Output:
 *
 */
void UART_TX_AddMessageToBuffer(UartTxMsgBufferStruct *uartMsg)
{
    uint8_t i = 0;
    uint8_t *pData = uartMsg->BufStruct->data;
    uint32_t dataSize = uartMsg->BufStruct->dataSize;

    uartTx_STR_Buf.BufStruct[uartTx_STR_Buf.RingBuff.ringPtr.iIndexIN].uartPort = uartMsg->BufStruct->uartPort;
    uartTx_STR_Buf.BufStruct[uartTx_STR_Buf.RingBuff.ringPtr.iIndexIN].dataSize = dataSize;

    for(i = 0; i < dataSize; i++)
    {
        uartTx_STR_Buf.BufStruct[uartTx_STR_Buf.RingBuff.ringPtr.iIndexIN].data[i] = *pData++;
    }

    DRV_RingBuffPtr__Input(&uartTx_STR_Buf.RingBuff.ringPtr, MAX_UART_TX_MESSAGE_BUFFER);
}

/*
 * Description: Call this from a polling routine.
 *              UartTxMessage() should be part of your code for your specific MCU. See function below this one for example
 *
 * Input: none
 * Output: none
 *
 */
void UART_SendMessage(void){
    int status = NO_ERROR;

    if (uartTx_STR_Buf.RingBuff.ringPtr.iCnt_Handle)
    {
        status = UART_TxMessage(&uartTx_STR_Buf, uartTx_STR_Buf.RingBuff.ringPtr.iIndexOUT);
        // We need to monitor the return status for a failed transmit so not to increment the pointer yet
        // so on the next loop we can try to send again
        if (status == NO_ERROR)
        {
            DRV_RingBuffPtr__Output(&uartTx_STR_Buf.RingBuff.ringPtr, MAX_UART_TX_MESSAGE_BUFFER);
        }
    }
}

//*********************************************************
//		Example UartTxMessage function defined for STM32
//*********************************************************
/*
int UartTxMessage(UartTxMsgBufferStruct *uartTxMessage)
{
	HAL_StatusTypeDef HAL_Status;
	UART_HandleTypeDef huart;
	uint32_t count = uartTxMessage->dataSize;

	switch(uartTxMessage->uartPort){
	case UART_PORT_1:
		huart.Instance = huart1.Instance;
		break;
	case UART_PORT_2:
		huart.Instance = huart2.Instance;
		break;
	}

	HAL_Status = HAL_UART_Transmit_IT(&huart, (uint8_t*) uartTxMessage->data, count);
	if(HAL_Status != HAL_OK)
	{
		return UART_TX_ERROR; // if not HAL_OK then we need to re-transmit on the next loop
	}

	return NO_ERROR;
}
*/


/*
 * Description: Add one byte to the data buffer.
 *
 * Input: The pointer to data array, the size/length of data
 * Output: No error or overflow
 */
int UART_AddCharToBuffer(uint8_t *data, uint32_t sizeOfData){
	int i;
    
    if(uartRx_ASCII_Buf.ringPtr. iCnt_OverFlow){
        // byte buffer is full. You should monitor the return status during development and increase buffer size to if overflow;
        return UART_BUFFER_OVERFLOW;
    }

	for(i = 0; i < sizeOfData; i++)
	{
		uartRx_ASCII_Buf.data[uartRx_ASCII_Buf.ringPtr.iIndexIN] = *data;
		DRV_RingBuffPtr__Input(&uartRx_ASCII_Buf.ringPtr, MAX_UART_RX_BYTE_BUFFER);
	}

    return NO_ERROR;
}

/*
 * Description: Add one byte to the data buffer.
 *
 * Input: index to which data buffer to write to, the pointer to data array, the size/length of data
 * Output: No error or overflow
 */
int UART_AddByteToBuffer(uint8_t *data, uint32_t sizeOfData)
{
	int i;
    
    if(uartRx_BINARY_Buf.ringPtr. iCnt_OverFlow){
        // byte buffer is full. You should monitor the return status during development and increase buffer size to if overflow;
        return UART_BUFFER_OVERFLOW;
    }

	for(i = 0; i < sizeOfData; i++)
	{
		uartRx_BINARY_Buf.data[uartRx_BINARY_Buf.ringPtr.iIndexIN] = *data;
		DRV_RingBuffPtr__Input(&uartRx_BINARY_Buf.ringPtr, MAX_UART_RX_BYTE_BUFFER);
	}

    return NO_ERROR;
}


/*
 * Description: Helper to copy the UART port and characters to a buffer structure
 *
 * Input: The Port number, the transmit data structure, the string to copy to array
 * Output: The uartTx_OUT structure
 *
 */
int UART_CopyStringToTxStruct(uint8_t uartPort, char *str_IN, UartTxMsgBufferStruct *uartTx_OUT)
{
    uint8_t *pData = uartTx_OUT->BufStruct->data; // pointer to data

    if(strlen(str_IN) == 0) return -1; // make sure there is some characters.
    memset(uartTx_OUT, 0, sizeof(*uartTx_OUT)); // clear variable
    uartTx_OUT->BufStruct->uartPort = uartPort; // save uart port
    uartTx_OUT->BufStruct->dataSize = strlen(str_IN);
    memcpy(pData, (uint8_t*)str_IN, strlen(str_IN)); // copy str to data array.

    return 0;
}

/*
 * Description: Helper to copy the UART port and binary data to a buffer structure
 *
 * Input: The UART port number, the string to copy, the data size, the transmit data structure to copy all parameters to
 * Output: The uartTx_OUT structure
 *
 */
int UART_CopyBinaryDataToTxStruct(uint8_t uartPort, uint8_t *dataIN, uint32_t sizeOfData, UartTxMsgBufferStruct *uartTx_OUT)
{
    uint8_t *pData = uartTx_OUT->BufStruct->data; // pointer to data

    uartTx_OUT->BufStruct->uartPort = uartPort; // save uart port
    uartTx_OUT->BufStruct->dataSize = sizeOfData;
    memcpy(pData, (uint8_t*)dataIN, sizeOfData); // copy binary data to structure array.

    return 0;
}


/*
 * Description: Gets the string length
 */
static uint32_t UART_StringMessageGetLength(void)
{
    return strlen((char*)uartRx_STR_Buf.BufStruct[uartRx_STR_Buf.RingBuff.ringPtr.iIndexOUT].data);
}

/*
 * Description: Gets the binary packet length
 */
static uint32_t UART_BinaryPacketGetLength(void)
{
    return uartRx_STR_Buf.BufStruct[uartRx_STR_Buf.RingBuff.ringPtr.iIndexOUT].dataSize;
}

