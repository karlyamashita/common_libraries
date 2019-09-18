#include "LSP.h"
#if defined USE_RAP

#include "main.h"
#include "Rap.h"
#include "mTimers.h"
#include "Sleep.h"

#ifdef GMLAN_V1

/*
the steps needed to get rap timer working
1. Call SetRapTimerFlag to enable/disable rap timer.
2. call CheckRapStatus in a polling routine
*/

uint8_t rapStatusTimer = 0; // timer instance
bool rapTimerFlag = false;

// call this to set/clear flag
void SetRapTimerFlag(bool flag) {
	static bool timerReseted = false;
	if(rapStatusTimer == 0) {
		rapStatusTimer = CreateTimer();
	}
	if(flag == true && timerReseted == false) {
		SetTimer(rapStatusTimer, 0);// clear timer
		timerReseted = true;
		rapTimerFlag = true;
	} else if(flag == false) {
		timerReseted = false;
		rapTimerFlag = flag;
	}
}	

// call this in polling routine. If time is reached then call rapCallback routine
void CheckRapStatus(uint32_t rapTimer, Callback callback) {
	if(rapTimerFlag != true) return; // if flag is not set then we're not doing rap yet
	if(GetTimer(rapStatusTimer) >= rapTimer) { // wait on specific time then do callback
		callback(true); 
	}
}
#endif // USE_GMLAN_V1

#if defined CHRYSLER_V2

uint8_t rapModeFlag = 0;
void ServiceRapMode(uint8_t *data){
	rapModeFlag = (data[1] & 0x02) >> 1; // byte1, bit1
}

uint8_t GetRapModeFlag(void) {
	return rapModeFlag;
}
#endif // CHRYSLER_V2

#if defined CHRYSLER_V5

uint8_t rapModeFlag = 0;
void ServiceRapMode(uint8_t *data){
	rapModeFlag = data[0] & 0x01; // byte0, bit0
}

uint8_t GetRapModeFlag(void) {
	return rapModeFlag;
}
#endif // CHRYSLER_V5

#endif // USE_RAP

