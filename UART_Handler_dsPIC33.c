/*
 * UART_Handler.c
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */



#include "main.h"



UartBufferStruct uart1_msg =
{
    .instance = UART_PORT_1,
    .rx.uartType = UART_ASCII
};

UartBufferStruct uart2_msg =
{
    .instance = UART_PORT_2,
    .rx.uartType = UART_ASCII
};


/*
 *  Microchip default callback
 *
 */
// UART1_Receive_CallBack for Classic
void UART1_RxCompleteCallback(void)
{
	uart1_msg.rx.irqByte = (uint8_t)U1RXREG;
    UART_AddByteToBuffer(&uart1_msg);
}

// UART2_Receive_CallBack for Classic
void UART2_RxCompleteCallback(void)
{
	uart2_msg.rx.irqByte = (uint8_t)U2RXREG;
	UART_AddByteToBuffer(&uart2_msg);
}


/*
 * Description: Sends string
 *
 * Input: Character buffer structure. The structure holds the UART base and the char array.
 * Output: HAL status
 */
int UART_TxMessage(UartBufferStruct *msg)
{
    uint16_t size = 0;
    uint8_t i = 0;
    uint8_t uartPort;
    uint8_t *ptr;

    if(msg->tx.ptr.cnt_Handle)
    {
        ptr = msg->tx.queue[msg->tx.ptr.index_OUT].data;
        size = msg->tx.queue[msg->tx.ptr.index_OUT].size;
        uartPort = msg->instance;

        while(size)
        {
            if(uartPort == 1)
            {
                if(!(U1STAHbits.UTXBF == 1))
                {
                    U1TXREG = msg->tx.queue[msg->tx.ptr.index_OUT].data[i];
                    size--;
                    i++;
                }
            }
            else if(uartPort == 2)
            {
                if(!(U2STAHbits.UTXBF == 1))
                {
                    U2TXREG = msg->tx.queue[msg->tx.ptr.index_OUT].data[i];
                    size--;
                    i++;
                }
            }
        }

        RingBuff_Ptr_Output(&msg->tx.ptr, UART_TX_MESSAGE_QUEUE_SIZE);
    }
    return NO_ERROR;
}

