/*
 * UART_Handler.c
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */



#include "main.h"


/*
 * Description: Init uart0 buffers
 */
UartBufferStruct uart0 =
{
    .instance = UART0_BASE,
    .rx.msgQueueSize = UART_RX_MESSAGE_QUEUE_SIZE,
    .tx.msgQueueSize = UART_TX_MESSAGE_QUEUE_SIZE
};

static void UART_TransmitMessage(UartBufferStruct *msg);

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
// this function should be registered when initializing the UART, UARTIntRegister(UART0_BASE, USART0_IRQHandler);
void USART0_IRQHandler(void)
{
    uint32_t ui32Status;
    uint8_t data[1];

    //
    // Get the interrupt status.
    //
    ui32Status = UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    MAP_UARTIntClear(UART0_BASE, ui32Status);

    if((ui32Status & UART_INT_TX) == UART_INT_TX)
    {
        UART_TransmitMessage(&uart0);
    }
    else if((ui32Status & UART_INT_RX) == UART_INT_RX)
    {
        //
        // Save available character in buffer.
        //
        if(MAP_UARTCharsAvail(UART0_BASE))
        {
            data[0] = (uint8_t)UARTCharGet(UART0_BASE);
            UART_IncrementCharPointer(&uart0, data, 1);
        }
    }
    else
    {
        // some other interrupt
        ToggleLedGrn();
    }
}


/*
 * Description: This will enable TX interrupt start the first byte transmit and continue interrupts until done.
 *              Must call from polling routine
 * Input: the UART data structure
 *
 */
int UART_TxMessage_IT(UartBufferStruct *msg)
{
    int status = 0;

    if(msg->tx.msgToSend_Pending == true) return UART_TX_PENDING;

    if(msg->tx.queuePtr.cnt_Handle)
    {
        msg->tx.msgToSend = &msg->tx.queue[msg->tx.queuePtr.index_OUT]; // set msgToSend to buffer index
        RingBuff_Ptr_Output(&msg->tx.queuePtr, msg->tx.msgQueueSize); // increment buffer index

        if(msg->tx.msgToSend->size > 1)
        {
            msg->tx.msgToSend_BytePtr = 0;
            msg->tx.msgToSend_Pending = true;
            UARTIntEnable(msg->instance, UART_INT_TX); // enable tx interrupts
            UART_TransmitMessage(msg); // start the transmit
        }
        else if(msg->tx.msgToSend->size == 1) // we can poll since this only 1 byte
        {
            UART_TxMessage(msg); // non interrupt send
        }
        else // the queue has no bytes to send.
        {
            return 1; // return non zero is an error
        }
    }

    return status;
}

/*
 * Description:
 */
static void UART_TransmitMessage(UartBufferStruct *msg)
{
    if(msg->tx.msgToSend_BytePtr < msg->tx.msgToSend->size)
    {
        if(UARTCharPutNonBlocking(msg->instance, msg->tx.msgToSend->data[msg->tx.msgToSend_BytePtr]) == true)
        {
            if(++msg->tx.msgToSend_BytePtr == msg->tx.msgToSend->size)
            {
                msg->tx.msgToSend_Pending = false; // clear for next message to be sent
                UARTIntDisable(msg->instance, UART_INT_TX); // done transmitting bytes, so disable the Tx interrupt
            }
        }
    }
}

/*
 * Description: UART Blocking method
 *
 */
int UART_TxMessage(UartBufferStruct *msg)
{
    int i = 0;

    if(msg->tx.queuePtr.cnt_Handle)
    {
        msg->tx.msgToSend = &msg->tx.queue[msg->tx.queuePtr.index_OUT];

        RingBuff_Ptr_Output(&msg->tx.queuePtr, msg->tx.msgQueueSize);

        for(i = 0; i < msg->tx.msgToSend->size; i++)
        {
            UARTCharPut(msg->instance, msg->tx.msgToSend->data[i]);
        }
    }

    return 0;
}


/*
 * Description: Check for a new messsage then Find instance, then call appropriate parser
 */
int UART_Parse(UartBufferStruct *msg)
{
    int status = 0;

    if(UART_RxMessagePending(msg))
    {
        switch (msg->instance)
        {
        case UART0_BASE:
            UART0_Parse(msg);
            break;
        }
    }

    return status;
}


/*
 * How to use:
 * 	You can use pointers to the buffers or you can use fixed size buffers. Use USE_BUFFER_POINTERS in UartCharBuffer.h file to enable/disable.
 *
 * 	Initializing buffers and passing the address to the pointers.
 *

// define the UART0 Rx buffers
uint8_t uart0RxIrqByteBuffer[UART_RX_IRQ_BYTE_SIZE] = {0}; // irq array
uint8_t uart0RxByteBuffer[UART_RX_BYTE_BUFFER_SIZE] = {0}; // byte array
UartMsgQueueStruct uart0RxMsgQueue[UART_RX_MESSAGE_QUEUE_SIZE] = {0}; // message array
UartRxBufferStruct uart0Rx =
{
	.instance = UART0_BASE,
	.uartIRQ_ByteBuffer = uart0RxIrqByteBuffer,
	.uartIRQ_ByteSize = UART_RX_IRQ_BYTE_SIZE, // optional
	.byteBuffer = uart0RxByteBuffer,
	.byteBufferSize = UART_RX_BYTE_BUFFER_SIZE, // optional
	.msgQueue = uart0RxMsgQueue
	.msgQueueSize = UART_RX_MESSAGE_QUEUE_SIZE // optional
};

// define the UART0 Tx buffers
UartMsgQueueStruct uart0TxMsgQueue[UART_TX_MESSAGE_QUEUE_SIZE] = {0};
UartTxBufferStruct uart0Tx =
{
	.instance = UART0_BASE,
	.msgQueue = uart0TxMsgQueue
	.msgQueueSize = UART_TX_MESSAGE_QUEUE_SIZE // optional
};

*/

