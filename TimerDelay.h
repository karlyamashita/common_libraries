#ifndef TIMER_DELAY_H
#define TIMER_DELAY_H

#include "main.h"

uint32_t TimerDelayRegister(void);
uint32_t TimerDelayGetValue(uint32_t timer);
void TimerDelaySYSTICK(void);
void TimerDelaySetValue(uint32_t timer, uint32_t value);


#endif // TIMER_DELAY_H
