#include "LSP.h"
#ifdef USE_DIMMER_INFORMATION
#include "main.h"
#include "DimmerInformation.h"

#if defined GMLAN_V1
uint8_t dimmerValue;

void ServiceDimmer(uint8_t *data) {
	dimmerValue = data[0]; // first byte is int dimming level
}

uint8_t GetDimmerValue(void) {
	return dimmerValue;
}

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
#endif // USE_DIMMER_INFORMATION
