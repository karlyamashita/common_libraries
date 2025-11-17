/*
 * I2C_Queue_STM32.h
 *
 *  Created on: Oct 21, 2025
 *      Author: karl.yamashita
 */

#ifndef INC_I2C_QUEUE_STM32_H_
#define INC_I2C_QUEUE_STM32_H_


typedef struct __I2C_GenericQueueDef
{
	I2C_HandleTypeDef *i2c_instance;

	uint8_t deviceAddr; // the slave address of device
	uint8_t data[8];
	uint8_t registerAddr[2]; // 8 or 16 bit address
	uint32_t dataSize;
	uint32_t regSize; // the size of the registerAddr, 1 or 2 bytes
	uint32_t timeout; // typically for STM32 that uses HAL timeout
	uint32_t transferType; // I2C_POLLING, I2C_INTERRUPT, I2C_DMA
	uint32_t readWriteType; //
	bool pending;

	void (*RxISR)(struct __I2C_GenericQueueDef *i2c);
	void (*TxISR)(struct __I2C_GenericQueueDef *i2c); // probably not needed but here nonetheless
	char *cmdPtr; // pointer to copy of command message
}I2C_GenericQueueDef;

typedef struct
{
	I2C_GenericQueueDef *queueData; // pointer to queue buffer
	I2C_GenericQueueDef *msgToParse; // pointer to message to write/read
	RING_BUFF_STRUCT ptr;
	uint32_t queueSize;
}I2C_QueueStruct_t;

int I2C_QUEUE_Add(I2C_QueueStruct_t *queue, I2C_GenericQueueDef *i2c);
int I2C_QUEUE_Transmit(I2C_QueueStruct_t *queue);

int I2C_QUEUE_Master_Receive_Generic_Method(I2C_GenericQueueDef *i2c);
int I2C_QUEUE_Master_Transmit_Generic_Method(I2C_GenericQueueDef *i2c);
int I2C_QUEUE_Mem_Read_Generic_Method(I2C_GenericQueueDef *i2c);
int I2C_QUEUE_Mem_Write_Generic_Method(I2C_GenericQueueDef *i2c);

#endif /* INC_I2C_QUEUE_STM32_H_ */
