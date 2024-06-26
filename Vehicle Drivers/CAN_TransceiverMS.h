#include "LSP.h"
#ifdef USE_CAN_TRANSCEIVER_MS
#ifndef CAN_TRANSCEIVER_MS_H
#define CAN_TRANSCEIVER_MS_H

#define CAN_CONTROLLER1         1       // CAN Controller number
#define CAN_CONTROLLER2			2

// MSCAN defines
#define CAN_SLEEP 0
#define CAN_NORMAL 1

void CAN_TransceiverSetMode(unsigned char CAN_Controller, unsigned char mode);

#endif // CAN_TRANSCEIVER_MS_H
#endif // USE_CAN_BUFFER_MS
