#include "LSP.h"
#ifdef USE_VEHICLE_SPEED
#include "main.h"
#include "VehicleSpeed.h"

#ifdef GMLAN_V1


unsigned char vssData[2];

void ServiceVehicleSpeed(unsigned char *data) {
		vssData[0] = data[2];
		vssData[1] = data[3];
}

unsigned int GetVehicleSpeed(void) {
	unsigned int vssSpeed;
	if ((vssData[0] > 0 || vssData[1] > 0) && vssData[0] != 0xFF) {
		vssSpeed = vssData[0];
		vssSpeed = (vssSpeed << 8) + vssData[1];
		vssSpeed = vssSpeed & 0x7FFF; // only 15 bits used per GM specs
#if defined USE_KM_H
		return (vssSpeed >> 6); //  vssSpeed * 0.015625 = km/h
#else
		return (vssSpeed >> 6) * 0.62137119223733; //  vssSpeed * 0.015625 = MPH
#endif
	}
	return 0;
}



#endif // GMLAN_V1

#ifdef CHRYSLER_V5
uint8_t vssData;

void ServiceVehicleSpeed(unsigned char *data) {
		vssData = data[7];
}

/*
return value in mph
*/
uint8_t GetVehicleSpeed(void) {
	return vssData;
}

#endif // CHRYSLER_V5

#ifdef CHRYSLER_V3
uint8_t vssData;

void ServiceVehicleSpeed(unsigned char *data) {
		vssData = data[7];
}

/*
return value in mph
*/
uint8_t GetVehicleSpeed(void) {
	return vssData;
}

#endif // CHRYSLER_V3

#ifdef CHRYSLER_V2
uint8_t vssData;

void ServiceVehicleSpeed(unsigned char *data) {
		vssData = data[7];
}

/*
return value in mph
*/
uint8_t GetVehicleSpeed(void) {
	return vssData;
}

#endif // CHRYSLER_V2

#endif // USE_VEHICLE_SPEED

