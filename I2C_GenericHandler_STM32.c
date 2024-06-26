/*
 * I2C_GenericHandler.c
 *
 *  Created on: Apr 24, 2024
 *      Author: karl.yamashita
 *
 *
 *      The object of this file is to have the same function names used in a I2C device drivers.
 *      For example, if using a TMP10x driver, that driver would be calling the same "generic" functions like below.
 *
       	I2C_Master_Receive_Generic(&i2c1);
		I2C_Master_Transmit_Generic(&i2c1);
		I2C_Master_Receive_Generic_IT(&i2c1);
		I2C_Master_Transmit_Generic_IT(&i2c1);
		I2C_Master_Receive_Generi_DMA(&i2c1);
		I2C_Master_Transmit_Generic_DMA(&i2c1);
		I2C_Mem_Read_Generic(&i2c1);
		I2C_Mem_Write_Generic(&i2c1);
		I2C_Mem_Read_Generic_IT(&i2c1);
		I2C_Mem_Write_Generic_IT(&i2c1);
		I2C_Mem_Read_Generic_DMA(&i2c1);
		I2C_Mem_Write_Generic_DMA(&i2c1);

		Then each of these functions will then call the specific uC's I2C driver actual function.
		This way you can use the same TMP10x driver without modifying it for another uCwhich makes it a universal driver.

		This is for STM32. If we were using a TM4C12x, we would have a I2C_Generic_TM4C.c/h file. It will have the same
		function names as the list above. But in each function, it'll call the TM4C I2C driver function.


		Instructions:
		1. Create a data array...
				uint8_t i2c_data[n] = {0};

		2. In your main or other file, create an instance of the I2C port. Assign the I2C instance and data array...
				I2C_GenericDef i2c1 =
				{
					.instance = hi2c1,
					.dataPtr = i2c_data
				};

		3. When you need to read/write some data, assign values to the members...
 *
 *
 *
 */


#include "main.h"


I2C_KeyTypeDef i2c_key = {0};


int I2C_InitGeneric(I2C_GenericDef *i2c, I2C_HandleTypeDef *hi2c, volatile uint8_t *data, uint8_t deviceAddress)
{
	// todo - verify arguments are not null

	i2c->instance = hi2c;
	i2c->dataPtr = (uint8_t*)data;
	i2c->deviceAddr  = deviceAddress;
	i2c->transferType = I2C_INTERRUPT_TRANSFER; // default

	i2c_key.keyCount += 1;
	i2c->key = i2c_key.keyCount;

	return ERROR_I2C_REGISTRATION_FULL;
	//return 0;
}


/*
 * Description: Depending on the MCU, it may also have Polling, and DMA.
 * 				You can override the default I2C_INTERRUPT_TRANSFER
 */
int I2C_SetTransferType(I2C_GenericDef *i2c, uint32_t transferType)
{
	i2c->transferType = transferType;

	return 0;
}

/*
 * Description: Receive some data in blocking mode
 *
 */
int I2C_Master_Receive_Generic(I2C_GenericDef *i2c)
{
	I2C_AssignKey(i2c);
	if(HAL_I2C_Master_Receive(i2c->instance, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize, i2c->timeout) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	return 0;
}

/*
 * Description: Transmit some data in blocking mode
 *
 */
int I2C_Master_Transmit_Generic(I2C_GenericDef *i2c)
{
	if(HAL_I2C_Master_Transmit(i2c->instance, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize, i2c->timeout) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	return 0;
}

/*
 * Description: Receive some data in interrupt mode
 *
 */
int I2C_Master_Receive_Generic_IT(I2C_GenericDef *i2c)
{
	I2C_AssignKey(i2c);
	if(HAL_I2C_Master_Receive_IT(i2c->instance, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	return 0;
}

/*
 * Description: Transmit some data in interrupt mode
 *
 */
int I2C_Master_Transmit_Generic_IT(I2C_GenericDef *i2c)
{
	if(HAL_I2C_Master_Transmit_IT(i2c->instance, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	return 0;
}

/*
 * Description: Receive some data in DMA mode
 *
 */
int I2C_Master_Receive_Generi_DMA(I2C_GenericDef *i2c)
{
	I2C_AssignKey(i2c);
	if(HAL_I2C_Master_Receive_DMA(i2c->instance, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	return 0;
}

/*
 * Description: Transmit some data in DMA mode
 *
 */
int I2C_Master_Transmit_Generic_DMA(I2C_GenericDef *i2c)
{
	if(HAL_I2C_Master_Transmit_DMA(i2c->instance, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	return 0;
}

/*
 * Description: Read data from memory/register in blocking mode
 *
 */
int I2C_Mem_Read_Generic(I2C_GenericDef *i2c)
{
	I2C_AssignKey(i2c);
	if(HAL_I2C_Mem_Read(i2c->instance, i2c->deviceAddr, i2c->registerAddr, i2c->regSize, i2c->dataPtr, i2c->dataSize, i2c->timeout) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	return 0;
}

/*
 * Description: Write data to memory/register in blocking mode
 *
 */
int I2C_Mem_Write_Generic(I2C_GenericDef *i2c)
{
	if(HAL_I2C_Mem_Write(i2c->instance, i2c->deviceAddr, i2c->registerAddr, i2c->regSize, i2c->dataPtr, i2c->dataSize, i2c->timeout) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	return 0;
}

/*
 * Description: Read data from memory/register in interrupt mode
 *
 */
int I2C_Mem_Read_Generic_IT(I2C_GenericDef *i2c)
{
	I2C_AssignKey(i2c);
	if(HAL_I2C_Mem_Read_IT(i2c->instance, i2c->deviceAddr, i2c->registerAddr, i2c->regSize, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	return 0;
}

/*
 * Description: Write data to memory/register in interrupt mode
 *
 */
int I2C_Mem_Write_Generic_IT(I2C_GenericDef *i2c)
{
	if(HAL_I2C_Mem_Write_IT(i2c->instance, i2c->deviceAddr, i2c->registerAddr, i2c->regSize, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	return 0;
}

/*
 * Description: Read data from memory/register in DMA mode
 *
 */
int I2C_Mem_Read_Generic_DMA(I2C_GenericDef *i2c)
{
	I2C_AssignKey(i2c);
	if(HAL_I2C_Mem_Read_DMA(i2c->instance, i2c->deviceAddr, i2c->registerAddr, i2c->regSize, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	return 0;
}

/*
 * Description: Write data to memory/register in DMA mode
 *
 */
int I2C_Mem_Write_Generic_DMA(I2C_GenericDef *i2c)
{
	if(HAL_I2C_Mem_Write_DMA(i2c->instance, i2c->deviceAddr, i2c->registerAddr, i2c->regSize, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	return 0;
}

/*
 * Description: When using one of the commands above, we assign the device's key to the Fifo.
 * 				When we get a callback, we check the current Fifo index to see which device's key matches, then call the callback.
 */
int I2C_AssignKey(I2C_GenericDef *i2c)
{
	i2c_key.keyIndex[i2c_key.ptr.index_IN] = i2c->key;
	RingBuff_Ptr_Input(&i2c_key.ptr, I2C_KEY_SIZE);

	return 0;
}
