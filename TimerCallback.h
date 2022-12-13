#ifndef TIMER_CALLBACK_H
#define TIMER_CALLBACK_H

#include "main.h"

// user defines
#define MAX_TIMER_CALLBACK 15 // increase if more callbacks are needed
#define MAX_TIMER_INSTANCE 2 // the number of timer instances. Default is 1 for milli seconds. Increase for each timer instances needed.



typedef void (*TimerCallback)(void);// no data is passed

enum TIMER_CALLBACK_RATE{
	TIMER_NO_REPEAT,
	TIMER_REPEAT
};

enum TIMER_CALLBACK_ENABLE{
    TIMER_DISABLED,
    TIMER_ENABLED
};

typedef struct TimerCallbackStruct{
    TimerCallback callback;// what function to callback

    bool timerShutDownEnable; // shut down after a certain time
    uint32_t timerShutDownValue; // the repeat timer to reach;
    uint32_t timerShutDownCount; // the current repeat timer

    bool timerRepetitionEnable;
    uint32_t timerRepetitionValue; // number of repetitions
    uint32_t timerRepetitionCount; // current number of repetitions

    bool timerEnabled; // enable/disable callback
    uint32_t timerValue; // the time to reach
    uint32_t timerCount; // the current timer count

    bool timerRepeat; // repeat or disable after callback. This overrides timerShutDownEnable, timerRepetitionEnable, timerEnabled

    uint32_t timerLastIndex; // this holds the amount of callback amounts registered. The first callback registered should never get deleted as it holds this index value.
}TimerCallbackStruct;


int TimerCallbackRegister(TimerCallbackStruct *timerCallback, TimerCallback callback, uint32_t timerValue, bool repeat);
int TimerCallbackRegisterStruct(TimerCallbackStruct * timerInstance);
int TimerCallbackShutDownRegister(TimerCallbackStruct *timerCallback, TimerCallback callback, uint32_t timerValue, bool repeat, uint32_t timerShutDownValue);
int TimerCallbackShutDownEnable(TimerCallbackStruct *timerCallback, TimerCallback callback, uint8_t enable);
int TimerCallbackClearShutDownTimer(TimerCallbackStruct *timerCallback, TimerCallback callback);

int TimerCallbackSetRepetition(TimerCallbackStruct *timerInstance, TimerCallback callback, uint32_t rep);

int TimerCallbackClearTimer(TimerCallbackStruct *timerCallback, TimerCallback callback);
int TimerCallbackDelete(TimerCallbackStruct *timerCallback, TimerCallback callback);
int TimerCallbackEnable(TimerCallbackStruct *timerCallback, TimerCallback callback, uint8_t enable);
int TimerCallbackResetEnable(TimerCallbackStruct *timerInstance, TimerCallback callback);
int TimerCallbackEnableStatus(TimerCallbackStruct *timerCallback, TimerCallback callback, uint8_t *status);
int TimerCallbackGetCurrentTimerValue(TimerCallbackStruct *timerCallback, TimerCallback callback, uint32_t *timerValue);
int TimerCallbackSetTimerRepeat(TimerCallbackStruct *timerCallback, TimerCallback callback, uint32_t timerValue, uint8_t repeat);
int TimerCallbackSetShutDownValue(TimerCallbackStruct *timerCallback, TimerCallback callback, uint32_t shutDownValue);
int TimerCallbackExists(TimerCallbackStruct *timerCallback, TimerCallback callback);
void TimerCallbackTick(TimerCallbackStruct *timerCallback);
void TimerCallbackCheck(TimerCallbackStruct *timerCallback);


#endif // TIMER_CALLBACK_H
