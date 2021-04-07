#ifndef TIMER_CALLBACK_H
#define TIMER_CALLBACK_H

#include "main.h"

#define MAX_TIMER_CALLBACK 8 // increase if more callbacks are needed

enum TIMER_CALLBACK_RATE{
	NO_REPEAT_TIMER,
	REPEAT_TIMER
};

typedef void (*TimerCallback)(void);// no data is passed

typedef struct TimerCallbackStruct{
    TimerCallback callback;// what function to callback
    uint8_t timerRepeat; // repeat or disable after callback
    uint8_t timerEnabled; // enable/disable callback
    uint32_t timerValue; // the time to reach
    uint32_t timerCount; // the current timer count
}TimerCallbackStruct;



int8_t TimerCallbackRegister(TimerCallback callback, uint32_t timerCountThreshold, uint8_t repeat);
uint8_t TimerCallbackClearTimer(TimerCallback callback);
uint8_t TimerCallbackDelete(TimerCallback callback);
uint8_t TimerCallbackEnable(TimerCallback callback, uint8_t mode) ;
uint8_t TimerCallbackEnableStatus(TimerCallback callback, uint8_t *status);
uint8_t TimerCallbackGetCurrentTimerValue(TimerCallback callback, uint32_t *timerValue);
uint8_t TimerCallbackSetTimerRepeat(TimerCallback callback, uint32_t timerThresholdValue, uint8_t repeat);
uint8_t TimerCallbackExists(TimerCallback callback);
void TimerCallbackIncrement(void);
void TimerCallbackCheck(void);
static void TimerCallbackSort(void);




#endif // TIMER_CALLBACK_H
