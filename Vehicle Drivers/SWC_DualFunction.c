#include "LSP.h"
#ifdef USE_SWC_DUAL_FUNCTION
#include "main.h"
#include "SWC_DualFunction.h"
#include "mTimers.h"
#include "SWC.h"
#include "GMLAN_Defines.h"
#include "SWC_SendDelayed.h"

// add you SWC commands you want to ignore for dual function
uint8_t swcCommandsToIgnore[] = {SWC_VOLUME_UP, SWC_VOLUME_DOWN, SWC_VOICE}; // GM SWC commands to ignore

uint8_t DualSWCTimer = 0; // timer instance

/*
function: put value to SWC_Data variable to be parsed later from polling. Since we have SWC blocking and Dual function we can't parse data here.
input: CAN data. First byte is only needed for GM.
output: none
*/
uint8_t GetDualSWC_ButtonStatus(uint8_t command) {
	static uint8_t mode;
	uint8_t i = 0;
	static uint8_t lastSwcCommand = 0;
	static bool swcIgnoredFlag = false;
	
	if(DualSWCTimer == 0) {
		DualSWCTimer = CreateTimer();
	}
	
	if(command == 0xff) return 0xff;

	if(command == SWC_BUTTON_RELEASE && swcIgnoredFlag == true) {
		swcIgnoredFlag = false;
		return SWC_BUTTON_RELEASE;
	}
	for(i = 0; i < sizeof(swcCommandsToIgnore); i++) {
		if(swcCommandsToIgnore[i] == command) {
			swcIgnoredFlag = true;
			return command; // ignore this button for dual function
		}
	}
	switch(mode) { // using switch case just in case some vehicles send SWC continously until button is released, then we don't want to reset the timer
		case 0:// wait for button value > 0
			if(command > SWC_BUTTON_RELEASE){ // some button value
				SetTimer(DualSWCTimer, 0); // reset timer				
				lastSwcCommand = command;	
				mode++;	
			}
		break;
		case 1:
			if(command == SWC_BUTTON_RELEASE) { // button released
				mode = 0;// reset mode back to zero
				if(GetTimer(DualSWCTimer) > 1000) { // long press so send (command + SWC_MAXIMUM_BUTTONS) // working for GM. May need to modify for other Manufacture vehicles
					AddSWC_CommandToBufferCAN1(lastSwcCommand + SWC_MAXIMUM_BUTTONS, 0); // no delay, send now
					lastSwcCommand = 0;// clear value
					AddSWC_CommandToBufferCAN1(SWC_BUTTON_RELEASE, 100);
				} else { // short press
					AddSWC_CommandToBufferCAN1(lastSwcCommand, 0); // no delay, send now
					lastSwcCommand = 0;// clear value
					AddSWC_CommandToBufferCAN1(SWC_BUTTON_RELEASE, 100); // send release command in 100ms
				}		
			}
		break;
	}
	return 0xff; // block command for now
}

#endif // USE_SWC_DUAL_FUNCTION

