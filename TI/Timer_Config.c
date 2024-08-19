/*
 * Timer_Config.c
 *
 *  Created on: Jul 18, 2024
 *      Author: karl.yamashita
 */


#include "main.h"

/*
 * Description: One-Shot or Periodic mode
 *
 * example;
 *
 *  TimerConfigDef timer0_Config = {0}; // create new variable
 *
 *  timer0_Config.timerBase = TIMER0_BASE;
    timer0_Config.loadCount = ((SysCtlClockGet()/25000) - 1);
    timer0_Config.config = TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC;
    timer0_Config.controlTrigger = true;
    timer0_Config.timerParam = TIMER_A;

    Timer_Config(&timer0_Config);
 */
void Timer_Config(TimerConfigDef *config)
{
    // enable timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));

    TimerConfigure(config->timerBase, config->config);

    // Set ADC sampling frequency to be 25KHz i.e. every 40uS.
    TimerLoadSet(config->timerBase, config->timerParam, config->loadCount);

    // Enable the ADC trigger output for Timer A.
    TimerControlTrigger(config->timerBase, config->timerParam, config->controlTrigger);

    TimerIntRegister(config->timerBase, config->timerParam, Timer0_IRQHandler);

    IntEnable(INT_TIMER0A);

    TimerIntEnable(config->timerBase, TIMER_TIMA_TIMEOUT);

    IntMasterEnable();

    // Enable Timer 0 which will start the whole application process.
    TimerEnable(config->timerBase, config->timerParam);
}
