#include "LSP.h"
#ifdef USE_RADIO_FUNCTIONS
#include "main.h"
#include "RadioFunctions.h"
#include "Clarion.h"
#include "Kenwood.h"
#include "Alpine.h"
#include "JVC.h"


uint8_t swcRadioTable[4][16] = {
	{ALPINE_VOLUME_UP, ALPINE_VOLUME_DOWN, ALPINE_MUTE, ALPINE_SOURCE},
	{CLARION_VOLUME_UP, CLARION_VOLUME_DOWN, CLARION_MUTE, CLARION_SOURCE},
	{KENWOOD_VOLUME_UP, KENWOOD_VOLUME_DOWN, KENWOOD_ATT, KENWOOD_SOURCE},
	{JVC_VOLUME_UP, JVC_VOLUME_DOWN, JVC_ATT, JVC_SOURCE},
};

void RadioCommands(uint8_t command){
	// todo get radio type
	switch(2) { // radio type
		case 0:
			AlpineCommand(command);
		break;
		case 1:
		//	ClarionCommand(command);
		break;
		case 2:
			KenwoodCommand(command);
		break;
		case 3:
			JVC_Command(command);
		break;
		case 4:
			//PioneerCommand(command);
		break;
		case 5:
			//SonyCommand(command);
		break;
		case 6:
			//Sony2Command(command);
		break;
		default:
			break;
	}
	// todo delay 108 ms between commands
}
#endif // USE_RADIO_FUNCTIONS
