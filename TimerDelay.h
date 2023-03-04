#ifndef TIMER_DELAY_H
#define TIMER_DELAY_H

#include "main.h"

uint8_t TimerRegister(void);

uint32_t TimerGetValue(uint8_t timer);
void TimerSYSTICK(void);
void TimerSetValue(uint8_t timer, uint32_t value);


#endif // TIMER_DELAY_H
