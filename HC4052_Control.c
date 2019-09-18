#include "LSP.h"
#ifdef USE_HC4052_CONTROL
#include "main.h"
#include "HC4052_Control.h"
#include "gpio.h"
#include "GPIO_Ports.h"

/*

74HCT4052 Dual 4-channel Analog multiplexer
*/

#if defined C3_M40_BOARD

void SetAnalogOutput(uint8_t analogOuput) {
	switch(analogOuput) {
		case 0: // rear
			PortC_Off(HC4052_S0_Pin);
			PortC_Off(HC4052_S1_Pin);
		break;
		case 1: // Front
			PortC_On(HC4052_S0_Pin);
			PortC_Off(HC4052_S1_Pin);
		break;
		case 2: // Left
			PortC_Off(HC4052_S0_Pin);
			PortC_On(HC4052_S1_Pin);
		break;
		case 3: // Right
			PortC_On(HC4052_S0_Pin);
			PortC_On(HC4052_S1_Pin);
		break;
		default:
			PortC_Off(HC4052_S0_Pin);
			PortC_Off(HC4052_S1_Pin);
		break;
	}
}

#endif // C3_M40_BOARD
#endif // USE_HC4052_CONTROL

