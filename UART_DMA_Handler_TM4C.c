/*
 * UART_Handler.c
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */


/*
 * Description: Only Tx uses DMA. Since we have variable strings being received we can't use DMA so we need to do standard 1 byte interrupt.
 */


/*
 * Description: User will need to include "RingBuffer.h" and "UartCharBuffer.h" files.
 */

#include "main.h"




bool echoMode = 0;


#pragma DATA_ALIGN(pui8ControlTable, 256)
uint8_t pui8ControlTable[256];


void __attribute__((weak)) UART_DMA_RxCallback(UART_DMA_Struct_t *msg)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(msg);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_UART_ErrorCallback can be implemented in the user file.
     */
}

void __attribute__((weak)) UART_DMA_TxCallback(UART_DMA_Struct_t *msg)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(msg);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_UART_ErrorCallback can be implemented in the user file.
     */
}

void __attribute__((weak)) UART_DMA_ErrorCallback(UART_DMA_Struct_t *msg)
{
    /* Prevent unused argument(s) compilation warning */
        UNUSED(msg);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_ErrorCallback can be implemented in the user file.
   */
}

/*
 * Description: This should be called from a registered interrupt function passing the UART_DMA_Struct_t that has the UART_BASE
 *
 */
void UART_DMA_IRQ_Handler(UART_DMA_Struct_t *msg)
{
    uint32_t interruptStatus = 0;
    uint32_t errorFlags = 0;

    interruptStatus = UARTIntStatus(msg->instance, true);
    UARTIntClear(msg->instance, interruptStatus);

    // If the UART1 DMA TX channel is disabled, that means the TX DMA transfer is done.
    if(!MAP_uDMAChannelIsEnabled(msg->config.udma_tx_channel))
    {
        msg->uart.tx.msgToSend_Pending = false; // DMA done sending

        if(msg->TxISR != NULL)
        {
            msg->TxISR(msg); // Check if there is more messages in queue
        }
    }

    // RX
   // errorFlags = (interruptStatus & (uint32_t) (UART_INT_OE | UART_INT_FE | UART_INT_PE | UART_INT_BE));
    if(errorFlags == 0)
    {
        if( (interruptStatus & (UART_INT_RX | UART_INT_RT)) != 0)
        {
            if(msg->RxISR != NULL)
            {
                msg->RxISR(msg);
            }
            return;
        }
    }

    // Error flags
    /*
    if(errorFlags != 0)
    {
        if((interruptStatus & UART_INT_OE) != 0)
        {
            msg->errorCode |= UART_RXERROR_OVERRUN;
        }

        if((interruptStatus & UART_INT_FE) != 0)
        {
            msg->errorCode |= UART_RXERROR_FRAMING;
        }

        if((interruptStatus & UART_INT_BE) != 0)
        {
            msg->errorCode |= UART_RXERROR_BREAK;
        }

        if((interruptStatus & UART_INT_PE) != 0)
        {
            msg->errorCode |= UART_RXERROR_PARITY;
        }

        UART_DMA_ErrorCallback(msg);
        return;
    }
    */
}

void UART_DMA_Transmit(UART_DMA_Struct_t * msg)
{
    if(msg->uart.tx.msgToSend_Pending) return; // DMA still sending

    if(msg->uart.tx.ptr.cnt_Handle) // message in queue
    {
        msg->uart.tx.msgToSend = &msg->uart.tx.queue[msg->uart.tx.ptr.index_OUT]; // point to queue

        RingBuff_Ptr_Output(&msg->uart.tx.ptr, msg->uart.tx.queueSize); // increment pointer

        MAP_uDMAChannelTransferSet(msg->config.udma_tx_channel | UDMA_PRI_SELECT, UDMA_MODE_BASIC,
                                   msg->uart.tx.msgToSend->data, (void *)(msg->instance), msg->uart.tx.msgToSend->size);

        msg->uart.tx.msgToSend_Pending = true; // flag that DMA is sending
        MAP_uDMAChannelEnable(msg->config.udma_tx_channel); // enable
    }
}


/*
 * Description: To add string messages to queue.
 */
void UART_DMA_NotifyUser(UART_DMA_Struct_t *msg, char *str, bool lineFeed)
{
    uint8_t strMsg[UART_TX_BYTE_BUFFER_SIZE] = {0};
    uint32_t size = strlen(str);

    memcpy(&strMsg, str, strlen(str));

    if(lineFeed == true)
    {
        strcat((char*)strMsg, "\r\n");
        size += 2;
    }

    //UART_TX_AddDataToBuffer(&msg->uart, strMsg, size);

    //UART_DMA_Transmit(msg);
}

/*
 * Description: To add binary data to queue
 */
void UART_DMA_AddBinaryData(UART_DMA_Struct_t *msg, uint8_t *data, uint32_t size)
{
    UART_TX_AddDataToBuffer(&msg->uart, data, size);
}

/*
 * Description: Initialize the TX for DMA. RX does not use DMA since it is variable length
 */
void UART_DMA_PeriperalInit(UART_DMA_Struct_t *msg, uint32_t sysClock)
{
    assert_param(IS_UART_BASE(msg->instance));
    assert_param(IS_SYS_CTL_PERIPHERAL(msg->config.sys_ctl_peripheral));
    assert_param(IS_UART_BAUD_RATE(msg->config.baudRate));

    msg->config.sysClock = sysClock;

    //
    // Enable the uDMA controller at the system level.  Enable it to continue
    // to run while the processor is in sleep.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    MAP_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UDMA);

    //
    // Enable the uDMA controller error interrupt.  This interrupt will occur
    // if there is a bus error during a transfer.
    //
    MAP_IntEnable(INT_UDMAERR);

    //
    // Enable the uDMA controller.
    //
    MAP_uDMAEnable();

    //
    // Point at the control table to use for channel control structures.
    //
    MAP_uDMAControlBaseSet(pui8ControlTable);


    //
    // Enable the UART peripheral, and configure it to operate even if the CPU
    // is in sleep.
    //
    MAP_SysCtlPeripheralEnable(msg->config.sys_ctl_peripheral);
    MAP_SysCtlPeripheralSleepEnable(msg->config.sys_ctl_peripheral);
    //
    // Configure the UART communication parameters.
    //
    MAP_UARTConfigSetExpClk(msg->instance, sysClock, msg->config.baudRate, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    //
    // Set both the TX and RX trigger thresholds to 4.  This will be used by
    // the uDMA controller to signal when more data should be transferred.  The
    // uDMA TX and RX channels will be configured so that it can transfer 4
    // bytes in a burst when the UART is ready to transfer more data.
    //
    UARTFIFOLevelSet(msg->instance, UART_FIFO_TX1_8, UART_FIFO_RX1_8);

    //
    // Enable the UART for operation, and enable the uDMA interface for both TX
    // and RX channels.
    //
    MAP_UARTEnable(msg->instance);
    MAP_UARTDMAEnable(msg->instance, UART_DMA_TX);

    // tx
    //
    // Put the attributes in a known state for the uDMA UART0TX channel.  These
    // should already be disabled by default.
    //
    MAP_uDMAChannelAttributeDisable(msg->config.udma_tx_channel, UDMA_ATTR_ALTSELECT | UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK);

    //
    // Set the USEBURST attribute for the uDMA UART TX channel.  This will
    // force the controller to always use a burst when transferring data from
    // the TX buffer to the UART.  This is somewhat more effecient bus usage
    // than the default which allows single or burst transfers.
    //
    MAP_uDMAChannelAttributeEnable(msg->config.udma_tx_channel, UDMA_ATTR_USEBURST);

    //
    // Configure the control parameters for the UART TX.  The uDMA UART TX
    // channel is used to transfer a block of data from a buffer to the UART.
    // The data size is 8 bits.  The source address increment is 8-bit bytes
    // since the data is coming from a buffer.  The destination increment is
    // none since the data is to be written to the UART data register.  The
    // arbitration size is set to 4, which matches the UART TX FIFO trigger
    // threshold.
    //
    MAP_uDMAChannelControlSet(msg->config.udma_tx_channel | UDMA_PRI_SELECT, UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_4);

    //
    // Now both the uDMA UART TX and RX channels are primed to start a
    // transfer.  As soon as the channels are enabled, the peripheral will
    // issue a transfer request and the data transfers will begin.
    //
    //MAP_uDMAChannelEnable(msg->config.udma_rx_channel);
    MAP_uDMAChannelEnable(msg->config.udma_tx_channel);
}


void UART_DMA_SetEchoMode(UART_DMA_Struct_t *msg, bool mode)
{
    msg->echoModeFlag = mode;
}

bool UART_DMA_GetEchoMode(UART_DMA_Struct_t *msg)
{
    return msg->echoModeFlag;
}


/*
 * Example setting up UART0
 *
UART_DMA_Struct_t uart0_msg =
{
    .uart.rx.uartType = UART_ASCII,
    .instance = UART0_BASE,
    .config.baudRate = 115200,
    .config.sys_ctl_peripheral = SYSCTL_PERIPH_UART0,
    .config.udma_tx_channel = UDMA_CHANNEL_UART0TX,
    .RxISR = UART_DMA_RxCallback,
    .TxISR = UART_DMA_TxCallback,
    .ErrorISR = UART_DMA_ErrorCallback,
    .uart.rx.queueSize = UART_RX_MESSAGE_QUEUE_SIZE,
    .uart.tx.queueSize = UART_TX_MESSAGE_QUEUE_SIZE
};

 */
