/* 
 * File:   I2C_Generic_Handler.h
 * Author: karl.yamashita
 *
 * Created on August 12, 2022, 12:30 PM
 */

#ifndef I2C_GENERIC_HANDLER_H
#define	I2C_GENERIC_HANDLER_H

#ifdef	__cplusplus
extern "C" {
#endif


enum TransferType_enum
{
    I2C_POLLING,
    I2C_INTERRUPT,
    I2C_DMA
};


#ifdef HAL_MODULE_ENABLED // STM32
typedef struct
{
    uint32_t pin;
    GPIO_TypeDef *port; // port base
}GPIO_CS_t; // chip select
#endif

#ifndef XMEGA
typedef struct __I2C_GenericDef_ // Microchip Studio compiler doesn't work with __I2C_GenericDef_
#else
typedef struct
#endif
{
#ifdef XMEGA
    TWI_t *twi_t;
#endif
#ifdef HAL_MODULE_ENABLED // STM32
    I2C_HandleTypeDef *i2c_instance;
#endif
#ifdef TARGET_IS_TM4C123_RB1
    uint32_t i2c_base;
#endif

    uint8_t deviceAddr; // the slave address of device
    uint8_t *dataPtr; // pointer to data array
    uint8_t registerAddr[2]; // 8 or 16 bit address
    uint32_t dataSize;
    RING_BUFF_STRUCT dataIdxPtr;
    uint32_t regSize; // the size of the registerAddr, 1 or 2 bytes
    uint32_t timeout; // typically for STM32 that uses HAL timeout
    uint32_t transferType; // I2C_POLLING, I2C_INTERRUPT, I2C_DMA
    // TODO - define CS for TI and XMEGA
#ifdef HAL_MODULE_ENABLED // STM32
    GPIO_CS_t cs; // chip select
#endif
#ifndef XMEGA // all but XMEGA can use these callbacks
    void (*RxISR)(struct __I2C_GenericDef_ *i2c);
    void (*TxISR)(struct __I2C_GenericDef_ *i2c); // probably not needed but here nonetheless
#endif
    char *cmdPtr; // pointer to copy of command message
}__attribute__((aligned(32))) I2C_GenericDef;


int I2C_Master_Receive_Generic_Method(I2C_GenericDef *i2c);
int I2C_Master_Transmit_Generic_Method(I2C_GenericDef *i2c);
int I2C_Mem_Read_Generic_Method(I2C_GenericDef *i2c);
int I2C_Mem_Write_Generic_Method(I2C_GenericDef *i2c);



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_GENERIC_HANDLER_H */

