/*
 * UART_Config.c
 *
 *  Created on: Jul 17, 2024
 *      Author: karl.yamashita
 */


#include "main.h"


/*
 * Description: Port for protocol messages
 *
 * Input:
 * Output:
 */
void USART_Config(UART_ConfigDef *config)
{
#define NEW_UART_CONFIG
#ifdef NEW_UART_CONFIG
    // enable peripheral
    MAP_SysCtlPeripheralEnable(config->sys_ctl_peripheral);
    while(!SysCtlPeripheralReady(config->sys_ctl_peripheral))
    {
    }

    MAP_IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinConfigure(config->GPIOPinConfigureRx);
    GPIOPinConfigure(config->GPIOPinConfigureTx);

    MAP_GPIOPinTypeUART(config->GPIOPinTypeBase, config->GPIOPinType);

    MAP_UARTConfigSetExpClk(config->uart_base, config->systemClock, config->baudRate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    MAP_IntEnable(config->IntEnable);
    MAP_UARTIntEnable(config->uart_base, UART_INT_RX | UART_INT_RT | UART_INT_TX);

    MAP_UARTTxIntModeSet(config->uart_base, UART_TXINT_MODE_EOT); // 6-12-2024

    UARTFIFOLevelSet(config->uart_base,  UART_FIFO_TX4_8, UART_FIFO_RX4_8);
    UARTFIFOEnable(config->uart_base);

    //register a handler
    UARTIntRegister(config->uart_base, config->UartIntRegister);

    /*
   uart0_config.uart_base = UART0_BASE;
    uart0_config.sys_ctl_peripheral = SYSCTL_PERIPH_UART0;
    uart0_config.systemClock = SysCtlClockGet();
    uart0_config.baudRate = 115200;
    uart0_config.GPIOPinConfigureRx = GPIO_PA0_U0RX;
    uart0_config.GPIOPinConfigureTx = GPIO_PA1_U0TX;
    uart0_config.GPIOPinType = UART0_RX_Pin | UART0_TX_Pin;
    uart0_config.IntEnable = INT_UART0;
    uart0_config.IntRegister = USART0_IRQHandler;
     */
#else


    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    {
    }

    MAP_IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, UART0_RX_Pin | UART0_TX_Pin);

    MAP_UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    MAP_IntEnable(INT_UART0);
    MAP_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT | UART_INT_TX);

    MAP_UARTTxIntModeSet(UART0_BASE, UART_TXINT_MODE_EOT); // 6-12-2024

    UARTFIFOLevelSet(UART0_BASE,  UART_FIFO_TX4_8, UART_FIFO_RX4_8);
    UARTFIFOEnable(UART0_BASE);

    //register a handler
    UARTIntRegister(UART0_BASE, USART0_IRQHandler);
#endif

}

