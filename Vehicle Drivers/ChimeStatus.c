#include "LSP.h"
#ifdef USE_CHIME_STATUS
#include "main.h"
#include "ChimeStatus.h"
#include "CAN.h"
#include "CAN_Buffer.h"
#include "GMLAN_CAN_ID.h"

CanTxMsgTypeDef canMessage;

void SendChimeStatus(bool status) {
	memset(&canMessage, 0, sizeof(canMessage));
	canMessage.ExtId = CHIME_STATUS_ID;    // Set extended ID for transmit message
	canMessage.IDE = CAN_ID_EXT;
	canMessage.RTR = CAN_RTR_DATA;
	canMessage.DLC = 1;
	
	canMessage.Data[0] = status;		
	AddTxBuffer1(&canMessage);
}

#endif // USE_CHIME_STATUS
