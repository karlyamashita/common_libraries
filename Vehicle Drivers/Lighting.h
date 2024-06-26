#ifdef USE_LIGHTING
#ifndef LIGHTING_H
#define LIGHTING_H

#if defined GMLAN_V1
typedef union LightingStatus {
	struct{
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct{
		//byte1
		unsigned turnSwitchActivation:2;
		unsigned rightTurnSignalActive:2;
		unsigned leftTurnSignalActive:2; 
		unsigned :2;
		//byte0
		unsigned lowBeamActive:1;
		unsigned parkingLightsActive:1;
		unsigned highBeamActive:1;
		unsigned highBeamIndicationOn:1;
		unsigned groundIlluminationRequest:1;
		unsigned frontFogLampsActive:1;
		unsigned flashToPassActive:1;
		unsigned dayTimeRunningLightsActive:1;
	}Status;
}LightingStatus;

#endif // GMLAN_V1

#if defined CHRYSLER_V3
typedef struct LightingStatus {
		unsigned :7;
		unsigned ParkingLightsActive:1;
}LightingStatus;

#endif// CHRYSLER_V3

void ServiceLightingStatus(uint8_t *data);

LightingStatus GetLightingStatus(void);

#endif // LIGHTING_H
#endif // USE_LIGHTING
