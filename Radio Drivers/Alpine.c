#include "LSP.h"
#ifdef USE_ALPINE
#include "main.h"
#include "RadioFunctions.h"
#include "Alpine.h"
#include "gpio_ports.h"

void AlpinePWM(unsigned char status) {
	if(status) {
		// pwm start
	} else {
		// pwm stop
	}
}

void AlpineStartBit(void)
{
	AlpinePWM(1);
	// delay 9ms
	AlpinePWM(0);
	// delay 4.4ms
}

void AlpineBit(unsigned char bit)
{
	AlpinePWM(1);
	//delay 560us
	AlpinePWM(0);
	if(bit == 0){
		//delay 560us
	} else {
		
		//delay 1.64ms
	}
}

void AlpineSendCommand(unsigned char command)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		if(command & 0x01)
		{
			AlpineBit(1);
		}
		else
		{
			AlpineBit(0);
		}
		command >>= 1;
	}
}

void Alpine(unsigned char command)
{
	AlpineStartBit();
	AlpineSendCommand((ALPINE_CUSTOMCODE >> 8) & 0xff); // custom code high byte
	AlpineSendCommand(ALPINE_CUSTOMCODE & 0xff); // custom code low byte
	AlpineSendCommand(command);
	command = ~command; // invert command
	AlpineSendCommand(command);
	AlpineBit(1);
}

void AlpineCommand(unsigned char command)
{
	switch(command)
	{
		case 0:
			break;
		case 1:
		   AlpineCommand(ALPINE_VOLUME_UP);
			break;
		case 2:
		   AlpineCommand(ALPINE_VOLUME_DOWN);
			break;
		case 3:
		   AlpineCommand(ALPINE_MUTE);
			break;
		case 4:
		   AlpineCommand(ALPINE_PRESET_UP);
			break;
		case 5:
		   AlpineCommand(ALPINE_PRESET_DOWN);
			break;
		case 6:
		   AlpineCommand(ALPINE_SOURCE);
			break;
		case 7:
		   AlpineCommand(ALPINE_TRACK_UP);
			break;
		case 8:
		   AlpineCommand(ALPINE_TRACK_DOWN);
			break;
		case 9:
		   AlpineCommand(ALPINE_POWER);
			break;
		case 10:
		   AlpineCommand(ALPINE_ENT_PLAY);
			break;
		case 11:
		   AlpineCommand(ALPINE_BAND_PROGRAM);;
			break;
		case 12:
		   AlpineCommand(ALPINE_RECEIVE_CALL);
			break;
		case 13:
		   AlpineCommand(ALPINE_END_CALL);
			break;
		case 14:
			 AlpineCommand(ALPINE_VOICE);
			break;
		default:
			break;
	}
}

#endif // USE_ALPINE


