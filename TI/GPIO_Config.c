/*
 * GPIO_Config.c
 *
 *  Created on: Jul 12, 2024
 *      Author: karl.yamashita
 */


#include "main.h"



/*
 * Description:
 *
 * Input:
 * Output:
 */
void GPIO_Config(void)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
    {
    }

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {
    }

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    }

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    // PORTB
    // output
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, PSC48V_EN_Pin);
    // input
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, _12V_ON_Pin | _12V_FAULT_Pin | TIB_SITE_ALERT_Pin | TEL_I2C_ALERT_Pin | PSC48V_FAULT_Pin | SW2_Pin);
    //MAP_GPIOPinTypeGPIOOutputOD(GPIO_PORTB_BASE, NULL); // open drain

    // PORTD
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; // unlock first
 //   HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= FT4232_Reset_3_Pin;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0; // lock
    // output
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, PSC48V_LIM_HI_0_Pin | PSC48V_LIM_HI_1_Pin);
//    MAP_GPIOPinTypeGPIOOutputOD(GPIO_PORTD_BASE, NULL); // make open drain

    // PORTF
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; // unlock first
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= TEST_GRN_LED_Pin;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0; // lock
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, MCU_RED_LED_Pin | MCU_GRN_LED_Pin | TEST_GRN_LED_Pin | TEST_RED_LED_Pin | _12V_EN_Pin);
    //MAP_GPIOPinTypeGPIOOutputOD(GPIO_PORTC_BASE, NULL);

    //
    // Set each of the button GPIO pins as an input with a pull-up.
    //
    // PORTB
    MAP_GPIODirModeSet(GPIO_PORTB_BASE,_12V_ON_Pin | _12V_FAULT_Pin | TIB_SITE_ALERT_Pin | TEL_I2C_ALERT_Pin | PSC48V_FAULT_Pin | SW2_Pin, GPIO_DIR_MODE_IN);
//    MAP_GPIOPadConfigSet(GPIO_PORTB_BASE, NULL, GPIO_STRENGTH_10MA, GPIO_PIN_TYPE_STD_WPU);

    // Init interrupts
    // PORTB
    MAP_GPIOIntTypeSet(GPIO_PORTB_BASE, _12V_ON_Pin | _12V_FAULT_Pin | TIB_SITE_ALERT_Pin | TEL_I2C_ALERT_Pin | PSC48V_FAULT_Pin | SW2_Pin, GPIO_BOTH_EDGES);

    //register a handler
    // PORTB
    MAP_GPIOIntRegister(GPIO_PORTB_BASE, PortB_IRQHandler);

    // enable interrupts for pins
    // PORTB
    MAP_GPIOIntEnable(GPIO_PORTB_BASE, _12V_ON_Pin | _12V_FAULT_Pin | TIB_SITE_ALERT_Pin | TEL_I2C_ALERT_Pin | PSC48V_FAULT_Pin | SW2_Pin);

}

