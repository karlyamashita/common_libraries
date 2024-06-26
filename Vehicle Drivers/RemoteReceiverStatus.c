#include "LSP.h"
#ifdef USE_REMOTE_RECEIVER_STATUS
#include "main.h"
#include "RemoteReceiverStatus.h"
#include "CAN.h"
#include "CAN_Buffer.h"
#include "GMLAN_defines.h"
#include "GMLAN_CAN_ID.h"


void SendRemoteReceiverStatus(RemoteReceiverStatus *remoteReceiverStatus) {

	CanTxMsgTypeDef canMessage;
	canMessage.ExtId = REMOTE_RECEIVER_STATUS_ID;	
  canMessage.IDE = CAN_ID_EXT;
	canMessage.RTR = CAN_RTR_DATA;										
	canMessage.DLC = 8;

	canMessage.Data[0] = remoteReceiverStatus->Bytes.byte0;
	canMessage.Data[1] = remoteReceiverStatus->Bytes.byte1;
	canMessage.Data[2] = remoteReceiverStatus->Bytes.byte2;
	canMessage.Data[3] = remoteReceiverStatus->Bytes.byte3;
	canMessage.Data[4] = remoteReceiverStatus->Bytes.byte4;
	canMessage.Data[5] = remoteReceiverStatus->Bytes.byte5;
	canMessage.Data[6] = remoteReceiverStatus->Bytes.byte6;
	canMessage.Data[7] = remoteReceiverStatus->Bytes.byte7;
	
	AddTxBuffer1(&canMessage);
}

void SendRemoteReceiverStatusFixedData(void) {
	RemoteReceiverStatus remoteReceiverStatus;
	
	remoteReceiverStatus.Status.currentStationPresetNumber = NOT_A_PRESET;
	remoteReceiverStatus.Status.currentStationService = GENERAL_SUBSCRIPTION;
	remoteReceiverStatus.Status.currentStationQuality = SUFFICIANT_SIGNAL;
	remoteReceiverStatus.Status.receiverDataType = AUDIO;
	remoteReceiverStatus.Status.receiverCommandStatus = VALID_COMMAND;
	remoteReceiverStatus.Status.receiverInitialinzingActive = false;
	remoteReceiverStatus.Status.receiverInformationAvailableActive  = false;
	remoteReceiverStatus.Status.currentTunedStation = 1;
	remoteReceiverStatus.Status.receiverScanningStatus = NO_ACTION;
	remoteReceiverStatus.Status.receiverEmergencyInterruptStatus = NO_ACTION;
	remoteReceiverStatus.Status.receiverTuningStatus = TUNED;
	remoteReceiverStatus.Status.receiverSeekingStatus = NO_ACTION;
	remoteReceiverStatus.Status.receiverTrafficInterruptStatus = NO_ACTION;
	remoteReceiverStatus.Status.receiverType = SAT_DIGITAL_AUDIO_RECEIVER_SYSTEM;	
	remoteReceiverStatus.Status.currentAudioProgramServiceIdentifier = 100;
	remoteReceiverStatus.Status.serviceProvider = XM_COMPANY;
	
	SendRemoteReceiverStatus(&remoteReceiverStatus);
}
#endif // USE_REMOTE_RECEIVER_STATUS
