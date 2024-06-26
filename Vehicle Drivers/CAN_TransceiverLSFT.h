#ifndef CAN_TRANSCEIVER_LSFT_H
#define CAN_TRANSCEIVER_LSFT_H

#if defined hcan1 || defined hcan2
#include "can.h"
#endif // hcan1
#define CAN_CONTROLLER1         1       // CAN Controller number
#define CAN_CONTROLLER2				 2

// LSFT defines
#define CAN_SLEEP 0
#define CAN_NORMAL 1
#define CAN_POWER_ON_STANDBY 2
#define CAN_GOTO_SLEEP 3

#if defined USE_1_BUFFER
void InitCAN_Transceiver(void);
void CAN_TransceiverSleep(void);
void CAN_TransceiverSetMode(unsigned char CAN_Controller, unsigned char mode);
#endif // USE_1_BUFFERS
#if defined USE_2_BUFFERS
void InitDualCanTransceiver(void);
void DualCanTransceiverSleep(void);
void CAN_DualTransceiverSetMode(unsigned char CAN_Controller, unsigned char mode);
#endif // USE_2_BUFFERS



#endif // CAN_TRANSCEIVER_LSFT_H
