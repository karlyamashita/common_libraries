#include "LSP.h"
#ifdef USE_KENWOOD
#include "main.h"
#include "RadioFunctions.h"
#include "Kenwood.h"
#include "gpio_ports.h"

unsigned char commandSentFlag = 0;

void KenwoodStartBit(void)
{	
	PortB_Off(SWC_Pin); // pull low
	// delay 9 ms
	PortB_On(SWC_Pin); // set high
	// delay 4.5 ms
}

void KenwoodBit(unsigned char bit)
{
	PortB_Off(SWC_Pin); // pull low
	// delay 560us
	PortB_On(SWC_Pin); // set high
	if(bit == 0) {
		// delay 560us
	} else {
		// delay 1.69ms
	}
}

void KenwoodContinueBit(void)
{
	PortB_Off(SWC_Pin); // pull low
	// delay 9ms
	PortB_On(SWC_Pin); // set high // low for 9ms then go high
	// delay 2.25ms
	PortB_Off(SWC_Pin); // pull low // wait 2.25ms then go low
	// delay 560us
	PortB_On(SWC_Pin); // set high // wait 560us then go high
}

void KenwoodSendDataByte(unsigned char command)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		if(command & 0x01)
		{
			KenwoodBit(1);
		}
		else
		{
			KenwoodBit(0);
		}
		command >>= 1;
	}	
}

void Kenwood(unsigned char command, unsigned char repeatFlag)
{
	if(!commandSentFlag)
	{
		KenwoodStartBit();
		KenwoodSendDataByte((KENWOOD_CUSTOM_CODE >> 8) & 0xff);	// custom code high byte
		KenwoodSendDataByte(KENWOOD_CUSTOM_CODE & 0xff); // custom code low byte
		KenwoodSendDataByte(command);
		command = ~command;// invert command
		KenwoodSendDataByte(command);
		KenwoodBit(1);
		commandSentFlag = 1;
	}
	else
	{
		if(repeatFlag)
		{
			KenwoodContinueBit();		
		}
	}
}

void KenwoodCommand(unsigned char command)
{
	switch(command)
	{
		case 0:
			commandSentFlag = 0;
			break;
		case 1:Kenwood(KENWOOD_VOLUME_UP, COMMAND_REPEAT);
			break;
		case 2:Kenwood(KENWOOD_VOLUME_DOWN, COMMAND_REPEAT);
			break;
		case 3:Kenwood(KENWOOD_ATT, COMMAND_NO_REPEAT);
			break;
		case 4:Kenwood(KENWOOD_SOURCE, COMMAND_NO_REPEAT);
			break;
		case 5:Kenwood(KENWOOD_SEEK_PLAY, COMMAND_NO_REPEAT);
			break;
		case 6:Kenwood(KENWOOD_TRACK_UP, COMMAND_REPEAT);
			break;
		case 7:Kenwood(KENWOOD_TRACK_DOWN, COMMAND_REPEAT);
			break;
		case 8:Kenwood(KENWOOD_DISC_FM_UP, COMMAND_REPEAT);
			break;
		case 9:Kenwood(KENWOOD_DISC_AM_DOWN, COMMAND_REPEAT);
			break;
		case 10:Kenwood(KENWOOD_TEL_PHONE, COMMAND_REPEAT);
			break;
		case 11:Kenwood(KENWOOD_VOICE, COMMAND_REPEAT);
			break;
		case 12:Kenwood(KENWOOD_ON_HOOK, COMMAND_REPEAT);
			break;
		case 13:Kenwood(KENWOOD_OFF_HOOK, COMMAND_REPEAT);
			break;
		case 14:Kenwood(KENWOOD_MUTE_ON_OFF, COMMAND_REPEAT);
			break;
		default:
			break;
	}	
}

#endif // USE_KENWOOD

