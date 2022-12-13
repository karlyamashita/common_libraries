/*
    UartCharBuffer.c

    Created on: Dec 2, 2019
        Author: Karl

        06-16-2021 Rev. 1.0.1 - Changed code to make more universal.
        10-19-2022 Rev. 1.0.2 - Can now send bytes instead of ASCII only
        11-30-2022 Rev. 1.0.3 - Renamed Functions. Created API calls so user doesn't need to deal with actual ring buffers and pointers,
       							 at least for the most part.
       					  	  - Now have separate ASCII and BINARY buffer.
       							 Typically for two UARTS, one for commands and the other for telemetry data.
        12-7-2022 Rev. 1.1.0 - Complete re-write.


        // Basic Info
        - This has a ring buffer for individual bytes and a ring buffer for messages.


        Note1: Adjust value in UartCharBuffer.h file accordingly.
			#define UART_RX_IRQ_BYTE_SIZE 1

			#define UART_RX_BYTE_BUFFER_SIZE 192 // this holds all the IRQ data
			#define UART_TX_BYTE_BUFFER_SIZE 192
			#define UART_RX_MESSAGE_QUEUE_SIZE 8 // buffer size of complete strings or packets.
			#define UART_TX_MESSAGE_QUEUE_SIZE 8 // buffer size of complete strings or packets.

			#define UART_RX_PACKET_SIZE 16 // The number of bytes for complete packet + checksum.


		Note2:
		- User will need to define global variables instance for each rx and tx UART port.
			UartRxBufferStruct uart2_rxMsg = {0};
			UartTxBufferStruct uart2_txMsg = {0};
		- User will need to create a message queue for each rx and tx UART port
			UartMsgQueueStruct rxMsgQueue_2[UART_RX_MESSAGE_QUEUE_SIZE];
			UartMsgQueueStruct txMsgQueue_2[UART_TX_MESSAGE_QUEUE_SIZE];
		- User will need to assign each buffer queue to an instance of UartRxBufferStruct created
			UART_InitRxBuffer(&uart2_rxMsg, rxMsgQueue_2);
			UART_InitTxBuffer(&uart2_txMsg, txMsgQueue_2);



 */

#include "main.h"
#include "UartCharBuffer.h"


static uint32_t UART_StringMessageGetLength(UartRxBufferStruct *buffer);

/*
 * Description: Point to array that holds the rx message queue
 *
 */
void UART_InitRxBuffer(UartRxBufferStruct *buffer, UartMsgQueueStruct *msgBuffer)
{
	buffer->BufStruct.msgQueue = msgBuffer;
}

/*
 * Description: Point to array that holds the tx message queue
 *
 */
void UART_InitTxBuffer(UartTxBufferStruct *buffer, UartMsgQueueStruct *msgBuffer)
{
	buffer->BufStruct.msgQueue = msgBuffer;
}

void UART_Add_IRQ_Byte(UartRxBufferStruct *buffer, uint8_t *char_in, uint32_t dataSize)
{
	int i;
	uint8_t *ptr = (uint8_t*)char_in;

	for(i = 0; i < dataSize; i++)
	{
		buffer->BufStruct.uartIRQ_ByteBuffer[i] = *ptr++;
	}
}


/*
 * Description: Sort UART character buffer and save string in message buffer. The string being received should have LF terminator
 *              This needs to be called from a polling routine.
 *
 * Input: Index pointer to which byte buffer to parse
 * Output: none
 */
void UART_SortRx_CHAR_Buffer(UartRxBufferStruct *buffer)
{
    if(buffer->RingBuff.bytePtr.iCnt_Handle)
    {
    	buffer->BufStruct.msgQueue[buffer->RingBuff.msgPtr.iIndexIN].msgData[buffer->BufStruct.sortPtr[buffer->RingBuff.msgPtr.iIndexIN].ptr++] = buffer->BufStruct.byteBuffer[buffer->RingBuff.bytePtr.iIndexOUT];
    	if(buffer->BufStruct.byteBuffer[buffer->RingBuff.bytePtr.iIndexOUT] == '\n')
    	{
            buffer->BufStruct.msgQueueSize[buffer->RingBuff.msgPtr.iIndexIN].dataSize = buffer->BufStruct.sortPtr[buffer->RingBuff.msgPtr.iIndexIN].ptr - 1;
            DRV_RingBuffPtr__Input(&buffer->RingBuff.msgPtr, UART_RX_MESSAGE_QUEUE_SIZE);
            buffer->BufStruct.sortPtr[buffer->RingBuff.msgPtr.iIndexIN].ptr = 0;
        }
    	DRV_RingBuffPtr__Output(&buffer->RingBuff.bytePtr, UART_RX_BYTE_BUFFER_SIZE);
    }
}

/*
 * Description: Sort UART Binary buffer and save binary packet in message buffer.
 *              This needs to be called from a polling routine.
 *
 * Input: Index pointer to which UART buffer to parse, the size of bytes to receive for a complete packet
 * Output: none
 */
void UART_SortRx_BINARY_Buffer(UartRxBufferStruct *buffer)
{
    uint32_t i = 0;
    uint8_t checkSum = 0; // MOD256 checksum
    uint8_t tempTelemetry[UART_RX_BYTE_BUFFER_SIZE];
    uint32_t pointer = 0;

    if (buffer->RingBuff.bytePtr.iCnt_Handle >= UART_RX_PACKET_SIZE)
    {
        // copy the bytes to a temporary array
        for(i = 0; i < UART_RX_PACKET_SIZE; i++)
        {
            pointer = buffer->RingBuff.bytePtr.iIndexOUT + i;
            if(pointer >= UART_RX_BYTE_BUFFER_SIZE)// past max range
            {
                pointer -= UART_RX_BYTE_BUFFER_SIZE;
            }

            tempTelemetry[i] = buffer->BufStruct.byteBuffer[pointer];
        }
        
        // calculate checksum
        for(i = 0; i < (UART_RX_PACKET_SIZE - 1); i++)
        {
            checkSum += tempTelemetry[i];
        }
        
        // verify checksum
        if(checkSum == tempTelemetry[UART_RX_PACKET_SIZE - 1]) // compare checksum to last index
        {
            // we have a crc match so save the packet to the Rx packet buffer
            for(i = 0; i < UART_RX_PACKET_SIZE; i++)
            {
                buffer->BufStruct.msgQueue[buffer->RingBuff.msgPtr.iIndexIN].msgData[i] = buffer->BufStruct.byteBuffer[buffer->RingBuff.bytePtr.iIndexOUT];
                DRV_RingBuffPtr__Output(&buffer->RingBuff.bytePtr, UART_RX_BYTE_BUFFER_SIZE );
            }
            checkSum = 0; // reset checksum
            DRV_RingBuffPtr__Input(&buffer->RingBuff.msgPtr, UART_RX_MESSAGE_QUEUE_SIZE); // increment rx packet pointer
        }
        else
        {
            DRV_RingBuffPtr__Output(&buffer->RingBuff.bytePtr, UART_RX_BYTE_BUFFER_SIZE ); // increment rx byte pointer
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
	char messageCopy[MAX_UART_RX_BYTE_BUFFER] = {0};

	if(UART_RxStringMessagePending(&uart2_rxMsg))
	{
		UART_RxStringMessageCopy(&uart2_rxMsg, msg);// copy current ring buffer message to msg
		UART_RxStringMessageCopyNoCRLF(&uart2_rxMsg, messageCopy); // copy of msg
		UART_RxStringMessageClear(&uart2_rxMsg); // clears the current ring buffer
		UART_RxStringMessageIncPtr(&uart2_rxMsg); // increment the ring buffer pointer.

		RemoveSpaces(ptr);

		// user can parse msg variable.
		if(strncmp(ptr, "setfirst:", strlen("setfirst:")) == 0)
		{
			ptr += strlen("setfirst:");
			firstDigit = atoi(ptr);
			sprintf(str, " %ld", firstDigit);
			ReturnGetMessageData(messageCopy, str);
			NotifyUser(&uart2_txMsg, messageCopy, true);
		}
		else ... more compares
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
bool UART_RxStringMessagePending(UartRxBufferStruct *buffer)
{
	if(buffer->RingBuff.msgPtr.iCnt_Handle != 0)
    {
        return true;
    }
    return false;
}

/*
 * Description: clears the current ring buffer array
 */
void UART_RxStringMessageClear(UartRxBufferStruct *buffer)
{
	memset(&buffer->BufStruct.msgQueue[buffer->RingBuff.msgPtr.iIndexOUT].msgData, 0, UART_RX_BYTE_BUFFER_SIZE);
}

/*
 * Description: Increments ring pointer. This should be called in main loop
 */
void UART_RxStringMessageIncPtr(UartRxBufferStruct *buffer)
{
	DRV_RingBuffPtr__Output(&buffer->RingBuff.msgPtr, UART_RX_MESSAGE_QUEUE_SIZE);
}

/*
 * Description: Copies message from ring buffer to retStr NOT including the CR and LF characters.
 */
void UART_RxStringMessageCopyNoCRLF(UartRxBufferStruct *buffer, char *retStr)
{
	memcpy(retStr, (char*)&buffer->BufStruct.msgQueue[buffer->RingBuff.msgPtr.iIndexOUT].msgData, UART_StringMessageGetLength(buffer) - 2);
}

/*
 * Description: Copies message from ring buffer to retStr including the CR and LF characters.
 */
void UART_RxStringMessageCopy(UartRxBufferStruct *buffer, char *retStr)
{
	memcpy(retStr, &buffer->BufStruct.msgQueue[buffer->RingBuff.msgPtr.iIndexOUT].msgData, UART_StringMessageGetLength(buffer));
}



// TODO - Need to combine these binary code with string code above as they are similar

/*
 * Description: Checks for pending packet ring buffer.
 * Input: none
 * Output: none
 *
 * Return: true if packet available, else returns false
 *
 */
bool UART_RxPacketPending(UartRxBufferStruct *buffer)
{
	if(buffer->RingBuff.msgPtr.iCnt_Handle != 0)
    {
        return true;
    }
    return false;
}

/*
 * Description: clears the current ring buffer array
 */
void UART_RxPacketClear(UartRxBufferStruct *buffer, uint32_t packetSize)
{
	memset(&buffer->BufStruct.msgQueue[buffer->RingBuff.msgPtr.iIndexOUT].msgData, 0, packetSize);
}

/*
 * Description: Increments ring pointer. This should be called in main loop
 */
void UART_RxPacketIncPtr(UartRxBufferStruct *buffer)
{
	DRV_RingBuffPtr__Output(&buffer->RingBuff.msgPtr, UART_RX_MESSAGE_QUEUE_SIZE);
}

/*
 * Description: Copies message from ring buffer to retStr including the CR and LF characters.
 */
void UART_RxPacketCopy(UartRxBufferStruct *buffer, uint32_t packetSize, uint8_t *retStr)
{
	memcpy(retStr, &buffer->BufStruct.msgQueue[buffer->RingBuff.msgPtr.iIndexOUT].msgData, packetSize);
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
void UART_TX_AddMessageToBuffer(UartTxBufferStruct *buffer, uint8_t *msg, uint32_t dataSize)
{
    uint8_t i = 0;
    uint8_t *pData = (uint8_t*)msg;

    for(i = 0; i < dataSize; i++)
    {
    	buffer->BufStruct.msgQueue[buffer->RingBuff.msgPtr.iIndexIN].msgData[i] = *pData++;
    }
    buffer->BufStruct.msgQueueSize[buffer->RingBuff.msgPtr.iIndexIN].dataSize = dataSize;
    DRV_RingBuffPtr__Input(&buffer->RingBuff.msgPtr, UART_TX_MESSAGE_QUEUE_SIZE);
}

uint32_t UART_TX_GetMessageSize(UartTxBufferStruct *buffer)
{
	return buffer->BufStruct.msgQueueSize[buffer->RingBuff.msgPtr.iIndexOUT].dataSize;
}

/*
 * Description: Call this from a polling routine.
 *              UartTxMessage() should be part of your code for your specific MCU. See function below this one for example
 *
 * Input: none
 * Output: none
 *
 */
void UART_SendMessage(UartTxBufferStruct *buffer, uint8_t uartPort)
{
    int status = NO_ERROR;

    if (buffer->RingBuff.msgPtr.iCnt_Handle)
    {
        status = UART_TxMessage(buffer, uartPort);
        // We need to monitor the return status for a failed transmit so not to increment the pointer yet
        // so on the next loop we can try to send again
        if (status == NO_ERROR)
        {
            DRV_RingBuffPtr__Output(&buffer->RingBuff.msgPtr, UART_TX_MESSAGE_QUEUE_SIZE);
        }
    }
}


/*
 * Description: Add one or more bytes to the byteBuffer. This would be called from a UART IRQ.
 *
 * Input: The pointer to data array, the size/length of data
 * Output: No error or overflow
 */
int UART_AddCharToBuffer(UartRxBufferStruct *buffer)
{
	int i;
    
	if(buffer->RingBuff.bytePtr.iCnt_OverFlow)
	{
        // byte buffer is full. You should monitor the return status during development and increase buffer size to if overflow;
        return UART_BUFFER_OVERFLOW;
    }

	for(i = 0; i < UART_RX_IRQ_BYTE_SIZE; i++)
	{
		buffer->BufStruct.byteBuffer[buffer->RingBuff.bytePtr.iIndexIN] = buffer->BufStruct.uartIRQ_ByteBuffer[i];
		DRV_RingBuffPtr__Input(&buffer->RingBuff.bytePtr, UART_RX_BYTE_BUFFER_SIZE);
	}

    return NO_ERROR;
}


/*
 * Description: Gets the rx message string length
 */
static uint32_t UART_StringMessageGetLength(UartRxBufferStruct *buffer)
{
    return strlen((char*)buffer->BufStruct.msgQueue[buffer->RingBuff.msgPtr.iIndexOUT].msgData);
}

