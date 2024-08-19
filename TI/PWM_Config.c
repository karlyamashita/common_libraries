/*
 * PWM_Config.c
 *
 *  Created on: Jul 23, 2024
 *      Author: karl.yamashita
 */


#include "main.h"



void PWM_Config(void)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    MAP_GPIOPinConfigure(GPIO_PE5_M0PWM5);
    MAP_GPIOPinTypePWM(GPIO_PORTB_BASE, FAN_PWM_Pin);

    //
    // Set the PWM period to 25kHz.  To calculate the appropriate parameter
    // use the following equation: N = (1 / f) * SysClk.  Where N is the
    // function parameter, f is the desired frequency, and SysClk is the
    // system clock frequency.
    //
    MAP_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, (SysCtlClockGet() / 25000));

}

