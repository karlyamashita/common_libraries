/*
 * I2C_GenericHandler.h
 *
 *  Created on: Apr 24, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_I2C_GENERICHANDLER_STM32_H_
#define INC_I2C_GENERICHANDLER_STM32_H_


// this should be defined in ErrorStatus.h, if not then assign 100 to avoid build error
#ifndef I2C_REGISTRATION_FULL
#define ERROR_I2C_REGISTRATION_FULL 100
#endif

#ifndef I2C_KEY_SIZE
#define I2C_KEY_SIZE 5 // default 5. Define in main.h to override this value
#endif

enum I2C_TransferType
{
	I2C_POLLING_TRANSFER,
	I2C_INTERRUPT_TRANSFER,
	I2C_DMA_TRANSFER
};

typedef struct __I2C_GenericDef
{
	I2C_HandleTypeDef *instance;

	uint8_t deviceAddr; // the slave address of device
	uint8_t *dataPtr; // pointer to data array
	uint16_t registerAddr; // register address
	uint32_t dataSize; // data size
	uint32_t regSize; //register size
	uint32_t timeout; // timeout in ms
	uint32_t transferType; // polling, interrupt, DMA
	// There can be multiple devices on the same I2C port.
	// So we need a key identifier for each device to distinguish them apart when getting an interrupt callback.
	uint32_t key;
}I2C_GenericDef;

typedef struct
{
	uint32_t keyCount; // keep track of how many I2C devices are registered
	uint32_t keyIndex[I2C_KEY_SIZE];
	RING_BUFF_STRUCT ptr;
}I2C_KeyTypeDef;


int I2C_InitGeneric(I2C_GenericDef *i2c, I2C_HandleTypeDef *hi2c, volatile uint8_t *data, uint8_t deviceAddress);
int I2C_SetTransferType(I2C_GenericDef *i2c, uint32_t transferType);

int I2C_Master_Receive_Generic(I2C_GenericDef *i2c);
int I2C_Master_Transmit_Generic(I2C_GenericDef *i2c);

int I2C_Master_Receive_Generic_IT(I2C_GenericDef *i2c);
int I2C_Master_Transmit_Generic_IT(I2C_GenericDef *i2c);

int I2C_Master_Receive_Generi_DMA(I2C_GenericDef *i2c);
int I2C_Master_Transmit_Generic_DMA(I2C_GenericDef *i2c);

int I2C_Mem_Read_Generic(I2C_GenericDef *i2c);
int I2C_Mem_Write_Generic(I2C_GenericDef *i2c);

int I2C_Mem_Read_Generic_IT(I2C_GenericDef *i2c);
int I2C_Mem_Write_Generic_IT(I2C_GenericDef *i2c);

int I2C_Mem_Read_Generic_DMA(I2C_GenericDef *i2c);
int I2C_Mem_Write_Generic_DMA(I2C_GenericDef *i2c);

int I2C_AssignKey(I2C_GenericDef *i2c);

#endif /* INC_I2C_GENERICHANDLER_STM32_H_ */
