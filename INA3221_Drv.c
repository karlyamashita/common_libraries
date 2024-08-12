/*
 * INA3221_Drv.c
 *
 *  Created on: Jun 15, 2024
 *      Author: codek
 */

#include "main.h"


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
	i2c->RxISR = callback;

	status = I2C_Mem_Write_Generic_Method(i2c);
	if(status != NO_ERROR)
	{
		return status;
	}

	return status;
}

/*
 * Description: channels can be a combination of the 3 channels or'd
 * 				INA3221_CH1, INA3221_CH2, INA3221_CH3
 *
 */
int INA3221_EnableCh(I2C_GenericDef *i2c, uint32_t channels)
{
	int status = 0;

	// prep i2c
	i2c->registerAddr[0] = INA3221_CONFIG;
	i2c->regSize = 1;
	i2c->dataSize = 2;

	// modify register bits
	if(channels & INA3221_CH1)
	{
	//	ina->config.Status.ch_En = 1;
	}
	if(channels & INA3221_CH2)
	{
	//	ina->config.Status.ch_En |= 1 << 1;
	}
	if(channels & INA3221_CH3)
	{
	//	ina->config.Status.ch_En |= 1 << 2;
	}
	// copy data to i2c dataPtr
	//INA3221_CopyRegisterToDataPtr(i2c, (uint8_t*)ina->config.Bytes.data);

	status = I2C_Mem_Read_Generic_Method(i2c);
	if(status != NO_ERROR)
	{
		return status;
	}

	return status;
}

void INA3221_CopyRegisterToDataPtr(I2C_GenericDef *i2c, uint8_t *regData)
{
	memcpy(i2c->dataPtr, regData, 2);
}


