#include "LSP.h"
#ifdef USE_GEAR_INFORMATION
#include "main.h"
#include "GearInformation.h"

#ifdef USING_GMLAN_V1

uint8_t gearInformation;

void ServiceGearInformation(uint8_t *data) {
	gearInformation = (data[1] >> 4 ) & 0x0F;
}

uint8_t GetGearInformation(void) {
	return gearInformation;
}
#endif // USING_GMLAN_V1

#ifdef USING_GMLAN_V2

uint8_t gearInformation;

void ServiceGearInformation(uint8_t *data) {
	gearInformation = (data[0]) & 0xF0; // low nibble
}

uint8_t GetGearInformation(void) {
	return gearInformation;
}

#endif // USING_GMLAN_V2

#ifdef CHRYSLER_V3

uint8_t gearInformation;

void ServiceGearInformation(uint8_t *data) {
	gearInformation = data[2] & 0x0F;
}

uint8_t GetGearInformation(void) {
	return gearInformation;
}

#endif // CHRYSLER_V3
#endif // USE_GEAR_INFORMATION
