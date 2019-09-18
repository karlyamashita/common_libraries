#include "LSP.h"
#ifdef USE_INFOTAINMENT_ALLOWED
#include "main.h"
#include "InfotainmentAllowed.h"
#include "GMLAN_Defines.h"
#include "GMLAN_CAN_ID.h"
#include "CAN.h"
#include "CAN_Buffer.h"
#include "mTimers.h"
#include "Sleep.h"


uint8_t infotainmentAllowedStatus; // when looking for status, else we're emulating radio so we would be sending status.

// gateway: typically this is called if we're a gateway receiving from radio
void ServiceInfotainmentAllowed(unsigned char *data) {
	infotainmentAllowedStatus = data[0] & 0x01;
}

bool GetInfotainmentAllowedStatus(void) {
	return infotainmentAllowedStatus;
}
// end gateway


// radio:
// typically this is called if interface is replacing headunit
void SendInfotainmentAllowedCallbackCAN2(int status) {
	CanTxMsgTypeDef canMessage;
	canMessage.ExtId = INFOTAINMENT_ALLOWED_ID;    // Set extended ID for transmit message
	canMessage.IDE = CAN_ID_EXT;
	canMessage.RTR = CAN_RTR_DATA;
	canMessage.DLC = 1;
	canMessage.Data[0] = status;
	AddTxBuffer2(&canMessage);
}

// call this when PowerMode is on or about to go to sleep
bool sentInfotainmentAllowed = false;
void SendInfotainmentAllowed(bool flag) {
	if(sentInfotainmentAllowed == true) {
		sentInfotainmentAllowed = false;
		SendInfotainmentAllowedCallbackCAN2(flag);
	} else {
		sentInfotainmentAllowed = true;
		SendInfotainmentAllowedCallbackCAN2(flag);
	}
}

#endif // USE_INFOTAINMENT_ALLOWED


