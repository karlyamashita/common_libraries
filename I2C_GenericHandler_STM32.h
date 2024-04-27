/*
 * I2C_GenericHandler.h
 *
 *  Created on: Apr 24, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_I2C_GENERICHANDLER_STM32_H_
#define INC_I2C_GENERICHANDLER_STM32_H_


enum
{
	I2C_GENERIC_PORT_1 = 1,
	I2C_GENERIC_PORT_2,
	I2C_GENERIC_PORT_3
};


typedef struct
{
	I2C_HandleTypeDef *instance;
	uint8_t slaveAddr;
	struct
	{
		uint8_t *data;
		uint32_t dataSize;
		uint16_t registerAddr;
		uint32_t regSize;
		uint32_t timeout;
	}Rx;
	struct
	{
		uint8_t *data;
		uint32_t dataSize;
		uint16_t registerAddr;
		uint32_t regSize;
		uint32_t timeout;
	}Tx;
}I2C_DataStruct;



int I2C_Master_Receive_Generic(I2C_DataStruct *msg);
int I2C_Master_Transmit_Generic(I2C_DataStruct *msg);

int I2C_Mem_Read_Generic(I2C_DataStruct *msg);
int I2C_Mem_Write_Generic(I2C_DataStruct *msg);

#endif /* INC_I2C_GENERICHANDLER_STM32_H_ */
