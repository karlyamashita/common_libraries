#include "LSP.h"
#ifdef USE_MIRROR_MOVEMENT_REQUEST
#include "main.h"
#include "MirrorMovementRequest.h"
#include "GMLAN_Defines.h"
#include "mTimers.h"

uint8_t MirrorMovementRequestTimer = 0; // timer instance
bool mirrorMovementRequestActiveFlag = false;
bool mirrorButtonReleased = true;

MirrorMovementRequest mirrorMovementRequest; // the data from the vehicle
#define mirrorSwitchActivationUpDown mirrorMovementRequest.Status.MirrorSwitchActivationUpDown 
#define mirrorSwitchActivationLeftRight mirrorMovementRequest.Status.MirrorSwitchActivationLeftRight 

MirrorStatus mirrorStatus; // the status of buttons if active/enactive after a delay

void ServiceMirrorMovementRequest(uint8_t *data) {
	if(MirrorMovementRequestTimer == 0) {
		MirrorMovementRequestTimer = CreateTimer();
	}
	
	mirrorMovementRequest.ByteArray.byte0 = data[0];
	mirrorMovementRequest.ByteArray.byte1 = data[1];
	
	if(mirrorMovementRequest.Status.MirrorSelectSwitchStatus == NO_MIRROR_SELECTED) {
		if((mirrorSwitchActivationUpDown == MIRROR_UP || mirrorSwitchActivationUpDown == MIRROR_DOWN) && mirrorButtonReleased) {
			SetTimer(MirrorMovementRequestTimer, 0);// clear timer
			mirrorMovementRequestActiveFlag = true;
			mirrorButtonReleased = false;
		}
		else if(mirrorSwitchActivationUpDown == NO_ACTIVATION) {
			mirrorMovementRequestActiveFlag = false;
			mirrorButtonReleased = true;
		}
	} 
}

/*
function: if mirrorMovementRequestActiveFlag is true then check for delay.
					If delay is reached while mirrorMovementRequestActiveFlag is set, then set/clear mirrorStatus flag for that mirror.
					Clear mirrorMovementRequestActiveFlag afterwards so polling doesn't run it again.
input: the delay time before setting mirrorStatus active flag
output: none
*/
void CheckMirrorMovementRequest(uint32_t delay) {
	if(mirrorMovementRequestActiveFlag == true) {
		if(GetTimer(MirrorMovementRequestTimer) >= delay) {
			mirrorMovementRequestActiveFlag = false; // clear flag
			switch(mirrorSwitchActivationUpDown) {
				case MIRROR_UP:
					mirrorStatus.mirrorUpActive ^= 1;
				break;
				case MIRROR_DOWN:
					mirrorStatus.mirrorDownActive ^= 1;
				break;
				default:
					break;
			}
			switch(mirrorSwitchActivationLeftRight) {
				case MIRROR_LEFT:
					mirrorStatus.mirrorLeftActive ^= 1;
				break;
				case MIRROR_RIGHT:
					mirrorStatus.mirrorRightActive ^= 1;
				break;
				default:
				break;
			}
		}
	}
}

/*
function: Get the status of the mirror buttons if active or not
			When getting the status be sure to create a new structure variable to hold the structure returned
input: none
output: mirror status structure
*/
MirrorStatus GetMirrorStatusActiveFlag(void) {
	return mirrorStatus;
}

#endif // USE_MIRROR_MOVEMENT_REQUEST

