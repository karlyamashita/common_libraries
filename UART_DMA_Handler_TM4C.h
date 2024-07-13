/*
 * UART_Handler.h
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */

#ifndef INC_UART_HANDLER_H_
#define INC_UART_HANDLER_H_



// todo - make another config file for these
#define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))

void assert_failed(uint8_t *file, uint32_t line);


#define IS_UART_BASE(__UARTBASE__) ( ((__UARTBASE__) == UART0_BASE) || ((__UARTBASE__) == UART1_BASE) )
#define IS_SYS_CTL_PERIPHERAL(__SYSCTLPERIPHERAL__) ( ((__SYSCTLPERIPHERAL__) == SYSCTL_PERIPH_UART0) || ((__SYSCTLPERIPHERAL__) == SYSCTL_PERIPH_UART1) )
#define IS_UART_BAUD_RATE(__UARTBAUDRATE__) ( ((__UARTBAUDRATE__) > 0) && ((__UARTBAUDRATE__) <= 921600) )



#if !defined(UNUSED)
#define UNUSED(X) (void)X         /* To avoid gcc/g++ warnings */
#endif /* UNUSED */


typedef struct
{
    uint32_t sys_ctl_peripheral;
    uint32_t baudRate;
    uint32_t sysClock;
    uint32_t udma_tx_channel;
}UART_DMA_Config_t;

typedef struct __UART_DMA_Struct_t
{
    uint32_t instance;
    UART_DMA_Config_t config;
    void (*RxISR)(struct __UART_DMA_Struct_t *msg); /*!< Function pointer on Rx IRQ handler */
    void (*TxISR)(struct __UART_DMA_Struct_t *msg); /*!< Function pointer on Tx IRQ handler */
    void (*ErrorISR)(struct __UART_DMA_Struct_t *msg); /*!< Function pointer on Error IRQ handler */
    uint32_t errorCode;
    UartBufferStruct uart;
    bool echoModeFlag;
}UART_DMA_Struct_t;


void UART_DMA_PeriperalInit(UART_DMA_Struct_t *msg, uint32_t sysClock);
void UART_DMA_IRQ_Handler(UART_DMA_Struct_t *msg);

void UART_DMA_Transmit(UART_DMA_Struct_t * msg);
void UART_DMA_NotifyUser(UART_DMA_Struct_t *msg, char *str, bool lineFeed);
void UART_DMA_AddBinaryData(UART_DMA_Struct_t *msg, uint8_t *data, uint32_t size);

void UART_DMA_SetEchoMode(UART_DMA_Struct_t *msg, bool mode);
bool UART_DMA_GetEchoMode(UART_DMA_Struct_t *msg);

void __attribute__((weak)) UART_DMA_RxCallback(UART_DMA_Struct_t *msg);
void __attribute__((weak)) UART_DMA_TxCallback(UART_DMA_Struct_t *msg);
void __attribute__((weak)) UART_DMA_ErrorCallback(UART_DMA_Struct_t *msg);

#endif /* INC_UART_HANDLER_H_ */
