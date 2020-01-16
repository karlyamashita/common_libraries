/*
 * CanBusActivity.c
 *
 *  Created on: Jan 15, 2020
 *      Author: Karl
 */
#include "LSP.h"
#ifdef USE_CAN_BUS_ACTIVITY_STATUS
#include "main.h"
#include "CanBusActivity.h"

uint8_t canBusActive;

/*
 * function: sets var to indicate CAN bus activity
 * input: 1 = activity
 * output: none
 */
void CanBusActivityStatus(uint8_t status){
	canBusActive = status;
}

/*
 * function: check if CAN bus has activity. You can call this function to get status and then create a routine to flash an LED.
 * input: none
 * output: 1 if activity, 0 if no activity
 */
uint8_t GetCanBusActivityStatus(void)
{
	if(canBusActive == 1) {
		canBusActive = 0;
		return 1;
	}
	return 0;
}
#endif // USE_CAN_BUS_ACTIVITY_STATUS
