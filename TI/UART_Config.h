/*
 * UART_Config.h
 *
 *  Created on: Jul 17, 2024
 *      Author: karl.yamashita
 */

#ifndef PERIPHERALCONFIG_UART_CONFIG_H_
#define PERIPHERALCONFIG_UART_CONFIG_H_


typedef struct
{
    uint32_t uart_base; // UART0_BASE
    uint32_t sys_ctl_peripheral; // SYSCTL_PERIPH_UART0
    uint32_t systemClock;
    uint32_t baudRate; // 115200
    uint32_t GPIOPinConfigureRx; // GPIO_PA0_U0RX
    uint32_t GPIOPinConfigureTx; // GPIO_PA1_U0TX
    uint32_t GPIOPinType; // UART0_RX_Pin | UART0_TX_Pin
    uint32_t GPIOPinTypeBase; // GPIO_PORTA_BASE
    uint32_t IntEnable; // INT_UART0
    void (*UartIntRegister)(void); // USART0_IRQHandler
}UART_ConfigDef;


void USART_Config(UART_ConfigDef *config);


#endif /* PERIPHERALCONFIG_UART_CONFIG_H_ */
