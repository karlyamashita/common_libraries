#include "LSP.h"
#ifdef USE_VIN

#include "main.h"
#include "VIN.h"
#include "can.h"

#if defined GMLAN_V1

unsigned char VIN_2_9[8];
unsigned char VIN_10_17[8];
unsigned char VIN_2_9_CheckedFlag = 0;
unsigned char VIN_10_17_CheckedFlag = 0;

void Service_VIN_2_9(unsigned char *data){
	unsigned char i;
	if(VIN_2_9_CheckedFlag) { // already got vin so no need to save again
		return;
	}
	for(i = 0; i < 8; i++) {
		VIN_2_9[i] = data[i];
	}
	VIN_2_9_CheckedFlag = 1;
}

void Service_VIN_10_17(unsigned char *data){
	unsigned char i;
	if(VIN_10_17_CheckedFlag) { // already got vin so no need to save again
		return;
	}
	for(i = 0; i < 8; i++) {
		VIN_10_17[i] = data[i];
	}
	VIN_10_17_CheckedFlag = 1;
}

int GetYear(void) {
	// todo return 10th digit as number
	return 0;
}

bool GetHatchbackFlag(void) {
	// todo return if hatchback
	return false;
}

void GetVIN_2_9(unsigned char *data) {
	int i;
	for(i = 0; i < 8; i++){
		data[i] = VIN_2_9[i];
	}
}

void GetVIN_10_17(unsigned char *data) {
	int i;
	for(i = 0; i < 8; i++){
		data[i] = VIN_10_17[i];
	}
}

#endif // GMLAN_V1

#if defined FORD


#endif // FORD

#if defined CHRYSLER_LSFTCAN



#endif // CHRYSLER_LSFTCAN

#if defined CHYSLER_MSCAN


#endif // MSCAN


#endif // USE_VIN








