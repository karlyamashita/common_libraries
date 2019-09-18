#ifndef TIMER_CALLBACK_H
#define TIMER_CALLBACK_H

#include "main.h"


enum TIMER_CALLBACK_RATE{
	NO_REPEAT_TIMER,
	REPEAT_TIMER
};

typedef void (*TimerCallback)(void);// no data is passed

int8_t CreateTimerCallback(TimerCallback callback, uint32_t timerCountThreshold, uint8_t repeat);
uint8_t ClearTimerCallbackTimer(TimerCallback callback);
uint8_t RemoveTimerCallback(TimerCallback callback);
uint8_t EnableTimerCallback(TimerCallback callback, uint8_t mode) ;
uint8_t GetTimerCallbackEnableStatus(TimerCallback callback, uint8_t *status);
uint8_t GetTimerCallbackCurrentTimerValue(TimerCallback callback, uint32_t *timerValue);
uint8_t SetTimerCallbackTimerValue(TimerCallback callback, uint32_t timerValue);
uint8_t SetCallbackTimerThresholdValue(TimerCallback callback, uint32_t timerThresholdValue, uint8_t repeat);
uint8_t GetTimerCallbackExists(TimerCallback callback);
void IncrementTimerCallback(void);
void CheckTimerCallback(void);
void SortTimerCallback(void);




#endif // TIMER_CALLBACK_H
