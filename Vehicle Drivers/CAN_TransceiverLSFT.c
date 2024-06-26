#include "LSP.h"
#ifdef USE_CAN_TRANSCEIVER_LSFT
#include "main.h"
#include "CAN_TransceiverLSFT.h"
#include "can.h"
#include "gpio.h"
#include "GPIO_Ports.h"

#if defined USE_1_BUFFER // single CAN transceiver
void InitCAN_Transceiver(void) {
	CAN_TransceiverSetMode(CAN_CONTROLLER1, CAN_NORMAL);
	hcan1.Instance = CAN1;
	HAL_CAN_MspInit(&hcan1);
}

void CAN_TransceiverSleep(void) {
	CAN_TransceiverSetMode(CAN_CONTROLLER1, CAN_SLEEP);
}

void CAN_TransceiverSetMode(unsigned char CAN_Controller, unsigned char mode) {
	if(mode) {
		PortB_Off(CAN_RS_Pin); // low = normal
	} else {
		PortB_On(CAN_RS_Pin); // high = sleep
	}
}
#endif // USE_1_BUFFERS

#if defined USE_2_BUFFERS // dual CAN transceiver
void InitDualCanTransceiver(void) {
	CAN_DualTransceiverSetMode(CAN_CONTROLLER1, CAN_NORMAL);
	CAN_DualTransceiverSetMode(CAN_CONTROLLER2, CAN_NORMAL);
	
	hcan1.Instance = CAN1;
	HAL_CAN_MspInit(&hcan1);
	hcan2.Instance = CAN2;
	HAL_CAN_MspInit(&hcan2);
}

void DualCanTransceiverSleep(void) {
	CAN_DualTransceiverSetMode(CAN_CONTROLLER1, CAN_SLEEP);
	CAN_DualTransceiverSetMode(CAN_CONTROLLER2, CAN_SLEEP);
}

void CAN_DualTransceiverSetMode(unsigned char CAN_Controller, unsigned char mode) {
	if(CAN_Controller == CAN_CONTROLLER1) {
		if(mode == CAN_NORMAL) {
			PortB_Off(CAN_RS_Pin); // low = normal
		} else {
			PortB_On(CAN_RS_Pin); // high = sleep
		}
	} else if(CAN_Controller == CAN_CONTROLLER2) {
		if(mode == CAN_NORMAL) {
			PortB_Off(CAN_RS_Pin); // low = normal
		} else {
			PortB_On(CAN_RS_Pin); // high = sleep
		}
	}
}
#endif // CAN2


#endif // USE_CAN_TRANSCEIVER_LSFT






