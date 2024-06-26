#include "LSP.h"
#ifdef USE_NCA_INTEGRATED_RADIO_CHASSIS
#include "main.h"
#include "NCA_IntegratedRadioChassis.h"
#include "GMLAN_CAN_ID.h"
#include "CAN.h"
#include "CAN_Buffer.h"

void SendNCA_IntegratedRadioChassis(void) {
	CanTxMsgTypeDef canMessage;
	canMessage.ExtId = NCA_INTEGRATED_RADIO_CHASSIS_ID;    // Set extended ID for transmit message
	canMessage.IDE = CAN_ID_EXT;
	canMessage.RTR = CAN_RTR_DATA;
	canMessage.DLC = 0;
	AddMessageToCAN_Buffer2(&canMessage);
}

#endif// USE_NCA_INTEGRATED_RADIO_CHASSIS
