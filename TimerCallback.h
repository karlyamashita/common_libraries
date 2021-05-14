#ifndef TIMER_CALLBACK_H
#define TIMER_CALLBACK_H

#include "main.h"

#define MAX_TIMER_CALLBACK 12 // increase if more callbacks are needed

typedef void (*TimerCallback)(void);// no data is passed

enum TIMER_CALLBACK_RATE{
	NO_REPEAT_TIMER,
	REPEAT_TIMER
};

enum TIMER_CALLBACK_ENABLE{
    T_DISABLED,
    T_ENABLED
};

typedef struct TimerCallbackStruct{
    TimerCallback callback;// what function to callback
    bool timerShutDownEnable; // shut down after a certain time
    uint32_t timerShutDownCount; // the current repeat timer
    uint32_t timerShutDownValue; // the repeat timer to reach;
    bool timerRepeat; // repeat or disable after callback
    bool timerEnabled; // enable/disable callback
    uint32_t timerValue; // the time to reach
    uint32_t timerCount; // the current timer count
}TimerCallbackStruct;


int8_t TimerCallbackRegister(TimerCallback callback, uint32_t timerValue, bool repeat, uint32_t timerShutDownValue);
int8_t TimerCallbackShutDownEnable(TimerCallback callback, uint8_t enable);
uint8_t TimerCallbackClearShutDownTimer(TimerCallback callback);
uint8_t TimerCallbackClearTimer(TimerCallback callback);
uint8_t TimerCallbackDelete(TimerCallback callback);
uint8_t TimerCallbackEnable(TimerCallback callback, uint8_t enable);
uint8_t TimerCallbackEnableStatus(TimerCallback callback, uint8_t *status);
uint8_t TimerCallbackGetCurrentTimerValue(TimerCallback callback, uint32_t *timerValue);
uint8_t TimerCallbackSetTimerRepeat(TimerCallback callback, uint32_t timerValue, uint8_t repeat);
uint8_t TimerCallbackExists(TimerCallback callback);
void TimerCallbackIncrement(void);
void TimerCallbackCheck(void);


#endif // TIMER_CALLBACK_H
