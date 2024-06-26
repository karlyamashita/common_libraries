#ifdef USE_AUDIO_SOURCE_STATUS
#ifndef AUDIO_SOURCE_STATUS_H
#define AUDIO_SOURCE_STATUS_H

#include "main.h"
#include "GMLAN_Defines.h"
#include "can.h"


typedef union AudioSourceStatus
{
	struct{
		unsigned char byte1;
		unsigned char byte0;
	}Bytes;
	struct{
		unsigned statusCode:4;
		unsigned channelType:3;
		unsigned sourceType:5;
		unsigned :4;
	}Status;
}AudioSourceStatus;

void ServiceAudioSourceStatusXM(unsigned char *data);

void ServiceAudioSourceStatusOnStar(unsigned char *data);

void SendAudioSourceStatusSoh(uint8_t sourceType, uint32_t sohTimer);

void SendAudioSourceStatusCAN1(uint32_t canID, AudioSourceStatus *audioSourceStatus);

#endif // AUDIO_SOURCE_STATUS_H

#endif // USE_AUDIO_SOURCE_STATUS
