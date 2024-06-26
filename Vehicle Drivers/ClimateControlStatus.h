#ifndef CLIMATE_CONTROL_STATUS_H
#define CLIMATE_CONTROL_STATUS_H

#include "main.h"

#ifdef USING_GMLAN_V2

typedef union ClimateControlStatusStruct {
	struct {
		uint8_t byte0;
	}Bytes;
	struct {
		uint8_t rearWindowDefoggerOn:1;
		uint8_t :7;// define later
	}Status;
	
}ClimateControlStatusStruct;

void ServiceClimateControlStatus(uint8_t *data);
uint8_t GetRearDefoggerOnStatus(void);

#endif // USING_GMLAN_V2

#endif // CLIMATE_CONTROL_STATUS_H
