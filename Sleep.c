#include "LSP.h"
#include "main.h"
#ifdef USE_SLEEP

#include "Sleep.h"
#include "mTimers.h"
#include "gpio.h"
#include "GPIO_Ports.h"

#if defined GMLAN_V1
#include "CAN_TransceiverGMLAN.h"
#endif
#if defined MS_CAN
#include "CAN_TransceiverMS.h"
#endif


uint8_t sleepTimer = 0; // timer instance

void SleepCheck(uint32_t theSleepTime, Callback callback) {
	if(sleepTimer == 0) {
		sleepTimer = CreateTimer();
	}
	if(GetTimer(sleepTimer) >= theSleepTime) {
		callback(1);
	}
}

// call this in CAN receive interrupt
void SleepReset(void) {
	if(sleepTimer == 0) return; // no timer created
	SetTimer(sleepTimer, 0);// clear timer		
}

/*

Here are some example call back routines to use in PollingRoutines. One is for single CAN transceiver and the other is for Dual CAN transcivers

void GotoSleepCallback(int status) { // Single CAN Transceiver
	if(status) { // if status is true then go to sleep
// todo turn off other stuff

		
// do not modify code below
		PortB_Off(LED1_Blue_Pin);
		PortB_Off(LED2_Red_Pin);
		CAN_TransceiverSleep();
		__WFI(); // todo wakes up from unknown interrupt, need to fix
		InitCAN_Transceiver();
	}
}

void GotoSleepCallback(int status) { // Dual CAN transceivers
	if(status) { // if status is true then go to sleep
// todo turn off other stuff		
		
		
// do not modify code below
		PortC_Off(LED1_Blue_Pin);
		PortC_Off(LED2_Red_Pin);
		DualCanTransceiverSleep();
		__WFI(); // todo wakes up from unknown interrupt, need to fix
		InitDualCanTransceiver();
	}
}
*/

#endif // USE_SLEEP

