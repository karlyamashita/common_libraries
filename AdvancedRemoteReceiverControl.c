#include "LSP.h"
#ifdef USE_ADVANCED_REMOTE_RECEIVER_CONTROL
#include "main.h"
#include "GMLAN_Defines.h"
#include "GMLAN_CAN_ID.h"
#include "AdvancedRemoteReceiverControl.h"
#include "CAN.h"
#include "CAN_Buffer.h"
#include "GMBT.h"
#include "SWC_SendDelayed.h"

AdvancedRemoteReceiverControl advancedRemoteReceiverControl;

void ServiceAdvancedRemoteReceiverControl(uint8_t *data) {
	advancedRemoteReceiverControl.Bytes.byte0 = data[0];
	advancedRemoteReceiverControl.Bytes.byte1 = data[1];
	advancedRemoteReceiverControl.Bytes.byte2 = data[2];
	//todo finish the rest of this
}

bool ServiceAdvancedRemoteReceiverControlAsGateway(uint8_t *data) {
	advancedRemoteReceiverControl.Bytes.byte0 = data[0];
	advancedRemoteReceiverControl.Bytes.byte1 = data[1];
	advancedRemoteReceiverControl.Bytes.byte2 = data[2];

	//todo finish the rest of this
	if(advancedRemoteReceiverControl.AdvancedRemoteReceiverControlBits.headUnitRemoteReceiverControllerTargetDevice == SAT_DIGITAL_AUDIO_RECEIVER_SYSTEM) {
		if(advancedRemoteReceiverControl.AdvancedRemoteReceiverControlBits.remoteReceiverTuneCommand == SCAN_DOWN) {
			if(GetA2DP_Status() == true) {
				SetA2DP_Status(0);// exit BT
				AddSWC_CommandToBufferCAN1(SWC_ARROW_RIGHT, 0); // send preset right now.
				AddSWC_CommandToBufferCAN1(SWC_BUTTON_RELEASE, 100); // send button release command in 200ms
			}	else {
				SetA2DP_Status(1); // enter BT
				AddSWC_CommandToBufferCAN1(SWC_ARROW_RIGHT, 0); // send preset right now.
				AddSWC_CommandToBufferCAN1(SWC_BUTTON_RELEASE, 100); // send button release command in 200ms
			}
			return false;
		} else { // todo check for knob or softbuttons
			if(GetA2DP_Status() == true) {
				
				return false;
			}
		}
	}
	
	return true;
}

void SendAdvancedRemoteReceiverControlCAN2(AdvancedRemoteReceiverControl *advancedRemoteReceiverControl) { 
	CanTxMsgTypeDef canMessage;
	
	canMessage.ExtId = AUDIO_MASTER_ARBITRATION_COMMAND_ID;    // Set extended ID for transmit message
	canMessage.DLC = 3;
	canMessage.IDE = CAN_ID_EXT;
	canMessage.RTR = CAN_RTR_DATA;
	
	canMessage.Data[0] = advancedRemoteReceiverControl->Bytes.byte0;
	canMessage.Data[1] = advancedRemoteReceiverControl->Bytes.byte1;
	canMessage.Data[2] = advancedRemoteReceiverControl->Bytes.byte2;
	
	AddTxBuffer2(&canMessage);
}

#endif // USE_ADVANCED_REMOTE_RECEIVER_CONTROL

