/*
 * TCA9546A_Drv.c
 *
 *  Created on: Jun 21, 2024
 *      Author: karl.yamashita
 */


#include "main.h"


void TCA9546A_Init(TCA9546A_HandlerTypeDef *tca, I2C_GenericDef *i2c, uint32_t transferType)
{
	tca->i2c_Instance = i2c;
	tca->i2c_Instance->timeout = 100;
	tca->i2c_Instance->transferType = transferType;
}


int TCA9546A_Write(TCA9546A_HandlerTypeDef *tca, void (*callback))
{
	int status = NO_ERROR;

	tca->i2c_Instance->dataSize = 1;
	tca->RxISR = callback;

    status = I2C_Master_Transmit_Generic(tca->i2c_Instance);

	return status;
}

/*
 * Description: Query TCA9546A for which channel is selected
 * Input: none
 * Output: Channel number
*/
int TCA9546A_Read(TCA9546A_HandlerTypeDef *tca, void (*callback))
{
	int status = NO_ERROR;

	tca->i2c_Instance->dataSize = 1;
	tca->RxISR = callback;

	status = I2C_Master_Receive_Generic(tca->i2c_Instance);

	return status;
}
