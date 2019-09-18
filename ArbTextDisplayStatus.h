#ifndef ARB_TEXT_DISPLAY_STATUS_H
#define ARB_TEXT_DISPLAY_STATUS_H

#include "main.h"

typedef union ArbTextDisplayStatus{
	struct {
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct{
		unsigned char messageOperationStatus;
		unsigned char iconIdentifier;
		unsigned sessionIdentifier:3;
		unsigned requestingDeviceIdentifier:3;
		unsigned displayDeviceIdentifier:4;
		unsigned :6;
	}Status;
}ArbTextDisplayStatus;

typedef struct DisplayStatus {
	bool displayTurnedOffUnavailable;
	bool invalidDataLength;
	bool invalidData;
	bool requestedDisplayNowOff;
	bool requestedDisplayNowOn;
	bool displayAvailable;
	bool dataTransferNotGranted;
	bool dataTransferGranted;
}DisplayStatus;

bool ServiceArbTextDisplayStatus(uint8_t *data);
ArbTextDisplayStatus GetArbTextDisplayStatus(void);
	

void SendArbTextDisplayStatus(ArbTextDisplayStatus *arbTextDisplayStatus);


#endif // ARB_TEXT_DISPLAY_STATUS_H
