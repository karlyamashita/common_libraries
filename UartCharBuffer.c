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
		RingBuff_Ptr_Input(&msg->rx.bytePtr, UART_RX_DATA_SIZE); // increment byte pointer
		if(msg->rx.irqByte == '\n') // check for LF
		{
			msg->rx.queue[msg->rx.ptr.index_IN].data[msg->rx.bytePtr.index_IN] = 0; // add null
			msg->rx.queue[msg->rx.ptr.index_IN].size = msg->rx.bytePtr.index_IN; // save size
			RingBuff_Ptr_Input(&msg->rx.ptr, UART_RX_QUEUE_SIZE); // increment queue index
			RingBuff_Ptr_Reset(&msg->rx.bytePtr); // reset byte pointer
		}
	}
	else if(msg->rx.uartType == UART_BINARY)
	{
		msg->rx.binaryBuffer[msg->rx.bytePtr.index_IN] = msg->rx.irqByte;
		RingBuff_Ptr_Input(&msg->rx.bytePtr, UART_RX_DATA_SIZE); // increment byte pointer
		if(msg->rx.bytePtr.index_IN >= msg->rx.packetSize) // check if byte pointer equals packet size
		{
			msg->rx.queue[msg->rx.ptr.index_IN].size = msg->rx.bytePtr.index_IN; // save size
			RingBuff_Ptr_Input(&msg->rx.ptr, UART_RX_QUEUE_SIZE); // increment queue index
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
    uint32_t i = 0;
    uint8_t tempTelemetry[UART_RX_DATA_SIZE] = {0};

    uint32_t sortPtr = 0;
    uint16_t cal_checksum = 0;
    uint16_t data_checksum = 0; // the checksum at end of packet

    if (msg->rx.bytePtr.cnt_Handle >= msg->rx.packetSize)
    {
        // copy the bytes to a temporary array
        for(i = 0; i < msg->rx.packetSize; i++)
        {
            sortPtr = msg->rx.bytePtr.index_OUT + i;
            if(sortPtr >= msg->rx.bytePtrSize)// past max range
            {
                sortPtr -= msg->rx.bytePtrSize;
            }

            tempTelemetry[i] = msg->rx.binaryBuffer[sortPtr];
        }

        // calculate checksum
        for(i = 0; i < (msg->rx.packetSize - 1); i++)
        {
            cal_checksum += tempTelemetry[i];
        }

        data_checksum = tempTelemetry[msg->rx.packetSize - 1]; // get last byte, the checksum

        if((uint8_t)(cal_checksum + data_checksum) != 0)
        {
            RingBuff_Ptr_Output(&msg->rx.bytePtr, msg->rx.bytePtrSize ); // increment rx byte pointer
            return; // no match so return
        }

        if( ((cal_checksum == 0) && (tempTelemetry[msg->rx.packetSize - 1] == 0)) || (tempTelemetry[0] == 0) ) // ignore packets that are all zeros or first byte is zero
        {
            for(i = 0; i < msg->rx.packetSize; i++)
            {
                RingBuff_Ptr_Output(&msg->rx.bytePtr, msg->rx.bytePtrSize ); // flush packet
            }
        }
        else
        {
            // we have a checksum match so save the packet to the Rx queue buffer
            for(i = 0; i < msg->rx.packetSize; i++)
            {
                //msg->rx.queue[msg->rx.ptr.index_IN].data[i] = msg->rx.binaryBuffer[i];
                msg->rx.queue[msg->rx.ptr.index_IN].data[i] = tempTelemetry[i];
                RingBuff_Ptr_Output(&msg->rx.ptr, msg->rx.queueSize);
            }
            msg->rx.queue[msg->rx.ptr.index_IN].size = msg->rx.packetSize;
            RingBuff_Ptr_Input(&msg->rx.ptr, msg->rx.queueSize); // increment rx queue pointer
            RingBuff_Ptr_Reset(&msg->rx.bytePtr);
        }
    }
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
        RingBuff_Ptr_Output(&msg->rx.ptr, UART_RX_QUEUE_SIZE);
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
    uint32_t i = 0;

    for(i = 0; i < size; i++)
    {
    	msgOut->tx.queue[msgOut->tx.ptr.index_IN].data[i] = *msgIN++;
    }
    msgOut->tx.queue[msgOut->tx.ptr.index_IN].size = size;
    RingBuff_Ptr_Input(&msgOut->tx.ptr, UART_TX_QUEUE_SIZE);
}


