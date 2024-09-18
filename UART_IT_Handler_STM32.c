/*
 * UART_IT_Handler_STM32.c
 *
 *  Created on: Sep 17, 2024
 *      Author: karl.yamashita
 *
 */

#include "main.h"




/*
 * Description: This would be called from a UART IRQ.
 * Input: The UartRxBufferStruct to increment pointer
 */
void UART_AddByteToBuffer(UartBufferStruct *msg)
{
	if(msg->rx.uartType == UART_ASCII)
	{
		msg->rx.queue[msg->rx.ptr.index_IN].data[msg->rx.bytePtr.index_IN] = msg->rx.irqByte; // save byte to current queue
		RingBuff_Ptr_Input(&msg->rx.bytePtr, msg->rx.bytePtrSize); // increment byte pointer
		if(msg->rx.irqByte == '\n') // check for LF
		{
			msg->rx.queue[msg->rx.ptr.index_IN].data[msg->rx.bytePtr.index_IN] = 0; // add null
			msg->rx.queue[msg->rx.ptr.index_IN].size = msg->rx.bytePtr.index_IN; // save size
			RingBuff_Ptr_Input(&msg->rx.ptr, msg->rx.queueSize); // increment queue index
			RingBuff_Ptr_Reset(&msg->rx.bytePtr); // reset byte pointer
		}
	}
	else if(msg->rx.uartType == UART_BINARY)
	{
		msg->rx.binaryBuffer[msg->rx.bytePtr.index_IN] = msg->rx.irqByte;
		RingBuff_Ptr_Input(&msg->rx.bytePtr, msg->rx.bytePtrSize); // increment byte pointer
	}
}

/*
 * Description: This will check to be sure the packetSize has been reached.
 * 				It will then validate the checksum using -MOD256 and copy to packet queue.
 * 				This should be call from polling routine.
 */
void UART_SortPacketBuffer(UartBufferStruct *msg)
{
    uint32_t i = 0;
    uint8_t tempTelemetry[UART_RX_BYTE_BUFFER_SIZE] = {0};

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
 * Description: Enables the HAL_UART_Receive_IT interrupt. Call before main while loop and in HAL_UART_RxCpltCallback
 */
void UART_EnableRxInterrupt(UartBufferStruct *msg)
{
	msg->rx.HAL_Status = HAL_UART_Receive_IT(msg->huart, &msg->rx.irqByte, 1);
}

/*
 * Description: If error flag then call UART_EnableRxInterrupt. Call from main while loop.
 *
 */
void UART_CheckRxIntError(UartBufferStruct *msg)
{
	if(msg->rx.HAL_Status != HAL_OK)
	{
		UART_EnableRxInterrupt(msg);
	}
}

/*
 * Description: User can change the packets size on the fly. Only needed for binary data, not for string data.
 * 				Note: If called during middle of receiving a packet, 1 or 2 packet(s) loss may occur.
 *
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
bool UART_RxMsgRdy(UartBufferStruct *msg)
{
	if(msg->rx.ptr.cnt_Handle)
    {
		msg->rx.msgToParse = &msg->rx.queue[msg->rx.ptr.index_OUT];
        RingBuff_Ptr_Output(&msg->rx.ptr, msg->rx.queueSize);
        return true;
    }

    return false;
}

/*
 * Description: This will add CR and LF to string before saving to tx buffer.
 * Input: Pointer to buffer, the string to add to tx buffer, the size of string, and CR/LF (carriage return/linefeed enable)
 *
 */
void UART_NotifyUser(UartBufferStruct *msg, char *str, uint32_t strlen, bool lineFeed)
{
    uint8_t strMsg[UART_TX_BYTE_BUFFER_SIZE] = {0};
    uint32_t size = strlen;

    memcpy(&strMsg, str, size);

    if(lineFeed == true)
    {
    	strcat((char*)strMsg, "\r\n");
    	size += 2;
    }

    UART_TX_AddDataToBuffer(msg, strMsg, size);
}



//************************************ Transmit **********************************

/*
 * Description: Add data to message buffer to be sent. User will need to create a UART Tx handler to be called from polling routine
 * Input: The message structure
 * Output: The updated msgOut.
 *
 */
void UART_TX_AddDataToBuffer(UartBufferStruct *msg, uint8_t *msgIN, uint32_t size)
{
    uint8_t i = 0;
    uint8_t *pData = (uint8_t*)msgIN;

    for(i = 0; i < size; i++)
    {
    	msg->tx.queue[msg->tx.ptr.index_IN].data[i] = *pData++;
    }
    msg->tx.queue[msg->tx.ptr.index_IN].size = size;
    RingBuff_Ptr_Input(&msg->tx.ptr, msg->tx.queueSize);

    UART_TxMessage_IT(msg); // try to send message in queue
}

/*
 * Description: Transmit any available messages. Called from UART_TX_AddDataToBuffer or HAL_UART_TxCpltCallback
 */
int UART_TxMessage_IT(UartBufferStruct *msg)
{
	int status = 0;
	UartDataStruct *ptr;

	if(msg->tx.ptr.cnt_Handle)
	{
		ptr = &msg->tx.queue[msg->tx.ptr.index_OUT];

		if(HAL_UART_Transmit_IT(msg->huart, ptr->data, ptr->size) == HAL_OK)
		{
			RingBuff_Ptr_Output(&msg->tx.ptr, msg->tx.queueSize);
		}
	}

	return status;
}


/*



// These callbacks should be added to your code.
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart2_msg.huart)
	{
		UART_AddByteToBuffer(&uart2_msg); // add byte to buffer.
		UART_EnableRxInterrupt(&uart2_msg); // enable interrupt again
	}
	else if(huart == uart1_msg.huart) // example of having more than 1 UART instance
	{
		UART_AddByteToBuffer(&uart1_msg);
		UART_EnableRxInterrupt(&uart1_msg);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart2_msg.huart)
	{
		UART_TxMessage_IT(&uart2_msg); // try and transmit if more messages in queue
	}
	else if(huart == uart1_msg.huart) // example of having more than 1 UART instance
	{
		UART_TxMessage_IT(&uart1_msg);
	}
}

// example of declaring a string packet
UartBufferStruct uart2 =
{
	.huart = &huart2,
	.rx.queueSize = UART_RX_MESSAGE_QUEUE_SIZE,
	.tx.queueSize = UART_TX_MESSAGE_QUEUE_SIZE,
	.rx.uartType = UART_ASCII,
};

// example of declaring a binary packet
UartBufferStruct uart2_msg =
{
	.huart = &huart2,
	.rx.uartType = UART_BINARY
	.rx.queueSize = UART_RX_MESSAGE_QUEUE_SIZE,
	.tx.queueSize = UART_TX_MESSAGE_QUEUE_SIZE,
	.rx.bytePtrSize = UART_RX_BYTE_BUFFER_SIZE,
	.rx.checksumType = CHECKSUM_MOD256,
	.rx.packetSize = 8,
};


 */
