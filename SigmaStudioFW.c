#include "LSP.h"
#ifdef USE_SIGMA_STUDIO_FW
#include "main.h"
#include "SigmaStudioFW.h"

#include "I2C.h"



void SIGMA_WRITE_REGISTER_BLOCK( devAddress, address, length, *pData ) {
#ifdef USING_I2C1
	if(HAL_I2C_Mem_Write(&hi2c1, (uint16_t)devAddress, (uint16_t)address, (uint8_t)pData, length, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY;
	}
#endif
#ifdef USING_I2C2
	if(HAL_I2C_Mem_Write(&hi2c2, (uint16_t)devAddress, (uint16_t)address, (uint8_t)pData, length, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY; 
	}
#endif
		
}


#endif // USE_SIGMA_STUDIO_FW
