#include "LSP.h"
#ifdef USE_CLARION
#include "main.h"
#include "RadioFunctions.h"
#include "Clarion.h"
#include "gpio_ports.h"

void ClarionStartBit(void){
	PortB_Off(SWC_Pin); // pull low
	// delay 9 ms
	PortB_On(SWC_Pin); // set high
	// delay 4.5 ms
}

void ClarionBit(unsigned char bit){
	PortB_Off(SWC_Pin); // pull low
	// delay 560us
	PortB_On(SWC_Pin); // set high
	if(bit == 0){
		// delay 560us
	} else {
		// delay 1.69ms
	}
}

void ClarionSendCommand(unsigned char command){
	unsigned char i;
	for(i = 0; i < 8; i++){
		if((command & 0x01) == 1)
		{
			ClarionBit(1);
		}
		else
		{
			ClarionBit(0);
		}
		command >>= 1;
	}	
}

void Clarion(unsigned char command, unsigned int customCode2Flag){
	unsigned int customeCode = CLARION_CUSTOM_CODE;
	if(customCode2Flag) {
		customeCode = CLARION_CUSTOM_CODE_2;
	}
	ClarionStartBit();
	ClarionSendCommand(customeCode >> 8);	// send custom high byte first
	ClarionSendCommand( (unsigned char) customeCode); // send custom low byte
	ClarionSendCommand(command); // send command
	command = ~command;// invert command
	ClarionSendCommand(command); // send inverted command
	ClarionBit(1); // need to send for older radios
}

void ClarionCommand(unsigned char command) {
	switch(command){
		case 0:
			break;
		case 1:Clarion(CLARION_VOLUME_UP, NULL);
			break;
		case 2:Clarion(CLARION_VOLUME_DOWN, NULL);
			break;
		case 3:Clarion(CLARION_MUTE, NULL);
			break;
		case 4:Clarion(CLARION_SOURCE, NULL);
			break;
		case 5:Clarion(CLARION_TRACK_UP, NULL);
			break;
		case 6:Clarion(CLARION_TRACK_DOWN, NULL);
			break;
		case 7:Clarion(CLARION_BAND, NULL);
			break;
		case 8:Clarion(CLARION_SEND_END, NULL);
			break;
		case 9:Clarion(CLARION_SEND, CLARION_CUSTOM_CODE_TRUE);
			break;
		case 10:Clarion(CLARION_END, CLARION_CUSTOM_CODE_TRUE);
			break;									
		default:
			break;
	}	
}
#endif // USE_CLARION


