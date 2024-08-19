/*
 * SystemTick_Config.c
 *
 *  Created on: Jul 12, 2024
 *      Author: karl.yamashita
 */

#include "main.h"

/*
 * Description: Enable system tick with 1ms interrupt
 *
 *
 */
void SystemTick_Config(void)
{
    // set tick period ms
    SysTickPeriodSet(SysCtlClockGet() / 1000);

    // Enable SysTick.
    SysTickEnable();

    // Enable the SysTick Interrupt.
    SysTickIntEnable();

    //register a handler
    SysTickIntRegister(SystemTick_Handler);

    // enable interrupt to processor
    IntMasterEnable();
}

