#ifndef TIMER_CALLBACK_H
#define TIMER_CALLBACK_H

#include "main.h"

// user defines
#define MAX_TIMER_CALLBACK 8 // increase if more callbacks are needed


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
    TimerCallback callback;// what function to callback. This is the first callback.

    bool timerCallback2Enabled; // new 12-25-2022
    TimerCallback callback2;// new 12-25-2022 secondary callback to call after the timer is disabled for the first callback.

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

int TimerCallbackRegisterRepetition(TimerCallbackStruct *timerInstance, TimerCallback callback);
int TimerCallbackResetRepetition(TimerCallbackStruct *timerInstance, TimerCallback callback, uint32_t time, uint32_t repetition);

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
