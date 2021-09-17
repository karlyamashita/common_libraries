/*
 * Uart_TM4C_IRQ_Handler.c
 *
 *  Created on: Jun 17, 2021
 *      Author: karl.yamashita
 */


#include "main.h"
#include "Uart_TM4C_IRQ_Handler.h"


extern uint8_t uartRxCharBuffer[MAX_UART_RX_CHAR_BUFFER_SINGLE];
extern RING_BUFF_INFO uartRxCharBufferPointer;


/*
 * Description: Transmit 8bit data over uart .
 *
 * Input: The uart base, reference to data, the size of data
 * Output: HAL status
 *
 */
HAL_StatusTypeDef UartTxData(uint32_t uartBase, uint8_t *data, uint8_t size){
    while(size)
    {
       UARTCharPut(uartBase, *data++);
       size--;
    }
    return HAL_OK;
}


/*
 * Description: Sends ASCII characters
 *
 * Input: Character buffer structure. The structure holds the uart base and the char array.
 * Output: HAL status
 */
HAL_StatusTypeDef UartTxMessage(UartCharBufferTxStruct *uartBufferPointer){
    uint32_t uart_base;
    uint8_t *pData = uartBufferPointer->data;
    uint8_t count = strlen((char*) pData);


    switch(uartBufferPointer->uartPort){
    case UART_PORT_1:
        uart_base = UART1_BASE;
        break;
    case UART_PORT_3:
        uart_base = UART3_BASE;
        break;
    }
    while(count--){
        UARTCharPut(uart_base, *pData++);
    }
    return HAL_OK;
}

/*
 *
 *
 *
 *
 */
void Uart_Receive(uint32_t uart_base){
    HAL_StatusTypeDef HAL_Status;
    char _char;
    uint32_t interruptStatus = UARTIntStatus(uart_base, true);
    UARTIntClear(uart_base, interruptStatus);

    if (uart_base == UART3_BASE)
    {
        if (UARTCharsAvail(uart_base))
        {
            _char = UARTCharGet(uart_base);
            HAL_Status = UartAddCharToBuffer(UART_PORT_3, &_char);
            if(HAL_Status != HAL_OK){
                // TODO - check HAL Status and act accordingly. For now we're going to lose a character if buffer is full.
            }
        }
    }
    else if(uart_base == UART1_BASE)
    {
        // n/a
    }
}


