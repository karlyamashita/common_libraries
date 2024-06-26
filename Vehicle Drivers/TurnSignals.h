#include "LSP.h"
#ifdef USE_TURN_SIGNALS
#ifndef TURN_SIGNALS_H
#define TURN_SIGNALS_H


#ifdef CHRYSLER_V3


typedef union TurnSignalStatus{
	struct {
		uint8_t byte0;
	}Byte;
	struct{
		unsigned leftSignal:1;
		unsigned rightSignal:1;
		unsigned :6;
	}Status;
}TurnSignalStatus;

void ServiceTurnSignalStatus(uint8_t *data);
TurnSignalStatus GetTurnSignalStatus(void);

TurnSignalStatus CheckTurnSignalDoubleClickStatus(void);
#endif // CHRYSLER_V3




#endif // TURN_SIGNALS_H
#endif // USE_TURN_SIGNALS
