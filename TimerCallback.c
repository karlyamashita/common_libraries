/*
Creates a timer and will callback a function when timer reaches threshold value.

Call this function "IncrementTimerCallback()" from SysTick_Handler() in stm32f1xx_it.c 
Call this function "CheckTimerCallback()" from polling routine.

*/

#include "LSP.h"
#ifdef USE_MTIMER_CALLBACK
#include "main.h"
#include "TimerCallback.h"

#define MAX_TIMER_CALLBACK 8 // increase if more callbacks are needed

typedef struct TimerCallbackStruct{
	TimerCallback callback;// what function to callback
	uint8_t timerRepeat; // repeat or disable after callback
	uint8_t timerEnabled;// enable/disable callback
	uint32_t timerCountThreshold; // the time to reach
	uint32_t timerCount; // the current timer count
}TimerCallbackStruct;

TimerCallbackStruct timerCallback[MAX_TIMER_CALLBACK] = {0}; // init array
uint8_t timerCallbackLastIndex = 0;// the number of callback timers used

/*
function: Iterate through TimerCallbackArray until a free spot is found in array. Copy callback and timerCount to array.
input: the function to callback, the timer count, and if it needs to repeat or disables itself after function is called.
output: the timer array pointer. 0 if no array available, -1 if defined already, -2 if null callback
*/
int8_t CreateTimerCallback(TimerCallback callback, uint32_t timerCountThreshold, uint8_t repeat) {
	int i = 0;
	if(callback == 0) return -2; // null callback
	while(timerCallback[i].callback != 0) {
		if(timerCallback[i].callback == callback) {
			return -1;// Callback already defined
		}
		if(i == MAX_TIMER_CALLBACK) {
			return 0;// Maximum timers reached
		}	
		i++;// next
	};
	timerCallback[i].timerCount = 0;// clear the timer
	timerCallback[i].timerCountThreshold = timerCountThreshold; 
	timerCallback[i].callback = callback;
	timerCallback[i].timerRepeat = repeat;
	timerCallback[i].timerEnabled = 1;
	timerCallbackLastIndex = i + 1;// number of timers created
	return i;
}

/*
function: Clears the timer so callback doesn't get fired yet. Same as SetCallbackTimerValue but clears timer only.
input: the callback
output: return 0 if successful
*/
uint8_t ClearTimerCallbackTimer(TimerCallback callback) {
	uint8_t i = 0;
	while(timerCallback[i].callback != callback) {
		if(i == timerCallbackLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
	timerCallback[i].timerCount = 0; // clear timer count
	return 0;
}

/*
function: frees up the array callback position
input: the callback
output: return 0 if successful
*/
uint8_t RemoveTimerCallback(TimerCallback callback) {
	uint8_t i = 0;
	while(timerCallback[i].callback != callback) {
		if( i == timerCallbackLastIndex) {
			return 1; // callback not found
		}
		i++;
	};
	timerCallback[i].callback = 0; // deinit callback routine
	SortTimerCallback();// sort the array
	timerCallbackLastIndex -= 1;// one less callback
	return 0;
}

/*
function:	Enable or disable the callback
input: the callback, enable or disable state
output: return 0 if successful
*/
uint8_t EnableTimerCallback(TimerCallback callback, uint8_t mode) {
	uint8_t i = 0;
	while(timerCallback[i].callback != callback) {
		if( i == timerCallbackLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
	timerCallback[i].timerEnabled = mode;
	return 0;
}

/*
function:	Get the callback enable status
input: the callback, the variable address to pass the enable status to
output: return 0 if successful
*/
uint8_t GetTimerCallbackEnableStatus(TimerCallback callback, uint8_t *enableStatus) {
	uint8_t i = 0;
	while(timerCallback[i].callback != callback) {
		if( i == timerCallbackLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
	*enableStatus = timerCallback[i].timerEnabled;
	return 0;
}

/*
function:	Get the callback's timer value
input: the callback, the variable address to pass the current timer value to
output: return 0 if successful
*/
uint8_t GetTimerCallbackCurrentTimerValue(TimerCallback callback, uint32_t *timerValue) {
	uint8_t i = 0;
	while(timerCallback[i].callback != callback) {
		if( i == timerCallbackLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
	*timerValue = timerCallback[i].timerCount;
	return 0;
}

/*
function:	See if timercallback exists
input: the callback
output: return 1 if callback exists
*/
uint8_t GetTimerCallbackExists(TimerCallback callback) {
	uint8_t i = 0;
	while(timerCallback[i].callback != callback) {
		if( i == timerCallbackLastIndex) {
			return 0;// callback not found
		}
		i++;
	};
	return 1;
}

/*
function:	Sets timer value to the callback
input: the callback, the timer value. 
output: return 0 if successful
*/
uint8_t SetCallbackTimerValue(TimerCallback callback, uint32_t timerValue) {
	uint8_t i = 0;
	while(timerCallback[i].callback != callback) {
		if( i == timerCallbackLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
	timerCallback[i].timerCount = timerValue; // new timer value
	return 0;
}

/*
function:	Updates timerThreshold value to the callback
input: the callback, the timerThreshold value. 
output: return 0 if successful
*/
uint8_t SetCallbackTimerThresholdValue(TimerCallback callback, uint32_t timerThresholdValue, uint8_t repeat) {
	uint8_t i = 0;
	while(timerCallback[i].callback != callback) {
		if( i == timerCallbackLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
	timerCallback[i].timerCountThreshold = timerThresholdValue; // new timer value
	timerCallback[i].timerRepeat = repeat;
	timerCallback[i].timerCount = 0;// clear the timer
	return 0;
}

/*
function: Call this function from SysTick_Handler() in stm32f1xx_it.c
input: none
output: none
*/
void IncrementTimerCallback(void) {
	int i = 0;
	while(i != timerCallbackLastIndex) { // iterate through all arrays
		if(timerCallback[i].timerEnabled && timerCallback[i].callback != 0) { // check if callback is enabled.
			timerCallback[i].timerCount += 1; // increment the timerCount
		}
		i++;
	}
}

/*
function: Checks if timer is reached and jumps to callback function. 
				Will enter and exit on each array pointer increment to reduce doing multiple callback functions in one run.
				Call this function from polling routine.
input: none
output: none
*/
void CheckTimerCallback(void) {
	int i = 0; // the array pointer
	while(i != timerCallbackLastIndex) { 
		if(timerCallback[i].timerEnabled == 1) {// timer is enable so now check if time is reached
			if(timerCallback[i].timerCount >= timerCallback[i].timerCountThreshold) {
				timerCallback[i].timerCount = 0;// clear timer
				timerCallback[i].callback();// jump to callback function
				if(timerCallback[i].timerRepeat == NO_REPEAT_TIMER) {// if no repeat then disable timer for this function
					timerCallback[i].timerEnabled = 0; // disable timer
				}
				i++; 
				return;// A callback function has been called so exit and re-enter at next array pointer. Avoid blocking.
			}	
		}
		i++; // iterate until no more array.
	}
	i = 0;// start at beggining of array
}

/*
function: Sorts and removes any blank callbacks in between other callbacks.
input: none
output: none
*/
void SortTimerCallback(void) {
	int i = 0;
	while(i != timerCallbackLastIndex - 1) { // go through all callbacks
		if(timerCallback[i].callback == 0) { // found zero at this index
			if(timerCallback[i+1].callback != 0) { // make sure next index is not zero.
				timerCallback[i].callback = timerCallback[i+1].callback;// shift following callback down one index
				timerCallback[i+1].callback = 0; // now delete callback from this index
			}
		}
		i++;
	}
}

#endif // USE_MTIMER_CALLBACK


