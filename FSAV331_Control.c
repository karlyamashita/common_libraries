#include "LSP.h"
#ifdef USE_FSAV331_CONTROL
#include "main.h"
#include "FSAV331_Control.h"
#include "gpio.h"
#include "GPIO_Ports.h"


/*
Dual Video switch
*/


#if defined C3_M40_BOARD


// todo need to see if OE1 or OE2 pins need to be enabled
void SetVideoOutput(uint8_t videoOuput) {
	switch(videoOuput) {
		case 0: // rear
			PortC_Off(FSAV_S0_Pin);
			PortC_Off(FSAV_S1_Pin);
		break;
		case 1: // Front
			PortC_On(FSAV_S0_Pin);
			PortC_Off(FSAV_S1_Pin);
		break;
		case 2: // Left
			PortC_Off(FSAV_S0_Pin);
			PortC_On(FSAV_S1_Pin);
		break;
		case 3: // Right
			PortC_On(FSAV_S0_Pin);
			PortC_On(FSAV_S1_Pin);
		break;
		default:
			PortC_Off(FSAV_S0_Pin);
			PortC_Off(FSAV_S1_Pin);
		break;
	}
}

#endif // C3_M40_BOARD
#endif // USE_FSAV331_CONTROL

