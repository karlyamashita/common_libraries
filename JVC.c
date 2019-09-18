#include "LSP.h"
#ifdef USE_JVC
#include "main.h"
#include "RadioFunctions.h"
#include "JVC.h"
#include "gpio_ports.h"

unsigned char startBitSentFlag = 0;
unsigned char repeatCounter = 0;

void JVCStartBit(void)
{	
	PortB_Off(SWC_Pin); // pull low
	// delay 8.44ms
	PortB_On(SWC_Pin); // set high
	// delay 4.22ms
}

void JVCBit(unsigned char bit)
{
	PortB_Off(SWC_Pin); // pull low
	// delay 560us
	if(bit == 0) {
		PortB_On(SWC_Pin); // set high
		// delay 560us
	}	else {
		PortB_On(SWC_Pin); // set high
		// delay 1.56ms	
	}
}

void JVCSendCommand(unsigned char command)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		if(command & 0x01)
		{
			JVCBit(1);
		}
		else
		{
			JVCBit(0);
		}
		command >>= 1;
	}	
}
void JVC(unsigned char command, unsigned char repeatFlag)
{
	if(repeatFlag || ((repeatFlag == 0) && (repeatCounter < 2))) {
		if(!startBitSentFlag)
		{
			JVCStartBit(); // Send start bit (first time only)
			startBitSentFlag = 1;
		}
		JVCSendCommand(JVC_CUSTOM_CODE);	
		JVCSendCommand(command);	
		JVCBit(1);	
		if(++repeatCounter > 2) repeatCounter = 2;// keep counter from rolling over
	}
}

void JVC_Command(unsigned char command)
{
	switch(command)
	{
		case 0:
			startBitSentFlag = 0;
			repeatCounter = 0;
			break;
		case 1:JVC(JVC_VOLUME_UP, 1); // 1 = repeat frames, 0 = send two frames only
			break;
		case 2:JVC(JVC_VOLUME_DOWN, 1);
			break;
		case 3:JVC(JVC_MUTE, 1);
			break;
		case 4:JVC(JVC_SOURCE, 1);
			break;
		case 5:JVC(JVC_TRACK_UP, 1);
			break;
		case 6:JVC(JVC_TRACK_DOWN, 1);
			break;
		case 7:JVC(JVC_BAND_DISC_UP, 1);
			break;
		case 8:JVC(JVC_PRESET_DISC_DOWN, 1);
			break;
		case 9:JVC(JVC_SELECT, 1);
			break;
		case 10:JVC(JVC_ATT, 1);
			break;
		case 11:JVC(JVC_PHONE_RECEIVE, 0);
			break;
		case 12:JVC(JVC_PHONE_REJECT, 0);
			break;
		case 13:JVC(JVC_VOICE_DIAL, 0);
			break;
		case 14:JVC(JVC_POWER, 1);
			break;
		default:
			break;
	}	
}
#endif // USE_JVC
