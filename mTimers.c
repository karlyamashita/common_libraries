#include "LSP.h"
#ifdef USE_MTIMERS
#include "main.h"
#include "mTimers.h"

#define MAX_TIMERS 16 // increase if more timers are needed

volatile uint32_t genericTimer[MAX_TIMERS] = {0}; // array that holds timer values for each timer instance
volatile uint8_t timerInstance = 0; // timer instance

/*
function: Declare an uint8_t variable in file and save the pointer value called from this function.
					This variable will be used to GetTimer or SetTimer
input: none
output: the new timer instance pointer
*/
uint8_t CreateTimer(void) {
	if(timerInstance == MAX_TIMERS) {
			printf("Maximum timers reached!");
		return 0; // added return 0; 10-10-17
	}
	++timerInstance;
	genericTimer[timerInstance] = 0;// clear the timer
	return timerInstance;
}

/*
function: If not already called, then call this function from SysTick_Handler() in stm32f1xx_it.c
input: none
output: none
*/
void HAL_SYSTICK_Callback(void) {
	uint8_t i;
	if(timerInstance == 0) return;   // no timers create so return
	for(i = 1; i < timerInstance + 1; i++) { // increment all timer instances
			genericTimer[i]++;
	}
}

// return the timer value
uint32_t GetTimer(uint8_t timer) {
    return genericTimer[timer];
}
// set the timer to a value
void SetTimer(uint8_t timer, uint32_t value) {
    genericTimer[timer] = value;
}

#endif // USE_MTIMERS
