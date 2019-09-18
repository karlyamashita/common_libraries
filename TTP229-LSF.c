#include "LSP.h"
#ifdef USE_TTP229_LSF

#include "main.h"
#include "TTP229-LSF.h"

#include "I2C.h"

/*
Capacitive touch controller
*/

uint8_t i2cData[2];

HAL_I2C_StateTypeDef GetButtonsPressed(void) {
	
#ifdef USING_I2C2
	if(HAL_I2C_Master_Receive(&hi2c2, (uint16_t)TTP229_LSF_ADDRESS, (uint8_t*)i2cData, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY; 
	}
#endif // USING_I2C2	
	return HAL_I2C_STATE_READY;
}


#endif // USE_TTP229_LSF
