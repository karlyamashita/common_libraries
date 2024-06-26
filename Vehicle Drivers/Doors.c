#include "LSP.h"
#ifdef USE_DOORS
#include "main.h"
#include "Doors.h"

#if defined GMLAN_V1

union DriverDoorStatus
{
	unsigned char StatusByte;
	struct
	{
		unsigned OpenSwitchActive:1;
		unsigned AjarSwitchActive:1;
	}Bits;
}DriverDoorStatus;
#define _DriverDoorStatus DriverDoorStatus.StatusByte
#define _DriverDoorOpenSwitchActive DriverDoorStatus.Bits.OpenSwitchActive
#define _DriverDoorAjarSwitchActive DriverDoorStatus.Bits.AjarSwitchActive // most other cars only use bit0 to indicate door open

void ServiceDriversDoorStatus(unsigned char *data){
	DriverDoorStatus.StatusByte = data[0];
}

void ServicePassengerDoorStatus(unsigned char *data){
	
}

bool GetDriversDoorStatus(void){
	return _DriverDoorAjarSwitchActive;
}	
	
#endif // GMLAN_V1

#if defined CHRYSLER_V3

void ServiceDriversDoorStatus(unsigned char *data){
	DriverDoorStatus.StatusByte = data[0];
}

bool GetDriversDoorStatus(void){
	return _DriverDoorAjarSwitchActive;
}



#endif// CHRYSLER_V4
#endif // USE_DOORS

