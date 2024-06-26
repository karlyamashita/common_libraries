#include "LSP.h"
#ifdef USE_TURN_SIGNALS
#include "main.h"
#include "TurnSignals.h"
#include "mTimers.h"


#if defined CHRYSLER_V3

TurnSignalStatus turnSignalStatus;
uint8_t turnSignalStatusTimer = 0; // timer instance

void ServiceTurnSignalStatus(uint8_t *data) {
	turnSignalStatus.Byte.byte0 = data[0];
}

TurnSignalStatus GetTurnSignalStatus
(void) {
	return turnSignalStatus;
}

bool turnSignlActiveFlag = false; // might not need this?
int turnSignalCount = 0;

TurnSignalStatus CheckTurnSignalDoubleClickStatus(void) {

	TurnSignalStatus turnSignalDoubleClickStatus = {0};
	if(turnSignalStatusTimer == 0) {
		turnSignalStatusTimer = CreateTimer(); 
	}
	if(turnSignalCount == 0 && (turnSignalStatus.Status.leftSignal || turnSignalStatus.Status.rightSignal)){
		SetTimer(turnSignalStatusTimer, 0);
		turnSignalCount++;// = 1, indicates first click
		turnSignlActiveFlag = true;
	}	else if(turnSignlActiveFlag == true && turnSignalCount > 0 && turnSignlActiveFlag && (!turnSignalStatus.Status.leftSignal && !turnSignalStatus.Status.rightSignal)){
		turnSignalCount++;// = 2, signals have been release
		turnSignlActiveFlag = false;
	} else if(!turnSignlActiveFlag && turnSignalCount == 2 && (turnSignalStatus.Status.leftSignal || turnSignalStatus.Status.rightSignal)){
		turnSignalCount++;// this should be 3 now, indicates 2 clicks, now check below routine if 2 clicks within 1 second
		turnSignlActiveFlag = true;
	} 
	
	if(GetTimer(turnSignalStatusTimer) >= 1000) { // check if double click within 1 second
		SetTimer(turnSignalStatusTimer, 1000);// just keep timer from over flowing
		if(	turnSignlActiveFlag == true && (turnSignalStatus.Status.leftSignal || turnSignalStatus.Status.rightSignal)){
			if(turnSignalCount >= 3) {
				turnSignalDoubleClickStatus.Byte.byte0 = turnSignalStatus.Byte.byte0; // return the current status
			} else {
				turnSignlActiveFlag = false;// reset
				turnSignalCount = 0;// reset
				turnSignalDoubleClickStatus.Byte.byte0 = 0; // even though signals are on, we want to return signals are off since we didn't see double click
			}
		}	else {
			turnSignlActiveFlag = false;// reset
				turnSignalCount = 0;// reset
		}
	}
	
	return turnSignalDoubleClickStatus;
}

#endif // CHRYSLER_V3
#endif // USE_TURN_SIGNALS
