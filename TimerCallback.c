/*
Creates a timer and will callback a function when timer reaches threshold value.


Create a callback "TimerCallbackRegister(<function to callback>, <time in ms>, <repeat = 1, no repeat = 0>, <shut down timer value>);"
Call this function "TimerCallbackIncrement()" from SysTick_Handler() in stm32f1xx_it.c
Call this function "TimerCallbackCheck()" from polling routine.

Call TimerCallbackEnable() to  enable/disable callback
Call TimerCallbackShutDownEnable() to enable/disable shutdown of callback.

*/

#include "main.h"
#include "TimerCallback.h"

TimerCallbackStruct timerCallback[MAX_TIMER_CALLBACK] = {0}; // init array
uint8_t timerCallbackLastIndex = 0;// the number of callback timers used

static void TimerCallbackSort(void);

/*
function: Register a callback function. Iterate through TimerCallbackArray until a free spot is found in array. Copy callback and timerCount to array. ShutDownEnable is false by default.
input: the function to callback, the timer value, and if it needs to repeat or disables itself after function is called. The timerShutDownValue for disabling the callback after set time.
output: the timer array pointer. 0 if no array available, -1 if defined already, -2 if null callback
*/
int8_t TimerCallbackRegister(TimerCallback callback, uint32_t timerValue, bool repeat, uint32_t timerShutDownValue) {
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

	if(timerShutDownValue < timerValue){ // timerShutDownValue should not be less than timerValue
	    timerShutDownValue = timerValue + 1;
	}
	timerCallback[i].timerCount = 0;// clear the timer
	timerCallback[i].timerValue = timerValue;
	timerCallback[i].callback = callback;
	timerCallback[i].timerRepeat = repeat;
	timerCallback[i].timerEnabled = 1;
	timerCallback[i].timerShutDownEnable = false;
	timerCallback[i].timerShutDownValue = timerShutDownValue;
	timerCallback[i].timerShutDownCount = 0; // clear shutdown timer
	timerCallbackLastIndex = i + 1;// number of timers created
	return i;
}

int8_t TimerCallbackShutDownEnable(TimerCallback callback, uint8_t enable) {
    uint8_t i = 0;
    while (timerCallback[i].callback != callback)
    {
        if (i == timerCallbackLastIndex)
        {
            return 1; // callback not found
        }
        i++;
    };
    timerCallback[i].timerShutDownEnable = enable;
    return 0;
}

uint8_t TimerCallbackClearShutDownTimer(TimerCallback callback) {
    uint8_t i = 0;
    while(timerCallback[i].callback != callback) {
        if(i == timerCallbackLastIndex) {
            return 1;// callback not found
        }
        i++;
    };
    timerCallback[i].timerShutDownCount = 0; // clear timerShutDownCount
    return 0;
}

/*
function: Clears the timer so callback doesn't get fired yet. Same as SetCallbackTimerValue but clears timer only.
input: the callback
output: return 0 if successful
*/
uint8_t TimerCallbackClearTimer(TimerCallback callback) {
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
uint8_t TimerCallbackDelete(TimerCallback callback) {
	uint8_t i = 0;
	while(timerCallback[i].callback != callback) {
		if( i == timerCallbackLastIndex) {
			return 1; // callback not found
		}
		i++;
	};
	timerCallback[i].callback = 0; // deinit callback routine
	TimerCallbackSort();// sort the array
	timerCallbackLastIndex -= 1;// one less callback
	return 0;
}

/*
function:	Enable or disable the callback
input: the callback, enable or disable state
output: return 0 if successful
*/
uint8_t TimerCallbackEnable(TimerCallback callback, uint8_t enable) {
	uint8_t i = 0;
	while(timerCallback[i].callback != callback) {
		if( i == timerCallbackLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
	timerCallback[i].timerEnabled = enable;
	return 0;
}

/*
function:	Get the callback enable status
input: the callback, the variable address to pass the enable status to
output: return 0 if successful
*/
uint8_t TimerCallbackEnableStatus(TimerCallback callback, uint8_t *enableStatus) {
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
uint8_t TimerCallbackGetCurrentTimerValue(TimerCallback callback, uint32_t *timerValue) {
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
function:	See if timer callback exists
input: the callback
output: return 1 if callback exists
*/
uint8_t TimerCallbackExists(TimerCallback callback) {
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
function:	Updates timer value to the callback and if repeats
input: the callback, the timerThreshold value. 
output: return 0 if successful
*/
uint8_t TimerCallbackSetTimerRepeat(TimerCallback callback, uint32_t timerValue, uint8_t repeat) {
	uint8_t i = 0;
	while(timerCallback[i].callback != callback) {
		if( i == timerCallbackLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
	timerCallback[i].timerValue = timerValue; // new timer value
	timerCallback[i].timerRepeat = repeat;
	timerCallback[i].timerCount = 0;// clear the timer
	return 0;
}

/*
function: Call this function from SysTick_Handler() in stm32f1xx_it.c
input: none
output: none
*/
void TimerCallbackIncrement(void) {
	int i = 0;
	while(i != timerCallbackLastIndex) { // iterate through all arrays
        if (timerCallback[i].callback != 0)
        {
            if (timerCallback[i].timerShutDownEnable) // check if shutdown is enabled
            {
                timerCallback[i].timerShutDownCount += 1; // increment the timerShutDownCount
            }

            if (timerCallback[i].timerEnabled) // check if callback is enabled.
            {
                timerCallback[i].timerCount += 1; // increment the timerCount
            }
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
void TimerCallbackCheck(void) {
	int i = 0; // the array pointer
	while(i != timerCallbackLastIndex) { 

	    if(timerCallback[i].timerShutDownEnable == 1) { // check for shutdown first
	        if(timerCallback[i].timerShutDownCount >= timerCallback[i].timerShutDownValue) {
	            timerCallback[i].timerShutDownCount = 0;
	            timerCallback[i].timerEnabled = 0; // disable timer
	        }
	    }

		if(timerCallback[i].timerEnabled == 1) {// timer is enable so now check if time is reached
			if(timerCallback[i].timerCount >= timerCallback[i].timerValue) {
				timerCallback[i].timerCount = 0;// clear timer
				timerCallback[i].callback();// jump to callback function
				if(timerCallback[i].timerRepeat == NO_REPEAT_TIMER) {// if no repeat then disable timer for this function
					timerCallback[i].timerEnabled = 0; // disable timer
				}
				i++; 
				return;// A callback function has been called so exit and re-enter at next array pointer. This avoids blocking.
			}	
		}
		i++; // iterate until no more array.
	}
	i = 0;// start at beginning of array
}

/*
function: Sorts and removes any blank callbacks in between other callbacks in timerCallback array.
input: none
output: none
*/
static void TimerCallbackSort(void) {
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


