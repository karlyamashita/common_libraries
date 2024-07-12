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


typedef void (*I2C_Callback)(void);// no data is passed

typedef struct
{
    uint32_t pin;
    uint32_t port; // port base
}GPIO_CS_t; // chip select

typedef struct __I2C_HandleTypeDef
{
    I2C_TypeDef                *Instance;      /*!< I2C registers base address                */

    volatile uint32_t              i2c_base;

    uint8_t                    *pBuffPtr;      /*!< Pointer to I2C transfer buffer            */

    uint16_t                   XferSize;       /*!< I2C transfer size                         */

    volatile uint16_t              XferCount;      /*!< I2C transfer counter                      */

    volatile uint32_t              XferOptions;    /*!< I2C sequantial transfer options, this parameter can
                                                    be a value of @ref I2C_XFEROPTIONS */

    volatile uint32_t              PreviousState;  /*!< I2C communication Previous state          */

    TM4C_StatusTypeDef(*XferISR)(struct __I2C_HandleTypeDef *ti2c, uint32_t ITFlags, uint32_t ITSources);

    TM4C_LockTypeDef                Lock;           /*!< I2C locking object                        */

    volatile TM4C_I2C_StateTypeDef State;           /*!< I2C communication state                   */

    volatile TM4C_I2C_ModeTypeDef    Mode;          /*!< I2C communication mode                    */

    volatile uint32_t              ErrorCode;      /*!< I2C Error code                            */

    volatile uint32_t              AddrEventCount; /*!< I2C Address Event counter                 */

    volatile uint32_t              Devaddress;     /*!< I2C Target device address                 */

    volatile uint32_t              MemaddressLSB;     /*!< I2C Target memory address MSB                */

    volatile uint32_t               MemaddressMSB;  /*!< I2C Target memory address LSB                */

}I2C_HandleTypeDef;


int I2C_Master_Receive(uint32_t i2c_base, uint16_t DevAddress, uint8_t *pData, uint32_t size);
int I2C_Master_Transmit(uint32_t i2c_base, uint16_t DevAddress, uint8_t *pData, uint32_t size);
int I2C_Master_ReadRegister(uint32_t i2c_base, uint16_t DevAddress, uint16_t regAddress, uint32_t regSize, uint8_t *pData_OUT, uint32_t size, uint32_t timeout);
TM4C_StatusTypeDef TM4C_I2C_Mem_Read_IT(I2C_HandleTypeDef *ti2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);

void TM4C_I2C_EV_IRQHandler(I2C_HandleTypeDef *msg);


int i2c_mem_read(char *retStr);
int TMP108_mem_read(uint16_t slaveAddress, uint8_t *value);

#endif /* I2C_HAL_H_ */
