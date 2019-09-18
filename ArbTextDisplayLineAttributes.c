#include "LSP.h"
#ifdef USE_ARB_TEXT_DISPLAY_LINE_ATTRIBUTES

#include "main.h"
#include "ArbTextDisplayLineAttributes.h"
#include "GMLAN_Defines.h"


uint8_t arbTextDisplayLineAttributesHead = 0;
uint8_t arbTextDisplayLineAttributesIPC = 0;
uint8_t arbTextDisplayLineAttributesRSE = 0;


void ServiceArbTextDisplayLineAttributes(uint8_t *data) {
	// todo find the line attributes
	uint8_t displayDeviceIdentifier = 0;
	displayDeviceIdentifier = data[0] >> 4;
	if(displayDeviceIdentifier == INFOTAINMENT_DISPLAY) {
		arbTextDisplayLineAttributesHead = data[1];
	} else if(displayDeviceIdentifier == CLUSTER_DISPLAY) {
		arbTextDisplayLineAttributesIPC = data[1];
	} else if (displayDeviceIdentifier == REAR_SEAT_DISPLAY) {
		arbTextDisplayLineAttributesRSE = data[1];
	}
}

//
// 0x1A = MP3 single line radio
// 0x12 = IPC
// 0x10 = Navigation radio
// 0x90 = RSE
uint8_t GetArbTextDisplayLineAttributesHead(void) {
	if(arbTextDisplayLineAttributesHead != 16 || arbTextDisplayLineAttributesHead != 18 || arbTextDisplayLineAttributesHead != 26) {
		arbTextDisplayLineAttributesHead = 16;
	}
	return arbTextDisplayLineAttributesHead;
}

uint8_t GetArbTextDisplayLineAttributesIPC(void) {
	if(arbTextDisplayLineAttributesIPC != 16 || arbTextDisplayLineAttributesIPC != 18 || arbTextDisplayLineAttributesHead != 26) {
		arbTextDisplayLineAttributesIPC = 16;
	}
	return arbTextDisplayLineAttributesIPC;
}

uint8_t GetArbTextDisplayLineAttributesRSE(void) {
	if(arbTextDisplayLineAttributesRSE != 16 || arbTextDisplayLineAttributesRSE != 18 || arbTextDisplayLineAttributesHead != 26) {
		arbTextDisplayLineAttributesRSE = 16;
	}
	return arbTextDisplayLineAttributesRSE;
}

#endif // USE_ARB_TEXT_DISPLAY_LINE_ATTRIBUTES

