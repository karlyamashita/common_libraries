#include "LSP.h"
#ifdef USE_CAN_TRANSCEIVER_FD
#ifndef CAN_TRANSCEIVER_FD_H
#define CAN_TRANSCEIVER_FD_H

#include "can.h"


#define CAN_CONTROLLER1         1       // CAN Controller number
#define CAN_CONTROLLER2			2

// MSCAN defines
#define CAN_SLEEP 0
#define CAN_NORMAL 1

#ifdef USE_CAN_BUFFER_1
void InitCAN_Transceiver(void);
void CAN_TransceiverSleep(void);
void CAN_TransceiverSetMode(unsigned char CAN_Controller, unsigned char mode);
#endif // USE_1_BUFFERS
#ifdef USE_CAN_BUFFER_2
void InitDualCanTransceiver(void);
void DualCanTransceiverSleep(void);
void CAN_DualTransceiverSetMode(unsigned char CAN_Controller, unsigned char mode);
#endif // USE_2_BUFFERS


#endif // CAN_TRANSCEIVER_FD_H
#endif // USE_CAN_TRANSCEIVER_FD
