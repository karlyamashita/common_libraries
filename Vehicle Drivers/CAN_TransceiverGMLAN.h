#include "main.h"
#ifndef CAN_TRANSCEIVER_H
#define CAN_TRANSCEIVER_H

#include "can.h"

#define CAN_CONTROLLER1         1       // CAN Controller number
#define CAN_CONTROLLER2				 2
// SWCAN defines
#define CAN_SLEEP 0
#define CAN_NORMAL 1
#define HIGH_SPEED 2
#define HVWU 3

#ifdef USE_1_CAN_TRANSCEIVER
void InitCAN_Transceiver(void);
void CAN_TransceiverSleep(void);
void CAN_TransceiverSetMode(unsigned char mode);
#else
void InitDualCanTransceiver(void);
void DualCanTransceiverSleep(void);
void CAN_DualTransceiverSetMode(unsigned char CAN_Controller, unsigned char mode);
#endif

#endif // CAN_TRANSCEIVER_H

