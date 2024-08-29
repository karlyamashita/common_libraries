/*
 * INA3221_Drv.c
 *
 *  Created on: Jun 15, 2024
 *      Author: codek
 */

#include "main.h"

/*
 * Description: Read INA3221 register and call the callback if registered.
 *
 */
int INA3221_Read(I2C_GenericDef *i2c, uint16_t reg, void (*callback)(struct __I2C_GenericDef_ *i2c))
{
	int status = NO_ERROR;

	i2c->registerAddr[0] = reg;
	i2c->regSize = 1;
	i2c->RxISR = callback;

	status = I2C_Mem_Read_Generic_Method(i2c);
	if(status != NO_ERROR)
	{
		return status;
	}

	return status;
}

/*
 * Description: Be sure to initialize I2C data pointer with data and data size prior to calling this.
 */
int INA3221_Write(I2C_GenericDef *i2c, uint16_t reg, void (*callback)(struct __I2C_GenericDef_ *i2c))
{
	int status = NO_ERROR;

	i2c->registerAddr[0] = reg;
	i2c->regSize = 1;
	i2c->TxISR = callback;

	status = I2C_Mem_Write_Generic_Method(i2c);
	if(status != NO_ERROR)
	{
		return status;
	}

	return status;
}

