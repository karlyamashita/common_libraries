
#ifndef POWER_MODE_H
#define POWER_MODE_H

#include "main.h"

#if defined USE_GMLAN_CAN_ID || defined USE_GMLAN_CAN_ID_V2
#include "GMLAN_Defines.h"
#endif // GMLAN_V1

#ifdef CHRYSLER_V2
#include "Chrysler_Defines.h"
#endif // CHRYSLER_V2

#ifdef CHRYSLER_V3
#include "Chrysler_Defines.h"
#endif // CHRYSLER_V3

#ifdef CHRYSLER_V4
#include "Chrysler_Defines.h"
#endif // CHRYSLER_V4

#ifdef CHRYSLER_V5
#include "Chrysler_Defines.h"
#endif // CHRYSLER_V5

#if defined USE_GMLAN_CAN_ID || defined USE_GMLAN_CAN_ID_V2
typedef struct PowerModeStatus{
	unsigned status:2;
}PowerModeStatus;
#endif // USE_GMLAN_CAN_ID and USE_GMLAN_CAN_ID_V2

#ifdef CHRYSLER_V2
//LSFTCAN
typedef union PowerModeStatusStruct{
	struct{
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned keySense:1;
		unsigned :4;
		unsigned keyPosition:3;
	}Status;
}PowerModeStatusStruct;
#endif // CHRYSLER_V2

#ifdef CHRYSLER_V3
// MSCAN
typedef union PowerModeStatusStruct{
	struct{
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned keySense:1;
		unsigned :4;
		unsigned keyPosition:3;
	}Status;
}PowerModeStatusStruct;
#endif // CHRYSLER_V3

#ifdef CHRYSLER_V4
// MSCAN
typedef union PowerModeStatusStruct{
	struct{
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned keyOutSense:1;
		unsigned keyInSense:1;
		unsigned :6;
		unsigned keyPosition:5;
		unsigned :3;
	}Status;
}PowerModeStatusStruct;
#endif // CHRYSLER_V4

#ifdef CHRYSLER_V5
// MSCAN
typedef union PowerModeStatusStruct{
	struct{
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned keyOutSense:1;
		unsigned keyInSense:1;
		unsigned :6;
		unsigned keyPosition:5;
		unsigned :3;
	}Status;
}PowerModeStatusStruct;
#endif // CHRYSLER_V5


void ServicePowerMode(uint8_t *data);
uint8_t GetPowerModeStatus(void);
void ClearPowerModeStatus(void);
bool GetKeySense(void);

#endif // POWER_MODE_H
