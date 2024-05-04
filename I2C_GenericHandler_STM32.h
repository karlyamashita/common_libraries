/*
 * I2C_GenericHandler.h
 *
 *  Created on: Apr 24, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_I2C_GENERICHANDLER_STM32_H_
#define INC_I2C_GENERICHANDLER_STM32_H_

// USER Adustable
#define I2C_RX_DATA_SIZE 8
#define I2C_TX_DATA_SIZE 8
#define I2C_RX_QUEUE_SIZE 2
#define I2C_TX_QUEUE_SIZE 2
// End USER Adjustable

typedef struct
{
	I2C_HandleTypeDef *instance;
	struct
	{
		struct
		{
			uint8_t slaveAddr;
			uint8_t data[I2C_RX_DATA_SIZE];
			uint16_t registerAddr;
			uint32_t dataSize;
			uint32_t regSize;
			uint32_t timeout;
			uint32_t timeoutTick;
			uint32_t deviceType;
			bool rxBusy;
		}Queue[I2C_RX_QUEUE_SIZE];
		// RINGBUFF
	}Rx;
	struct
	{
		struct
		{
			uint8_t slaveAddr;
			uint8_t data[I2C_TX_DATA_SIZE];
			uint16_t registerAddr;
			uint32_t dataSize;
			uint32_t regSize;
			uint32_t timeout;
			uint32_t deviceType;
		}Queue[I2C_TX_QUEUE_SIZE];
	// RINGBUFF
	}Tx;
}I2C_DataStruct;



int I2C_Master_Receive_Generic(I2C_DataStruct *msg);
int I2C_Master_Transmit_Generic(I2C_DataStruct *msg);

int I2C_Mem_Read_Generic(I2C_DataStruct *msg);
int I2C_Mem_Write_Generic(I2C_DataStruct *msg);

#endif /* INC_I2C_GENERICHANDLER_STM32_H_ */
