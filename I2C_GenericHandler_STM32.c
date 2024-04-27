/*
 * I2C_GenericHandler.c
 *
 *  Created on: Apr 24, 2024
 *      Author: karl.yamashita
 */


#include "main.h"

/*
 * Description: Receive some data
 *
 */
int I2C_Master_Receive_Generic(I2C_DataStruct *msg)
{
	if(HAL_I2C_Master_Receive(msg->instance, msg->slaveAddr, msg->Rx.data, msg->Rx.dataSize, msg->Rx.timeout) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	return 0;
}

/*
 * Description: Transmit some data
 *
 */
int I2C_Master_Transmit_Generic(I2C_DataStruct *msg)
{
	if(HAL_I2C_Master_Transmit(msg->instance, msg->slaveAddr, msg->Tx.data, msg->Tx.dataSize, msg->Tx.timeout) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	return 0;
}

/*
 * Description: Read data from memory/register
 *
 */
int I2C_Mem_Read_Generic(I2C_DataStruct *msg)
{
	if(HAL_I2C_Mem_Read(msg->instance, msg->slaveAddr, msg->Rx.registerAddr, msg->Rx.regSize, msg->Rx.data, msg->Rx.dataSize, msg->Rx.timeout) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	return 0;
}

/*
 * Description: Write data to memory/register
 *
 */
int I2C_Mem_Write_Generic(I2C_DataStruct *msg)
{
	if(HAL_I2C_Mem_Write(msg->instance, msg->slaveAddr, msg->Tx.registerAddr, msg->Tx.regSize, msg->Tx.data, msg->Tx.dataSize, msg->Tx.timeout) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	return 0;
}

