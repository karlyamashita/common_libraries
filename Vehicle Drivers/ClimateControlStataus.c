#include "LSP.h"
#ifdef USE_CLIMATE_CONTROL_STATUS
#include "main.h"
#include "ClimateControlStatus.h"

#ifdef USING_GMLAN_V2
ClimateControlStatusStruct climateControlStatusStruct;

void ServiceClimateControlStatus(uint8_t *data) {
	climateControlStatusStruct.Bytes.byte0 = data[0] & 0x01;
}

uint8_t GetRearDefoggerOnStatus(void) {
	return climateControlStatusStruct.Status.rearWindowDefoggerOn;
}
#endif // USING_GMLAN_V2


#endif // USE_CLIMATE_CONTROL_STATUS
