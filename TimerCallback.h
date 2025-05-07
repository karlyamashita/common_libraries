#ifndef TIMER_CALLBACK_H
#define TIMER_CALLBACK_H

#include "main.h"

// user defines
#define MAX_TIMER_CALLBACK 8 // increase if more callbacks are needed


typedef void (*TimerCallback)(void);// no data is passed

enum TIMER_CALLBACK_RATE
{
	TIMER_NO_REPEAT,
	TIMER_REPEAT
};

enum TIMER_CALLBACK_ENABLE
{
    TIMER_DISABLED,
    TIMER_ENABLED
};

typedef struct __TimerCallbackStruct
{
	struct
	{
		TimerCallback callback;// what function to callback. This is the first callback.

		bool timerCallback2Enabled; // new 12-25-2022
		TimerCallback callback2;// new 12-25-2022 secondary callback to call after the timer is disabled for the first callback.

		bool timerTimeoutEnable; // shut down after a certain time
		uint32_t timerTimeoutValue; // the shutdown time to reach;
		uint32_t timerTimeoutTick; // the current shutdown tick

		bool timerRepetitionEnable;
		uint32_t timerRepetitionValue; // number of repetitions
		uint32_t timerRepetitionTick; // current number of repetitions

		bool timerEnabled; // enable/disable callback
		uint32_t timerValue; // the time to reach
		uint32_t timerTick; // the current timer count

		bool timerRepeat; // repeat or disable after callback. This overrides timerShutDownEnable, timerRepetitionEnable, timerEnabled
	}Instance[MAX_TIMER_CALLBACK];

    uint32_t timerLastIndex; // this holds the amount of callback amounts registered. The first callback registered should never get deleted as it holds this index value.
}TimerCallbackStruct;


int TimerCallbackRegisterOnly(TimerCallbackStruct *timer, TimerCallback callback);
int TimerCallbackRegister2nd(TimerCallbackStruct *timer, TimerCallback callback, TimerCallback callback2);
int TimerCallbackRegister2ndDisable(TimerCallbackStruct *timer, TimerCallback callback);

int TimerCallbackRegisterStruct(TimerCallbackStruct * timer, TimerCallbackStruct * timerAdd);

int TimerCallbackTimeoutStart(TimerCallbackStruct *timer, TimerCallback callback, uint32_t timerValue, uint32_t timerShutDownValue);
int TimerCallbackTimeoutDisable(TimerCallbackStruct *timer, TimerCallback callback);
int TimerCallbackTimeoutReset(TimerCallbackStruct *timer, TimerCallback callback);

int TimerCallbackRepetitionStart(TimerCallbackStruct *timer, TimerCallback callback, uint32_t time, uint32_t repetition);
int TimerCallbackRepetitionDisable(TimerCallbackStruct *timer, TimerCallback callback);
int TimerCallbackRepetitionResetTimer(TimerCallbackStruct *timer, TimerCallback callback);

int TimerCallbackTimerStart(TimerCallbackStruct *timer, TimerCallback callback, uint32_t timerValue, uint8_t repeat);
int TimerCallbackDisable(TimerCallbackStruct *timer, TimerCallback callback);
int TimerCallbackResetTimer(TimerCallbackStruct *timer, TimerCallback callback);

int TimerCallbackDelete(TimerCallbackStruct *timer, TimerCallback callback);

int TimerCallbackGetCurrentTimerValue(TimerCallbackStruct *timer, TimerCallback callback, uint32_t *timerValue);


int TimerCallbackExists(TimerCallbackStruct *timer, TimerCallback callback);

void TimerCallbackTick(TimerCallbackStruct *timer);
void TimerCallbackPoll(TimerCallbackStruct *timer);

void TimerCallbackHandlerTick(void);

#endif // TIMER_CALLBACK_H
