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

enum I2C_TransferType
{
    I2C_POLLING_TRANSFER,
    I2C_INTERRUPT_TRANSFER,
    I2C_DMA_TRANSFER
};

typedef struct
{
    uint32_t i2c_base;

    uint8_t deviceAddr; // the slave address of device
    uint8_t *dataPtr; // pointer to data array
    uint16_t registerAddr;
    uint32_t dataSize;
    uint32_t regSize;
    uint32_t timeout;
    uint32_t transferType;
}I2C_GenericDef;

int I2C_InitGeneric(I2C_GenericDef *i2c, uint32_t i2c_base, volatile uint8_t *data, uint8_t deviceAddress);
    
int I2C_Master_Receive_Generic(I2C_GenericDef *msg);
int I2C_Master_Transmit_Generic(I2C_GenericDef *msg);

int I2C_Master_Receive_Generic_IT(I2C_GenericDef *msg);
int I2C_Master_Transmit_Generic_IT(I2C_GenericDef *msg);

int I2C_Master_Receive_Generi_DMA(I2C_GenericDef *msg);
int I2C_Master_Transmit_Generic_DMA(I2C_GenericDef *msg);

int I2C_Mem_Read_Generic(I2C_GenericDef *msg);
int I2C_Mem_Write_Generic(I2C_GenericDef *msg);

int I2C_Mem_Read_Generic_IT(I2C_GenericDef *msg);
int I2C_Mem_Write_Generic_IT(I2C_GenericDef *msg);

int I2C_Mem_Read_Generic_DMA(I2C_GenericDef *msg);
int I2C_Mem_Write_Generic_DMA(I2C_GenericDef *msg);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_GENERIC_HANDLER_H */

