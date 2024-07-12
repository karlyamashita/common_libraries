/*
 * These are I2C read/write function using generic names. 
 * Here you'll define for your specific MCU as each MCU use their own proprietary functions/routines.
 * 
 * 
 * 
 * Note for Microchip: You should have a timer tick routine of 1ms and a call TMR1_GetTick() to get the value.
 *                     MCC should have generated i2c{x}.h file(s) where x is the peripheral I2C number. For this project we have two I2C peripherals being used.
 * 
 */

#include "main.h"


int I2C_InitGeneric(I2C_GenericDef *i2c, uint32_t i2c_base, volatile uint8_t *data, uint8_t deviceAddress)
{
    // todo - verify arguments are not null

    i2c->i2c_base = i2c_base;
    i2c->dataPtr = (uint8_t*)data;
    i2c->deviceAddr = deviceAddress;

    return 0;
}


/*
 * Description: Receive data
 * 
 * 
 */
int I2C_Master_Receive_Generic(I2C_GenericDef *i2c)
{
    int status = NO_ERROR;

    status = I2C_Master_Receive(i2c->i2c_base, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize);
    if(status != 0)
    {
        status = I2C_ERROR_READ;
    }

    return status;
}

/*
 * Description: Transmit data
 * 
 * 
 */
int I2C_Master_Transmit_Generic(I2C_GenericDef *i2c)
{
    int status = NO_ERROR;

    status = I2C_Master_Transmit(i2c->i2c_base, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize);
    if(status != 0)
    {
        status = I2C_ERROR_WRITE;
    }
       
    return status;    
}

/*
 * Description: Reads register data
 * 
 */
int I2C_Mem_Read_Generic(I2C_GenericDef *i2c)
{  
    int status = NO_ERROR;

    status = I2C_Master_ReadRegister(i2c->i2c_base, i2c->deviceAddr, i2c->registerAddr, i2c->regSize, i2c->dataPtr, i2c->dataSize, i2c->timeout);
    if(status != 0)
    {
        status = I2C_ERROR_READ;
    }

    return status;
}

/*
 * Description: Writes register data
 *
 */
int I2C_Mem_Write_Generic(I2C_GenericDef *i2c)
{


    return I2C_Master_Transmit_Generic(i2c);


}


