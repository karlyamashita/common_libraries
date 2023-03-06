/*
 *  Description: This sets up a generic timer that you can register.
 *              Use TimerDelayGetValue(<timer instance>) to get that timer instance value.
 *              You can test the return value to see if the value is >= to a certain value like 1000 for 1 second.
 *
 *
 *
 */

#include "main.h"
#include "TimerDelay.h"


#define MAX_TIMERS 8 // increase if more timers are needed

volatile uint32_t genericTimer[MAX_TIMERS] = {0}; // array that holds timer values for each timer instance
volatile uint8_t timerInstance = 0; // timer instance

/*
function: Declare an uint8_t variable in your code and save the pointer value called from this function.
					This variable will be used to GetTimer or SetTimer
					e.g. uint8_t myTimer = TimerDelayRegister();
						TimerDelaySetValue(myTimer, 0);
					Note: Currently there is no function to unregister a timer.
input: none
output: none
return: the new timer instance pointer
*/
uint8_t TimerDelayRegister(void) {
	if(timerInstance == MAX_TIMERS) {
			printf("Maximum timers reached!");
		return 0; // added return 0; 10-10-17
	}
	++timerInstance;
	genericTimer[timerInstance] = 0;// clear the timer
	return timerInstance;
}

/*
function: Call this function from SysTick_Handler() in stm32f1xx_it.c
			SysTick is typically a 1ms tick.
input: none
output: none
*/
void TimerDelaySYSTICK(void) {
	uint8_t i;
	if(timerInstance == 0) return;   // no timers create so return
	for(i = 1; i < timerInstance + 1; i++) { // increment all timer instances
			genericTimer[i]++;
	}
}

/*
function: Call this function to return the timer value
input: The timer instance
output: none
*/
uint32_t TimerDelayGetValue(uint8_t timer) {
    return genericTimer[timer];
}

/*
function: Set the timer instance to a value, usually you would reset the value to zero.
input: The timer instance and a value
output: none
*/
void TimerDelaySetValue(uint8_t timer, uint32_t value) {
    genericTimer[timer] = value;
}

