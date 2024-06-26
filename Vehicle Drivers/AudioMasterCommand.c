#include "LSP.h"
#ifdef USE_AUDIO_MASTER_COMMAND
#include "main.h"
#include "AudioMasterCommand.h"
#include "AudioSourceStatus.h"
#include "GMLAN_Defines.h"
#include "GMLAN_CAN_ID.h"
#include "CAN.h"
#include "CAN_Buffer.h"
#include "mTimers.h"
#include "GMBT.h"

uint8_t AudioMasterArbitrationCommandTimer = 0; // timer instance


/*
function: poll to send AudioMasterArbitrationCommand after time is reached like 1.2 seconds.
input: The soh time
output: none
*/
void SendAudioMasterArbitrationCommandsoh(uint32_t sohTimer) {
	AudioMasterArbitrationCommand audioMasterArbitrationCommand;
	if(AudioMasterArbitrationCommandTimer == 0) {
		AudioMasterArbitrationCommandTimer = CreateTimer();
	}
	if(GetTimer(AudioMasterArbitrationCommandTimer) >= sohTimer) {
		SetTimer(AudioMasterArbitrationCommandTimer, 0); // reset timer
		audioMasterArbitrationCommand.Status.command = AUDIO_MASTER_ARB_NO_ACTION;
		audioMasterArbitrationCommand.Status.channelType = NO_CHANNEL;
		audioMasterArbitrationCommand.Status.sourceType = AUDIO_SUPERVISOR;
		SendAudioMasterArbitrationCommandCAN2(&audioMasterArbitrationCommand);
	}
}

void SendAudioMasterArbitrationCommandCAN2(AudioMasterArbitrationCommand *audioMasterArbitrationCommand){
	CanTxMsgTypeDef canMessage;
	
	canMessage.ExtId = AUDIO_MASTER_ARBITRATION_COMMAND_ID;    // Set extended ID for transmit message
	canMessage.DLC = 2;
	canMessage.IDE = CAN_ID_EXT;
	canMessage.RTR = CAN_RTR_DATA;
	canMessage.Data[0] = audioMasterArbitrationCommand->Bytes.byte0;
	canMessage.Data[1] = audioMasterArbitrationCommand->Bytes.byte1;
	AddTxBuffer2(&canMessage);
}
	
#ifndef AUDIO_SOURCE_STATUS_H
#warning "Be sure to include AudioSourceStatus.c file"
#endif
		
// AudioMasterArbitrationCommand
void ServiceAudioMasterArbitrationCommand(uint8_t *data) {
	AudioMasterArbitrationCommand audioMasterArbitrationCommand;
	AudioSourceStatus audioSourceStatus;
	audioMasterArbitrationCommand.Bytes.byte0 = data[0];
	audioMasterArbitrationCommand.Bytes.byte1 = data[1];
	
	switch(audioMasterArbitrationCommand.Status.sourceType) {
		case SATELLITE_DIGITAL_AUDIO_RECEIVER:
			switch(audioMasterArbitrationCommand.Status.command) {
				case AUDIO_MASTER_ARB_CONNECT:
					audioSourceStatus.Status.sourceType = SATELLITE_DIGITAL_AUDIO_RECEIVER;
					audioSourceStatus.Status.channelType = STEREO;
					audioSourceStatus.Status.statusCode = AUDIO_SOURCE_PRODUCING_SILENCE;
					SendAudioSourceStatusCAN1(AUDIO_SOURCE_STATUS_XM_ID, &audioSourceStatus);
				break;
				case AUDIO_MASTER_ARB_DISCONNECT:
					audioSourceStatus.Status.sourceType = SATELLITE_DIGITAL_AUDIO_RECEIVER;
					audioSourceStatus.Status.channelType = STEREO;
					audioSourceStatus.Status.statusCode = AUDIO_SOURCE_DEVICE_PRESENT;
					SendAudioSourceStatusCAN1(AUDIO_SOURCE_STATUS_XM_ID, &audioSourceStatus);
				break;
				case AUDIO_MASTER_ARB_CONNECTION_COMPLETE:
					audioSourceStatus.Status.sourceType = SATELLITE_DIGITAL_AUDIO_RECEIVER;
					audioSourceStatus.Status.channelType = STEREO;
					audioSourceStatus.Status.statusCode = AUDIO_SOURCE_PRODUCING_AUDIO;
					SendAudioSourceStatusCAN1(AUDIO_SOURCE_STATUS_XM_ID, &audioSourceStatus);
				break;
				case AUDIO_MASTER_ARB_DO_NOT_CONNECT:
					
				break;
				case AUDIO_MASTER_ARB_NO_ACTION:
					
				break;
			}
		break; // SATELLITE_DIGITAL_AUDIO_RECEIVER
		case PHONE:
					
		break; // PHONE
	}
}

#endif // USE_AUDIO_MASTER_COMMAND

