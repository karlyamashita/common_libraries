#include "LSP.h"
#ifdef USE_BRAKE
#include "main.h"
#include "Brake.h"

#if defined GMLAN_V1

#define BRAKE_DATA_LENGTH 4

unsigned char brakeArray[BRAKE_DATA_LENGTH];
unsigned char footBrakeStatus;
unsigned char parkingBrakeStatus;

void ServiceParkingFootBrake(unsigned char *data){
		unsigned char i;
		for (i = 0; i < BRAKE_DATA_LENGTH; i++) {
		brakeArray[i] = data[i];
	}
}

unsigned char GetParkingBrakeStatus(void){
	if ((brakeArray[1] & 0x02) == 0x02) {
		parkingBrakeStatus = 1;
	} else {
		parkingBrakeStatus = 0;
	}
	return parkingBrakeStatus;
}

unsigned char GetFootBrakeStatus(void) {
	if ((brakeArray[1] & 0x80) == 0x80) { 
		footBrakeStatus = 1;
	} else {
		footBrakeStatus = 0;
	}
	return footBrakeStatus;
}


#endif // GMLAN_V1
#endif // USE_BRAKE
