#include "LSP.h"
#ifdef USE_CAN_TRANSCEIVER_MS
#include "main.h"
#include "CAN_TransceiverMS.h"
#include "GPIO_Ports.h"


/*
 * function: sets up the CAN transceiver mode
 * input: CAN channel - CAN_CONTROLLER1, CAN_CONTROLLER2. CAN mode - CAN_SLEEP, CAN_NORMAL
 * output; none
 * note: if transceiver is always on, then do not include this library, or you'll need to define an I/O CAN1_Mode0_Pin
 */
void CAN_TransceiverSetMode(unsigned char CAN_Controller, unsigned char mode) {
	if(mode) {
		PortB_Off(CAN1_Mode0_Pin); // low = normal
	} else {
		PortB_On(CAN1_Mode0_Pin); // high = sleep
	}
}

#endif // USE_CAN_TRANSCEIVER_MS

