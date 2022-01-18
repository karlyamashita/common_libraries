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


typedef union {
    struct{
        uint8_t data[2];
    }Data;
    struct{
        uint8_t i2c_register;
        uint8_t data;
    }Status;
}I2C_DataStruct;


HAL_StatusTypeDef I2C_Master_Receive(uint32_t i2c_base, uint16_t DevAddress, uint8_t *pData, uint32_t size);
HAL_StatusTypeDef I2C_Master_Transmit(uint32_t i2c_base, uint16_t DevAddress, uint8_t *pData, uint32_t size);
HAL_StatusTypeDef I2C_Master_ReadRegister(uint32_t i2c_base, uint16_t DevAddress, uint8_t *regAddress, uint32_t regSize, uint8_t *pData_OUT, uint32_t size);
HAL_StatusTypeDef I2C_EV_IRQHandler(uint32_t i2c_base);


#endif /* I2C_HAL_H_ */
