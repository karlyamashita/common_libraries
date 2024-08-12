/*
 * TMP10x_Drv.c
 *
 *  Created on: Jun 17, 2024
 *      Author: karl.yamashita
 *
 *
 *      The driver is the same for all of the TMP10x sensors. The only thing that changes is the configuration register.
 *      In the TMP10x.h file, select the define for your TMP10x that you are using which will select the correct structure.
 *
 */

#include "main.h"


int TMP10x_Read(I2C_GenericDef *i2c, uint16_t reg, void (*callback)(struct __I2C_GenericDef_ *i2c))
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
int TMP10x_Write(I2C_GenericDef *i2c, uint16_t reg, void (*callback)(struct __I2C_GenericDef_ *i2c))
{
	int status = NO_ERROR;

	i2c->registerAddr[0] = reg;
	i2c->regSize = 1;
	i2c->RxISR = callback;

	status = I2C_Mem_Write_Generic_Method(i2c);
	if(status != NO_ERROR)
	{
		return status;
	}

	return status;
}
