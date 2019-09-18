#include "LSP.h"
#ifdef USE_AUDIO_SOURCE_STATUS
#include "main.h"
#include "AudioSourceStatus.h"
#include "AudioMasterCommand.h"
#include "GMLAN_Defines.h"
#include "GMLAN_CAN_ID.h"
#include "can.h"
#include "CAN_Buffer.h"
#include "mTimers.h"
#include "GMBT.h"

/*

OnStar and XM routines

*/

uint8_t AudioSourceStatusTimer = 0;


// XM AudioSourceStatus
void ServiceAudioSourceStatusXM(unsigned char *data) {
	AudioSourceStatus audioSourceStatus;
	AudioMasterArbitrationCommand audioMasterArbitrationCommand;
	audioSourceStatus.Bytes.byte0 = data[0];
	audioSourceStatus.Bytes.byte1 = data[1];
	if(audioSourceStatus.Status.sourceType == SATELLITE_DIGITAL_AUDIO_RECEIVER) {
		audioMasterArbitrationCommand.Status.channelType = STEREO;
		audioMasterArbitrationCommand.Status.sourceType = SATELLITE_DIGITAL_AUDIO_RECEIVER;
		switch(audioSourceStatus.Status.statusCode) {
			case AUDIO_SOURCE_AUDIO_AVAILABLE:
				audioMasterArbitrationCommand.Status.command = AUDIO_MASTER_ARB_CONNECT;
				SendAudioMasterArbitrationCommandCAN2(&audioMasterArbitrationCommand);
			break;
			case AUDIO_SOURCE_AUDIO_NOT_AVAILABLE:
				audioMasterArbitrationCommand.Status.command = AUDIO_MASTER_ARB_DISCONNECT;
				SendAudioMasterArbitrationCommandCAN2(&audioMasterArbitrationCommand);
			break;
			case AUDIO_SOURCE_PRODUCING_SILENCE:
				audioMasterArbitrationCommand.Status.command = AUDIO_MASTER_ARB_CONNECTION_COMPLETE;
				SendAudioMasterArbitrationCommandCAN2(&audioMasterArbitrationCommand);
			break;
			case AUDIO_SOURCE_PRODUCING_AUDIO:
				
			break;
			case AUDIO_SOURCE_DEVICE_PRESENT:
				
			break;
		}
	} 
}

// OnStar AudioSourceStatus
void ServiceAudioSourceStatusOnStar(unsigned char *data) {
	AudioSourceStatus audioSourceStatus;
	AudioMasterArbitrationCommand audioMasterArbitrationCommand;
	audioSourceStatus.Bytes.byte0 = data[0];
	audioSourceStatus.Bytes.byte1 = data[1];
	if(audioSourceStatus.Status.sourceType == PHONE) {
		audioMasterArbitrationCommand.Status.channelType = MONO;
		audioMasterArbitrationCommand.Status.sourceType = PHONE;
		switch(audioSourceStatus.Status.statusCode) {
			case AUDIO_SOURCE_AUDIO_AVAILABLE:			
				audioMasterArbitrationCommand.Status.command = AUDIO_MASTER_ARB_CONNECT;
				SendAudioMasterArbitrationCommandCAN2(&audioMasterArbitrationCommand);
			break;
			case AUDIO_SOURCE_AUDIO_NOT_AVAILABLE:
				audioMasterArbitrationCommand.Status.command = AUDIO_MASTER_ARB_DISCONNECT;
				SendAudioMasterArbitrationCommandCAN2(&audioMasterArbitrationCommand);
			break;
			case AUDIO_SOURCE_PRODUCING_SILENCE:
				audioMasterArbitrationCommand.Status.command = AUDIO_MASTER_ARB_CONNECTION_COMPLETE;
				SendAudioMasterArbitrationCommandCAN2(&audioMasterArbitrationCommand);
			break;
			case AUDIO_SOURCE_PRODUCING_AUDIO:
				
			break;
			case AUDIO_SOURCE_DEVICE_PRESENT:
				
			break;
		}
	} 
}

/*
function: poll to send AudioSourceStatus soh after time is reached like 1.2 seconds.
input: The soh time and the source type
output: none
*/
void SendAudioSourceStatusSoh(uint8_t sourceType, uint32_t sohTimer) {
	AudioSourceStatus audioSourceStatus;
	if(AudioSourceStatusTimer == 0) {
		AudioSourceStatusTimer = CreateTimer();
	}
	if(GetTimer(AudioSourceStatusTimer) >= sohTimer) {
		SetTimer(AudioSourceStatusTimer, 0); // reset timer
		audioSourceStatus.Status.sourceType = sourceType;
		audioSourceStatus.Status.channelType = NO_ACTION;
		audioSourceStatus.Status.statusCode = AUDIO_SOURCE_DEVICE_PRESENT;
		SendAudioSourceStatusCAN1(AUDIO_SOURCE_STATUS_XM_ID, &audioSourceStatus);
	}
}

// routines to send as a AudioSourceStatus
void SendAudioSourceStatusCAN1(uint32_t canID, AudioSourceStatus *audioSourceStatus) {
	CanTxMsgTypeDef canMessage;
	
	canMessage.ExtId = canID;    // Set extended ID for transmit message
	canMessage.DLC = 2;
	canMessage.IDE = CAN_ID_EXT;
	canMessage.RTR = CAN_RTR_DATA;
	canMessage.Data[0] = audioSourceStatus->Bytes.byte0;
	canMessage.Data[1] = audioSourceStatus->Bytes.byte1;
	AddTxBuffer1(&canMessage);
}

#endif // USE_AUDIO_SOURCE_STATUS

