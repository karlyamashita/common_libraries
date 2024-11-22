/*
 * i2c_hal.h
 *
 *  Created on: Mar 5, 2021
 *      Author: karl.yamashita
 */

#ifndef I2C_HAL_H_
#define I2C_HAL_H_

#include "main.h"

// I2C defines
#ifndef _I2C_BUFFER_DEFINES
#define _I2C_BUFFER_DEFINES
#define MAX_I2C_DATA_BUFFER 2// 2 bytes, register and data byte
#define MAX_I2C_MESSAGE_BUFFER 2
#endif


#define I2C_TIMEOUT 125

// polling
int I2C_Master_Receive(uint32_t i2c_base, uint16_t DevAddress, uint8_t *pData, uint32_t size, uint32_t timeout);
int I2C_Master_Transmit(uint32_t i2c_base, uint16_t DevAddress, uint8_t *pData, uint32_t size, uint32_t timeout);
int I2C_Master_Mem_Read(uint32_t i2c_base, uint16_t DevAddress, uint16_t regAddress, uint32_t regSize, uint8_t *pData_OUT, uint32_t size, uint32_t timeout);
int I2C_Master_Mem_Write(uint32_t i2c_base, uint16_t DevAddress, uint16_t regAddress, uint32_t regSize, uint8_t *pData_OUT, uint32_t readSize, uint32_t timeout);
// interrupt
int I2C_Master_Mem_Read_Generic_IT(uint32_t i2c_base, uint16_t DevAddress, uint16_t regAddress, uint32_t regSize, uint8_t *pData_OUT, uint32_t readSize);
int I2C_Master_Mem_Write_Generic_IT(uint32_t i2c_base, uint16_t DevAddress, uint16_t regAddress, uint32_t regSize, uint8_t *pData, uint32_t writeSize);

#endif /* I2C_HAL_H_ */
