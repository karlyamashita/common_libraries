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
UartBufferStruct uart0_msg =
{
    .rx.uartType = UART_ASCII,
    .uart_base = UART0_BASE,
    .rx.queueSize = UART_RX_QUEUE_SIZE,
    .tx.queueSize = UART_TX_QUEUE_SIZE,
    .rx.dataSize = UART_RX_DATA_SIZE,
    .rx.RxIRQ = TM4C_UART_Rx_IRQ,
    .tx.TxIRQ = TM4C_UART_Tx_IRQ,
    .ErrorIRQ = TM4C_UART_ErrorCallback
};

UartBufferStruct uart6_msg =
{
    .rx.uartType = UART_ASCII,
    .uart_base = UART6_BASE,
    .rx.queueSize = UART_RX_QUEUE_SIZE,
    .tx.queueSize = UART_TX_QUEUE_SIZE,
    .rx.dataSize = UART_RX_DATA_SIZE,
    .rx.RxIRQ = TM4C_UART_Rx_IRQ,
    .tx.TxIRQ = TM4C_UART_Tx_IRQ,
    .ErrorIRQ = TM4C_UART_ErrorCallback
};

bool echoMode = 0;

static void UART_TransmitMessage(UartBufferStruct *msg);


// Called from USART0_IRQHandler
void TM4C_USART_IRQHandler(UartBufferStruct *msg)
{
    uint32_t ui32Status;
    uint32_t errorStatus;

    // Get error status
    errorStatus = UARTRxErrorGet(msg->uart_base);
    UARTRxErrorClear(msg->uart_base);
    if(errorStatus != 0)
    {
        msg->errorStatus = errorStatus;
        if(msg->ErrorIRQ != NULL)
        {
            msg->ErrorIRQ(msg);
        }
    }

    // Get interrupt status
    ui32Status = UARTIntStatus(msg->uart_base, true);
    UARTIntClear(msg->uart_base, ui32Status);

    // test for tx interrupt flag
    if( (ui32Status & UART_INT_TX) == UART_INT_TX)
    {
        if(msg->tx.TxIRQ != NULL)
        {
            msg->tx.TxIRQ(msg);
        }
    }
    // test for rx/rt interrupt flag
    else if(((ui32Status & UART_INT_RX) == UART_INT_RX) || ((ui32Status & UART_INT_RT) == UART_INT_RT))
    {
        if(msg->rx.RxIRQ != NULL)
        {
            msg->rx.RxIRQ(msg);
        }
    }
}

__weak void TM4C_UART_Rx_IRQ(UartBufferStruct *msg)
{
    while (UARTCharsAvail(msg->uart_base))
    {
        msg->rx.irqByte = (uint8_t)UARTCharGet(msg->uart_base);
        if(echoMode == 1)
        {
            UARTCharPut(msg->uart_base, (unsigned char)msg->rx.irqByte); // echo character
        }

        UART_AddByteToBuffer(msg); // save character to char buffer
    }
}

/*
 * Description: Transmit byte done.
 */
__weak void TM4C_UART_Tx_IRQ(UartBufferStruct *msg)
{
    UART_TransmitMessage(msg);
}

__weak void TM4C_UART_ErrorCallback(UartBufferStruct *msg)
{
    if((msg->errorStatus & UART_RXERROR_OVERRUN) == UART_RXERROR_OVERRUN)
    {
        // There is an overrun. Get the byte from data register fast! But we'll discard it, so the message will be corrupted.
        if(UARTCharsAvail(msg->uart_base))
        {
           (uint8_t)UARTCharGet(msg->uart_base);
        }
    }

    if((msg->errorStatus & UART_RXERROR_FRAMING) == UART_RXERROR_FRAMING)
    {

    }
}


/*
 * Description: This will enable TX interrupt, start the first byte transmit and continue interrupts until done.
 * Input: the UART data structure
 *
 */
int UART_TxMessage_IT(UartBufferStruct *msg)
{
    int status = 0;

    if(msg->tx.msgToSend_Pending == true) return UART_TX_PENDING;

    if(msg->tx.ptr.cnt_Handle)
    {
        msg->tx.msgToSend = &msg->tx.queue[msg->tx.ptr.index_OUT]; // set msgToSend to buffer index
        RingBuff_Ptr_Output(&msg->tx.ptr, msg->tx.queueSize); // increment buffer index

        if(msg->tx.msgToSend->size > 1)
        {
            msg->tx.msgToSend_BytePtr = 0;
            msg->tx.msgToSend_Pending = true;
            UART_TransmitMessage(msg); // start the transmit
        }
        else // the queue has no bytes to send.
        {
            return 1; // return non zero is an error
        }
    }

    return status;
}

/*
 * Description: This will transmit using DMA
 * Input: the UART data structure
 *
 */
int UART_TxMessage_DMA(UartBufferStruct *msg)
{
    int status = 0;
/*
    if(msg->uart.tx.ptr.cnt_Handle) // message in queue
    {
        msg->uart.tx.msgToSend = &msg->uart.tx.queue[msg->uart.tx.ptr.index_OUT]; // point to queue

        RingBuff_Ptr_Output(&msg->uart.tx.ptr, msg->uart.tx.queueSize); // increment pointer

        MAP_uDMAChannelTransferSet(msg->config.udma_tx_channel | UDMA_PRI_SELECT, UDMA_MODE_BASIC,
                                   msg->uart.tx.msgToSend->data, (void *)(msg->instance), msg->uart.tx.msgToSend->size);

        msg->uart.tx.msgToSend_Pending = true; // flag that DMA is sending
        MAP_uDMAChannelEnable(msg->config.udma_tx_channel); // enable
    }
*/
    return status;
}


/*
 * Description: Called from UART_TxMessage_IT and Tx Interrupt
 */
static void UART_TransmitMessage(UartBufferStruct *msg)
{
    if(msg->tx.msgToSend_BytePtr < msg->tx.msgToSend->size)
    {
        if(UARTCharPutNonBlocking(msg->uart_base, msg->tx.msgToSend->data[msg->tx.msgToSend_BytePtr]) == true)
        {
            if(++msg->tx.msgToSend_BytePtr == msg->tx.msgToSend->size)
            {
                msg->tx.msgToSend_Pending = false; // clear for next message to be sent
                UART_TxMessage_IT(msg); // transmit next message in queue if available
            }
        }
    }
}

void SetEchoMode(bool mode)
{
    echoMode = mode;
}

bool GetEchoMode(void)
{
    return echoMode;
}


void UART_NotifyUser(UartBufferStruct *msg, char *str, uint32_t size, bool lineFeed)
{
    uint8_t strMsg[UART_TX_DATA_SIZE] = {0};

    memcpy(&strMsg, str, size);

    if(lineFeed == true)
    {
        strcat((char*)strMsg, "\r\n");
        size += 2;
    }

    UART_TX_AddDataToBuffer(msg, strMsg, size);
}




/*
 * How to use:
 *
 * 	Below is an example on how to Initialize a buffer
 *

// define the UART0 Rx buffers
UartBufferStruct uart0_msg =
{
    .rx.uartType = UART_ASCII,
    .uart_base = UART0_BASE, // The UART base
    .rx.queueSize = UART_RX_QUEUE_SIZE,
    .tx.queueSize = UART_TX_QUEUE_SIZE,
    .rx.dataSize = UART_RX_DATA_SIZE,
    .rx.RxIRQ = TM4C_UART_Rx_IRQ, // The interrupt Function
    .tx.TxIRQ = TM4C_UART_Tx_IRQ, // The interrupt Function
    .ErrorIRQ = TM4C_UART_ErrorCallback // The interrupt Function
};

*/

