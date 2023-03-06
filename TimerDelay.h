#ifndef TIMER_DELAY_H
#define TIMER_DELAY_H

#include "main.h"

uint8_t TimerDelayRegister(void);

uint32_t TimerDelayGetValue(uint8_t timer);
void TimerDelaySYSTICK(void);
void TimerDelaySetValue(uint8_t timer, uint32_t value);


#endif // TIMER_DELAY_H
