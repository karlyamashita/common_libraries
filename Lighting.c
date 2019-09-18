#include "LSP.h"
#ifdef USE_LIGHTING

#include "main.h"
#include "lighting.h"
#include "can.h"
#include "gpio.h"

#if defined GMLAN_V1

LightingStatus lightingStatus;
#define lightingStatusByte0	lightingStatus.Bytes.byte0
#define lightingStatusByte1	lightingStatus.Bytes.byte1
#define leftTurnSignalActive lightingStatus.Status.leftTurnSignalActive
#define rightTurnSignalActive lightingStatus.Status.rightTurnSignalActive
#define parkingLightActive lightingStatus.Status.parkingLightsActive
#define highBeamActive lightingStatus.Status.highBeamActive

// lighting
void ServiceLightingStatus(unsigned char *data) {
	lightingStatusByte0 = data[0]; // byte 0
	lightingStatusByte1 = data[1]; // byte 1
}


LightingStatus GetLightingStatus(void) {
	return lightingStatus;
}


// end lighting


#endif // GMLAN_V1

#if defined FORD
uint8_t dimmerValue;

void ServiceDimmer(uint8_t *data) {
	dimmerValue = data[0]; // first byte is int dimming level
}

uint8_t GetDimmerValue(void) {
	return dimmerValue;
}


#endif // FORD

#if defined CHRYSLER_LSFTCAN



#endif // CHRYSLER_LSFTCAN

#if defined CHRYSLER_V3

bool lightingStatus;

void ServiceLightingStatus(unsigned char *data) {
	lightingStatus = data[0] >> 7;
}


bool GetLightingStatus(void) {
	return lightingStatus;
}

#endif // MSCAN
#endif // USE_LIGHTING


