#include "LSP.h"
#ifdef USE_CAN_TRANSCEIVER_GMLAN
#include "main.h"
#include "CAN_TransceiverGMLAN.h"
#include "can.h"
#include "gpio.h"
#include "GPIO_Ports.h"

#ifdef USE_1_CAN_TRANSCEIVER // single CAN transceiver
void InitCAN_Transceiver(void) {
	CAN_TransceiverSetMode(CAN_NORMAL);
}

void CAN_TransceiverSleep(void) {
	CAN_TransceiverSetMode(CAN_SLEEP);
}

void CAN_TransceiverSetMode(unsigned char mode) {
	switch(mode) {
		case CAN_SLEEP:
			PortB_Off(CAN1_Mode0_Pin);
			PortB_Off(CAN1_Mode1_Pin);
		break;
		case CAN_NORMAL:
			PortB_On(CAN1_Mode0_Pin);
			PortB_On(CAN1_Mode1_Pin);
		break;
		case HIGH_SPEED:
			PortB_On(CAN1_Mode0_Pin);
			PortB_Off(CAN1_Mode1_Pin);
		break;
		case HVWU:
			PortB_Off(CAN1_Mode0_Pin);
			PortB_On(CAN1_Mode1_Pin);
		break;			
	}
}
#else // dual CAN transceiver
void InitDualCanTransceiver(void) {
	CAN_DualTransceiverSetMode(CAN_CONTROLLER1, CAN_NORMAL);
	CAN_DualTransceiverSetMode(CAN_CONTROLLER2, CAN_NORMAL);
}

void DualCanTransceiverSleep(void) {
	CAN_DualTransceiverSetMode(CAN_CONTROLLER1, CAN_SLEEP);
	CAN_DualTransceiverSetMode(CAN_CONTROLLER2, CAN_SLEEP);	
}

#ifndef CAN1_Mode0_Pin
#warning "CAN1_Mode0_Pin not defined"
#define CAN1_Mode0_Pin 0
#endif

#ifndef CAN1_Mode1_Pin
#warning "CAN1_Mode1_Pin not defined"
#define CAN1_Mode1_Pin 0
#endif

#ifndef CAN2_Mode0_Pin
#warning "CAN2_Mode0_Pin not defined"
#define CAN2_Mode0_Pin 0
#endif

#ifndef CAN2_Mode1_Pin
#warning "CAN2_Mode1_Pin not defined"
#define CAN2_Mode1_Pin 0
#endif

void CAN_DualTransceiverSetMode(unsigned char CAN_Controller, unsigned char mode) {
	if(CAN_Controller == CAN_CONTROLLER1) {
		switch(mode) {
			case CAN_SLEEP:
				PortC_Off(CAN1_Mode0_Pin);
				PortC_Off(CAN1_Mode1_Pin);
			break;
			case CAN_NORMAL:
				PortC_On(CAN1_Mode0_Pin);
				PortC_On(CAN1_Mode1_Pin);
			break;
			case HIGH_SPEED:
				PortC_On(CAN1_Mode0_Pin);
				PortC_Off(CAN1_Mode1_Pin);
			break;
			case HVWU:
				PortC_Off(CAN1_Mode0_Pin);
				PortC_On(CAN1_Mode1_Pin);
			break;			
		}
	} else if(CAN_Controller == CAN_CONTROLLER2){
		switch(mode) {
			case CAN_SLEEP:
				PortC_Off(CAN2_Mode0_Pin);
				PortC_Off(CAN2_Mode1_Pin);
			break;
			case CAN_NORMAL:
				PortC_On(CAN2_Mode0_Pin);
				PortC_On(CAN2_Mode1_Pin);
			break;
			case HIGH_SPEED:
				PortC_On(CAN2_Mode0_Pin);
				PortC_Off(CAN2_Mode1_Pin);
			break;
			case HVWU:
				PortC_Off(CAN2_Mode0_Pin);
				PortC_On(CAN2_Mode1_Pin);
			break;
		}
	}
}

#endif // USE_1_CAN_TRANSCEIVER
#endif // USE_CAN_TRANSCEIVER_GMLAN


