#ifdef USE_AUDIO_MASTER_COMMAND
#ifndef AUDIO_MASTER_ARBITRATION_COMMAND_H
#define AUDIO_MASTER_ARBITRATION_COMMAND_H

#include <stdbool.h>
#include "can.h"
#include "main.h"

typedef union AudioMasterArbCommand{
	struct{
		unsigned char byte1;
		unsigned char byte0;
	}Bytes;
	struct{
		unsigned sourceType:5;
		unsigned :3;
		unsigned channelType:3;
		unsigned command:4;
		unsigned :1;
	}Status;
}AudioMasterArbitrationCommand;

void SendAudioMasterArbitrationCommandSoh(uint32_t sohTimer);
void SendAudioMasterArbitrationCommandCAN2(AudioMasterArbitrationCommand *audioMasterArbitrationCommand);

bool ServiceAudioMasterArbitrationCommandAsGateway(uint8_t *data);


#endif // AUDIO_MASTER_ARBITRATION_COMMAND_H

#endif // USE_AUDIO_MASTER_COMMAND

