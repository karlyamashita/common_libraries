/*
    UartCharBuffer.c

    Created on: Dec 2, 2019
        Author: Karl




        // Basic Info
        - This has a ring buffer for individual bytes and a ring buffer for messages.


        Note1: Adjust value in UartCharBuffer.h file accordingly.
			#define UART_RX_IRQ_BYTE_SIZE 1

			#define UART_RX_BYTE_BUFFER_SIZE 192 // this holds all the IRQ data
			#define UART_TX_BYTE_BUFFER_SIZE 192
			#define UART_RX_MESSAGE_QUEUE_SIZE 8 // buffer size of complete strings or packets.
			#define UART_TX_MESSAGE_QUEUE_SIZE 8 // buffer size of complete strings or packets.

			#define UART_RX_PACKET_SIZE 16 // The number of bytes for complete packet + checksum.


        Note2: Must include RingBuffer.c/h files.

 */

#include "main.h"
#include "UartCharBuffer.h"


/*
 * Description: Add one or more bytes to the byteBuffer. This would be called from a UART IRQ.
 *
 * Input: The UartRxBufferStruct to save the data to, the pointer to data array (uartIRQ_ByteBuffer), the size of data
 * Output: No error or overflow
 */
int UART_AddByteToBuffer(UartRxBufferStruct *msg, uint8_t *data, uint32_t size)
{
	int i;

	if(msg->bytePtr.cnt_OverFlow)
	{
        // byte buffer is full. You should monitor the return status during development and increase buffer size if there is an overflow;
        return UART_BUFFER_OVERFLOW;
    }

	for(i = 0; i < size; i++)
	{
		msg->byteBuffer[msg->bytePtr.index_IN] = data[i];
		RingBuff_Ptr_Input(&msg->bytePtr, UART_RX_BYTE_BUFFER_SIZE);
	}

    return 0;
}


/*
 * Description: Sort UART character buffer and save string in message buffer. The string being received should have LF terminator
 *              This needs to be called from a polling routine.
 *
 * Input: Pointer to which UartRxBufferStruct buffer to parse
 * Output: none
 */
void UART_SortRx_CHAR_Buffer(UartRxBufferStruct *msg)
{
    if(msg->bytePtr.cnt_Handle)
    {
    	msg->msgQueue[msg->msgPtr.index_IN].data[msg->sortPtr++] = msg->byteBuffer[msg->bytePtr.index_OUT];
    	if(msg->byteBuffer[msg->bytePtr.index_OUT] == '\n')
    	{
            msg->msgQueue[msg->msgPtr.index_IN].size = msg->sortPtr;
            RingBuff_Ptr_Input(&msg->msgPtr, UART_RX_MESSAGE_QUEUE_SIZE);
            msg->sortPtr = 0;// reset
        }
    	RingBuff_Ptr_Output(&msg->bytePtr, UART_RX_BYTE_BUFFER_SIZE);
    }
}


/*
 * Description: Sort UART Binary buffer and save binary packet in message buffer. Checksum is using MOD256.
 * 				When the number of bytes have been received and the checksum matches the sum of the bytes,
 * 				then we save packet to message buffer.
 *              This needs to be called from a polling routine.
 *
 * Input: Pointer to which UartRxBufferStruct buffer to parse, Checksum type (currently not implemented) but default is MOD256
 * Output: none
 *
 * example;
 * 		UART_SortRx_BINARY_Buffer(&rxMsg2, CHECKSUM_MOD256); // called from a main while loop
 *
 */
void UART_SortRx_BINARY_Buffer(UartRxBufferStruct *msg, CheckSumType checkSumType)
{
    uint32_t i = 0;
    uint8_t checkSum = 0; // MOD256 checksum
    uint8_t tempTelemetry[UART_RX_BYTE_BUFFER_SIZE] = {0};
    uint32_t pointer = 0;

    if (msg->bytePtr.cnt_Handle >= msg->packetSize)
    {
        // copy the bytes to a temporary array
        for(i = 0; i < msg->packetSize; i++)
        {
            pointer = msg->bytePtr.index_OUT + i;
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
        			RingBuff_Ptr_Output(&msg->bytePtr, UART_RX_BYTE_BUFFER_SIZE );
				}
        	}
        	else
        	{
				// we have a crc match so save the packet to the Rx packet buffer
				for(i = 0; i < msg->packetSize; i++)
				{
					msg->msgQueue[msg->msgPtr.index_IN].data[i] = msg->byteBuffer[msg->bytePtr.index_OUT];
					RingBuff_Ptr_Output(&msg->bytePtr, UART_RX_BYTE_BUFFER_SIZE );
				}
				msg->msgQueue[msg->msgPtr.index_IN].size = msg->packetSize;
				checkSum = 0; // reset checksum
				RingBuff_Ptr_Input(&msg->msgPtr, UART_RX_MESSAGE_QUEUE_SIZE); // increment rx packet pointer
        	}
        }
        else
        {
        	RingBuff_Ptr_Output(&msg->bytePtr, UART_RX_BYTE_BUFFER_SIZE ); // increment rx byte pointer
        }
    }
}

/*
 * Description: User will need to initialize the expected packet size. This will help the UART_SortRx_BINARY_Buffer function to extract the packet
 * 				correctly. Only needed for binary data, not for string commands.
 */
void UART_InitPacketSize(UartRxBufferStruct *msg, uint32_t size)
{
	msg->packetSize = size;
}

/*
 * Description: Checks for pending message in message ring buffer. Automatically increments pointer after copying to msgOUT.
 * 				This should be called after the buffer has been sorted using one of the two sort functions
 * Input: UartRxBufferStruct to grab message from.
 * Output: the UartMsgQueueStruct to save the message to
 *
 * Return: true if message available, else returns false
 *
 */
bool UART_RxMessagePending(UartRxBufferStruct *msg)
{
    int i = 0;
    uint8_t size;

	if(msg->msgPtr.cnt_Handle)
    {
        size = msg->msgQueue[msg->msgPtr.index_OUT].size;
        msg->msgToParse.size = size;
        for(i = 0; i < size; i++)
        {
            msg->msgToParse.data[i] = msg->msgQueue[msg->msgPtr.index_OUT].data[i];
        }
        memset(&msg->msgQueue[msg->msgPtr.index_OUT].data, 0 , sizeof(msg->msgQueue[msg->msgPtr.index_OUT].data));
        RingBuff_Ptr_Output(&msg->msgPtr, UART_RX_MESSAGE_QUEUE_SIZE);
        return true;
    }

    return false;
}

// Misc
uint32_t UART_GetRxInstance(UartRxBufferStruct *msg)
{
	return msg->instance;
}

bool UART_GetRxIntErrorFlag(UartRxBufferStruct *msg)
{
	return msg->UART_RxEnErrorFlag;
}

void UART_SetRxIntErrorFlag(UartRxBufferStruct *msg, bool status)
{
	msg->UART_RxEnErrorFlag = status;
}

/*
 * Description: If using buffer pointers, user will need to declare some variables and call this to pass address to them.
 *
 *	 example declarations;
 *		UartRxBufferStruct rxMsg2 = {0};
 *		uint8_t uart2RxIrqByteBuffer[UART_RX_IRQ_BYTE_SIZE] = {0}; // irq array
 *		uint8_t uart2RxByteBuffer[UART_RX_BYTE_BUFFER_SIZE] = {0}; // byte array
 *		UartMsgQueueStruct uart2RxMsgQueue[UART_RX_MESSAGE_QUEUE_SIZE] = {0}; // message array
 *
 *	Somewhere before main call this
 *		UART_RxBufferInit(&rxMsg2, uart2RxIrqByteBuffer, uart2RxByteBuffer, uart2RxMsgQueue);
 *
 * Input: the above declarations
 * Output: none
 */
#ifdef USE_BUFFER_POINTERS
void UART_RxBufferInit(UartRxBufferStruct *msg, uint8_t *irqBuffer, uint8_t *byteBuffer, UartMsgQueueStruct *queueBuffer)
{
	msg->uartIRQ_ByteBuffer = irqBuffer;
	msg->byteBuffer = byteBuffer;
	msg->msgQueue = queueBuffer;
}

/*
 * Description: If using buffer pointers, user will need to declare some variables and call this to pass the address to them.
 *
 * 	example declarations;
 * 		UartTxBufferStruct txMsg2 = {0};
 * 		UartMsgQueueStruct uart2TxMsgQueue[UART_TX_MESSAGE_QUEUE_SIZE] = {0};
 *
 * 	Somewhere before main call
 * 		UART_TxBufferInit(&txMsg2, uart2TxMsgQueue);
 *
 * Input: the above declarations
 * Output: none
 */
void UART_TxBufferInit(UartTxBufferStruct *msg, UartMsgQueueStruct *queue)
{
	msg->msgQueue = queue;
}
#endif

//************************************ Transmit **********************************

/*
 * Description: Add data to message buffer to be sent. Be sure to call UartSendMessage() in a polling routine
 * Input: The message structure
 * Output:
 *
 */
void UART_TX_AddDataToBuffer(UartTxBufferStruct *msgOut, uint8_t *msgIN, uint32_t size)
{
    uint8_t i = 0;
    uint8_t *pData = (uint8_t*)msgIN;

    for(i = 0; i < size; i++)
    {
    	msgOut->msgQueue[msgOut->msgPtr.index_IN].data[i] = *pData++;
    }
    msgOut->msgQueue[msgOut->msgPtr.index_IN].size = size;
    RingBuff_Ptr_Input(&msgOut->msgPtr, UART_TX_MESSAGE_QUEUE_SIZE);
}


/*
 * Description: Call this from a polling routine.
 *              UartTxMessage() call below is generic function. The function should be part of your project code for your specific MCU.
 *
 * Input: none
 * Output: none
 *
 */
void UART_SendMessage(UartTxBufferStruct *msg)
{
    int status = 0;
    
    if (msg->msgPtr.cnt_Handle)
    {
        if(!msg->txPending)
        {
            status = UART_TxMessage(msg);
            if (status == 0)
            {
            	RingBuff_Ptr_Output(&msg->msgPtr, UART_TX_MESSAGE_QUEUE_SIZE);
            }
        }
    }
}


/*
 * How to use:
 * 	You can use pointers to the buffers or you can use fixed size buffers. Use USE_BUFFER_POINTERS in h file to enable/disable.
 *
 * 	Initializing buffers and passing the address to the pointers.
 *

// define the UART2 Rx buffers
uint8_t uart2RxIrqByteBuffer[UART_RX_IRQ_BYTE_SIZE] = {0}; // irq array
uint8_t uart2RxByteBuffer[UART_RX_BYTE_BUFFER_SIZE] = {0}; // byte array
UartMsgQueueStruct uart2RxMsgQueue[UART_RX_MESSAGE_QUEUE_SIZE] = {0}; // message array
UartRxBufferStruct uart2Rx =
{
	.instance = UART_PORT_2,
	.byteBuffer = uart2RxByteBuffer,
	.uartIRQ_ByteBuffer = uart2RxIrqByteBuffer,
	.msgQueue = uart2RxMsgQueue
};

// define the UART2 Tx buffers
UartMsgQueueStruct uart2TxMsgQueue[UART_TX_MESSAGE_QUEUE_SIZE] = {0};
UartTxBufferStruct uart2Tx =
{
	.instance = UART_PORT_2,
	.msgQueue = uart2TxMsgQueue
};


 */






