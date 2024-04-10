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
 * Description: This would be called from a UART IRQ.
 * Input: The UartRxBufferStruct to increment pointer
 */
void UART_AddByteToBuffer(UartBufferStruct *msg)
{
	if(msg->rx.uartType == UART_ASCII)
	{
		msg->rx.queue[msg->rx.ptr.index_IN].data[msg->rx.bytePtr.index_IN] = msg->rx.irqByte; // save byte to current queue
		RingBuff_Ptr_Input(&msg->rx.bytePtr, UART_RX_BYTE_BUFFER_SIZE); // increment byte pointer
		if(msg->rx.irqByte == '\n') // check for LF
		{
			msg->rx.queue[msg->rx.ptr.index_IN].data[msg->rx.bytePtr.index_IN] = 0; // add null
			msg->rx.queue[msg->rx.ptr.index_IN].size = msg->rx.bytePtr.index_IN; // save size
			RingBuff_Ptr_Input(&msg->rx.ptr, UART_RX_MESSAGE_QUEUE_SIZE); // increment queue index
			RingBuff_Ptr_Reset(&msg->rx.bytePtr); // reset byte pointer
		}
	}
	else if(msg->rx.uartType == UART_BINARY)
	{
		msg->rx.binaryBuffer[msg->rx.bytePtr.index_IN] = msg->rx.irqByte;
		RingBuff_Ptr_Input(&msg->rx.bytePtr, UART_RX_BYTE_BUFFER_SIZE); // increment byte pointer
		if(msg->rx.bytePtr.index_IN >= msg->rx.packetSize) // check if byte pointer equals packet size
		{
			msg->rx.queue[msg->rx.ptr.index_IN].size = msg->rx.bytePtr.index_IN; // save size
			RingBuff_Ptr_Input(&msg->rx.ptr, UART_RX_MESSAGE_QUEUE_SIZE); // increment queue index
			RingBuff_Ptr_Reset(&msg->rx.bytePtr); // reset byte pointer
		}
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
void UART_SortRx_BINARY_Buffer(UartBufferStruct *msg, CheckSumType checkSumType)
{
	/*
    uint32_t i = 0;
    uint8_t tempTelemetry[UART_RX_BYTE_BUFFER_SIZE] = {0};

	uint32_t sortPtr = 0;
	uint8_t checksum = 0;

    if (msg->rx.bytePtr.cnt_Handle >= msg->rx.packetSize)
    {
        // copy the bytes to a temporary array
        for(i = 0; i < msg->rx.packetSize; i++)
        {
        	sortPtr = msg->rx.bytePtr.index_OUT + i;
            if(sortPtr >= UART_RX_BYTE_BUFFER_SIZE)// past max range
            {
            	sortPtr -= UART_RX_BYTE_BUFFER_SIZE;
            }

            tempTelemetry[i] = msg->rx.binaryBuffer[sortPtr];
        }

        // calculate checksum
        for(i = 0; i < (msg->rx.packetSize - 1); i++)
        {
            checksum += tempTelemetry[i];
        }
        
        // verify checksum
        if(checksum == tempTelemetry[msg->rx.packetSize - 1]) // compare checksum to last index
        {
        	if( ((checksum == 0) && (tempTelemetry[msg->rx.packetSize - 1] == 0)) || (tempTelemetry[0] == 0) ) // ignore packets that are all zeros or first byte is zero
        	{
        		for(i = 0; i < msg->rx.packetSize; i++)
				{
        			RingBuff_Ptr_Output(&msg->rx.bytePtr, UART_RX_BYTE_BUFFER_SIZE );
				}
        	}
        	else
        	{
				// we have a crc match so save the packet to the Rx queue buffer
				for(i = 0; i < msg->rx.packetSize; i++)
				{
					msg->rx.queue[msg->rx.ptr.index_IN].data[i] = msg->rx.binaryBuffer[i];
					RingBuff_Ptr_Output(&msg->rx.ptr, UART_RX_BYTE_BUFFER_SIZE );
				}
				msg->rx.queue[msg->rx.ptr.index_IN].size = msg->rx.packetSize;
				RingBuff_Ptr_Input(&msg->rx.ptr, UART_RX_MESSAGE_QUEUE_SIZE); // increment rx queue pointer
				RingBuff_Ptr_Reset(&msg->rx.bytePtr);
        	}
        }
        else
        {
        	RingBuff_Ptr_Output(&msg->rx.bytePtr, UART_RX_BYTE_BUFFER_SIZE ); // increment rx byte pointer
        }
    }
    */
}

/*
 * Description: User will need to initialize the expected packet size. This will help the UART_SortRx_BINARY_Buffer function to extract the packet
 * 				correctly. Only needed for binary data, not for string commands.
 */
void UART_InitPacketSize(UartBufferStruct *msg, uint32_t size)
{
	msg->rx.packetSize = size;
}

/*
 * Description: Checks for pending message in message ring buffer. Automatically increments ring buffer pointer after setting msgQueue address to msgToParse.
 * 				This should be called after the buffer has been sorted using one of the two sort functions
 * Input: UartRxBufferStruct to grab message from.
 * Output: msgToParse points to msgQueue to parse
 *
 * Return: true if message available, else returns false
 *
 */
bool UART_RxMessagePending(UartBufferStruct *msg)
{
	if(msg->rx.ptr.cnt_Handle)
    {
		msg->rx.msgToParse = &msg->rx.queue[msg->rx.ptr.index_OUT];
        RingBuff_Ptr_Output(&msg->rx.ptr, UART_RX_MESSAGE_QUEUE_SIZE);
        return true;
    }

    return false;
}


//************************************ Transmit **********************************

/*
 * Description: Add data to message buffer to be sent. User will need to create a UART Tx handler to be called from polling routine
 * Input: The message structure
 * Output: The updated msgOut.
 *
 */
void UART_TX_AddDataToBuffer(UartBufferStruct *msgOut, uint8_t *msgIN, uint32_t size)
{
    uint8_t i = 0;
    uint8_t *pData = (uint8_t*)msgIN;

    for(i = 0; i < size; i++)
    {
    	msgOut->tx.queue[msgOut->tx.ptr.index_IN].data[i] = *pData++;
    }
    msgOut->tx.queue[msgOut->tx.ptr.index_IN].size = size;
    RingBuff_Ptr_Input(&msgOut->tx.ptr, UART_TX_MESSAGE_QUEUE_SIZE);

    UART_TxMessage_IT(msgOut); // try sending if queue is empty
}


