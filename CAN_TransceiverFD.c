#include "LSP.h"
#ifdef USE_CAN_TRANSCEIVER_FD
#include "main.h"
#include "CAN_TransceiverFD.h"
#include "can.h"
#include "gpio.h"
#include "GPIO_Ports.h"

#ifdef USE_CAN_BUFFER_1 // single CAN transceiver
void InitCAN_Transceiver(void) {
	CAN_TransceiverSetMode(CAN_CONTROLLER1, CAN_NORMAL);
}

void CAN_TransceiverSleep(void) {
	CAN_TransceiverSetMode(CAN_CONTROLLER1, CAN_SLEEP);
}

void CAN_TransceiverSetMode(unsigned char CAN_Controller, unsigned char mode) {
	if(mode) {
		PortB_Off(CAN1_Mode0_Pin); // low = normal
	} else {
		PortB_On(CAN1_Mode0_Pin); // high = sleep
	}
}
#endif // USE_1_BUFFERS

#ifdef USE_CAN_BUFFER_2 // dual CAN transceiver
void InitDualCanTransceiver(void) {
	CAN_DualTransceiverSetMode(CAN_CONTROLLER1, CAN_NORMAL);
	CAN_DualTransceiverSetMode(CAN_CONTROLLER2, CAN_NORMAL);
}

void DualCanTransceiverSleep(void) {
	CAN_DualTransceiverSetMode(CAN_CONTROLLER1, CAN_SLEEP);
	CAN_DualTransceiverSetMode(CAN_CONTROLLER2, CAN_SLEEP);
}

void CAN_DualTransceiverSetMode(unsigned char CAN_Controller, unsigned char mode) {
	if(CAN_Controller == CAN_CONTROLLER1) {
		if(mode == CAN_NORMAL) {
			PortB_Off(CAN1_Mode0_Pin); // low = normal
		} else {
			PortB_On(CAN1_Mode0_Pin); // high = sleep
		}
	} else if(CAN_Controller == CAN_CONTROLLER2) {
		if(mode == CAN_NORMAL) {
			PortB_Off(CAN2_Mode0_Pin); // low = normal
		} else {
			PortB_On(CAN2_Mode0_Pin); // high = sleep
		}
	}
}

#endif // USE_CAN_BUFFER_2

#endif // USE_CAN_TRANSCEIVER_FD

/*
 *
 * older board. Just keep in case we need for later
void CAN_DualTransceiverSetMode(unsigned char CAN_Controller, unsigned char mode) {
	if(CAN_Controller == CAN_CONTROLLER1) {
		if(mode == CAN_NORMAL) {
			PortA_Off(CAN_RS_Pin); // low = normal // CR4000 uses PortA
			PortA_Off(Data_Enable_Pin); // low = enabled // CR4000 has buffer chip that needs to be enabled on PortA
		} else {
			PortA_On(CAN_RS_Pin); // high = sleep
			PortA_On(Data_Enable_Pin);// high = disabled
		}
	} else if(CAN_Controller == CAN_CONTROLLER2) {
		if(mode == CAN_NORMAL) {
			PortA_Off(CAN_RS_Pin); // low = normal
			PortA_Off(Data_Enable_Pin); // low = enabled
		} else {
			PortA_On(CAN_RS_Pin); // high = sleep
			PortA_On(Data_Enable_Pin); // high disabled
		}
	}
}
*/



