#include "LSP.h"
#ifdef USE_POWER_MODE
#include "main.h"
#include "PowerMode.h"


#ifdef USE_GMLAN_CAN_ID

PowerModeStatus powerModeStatus;
#define powerModeStatus powerModeStatus.status

void ServicePowerMode(unsigned char *data) {
	powerModeStatus = (data[0] & 0x03);
}

unsigned char GetPowerModeStatus(void) {
	return powerModeStatus;
}
#endif // USE_GMLAN_CAN_ID

#ifdef USE_GMLAN_CAN_ID_V2

PowerModeStatus powerModeStatus;
#define powerModeStatus powerModeStatus.status

void ServicePowerMode(unsigned char *data) {
	powerModeStatus = (data[0] & POWER_MODE_MASK);
}

unsigned char GetPowerModeStatus(void) {
	return powerModeStatus;
}

#endif // USE_GMLAN_CAN_ID_V2

#if defined FORD

void ServicePowerMode(unsigned char *data) {

}

unsigned char GetPowerModeStatus(void) {
	return 0;
}

#endif // FORD

#ifdef CHRYSLER_V2

PowerModeStatusStruct powerModeStatus;

void ServicePowerMode(uint8_t *data) {
	powerModeStatus.Bytes.byte0 = data[0] & 0xE1;
}

uint8_t GetPowerModeStatus(void) {
	return powerModeStatus.Status.keyPosition;
}

bool GetKeySense(void){
	return powerModeStatus.Status.keySense;
}

#endif // CHRYSLER_V3

#ifdef CHRYSLER_V3

PowerModeStatusStruct powerModeStatus;

void ServicePowerMode(uint8_t *data) {
	powerModeStatus.Bytes.byte0 = data[0] & 0xE1;
}

uint8_t GetPowerModeStatus(void) {
	return powerModeStatus.Status.keyPosition;
}

bool GetKeySense(void){
	return powerModeStatus.Status.keySense;
}

#endif // CHRYSLER_V3

#ifdef CHRYSLER_V4

PowerModeStatusStruct powerModeStatus;

void ServicePowerMode(uint8_t *data) {
	powerModeStatus.Bytes.byte0 = data[0] & 0x1F;
	powerModeStatus.Bytes.byte1 = data[1] & 0x03;
}

uint8_t GetPowerModeStatus(void) {
	return powerModeStatus.Status.keyPosition;
}

void ClearPowerModeStatus(void) {
	powerModeStatus.Status.keyPosition = 0;
}

bool GetKeyInSense(void){
	return powerModeStatus.Status.keyInSense;
}

#endif // CHRYSLER_V4

#ifdef CHRYSLER_V5

PowerModeStatusStruct powerModeStatus;

void ServicePowerMode(uint8_t *data) {
	powerModeStatus.Bytes.byte0 = data[0] & 0x1F;
	powerModeStatus.Bytes.byte1 = data[1] & 0x03;
}

uint8_t GetPowerModeStatus(void) {
	return powerModeStatus.Status.keyPosition;
}

void ClearPowerModeStatus(void) {
	powerModeStatus.Status.keyPosition = 0;
}

bool GetKeyInSense(void){
	return powerModeStatus.Status.keyInSense;
}

#endif // CHRYSLER_V5

#endif // USE_POWER_MODE

