/*
 * MAX_31760.c
 *
 *  Created on: Nov 28, 2019
 *      Author: Karl
 */

#include "LSP.h"
#ifdef USE_MAX_31760

#include "main.h"
#include "MAX_31760.h"

extern I2C_HandleTypeDef hi2c1;


HAL_StatusTypeDef status;
HAL_StatusTypeDef SendMax31760(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t regData) {

	uint8_t data[2];

	data[0] = reg;
	data[1] = regData;

	//status = HAL_I2C_Master_Seq_Transmit_IT(hi2c, MAX31760_SLAVE_ADDRESS_WRITE, data, sizeof(data), I2C_FIRST_AND_NEXT_FRAME);
	status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) MAX31760_SLAVE_ADDRESS_WRITE, (uint8_t*) data, 2, 100);

	return status;
}

#endif

