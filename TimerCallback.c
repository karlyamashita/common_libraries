/*

Creates a timer and will callback a function when timer reaches threshold value.


Good for
- Debouncing a push button switch
- Blinking an LED or sending telemetry data at an interval
- Debouncing a push button switch and counting the number of times the PB is pressed. This requires more than one callback function.
- And anything else you can imagine.




- Define timer instance some where in main or polling file. // TimerCallbackStruct timerInstanceMilliSeconds[MAX_TIMER_CALLBACK] = {0};
- Create a callback passing timer instance and parameters. // TimerCallbackRegister(timerInstanceMilliSeconds, <function to callback>, 100, TIMER_REPEAT);
					Note: The very first callback that is registered holds the timerLastIndex value. This callback should never be deleted or the index value will be lost.
- Call this function "TimerCallbackIncrement(&timerInstanceMilliSeconds)" from SysTick_Handler() in stm32f1xx_it.c
- Call this function "TimerCallbackCheck(&timerInstanceMilliSeconds)" from polling routine.
- Call TimerCallbackEnable(&timerInstanceMilliSeconds, <function to callback>, TIMER_ENABLED) to  enable/disable callback
- Call TimerCallbackShutDownEnable(&timerInstanceMilliSeconds, <function to callback>, TIMER_ENABLED) to enable/disable shutdown of callback.
- Make sure to adjust define values in header file accordingly to your needs.


Revision 2 - 02/17/2022, Added of passing TimerCallbackStruct Instance so that you can have more than one timer instance. One typically for Systick at 1ms and maybe another for micro seconds.
Rev 2.1 - 10/21/2022, Added callback repetition.
Rev 2.2 - 11/22/2022, Added TimerCallbackRegisterStruct function to pass structure data instead of individual arguments.

*/


#include "main.h"
#include "TimerCallback.h"

TimerCallbackStruct timerCallback[MAX_TIMER_CALLBACK]; // default instance. Use extern where needed

static void TimerCallbackSort(TimerCallbackStruct *timerCallback);


/*
function: Register a callback function. Iterates through TimerCallbackArray until a free spot is found in array, then copy callback and timerCount to array.
            If a bunch of parameters need to be set, use the TimerCallbackRegisterStruct() instead.
input: the timer instance, the function to callback, the timer value, and if it needs to repeat or not repeat after the function is called.
output: the timer array pointer. 0 if no array available, -1 if defined already, -2 if null callback
*/
int TimerCallbackRegister(TimerCallbackStruct *timerInstance, TimerCallback callback, uint32_t timerValue, bool repeat) {
	int i = 0;

	if(callback == 0) return -2; // null callback

	while(timerInstance[i].callback != 0) {
		if(timerInstance[i].callback == callback) {
			return -1;// Callback already defined
		}

		if(i == MAX_TIMER_CALLBACK) {
			return 0;// Maximum timers reached
		}	
		i++;// next
	};

	timerInstance[i].callback = callback;

	timerInstance[i].timerShutDownEnable = false;
	timerInstance[i].timerShutDownValue = 0xFFFFFFFF;
	timerInstance[i].timerShutDownCount = 0; // clear shutdown timer

	timerInstance[i].timerRepetitionEnable = false;
    timerInstance[i].timerRepetitionValue = 0;
    timerInstance[i].timerRepetitionCount = 0;

    timerInstance[i].timerEnabled = true;
    timerInstance[i].timerValue = timerValue;
    timerInstance[i].timerCount = 0;// clear the timer

    timerInstance[i].timerRepeat = repeat;

	timerInstance[0].timerLastIndex = i + 1; // only stored in first callback, index 0.

	return i;
}

/*
 * Description: Uses a single structure to save all the parameters instead of passing a bunch of arguments. Be sure to initialize new instance to zero.
 *              Note, has not been tested thoroughly.
 */
int TimerCallbackRegisterStruct(TimerCallbackStruct * timerInstance)
{
    int i = 0;
    
    while(timerCallback[i].callback != 0) {
		if(i == MAX_TIMER_CALLBACK) {
			return 0;// Maximum timers reached
		}	
		i++;// next
	};

    timerCallback[i].callback = timerInstance->callback;
    
    timerCallback[i].timerShutDownEnable = timerInstance->timerShutDownEnable;
	timerCallback[i].timerShutDownValue = timerInstance->timerShutDownValue;
	timerCallback[i].timerShutDownCount = 0; // clear shutdown timer

	timerCallback[i].timerRepetitionEnable = timerInstance->timerRepetitionEnable;
    timerCallback[i].timerRepetitionValue = 0; // clear
    timerCallback[i].timerRepetitionCount = 0; // clear

    timerCallback[i].timerEnabled = timerInstance->timerEnabled;
    timerCallback[i].timerValue = timerInstance->timerValue;
    timerCallback[i].timerCount = 0;// clear the timer

    timerCallback[i].timerRepeat = timerInstance->timerRepeat;

	timerCallback[0].timerLastIndex = i + 1; // only stored in first callback, index 0.
    
    return i;
}

/*
function: Register a callback function with a timer shut down value. Iterate through TimerCallbackArray until a free spot is found in array. Copy callback and timerCount to array. ShutDownEnable is false by default.
input: timer instance, the function to callback, the timer value, and if it needs to repeat or disables itself after function is called. The timerShutDownValue for disabling the callback after set time.
output: the timer array pointer. 0 if no array available, -1 if defined already, -2 if null callback
*/
int TimerCallbackShutDownRegister(TimerCallbackStruct *timerInstance, TimerCallback callback, uint32_t timerValue, bool repeat, uint32_t timerShutDownValue) {
	int i = 0;

	if(callback == 0) return -2; // null callback
	while(timerInstance[i].callback != 0) {
		if(timerInstance[i].callback == callback) {
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

    timerInstance[i].callback = callback;

    timerInstance[i].timerEnabled = 1;
	timerInstance[i].timerValue = timerValue;
    timerInstance[i].timerCount = 0;// clear the timer

	timerInstance[i].timerShutDownEnable = false;
	timerInstance[i].timerShutDownValue = timerShutDownValue;
	timerInstance[i].timerShutDownCount = 0; // clear shutdown timer

    timerInstance[i].timerRepetitionEnable = false;
    timerInstance[i].timerRepetitionValue = 0;
    timerInstance[i].timerRepetitionCount = 0;

    timerInstance[i].timerRepeat = repeat;

	timerInstance[0].timerLastIndex = i + 1; // only stored in first callback, index 0.
	return i;
}

/*
 * Description: Enable/Disable the timer shutdown
 *
 *
 */
int TimerCallbackShutDownEnable(TimerCallbackStruct *timerInstance, TimerCallback callback, uint8_t enable) {
    uint8_t i = 0;

    while (timerInstance[i].callback != callback)
    {
        if (i == timerInstance[0].timerLastIndex)
        {
            return 1; // callback not found
        }
        i++;
    };
    timerInstance[i].timerShutDownEnable = enable;
    return 0;
}

/*
 * Description: Clears the timer shutdown timer count
 *
 */
int TimerCallbackClearShutDownTimer(TimerCallbackStruct *timerInstance, TimerCallback callback) {
    uint8_t i = 0;

    while(timerInstance[i].callback != callback) {
        if(i == timerInstance[0].timerLastIndex) {
            return 1;// callback not found
        }
        i++;
    };
    timerInstance[i].timerShutDownCount = 0; // clear timerShutDownCount
    return 0;
}

/*
 * Description: You can set how many callback repetitions to do before ending callbacks
 *
 *
 */
int TimerCallbackSetRepetition(TimerCallbackStruct *timerInstance, TimerCallback callback, uint32_t repetition)
{
    uint8_t i = 0;

    while(timerInstance[i].callback != callback) {
        if( i == timerInstance[0].timerLastIndex) {
            return 1;// callback not found
        }
        i++;
    };

    timerInstance[i].timerRepetitionValue = repetition;
    timerInstance[i].timerRepetitionCount = 0;
    timerInstance[i].timerRepetitionEnable = true;

    return 0;
}


/*
function: Clears the timer so callback doesn't get fired yet. Same as SetCallbackTimerValue but clears timer only.
input: timer instance, the callback
output: return 0 if successful
*/
int TimerCallbackClearTimer(TimerCallbackStruct *timerInstance, TimerCallback callback) {
	uint8_t i = 0;

	while(timerInstance[i].callback != callback) {
		if(i == timerInstance[0].timerLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
    timerInstance[i].timerCount = 0; // clear timer count
	return 0;
}

/*
function: frees up the array callback position
input: timer instance, the callback
output: return 0 if successful
*/
int TimerCallbackDelete(TimerCallbackStruct *timerInstance, TimerCallback callback) {
	uint8_t i = 0;

    while(timerInstance[i].callback != callback) {
		if( i == timerInstance[0].timerLastIndex) {
			return 1; // callback not found
		}
		i++;
	};
	timerInstance[i].callback = 0; // deinit callback routine
	TimerCallbackSort(timerInstance);// sort the array
    timerInstance[0].timerLastIndex -= 1; // one less callback
	return 0;
}

/*
function:	Enable or disable the callback. The timerCount is not cleared. If you want to clear the timerCount then use TimerCallbackResetStart()
input: timer instance, the callback, enable or disable state
output: return 0 if successful
*/
int TimerCallbackEnable(TimerCallbackStruct *timerInstance, TimerCallback callback, uint8_t enable) {
	uint8_t i = 0;

	while(timerInstance[i].callback != callback) {
		if( i == timerInstance[0].timerLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
    timerInstance[i].timerEnabled = enable;
	return 0;
}

/*
function: Resets the timerCount value to zero and enables it. Good for debouncing switch
input: timer instance, the callback
output: return 0 if successful
 */
int TimerCallbackResetEnable(TimerCallbackStruct *timerInstance, TimerCallback callback)
{
	uint8_t i = 0;

	while(timerInstance[i].callback != callback) {
		if( i == timerInstance[0].timerLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
	timerInstance[i].timerCount = 0;
	timerInstance[i].timerEnabled = true;
	return 0;
}

/*
function:	Get the callback enable status
input: timer instance, the callback, the variable address to pass the enable status to
output: return 0 if successful
*/
int TimerCallbackEnableStatus(TimerCallbackStruct *timerInstance, TimerCallback callback, uint8_t *enableStatus) {
	uint8_t i = 0;

	while(timerInstance[i].callback != callback) {
		if( i == timerInstance[0].timerLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
	*enableStatus = timerInstance[i].timerEnabled;
	return 0;
}

/*
function:	Get the callback's timer value
input: timer instance, the callback, the variable address to pass the current timer value to
output: return 0 if successful
*/
int TimerCallbackGetCurrentTimerValue(TimerCallbackStruct *timerInstance, TimerCallback callback, uint32_t *timerValue) {
	uint8_t i = 0;

	while(timerInstance[i].callback != callback) {
		if( i == timerInstance[0].timerLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
	*timerValue = timerInstance[i].timerCount;
	return 0;
}

/*
function:	See if timer callback exists
input: timer instance, the callback
output: return 1 if callback exists
*/
int TimerCallbackExists(TimerCallbackStruct *timerInstance, TimerCallback callback) {
	uint8_t i = 0;

	while(timerInstance[i].callback != callback) {
		if( i == timerInstance[0].timerLastIndex) {
			return 0;// callback not found
		}
		i++;
	};
	return 1;
}

/*
function:	Updates the callback timer value and if repeats
input: timer instance, the callback, the timerThreshold value.
output: return 0 if successful
*/
int TimerCallbackSetTimerRepeat(TimerCallbackStruct *timerInstance, TimerCallback callback, uint32_t timerValue, uint8_t repeat) {
	uint8_t i = 0;

	while(timerInstance[i].callback != callback) {
		if( i == timerInstance[0].timerLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
	timerInstance[i].timerValue = timerValue; // new timer value
	timerInstance[i].timerRepeat = repeat;
	timerInstance[i].timerCount = 0;// clear the timer
	return 0;
}

/*
function:	Updates shut down value. Shut down enable is false by default. Use TimerCallbackShutDownEnable() to enable.
input: timer instance, the callback, the shut down value.
output: return 0 if successful
*/
int TimerCallbackSetShutDownValue(TimerCallbackStruct *timerInstance, TimerCallback callback, uint32_t shutDownValue) {
	uint8_t i = 0;

	while(timerInstance[i].callback != callback) {
		if( i == timerInstance[0].timerLastIndex) {
			return 1;// callback not found
		}
		i++;
	};
	timerInstance[i].timerShutDownEnable = false;
	timerInstance[i].timerShutDownValue = shutDownValue;
	timerInstance[i].timerShutDownCount = 0; // clear shutdown timer
	return 0;
}

/*
function: Call this function from SysTick_Handler() in stm32f1xx_it.c
input: timer instance
output: none
*/
void TimerCallbackTick(TimerCallbackStruct *timerInstance) {
	int i = 0;

	while(i != timerInstance[0].timerLastIndex) { // iterate through all arrays
        if (timerInstance[i].callback != 0)
        {
            if (timerInstance[i].timerShutDownEnable) // check if shutdown is enabled
            {
                timerInstance[i].timerShutDownCount += 1; // increment the timerShutDownCount
            }

            if (timerInstance[i].timerEnabled) // check if callback is enabled.
            {
            	timerInstance[i].timerCount += 1; // increment the timerCount
            }
        }
		i++;
	}
}

/*
function: Checks if timer is reached and jumps to callback function. 
				Will enter and exit on each array pointer increment. This is to reduce doing multiple callback functions in one call.
				Call this function from polling routine.
input: timer instance
output: none
*/
void TimerCallbackCheck(TimerCallbackStruct *timerInstance) {
	int i = 0; // the array pointer

	while(i != timerInstance[0].timerLastIndex) {
	    if(timerInstance[i].timerShutDownEnable == 1) { // check for shutdown first
	        if(timerInstance[i].timerShutDownCount >= timerInstance[i].timerShutDownValue) {
	        	timerInstance[i].timerShutDownCount = 0;
	        	timerInstance[i].timerEnabled = 0; // disable timer
	        }
	    }

		if(timerInstance[i].timerEnabled == 1) {// timer is enable so now check if time is reached
			if(timerInstance[i].timerCount >= timerInstance[i].timerValue) {
				timerInstance[i].timerCount = 0;// clear timer
				timerInstance[i].callback();// jump to callback function
				if(timerInstance[i].timerRepetitionEnable) // new 4-27-2022, has not been tested yet.
				{
				    if(++timerInstance[i].timerRepetitionCount >= timerInstance[i].timerRepetitionValue)
				    {
				        timerInstance[i].timerEnabled = 0; // disable timer
				    }
				}
				if(timerInstance[i].timerRepeat == TIMER_NO_REPEAT) {// if no repeat then disable timer for this function
					timerInstance[i].timerEnabled = 0; // disable timer
				}
				i++;
				return;// A callback function has been called so exit and re-enter at next array pointer. This avoids blocking.
			}
		}
		i++; // iterate until no more array.
	}
	i = 0;// resets to beginning of array
}

/*
function: When a callback is deleted, this will sort and remove any blank callback in between other callbacks in timerCallback array.
input: timer instance
output: none
*/
static void TimerCallbackSort(TimerCallbackStruct *timerInstance) {
	int i = 0;

	while(i != timerInstance[0].timerLastIndex - 1) { // go through all callbacks
		if(timerInstance[i].callback == 0) { // found zero at this index
			if(timerInstance[i+1].callback != 0) { // make sure next index is not zero.
				timerInstance[i].callback = timerInstance[i+1].callback;// shift following callback down one index
				timerInstance[i+1].callback = 0; // now delete callback from this index
			}
		}
		i++;
	}
}


