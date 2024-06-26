#include "LSP.h"
#ifdef USE_NCA_FROM_SDAR
#include "main.h"
#include "NCA_From_SDAR.h"
#include "CAN.h"
#include "CAN_Buffer.h"
#include "GMLAN_CAN_ID.h"
#include "mTimers.h"


uint8_t NCA_From_SDAR_Timer = 0;// timer instance
bool sdarPresentFlag = false;

/*
function: set a flag when receiving a CAN message. There is no data to be passed.
input: none
output: none
*/
void SetNCA_From_SDAR_PresentFlag(bool status) {
	sdarPresentFlag = status;
}

bool GetNCA_From_SDAR_PresentFlag(void) {
	return sdarPresentFlag;
}

/*
function: Send NCA_From_SDAR ID with delay. You can call this in a polling routine with a delay like 1.2seconds
					or respond to Radio's NCA with no delay
input: the delay time. 0 = send right away.
output: none
*/
void SendNCA_FROM_SDAR(uint32_t delay) {
	if(NCA_From_SDAR_Timer == 0) {
		NCA_From_SDAR_Timer = CreateTimer();
	}
	CanTxMsgTypeDef canMessage;

	if(GetTimer(NCA_From_SDAR_Timer) >= delay) {
		SetTimer(NCA_From_SDAR_Timer, 0);// clear timer for next transmission
		canMessage.ExtId = NCA_FROM_SDAR_ID;    // Set extended ID for transmit message
		canMessage.DLC = 0;
		canMessage.IDE = CAN_ID_EXT;
		canMessage.RTR = CAN_RTR_DATA;
		AddTxBuffer1(&canMessage);
	}
}


/*
function: if no XM present then emulate the data so radio is happy
input: none
output: none
*/
void Emulate_NCA_From_SDAR(void) {
	if(sdarPresentFlag == true) return;// we don't need to emulate
	SendNCA_FROM_SDAR(0); // send with no delay
}
#endif // USE_NCA_FROM_SDAR
