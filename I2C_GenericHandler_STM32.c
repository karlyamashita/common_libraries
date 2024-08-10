/*
 * I2C_GenericHandler.c
 *
 *  Created on: Apr 24, 2024
 *      Author: karl.yamashita
 */


#include "main.h"


/*
 * Instructions: User should create a variable of I2C_GenericDef.
 * 				User should also create a data buffer and assign to dataPtr
 * 				Example below

uint8_t tmp102_dataRx[3]; // REG and 2 data bytes. Increase if register address and register data is larger

I2C_DataStruct tmp102_data_1 =
{
	.instance = &hi2c1, // Assign i2c instance
	.slaveAddr = (uint8_t) (TMP102_SLAVE_ADDRESS_00 << 1), // be sure to left shift Slave address
	.cs.port = CS1_GPIO_Port, // Assign PORT
	.cs.pin = CS1_Pin, // Assign Port pin
	.dataPtr = tmp102_dataRx, // Assign Rx/Tx data array
	.timeout = 100, // used in HAL polling timeout
};

 *
 *
 */

// Prototypes
// Polling
static int I2C_Master_Receive_Generic(I2C_GenericDef *i2c);
static int I2C_Master_Transmit_Generic(I2C_GenericDef *i2c);
static int I2C_Mem_Read_Generic(I2C_GenericDef *i2c);
static int I2C_Mem_Write_Generic(I2C_GenericDef *i2c);

// interrupt
static int I2C_Master_Receive_Generic_IT(I2C_GenericDef *i2c);
static int I2C_Master_Transmit_Generic_IT(I2C_GenericDef *i2c);
static int I2C_Mem_Read_Generic_IT(I2C_GenericDef *i2c);
static int I2C_Mem_Write_Generic_IT(I2C_GenericDef *i2c);

// DMA
static int I2C_Master_Receive_Generic_DMA(I2C_GenericDef *i2c);
static int I2C_Master_Transmit_Generic_DMA(I2C_GenericDef *i2c);
static int I2C_Mem_Read_Generic_DMA(I2C_GenericDef *i2c);
static int I2C_Mem_Write_Generic_DMA(I2C_GenericDef *i2c);


/*
 * Description: Calling this function will use specific I2C driver, Polling, Interrupt or DMA.
 */
int I2C_Master_Receive_Generic_Method(I2C_GenericDef *i2c)
{
	int status = NO_ERROR;

	switch(i2c->transferType)
	{
		case I2C_POLLING:
			status = I2C_Master_Receive_Generic(i2c);
			if(i2c->RxISR)
			{
				i2c->RxISR(i2c); // There is no interrupt so we manually call the callback
			}
			break;
		case I2C_INTERRUPT:
			status = I2C_Master_Receive_Generic_IT(i2c);
			break;
		case I2C_DMA:
			status = I2C_Master_Receive_Generic_DMA(i2c);
			break;
		default:
			status = I2C_Master_Receive_Generic(i2c);
			break;
	}

	return status;
}

/*
 * Description: Calling this function will use specific I2C driver, Polling, Interrupt or DMA.
 */
int I2C_Master_Transmit_Generic_Method(I2C_GenericDef *i2c)
{
	int status = NO_ERROR;

	switch(i2c->transferType)
	{
		case I2C_POLLING:
			status = I2C_Master_Transmit_Generic(i2c);
			if(i2c->TxISR)
			{
				i2c->TxISR(i2c); // There is no interrupt so we manually call the callback
			}
			break;
		case I2C_INTERRUPT:
			status = I2C_Master_Transmit_Generic_IT(i2c);
			break;
		case I2C_DMA:
			status = I2C_Master_Transmit_Generic_DMA(i2c);
			break;
		default:
			status = I2C_Master_Transmit_Generic(i2c);
			break;
	}

	return status;
}

/*
 * Description: Calling this function will use specific I2C driver, Polling, Interrupt or DMA.
 */
int I2C_Mem_Read_Generic_Method(I2C_GenericDef *i2c)
{
	int status = NO_ERROR;

	switch(i2c->transferType)
	{
		case I2C_POLLING:
			status = I2C_Mem_Read_Generic(i2c);
			if(status == NO_ERROR)
			{
				if(i2c->RxISR)
				{
					i2c->RxISR(i2c); // There is no interrupt so we manually call the callback
				}
			}
			break;
		case I2C_INTERRUPT:
			status = I2C_Mem_Read_Generic_IT(i2c);
			break;
		case I2C_DMA:
			status = I2C_Mem_Read_Generic_DMA(i2c);
			break;
		default:
			status = I2C_Mem_Read_Generic(i2c);
			break;
	}

	return status;
}

/*
 * Description: Calling this function will use specific I2C driver, Polling, Interrupt or DMA.
 */
int I2C_Mem_Write_Generic_Method(I2C_GenericDef *i2c)
{
	int status = NO_ERROR;

	switch(i2c->transferType)
	{
		case I2C_POLLING:
			status = I2C_Mem_Write_Generic(i2c);
			if(status == NO_ERROR)
			{
				if(i2c->TxISR)
				{
					i2c->TxISR(i2c); // There is no interrupt so we manually call the callback
				}
			}
			break;
		case I2C_INTERRUPT:
			status = I2C_Mem_Write_Generic_IT(i2c);
			break;
		case I2C_DMA:
			status = I2C_Mem_Write_Generic_DMA(i2c);
			break;
		default:
			status = I2C_Mem_Write_Generic(i2c);
			break;
	}

	return status;
}


// ****************** Polling ****************

/*
 * Description: Receive some data with polling
 *
 */
static int I2C_Master_Receive_Generic(I2C_GenericDef *i2c)
{
	// enable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0) // use CS
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_RESET); // pull CS pin low
	}

	if(HAL_I2C_Master_Receive(i2c->i2c_instance, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize, i2c->timeout) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	// Disable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0)
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_SET); // set CS pin high
	}

	return 0;
}

/*
 * Description: Transmit some data with polling
 *
 */
static int I2C_Master_Transmit_Generic(I2C_GenericDef *i2c)
{
	// enable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0) // use CS
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_RESET); // pull CS pin low
	}

	if(HAL_I2C_Master_Transmit(i2c->i2c_instance, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize, i2c->timeout) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	// Disable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0)
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_SET); // set CS pin high
	}

	return 0;
}



/*
 * Description: Read data from memory/register using polling
 *
 */
static int I2C_Mem_Read_Generic(I2C_GenericDef *i2c)
{
	// enable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0) // use CS
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_RESET); // pull CS pin low
	}

	if(HAL_I2C_Mem_Read(i2c->i2c_instance, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->dataPtr, i2c->dataSize, i2c->timeout) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	// Disable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0)
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_SET); // set CS pin high
	}

	return 0;
}

/*
 * Description: Write data to memory/register using polling
 *
 */
static int I2C_Mem_Write_Generic(I2C_GenericDef *i2c)
{
	// enable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0) // use CS
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_RESET); // pull CS pin low
	}

	if(HAL_I2C_Mem_Write(i2c->i2c_instance, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->dataPtr, i2c->dataSize, i2c->timeout) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	// Disable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0)
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_SET); // set CS pin high
	}

	return 0;
}

// ***************** Interrupt ******************

/*
 * Description: Receive some data with interrupt
 *
 */
static int I2C_Master_Receive_Generic_IT(I2C_GenericDef *i2c)
{
	// enable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0) // use CS
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_RESET); // pull CS pin low
	}

	if(HAL_I2C_Master_Receive_IT(i2c->i2c_instance, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	// Disable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0)
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_SET); // set CS pin high
	}

	return 0;
}

/*
 * Description: Transmit some data with interrupt
 *
 */
static int I2C_Master_Transmit_Generic_IT(I2C_GenericDef *i2c)
{
	// enable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0) // use CS
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_RESET); // pull CS pin low
	}

	if(HAL_I2C_Master_Transmit_IT(i2c->i2c_instance, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	// Disable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0)
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_SET); // set CS pin high
	}

	return 0;
}

/*
 * Description: Read memory/register in interrupt mode. Be sure to assign a callback function.
 */
static int I2C_Mem_Read_Generic_IT(I2C_GenericDef *i2c)
{
	// enable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0) // use CS
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_RESET); // pull CS pin low
	}

	// Read memory
	if(HAL_I2C_Mem_Read_IT(i2c->i2c_instance, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	// Disable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0)
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_SET); // set CS pin high
	}

	return 0;
}

/*
 * Description: Write memory/register in interrupt mode
 */
static int I2C_Mem_Write_Generic_IT(I2C_GenericDef *i2c)
{
	// enable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0) // use CS
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_RESET); // pull CS pin low
	}

	if(HAL_I2C_Mem_Write_IT(i2c->i2c_instance, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	// Disable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0)
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_SET); // set CS pin high
	}

	return 0;
}

// ****************** DMA *************************
/*
 * Description: Transmit some data with DMA
 *
 */
static int I2C_Master_Transmit_Generic_DMA(I2C_GenericDef *i2c)
{
	// enable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0) // use CS
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_RESET); // pull CS pin low
	}

	if(HAL_I2C_Master_Transmit_DMA(i2c->i2c_instance, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	// Disable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0)
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_SET); // set CS pin high
	}

	return 0;
}

/*
 * Description: Receive some data with DMA
 *
 */
static int I2C_Master_Receive_Generic_DMA(I2C_GenericDef *i2c)
{
	// enable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0) // use CS
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_RESET); // pull CS pin low
	}

	if(HAL_I2C_Master_Receive_DMA(i2c->i2c_instance, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	// Disable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0)
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_SET); // set CS pin high
	}

	return 0;
}

/*
 * Description: Read memory/register in DMA mode. Be sure to assign a callback function.
 */
static int I2C_Mem_Read_Generic_DMA(I2C_GenericDef *i2c)
{
	// enable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0) // use CS
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_RESET); // pull CS pin low
	}

	// Read memory
	if(HAL_I2C_Mem_Read_DMA(i2c->i2c_instance, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	// Disable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0)
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_SET); // set CS pin high
	}

	return 0;
}

/*
 * Description: Write memory/register in DMA mode
 */
static int I2C_Mem_Write_Generic_DMA(I2C_GenericDef *i2c)
{
	// enable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0) // use CS
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_RESET); // pull CS pin low
	}

	if(HAL_I2C_Mem_Write_DMA(i2c->i2c_instance, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->dataPtr, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	// Disable CS
	if(i2c->cs.port != NULL && i2c->cs.pin != 0)
	{
		HAL_GPIO_WritePin(i2c->cs.port, i2c->cs.pin, GPIO_PIN_SET); // set CS pin high
	}

	return 0;
}
