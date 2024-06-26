#include "LSP.h"
#ifdef USE_CHIME_COMMAND
#include "main.h"
#include "ChimeCommand.h"
#include "CAN.h"
#include "CAN_Buffer.h"
#include "GMLAN_CAN_ID.h"
#include "GMLAN_Defines.h"


ChimeCommand chimeCommand;

void ServiceChimeCommand(uint8_t *data) {
	chimeCommand.Bytes.byte0 = data[0];
	chimeCommand.Bytes.byte1 = data[1];
	chimeCommand.Bytes.byte2 = data[2];
	chimeCommand.Bytes.byte3 = data[3];
	chimeCommand.Bytes.byte4 = data[4];
}

void CheckChimeCommand(void) {
	
	
	
}




void SendChimeCommand(ChimeCommand *chimeCommand) {
	CanTxMsgTypeDef chimeCanMessage;
	memset(&chimeCanMessage, 0, sizeof(chimeCanMessage));
	chimeCanMessage.ExtId = CHIME_COMMAND_IRC_ID;    // Set extended ID for transmit message
	chimeCanMessage.IDE = CAN_ID_EXT;
	chimeCanMessage.RTR = CAN_RTR_DATA;
	chimeCanMessage.DLC = 5;
	chimeCanMessage.Data[0] = chimeCommand->Bytes.byte0;
	chimeCanMessage.Data[1] = chimeCommand->Bytes.byte1;
	chimeCanMessage.Data[2] = chimeCommand->Bytes.byte2;
	chimeCanMessage.Data[3] = chimeCommand->Bytes.byte3;
	chimeCanMessage.Data[4] = chimeCommand->Bytes.byte4;
	
	AddTxBuffer2(&chimeCanMessage);
}

void SendChimeCommandPreset(uint8_t type, uint8_t repetition) {
	// todo make preset chimes to send
	/*
#define CLOCK 0
#define CLACK 1
#define CLICK 2
#define CLICK_CLACK 3
#define BEEP_750 4
#define BEEP_2000 5
#define GONG_750 6
#define GONG_2000 7
#define HARP 8
#define HIGH_FREQUENCY_BROADBAND 9
*/
	
	ChimeCommand chimeCanMessage;
	
	memset(&chimeCanMessage, 0, sizeof(chimeCanMessage));
	switch(type) {
		case BEEP_750:
			
		break;
		case BEEP_2000:
			chimeCanMessage.Chime.sC_SndCdnPrd = 30; //  300ms = 30, 200ms = 20
			chimeCanMessage.Chime.sC_SndDutCyc = 50;
		break;
		case GONG_750:
			
		break;
		case GONG_2000:
			
		break;
		case CLICK_CLACK:
			
		break;
		default:
			return;
	}

	chimeCanMessage.Chime.driverFront = true;
	chimeCanMessage.Chime.sC_NmofRp = repetition;
	chimeCanMessage.Chime.sC_SndPriority = 120;
	chimeCanMessage.Chime.sC_SndTne = type;
	
	SendChimeCommand(&chimeCanMessage);
}

#endif // USE_CHIME_COMMAND

