/*
 * CAN_Activity.c
 *
 *  Created on: Jun 8, 2021
 *      Author: codek
 */

#include "LSP.h"

#ifdef USE_CAN_BUS_ACTIVITY_STATUS
#include "CanBusActivity.h"

/*
 * Description: You can use this to indicate activity like blinking an LED.
 *
 * Input:
 * Output:
 *
 */
void CanBusActivityStatus(uint8_t value){

#ifdef USE_MTIMER_CALLBACK
	// you should have already defined a callback using "GotoSleep" variable or create your own variable and replace in function in next line.
	ClearTimerCallbackTimer(GotoSleep);
#endif// USE_MTIMER_CALLBACK

}






#endif// USE_CAN_BUS_ACTIVITY_STATUS

