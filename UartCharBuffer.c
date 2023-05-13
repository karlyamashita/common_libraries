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


        Note2: Must include RingBuff.c/h files




 */

#include "main.h"
#include "UartCharBuffer.h"


static uint32_t UART_StringMessageGetLength(UartRxBufferStruct *msg);


void UART_Add_IRQ_Byte(UartRxBufferStruct *buffer, uint8_t *char_in, uint32_t dataSize)
{
	int i;
	uint8_t *ptr = (uint8_t*)char_in;

	for(i = 0; i < dataSize; i++)
	{
		buffer->uartIRQ_ByteBuffer[i] = *ptr++;
	}
}


/*
 * Description: Sort UART character buffer and save string in message buffer. The string being received should have LF terminator
 *              This needs to be called from a polling routine.
 *
 * Input: Index pointer to which byte buffer to parse
 * Output: none
 */
void UART_SortRx_CHAR_Buffer(UartRxBufferStruct *msg)
{
    if(msg->bytePtr.iCnt_Handle)
    {
    	msg->msgQueue[msg->msgPtr.iIndexIN].data[msg->sortPtr[msg->msgPtr.iIndexIN].ptr++] = msg->byteBuffer[msg->bytePtr.iIndexOUT];
    	if(msg->byteBuffer[msg->bytePtr.iIndexOUT] == '\n')
    	{
            msg->msgQueueSize[msg->msgPtr.iIndexIN].dataSize = msg->sortPtr[msg->msgPtr.iIndexIN].ptr;
            DRV_RingBuffPtr__Input(&msg->msgPtr, UART_RX_MESSAGE_QUEUE_SIZE);
            msg->sortPtr[msg->msgPtr.iIndexIN].ptr = 0;
        }
    	DRV_RingBuffPtr__Output(&msg->bytePtr, UART_RX_BYTE_BUFFER_SIZE);
    }
}


/*
 * Description: Sort UART Binary buffer and save binary packet in message buffer. Checksum is using MOD256.
 * 				When the number of bytes have been recevied and the checksum matches the sum of the bytes,
 * 				then save packet to message buffer.
 *              This needs to be called from a polling routine.
 *
 * Input: Index pointer to which UART buffer to parse, the size of bytes to receive for a complete packet
 * Output: none
 */
void UART_SortRx_BINARY_Buffer(UartRxBufferStruct *msg)
{
    uint32_t i = 0;
    uint8_t checkSum = 0; // MOD256 checksum
    uint8_t tempTelemetry[UART_RX_BYTE_BUFFER_SIZE];
    uint32_t pointer = 0;

    if (msg->bytePtr.iCnt_Handle >= msg->packetSize)
    {
        // copy the bytes to a temporary array
        for(i = 0; i < msg->packetSize; i++)
        {
            pointer = msg->bytePtr.iIndexOUT + i;
            if(pointer >= UART_RX_BYTE_BUFFER_SIZE)// past max range
            {
                pointer -= UART_RX_BYTE_BUFFER_SIZE;
            }

            tempTelemetry[i] = msg->byteBuffer[pointer];
        }
        
        // calculate checksum
        for(i = 0; i < (msg->packetSize - 1); i++)
        {
            checkSum += tempTelemetry[i];
        }
        
        // verify checksum
        if(checkSum == tempTelemetry[msg->packetSize - 1]) // compare checksum to last index
        {

        	if( ((checkSum == 0) && (tempTelemetry[msg->packetSize - 1] == 0)) || (tempTelemetry[0] == 0) ) // ignore packets that are all zeros or first byte is zero
        	{
        		for(i = 0; i < msg->packetSize; i++)
				{
					DRV_RingBuffPtr__Output(&msg->bytePtr, UART_RX_BYTE_BUFFER_SIZE );
				}
        	}
        	else
        	{
				// we have a crc match so save the packet to the Rx packet buffer
				for(i = 0; i < msg->packetSize; i++)
				{
					msg->msgQueue[msg->msgPtr.iIndexIN].data[i] = msg->byteBuffer[msg->bytePtr.iIndexOUT];
					DRV_RingBuffPtr__Output(&msg->bytePtr, UART_RX_BYTE_BUFFER_SIZE );
				}
				msg->msgQueueSize[msg->msgPtr.iIndexIN].dataSize = msg->packetSize;
				checkSum = 0; // reset checksum
				DRV_RingBuffPtr__Input(&msg->msgPtr, UART_RX_MESSAGE_QUEUE_SIZE); // increment rx packet pointer
        	}
        }
        else
        {
            DRV_RingBuffPtr__Output(&msg->bytePtr, UART_RX_BYTE_BUFFER_SIZE ); // increment rx byte pointer
        }
    }
}

void UART_InitPacketSize(UartRxBufferStruct *msg, uint32_t size)
{
	msg->packetSize = size;
}


/*
 * Description: Add message to message buffer to be sent. Use either UartCopyStringToTxStruct() or UartCopyBinaryDataToTxStruct() to build message first
 *              Be sure to call UartSendMessage() in a polling routine
 *
 * Input: The message structure
 * Output:
 *
 */
void UART_TX_AddMessageToBuffer(UartTxBufferStruct *msgOut, uint8_t uartPort, uint8_t *msgIN, uint32_t dataSize)
{
    uint8_t i = 0;
    uint8_t *pData = (uint8_t*)msgIN;

    for(i = 0; i < dataSize; i++)
    {
    	msgOut->msgQueue[msgOut->msgPtr.iIndexIN].data[i] = *pData++;
    }
    msgOut->uartPort[msgOut->msgPtr.iIndexIN] = uartPort;
    msgOut->msgQueueSize[msgOut->msgPtr.iIndexIN].dataSize = dataSize;
    DRV_RingBuffPtr__Input(&msgOut->msgPtr, UART_TX_MESSAGE_QUEUE_SIZE);
}

uint32_t UART_TX_GetMessageSize(UartTxBufferStruct *buffer)
{
	return buffer->msgQueueSize[buffer->msgPtr.iIndexOUT].dataSize;
}

/*
 * Description: Call this from a polling routine.
 *              UartTxMessage() should be part of your code for your specific MCU. See function below this one for example
 *
 * Input: none
 * Output: none
 *
 */
void UART_SendMessage(UartTxBufferStruct *msg)
{
    int status = NO_ERROR;
    
    if (msg->msgPtr.iCnt_Handle)
    {
        status = UART_TxMessage(msg);
        // We need to monitor the return status for a failed transmit so not to increment the pointer yet
        // so on the next loop we can try to send again
        if (status == NO_ERROR)
        {
            DRV_RingBuffPtr__Output(&msg->msgPtr, UART_TX_MESSAGE_QUEUE_SIZE);
        }
    }
}


/*
 * Description: Add one or more bytes to the byteBuffer. This would be called from a UART IRQ.
 *
 * Input: The pointer to data array, the size/length of data
 * Output: No error or overflow
 */
int UART_AddCharToBuffer(UartRxBufferStruct *msg)
{
	int i;
    
	if(msg->bytePtr.iCnt_OverFlow)
	{
        // byte buffer is full. You should monitor the return status during development and increase buffer size to if overflow;
        return UART_BUFFER_OVERFLOW;
    }

	for(i = 0; i < UART_RX_IRQ_BYTE_SIZE; i++)
	{
		msg->byteBuffer[msg->bytePtr.iIndexIN] = msg->uartIRQ_ByteBuffer[i];
		DRV_RingBuffPtr__Input(&msg->bytePtr, UART_RX_BYTE_BUFFER_SIZE);
	}

    return NO_ERROR;
}


/*
 * Description: Gets the rx message string length
 */
static uint32_t UART_StringMessageGetLength(UartRxBufferStruct *msg)
{
    return strlen((char*)msg->msgQueue[msg->msgPtr.iIndexOUT].data);
}

/*
 * Description: Checks for pending string message in message ring buffer. Automatically increments pointer after saving to msgOUT.
 * Input: none
 * Output: none
 *
 * Return: true if message available, else returns false
 *
 */
bool UART_RxStringMessagePending(UartRxBufferStruct *msg, UartMsgQueueStruct *msgOut)
{
    int i = 0;
    uint8_t dataLen;
    
	if(msg->msgPtr.iCnt_Handle)
    {
        dataLen = msg->msgQueueSize[msg->msgPtr.iIndexOUT].dataSize;
        for(i = 0; i < dataLen; i++)
        {
            msgOut->data[i] = msg->msgQueue[msg->msgPtr.iIndexOUT].data[i];
        }
        memset(&msg->msgQueue[msg->msgPtr.iIndexOUT].data, 0 , sizeof(msg->msgQueue[msg->msgPtr.iIndexOUT].data));
        DRV_RingBuffPtr__Output(&msg->msgPtr, UART_RX_MESSAGE_QUEUE_SIZE);
        return true;
    }
    
    return false;
}

/*
 * Description: Copies message from ring buffer to retStr NOT including the CR and LF characters.
 */
void UART_RxStringMessageCopyNoCRLF(UartRxBufferStruct *msg, char *retStr)
{
	memcpy(retStr, (char*)&msg->msgQueue[msg->msgPtr.iIndexOUT].data, UART_StringMessageGetLength(msg) - 2);
}

/*
 * Description: Copies message from ring buffer to retStr including the CR and LF characters.
 */
void UART_RxStringMessageCopy(UartRxBufferStruct *msg, char *retStr)
{
	memcpy(retStr, &msg->msgQueue[msg->msgPtr.iIndexOUT].data, UART_StringMessageGetLength(msg));
}


