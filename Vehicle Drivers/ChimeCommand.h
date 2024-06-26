#ifndef CHIME_COMMAND_H
#define CHIME_COMMAND_H

typedef union ChimeCommand{
	struct{
		uint8_t byte4;
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct{
		uint8_t sC_SndPriority;
		uint8_t sC_SndDutCyc;
		uint8_t sC_NmofRp;
		uint8_t sC_SndCdnPrd;
		unsigned sC_SndTne:4;
		unsigned rightRear:1;
		unsigned leftRear:1;
		unsigned passengerFront:1;
		unsigned driverFront:1;
	}Chime;
}ChimeCommand;
#define chimeArrayChime ChimeCommand.chimeArray
#define sC_SoundToneChime ChimeCommand.Chime.sC_SndTone
#define rightRearChime ChimeCommand.Chime.rightRear
#define leftRearChime ChimeCommand.Chime.leftRear
#define passengerFrontChime ChimeCommand.Chime.passengerFront
#define driverFrontChime ChimeCommand.Chime.driverFront
#define sC_TimeChime ChimeCommand.Chime.sC_SndCdnPrd
#define sC_NumberOfRepetitionChime ChimeCommand.Chime.sC_NmofRp
#define sC_DutyCycleChime ChimeCommand.Chime.sC_SndDutCyc
#define sC_PriorityChime ChimeCommand.Chime.sC_SndPriority



void ServiceChimeCommand(uint8_t *data);
void SendChimeCommand(ChimeCommand *chimeCommand);
void SendChimeCommandPreset(uint8_t type, uint8_t repetition);




#endif // CHIME_COMMAND_H
