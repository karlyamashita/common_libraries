/*
 * Timer_Config.h
 *
 *  Created on: Jul 18, 2024
 *      Author: karl.yamashita
 */

#ifndef PERIPHERALCONFIG_TIMER_CONFIG_H_
#define PERIPHERALCONFIG_TIMER_CONFIG_H_


typedef struct
{
    uint32_t timerBase;
    uint32_t loadCount;
    uint32_t config; // TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC
    uint32_t timerParam; // TIMER_A | TIMER_B | TIMER_BOTH
    uint32_t controlTrigger; // true/false
}TimerConfigDef;


void Timer_Config(TimerConfigDef *config);


#endif /* PERIPHERALCONFIG_TIMER_CONFIG_H_ */
