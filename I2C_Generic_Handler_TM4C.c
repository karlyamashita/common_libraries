/*
 * These are I2C read/write function using generic names. 
 * Here you'll define for your specific MCU as each MCU use their own proprietary functions/routines.
 * 
 * 
 * 
 */

#include "main.h"


/*
 * Instructions: User should create a variable of I2C_GenericDef.
 *              User should also create a data buffer and assign to dataPtr
 *              Example below

uint8_t tmp102_dataRx[3]; // REG and 2 data bytes. Increase if register address and register data is larger

I2C_DataStruct tmp102_data_1 =
{
    .i2c_base = I2C0_BASE, // Assign i2c instance
    .slaveAddr = (uint8_t) (TMP102_SLAVE_ADDRESS_00 << 1), // be sure to left shift Slave address
    .cs.port = CS1_GPIO_Port, // Assign PORT
    .cs.pin = CS1_Pin, // Assign Port pin
    .dataPtr = tmp102_dataRx, // Assign Rx/Tx data array
    .timeout = 100, // used in HAL polling timeout
};

 *
 *
 */


// Polling
static int I2C_Master_Receive_Generic(I2C_GenericDef *i2c);
static int I2C_Master_Transmit_Generic(I2C_GenericDef *i2c);
static int I2C_Mem_Read_Generic(I2C_GenericDef *i2c);
static int I2C_Mem_Write_Generic(I2C_GenericDef *i2c);
// Interrupt
static int I2C_Mem_Read_Generic_IT(I2C_GenericDef *i2c);

/*
 * Description: Calling this function will use specific I2C driver, Polling, Interrupt or DMA.
 */
int I2C_Master_Receive_Generic_Method(I2C_GenericDef *i2c)
{
    int status = NO_ERROR;

    switch(i2c->transferType)
    {
        case I2C_POLLING:
            status = I2C_Master_Receive_Generic(i2c);
            if(i2c->RxISR)
            {
                i2c->RxISR(i2c); // There is no interrupt so we manually call the callback
            }
            break;
        case I2C_INTERRUPT:
            //status = I2C_Master_Receive_Generic_IT(i2c);
            break;
        case I2C_DMA:
            //status = I2C_Master_Receive_Generic_DMA(i2c);
            break;
        default:
            status = I2C_Master_Receive_Generic(i2c);
            break;
    }

    return status;
}

/*
 * Description: Calling this function will use specific I2C driver, Polling, Interrupt or DMA.
 */
int I2C_Master_Transmit_Generic_Method(I2C_GenericDef *i2c)
{
    int status = NO_ERROR;

    switch(i2c->transferType)
    {
        case I2C_POLLING:
            status = I2C_Master_Transmit_Generic(i2c);
            if(i2c->TxISR)
            {
                i2c->TxISR(i2c); // There is no interrupt so we manually call the callback
            }
            break;
        case I2C_INTERRUPT:
            //status = I2C_Master_Transmit_Generic_IT(i2c);
            break;
        case I2C_DMA:
            //status = I2C_Master_Transmit_Generic_DMA(i2c);
            break;
        default:
            status = I2C_Master_Transmit_Generic(i2c);
            break;
    }

    return status;
}

/*
 * Description: Calling this function will use specific I2C driver, Polling, Interrupt or DMA.
 */
int I2C_Mem_Read_Generic_Method(I2C_GenericDef *i2c)
{
    int status = NO_ERROR;

    switch(i2c->transferType)
    {
        case I2C_POLLING:
            status = I2C_Mem_Read_Generic(i2c);
            if(i2c->RxISR)
            {
                i2c->RxISR(i2c); // There is no interrupt so we manually call the callback
            }
            break;
        case I2C_INTERRUPT:
            status = I2C_Mem_Read_Generic_IT(i2c);
            break;
        case I2C_DMA:
            //status = I2C_Mem_Read_Generic_DMA(i2c);
            break;
        default:
            status = I2C_Mem_Read_Generic(i2c);
            break;
    }

    return status;
}

/*
 * Description: Calling this function will use specific I2C driver, Polling, Interrupt or DMA.
 */
int I2C_Mem_Write_Generic_Method(I2C_GenericDef *i2c)
{
    int status = NO_ERROR;

    switch(i2c->transferType)
    {
        case I2C_POLLING:
            status = I2C_Mem_Write_Generic(i2c);
            if(i2c->TxISR)
            {
                i2c->TxISR(i2c); // There is no interrupt so we manually call the callback
            }
            break;
        case I2C_INTERRUPT:
            //status = I2C_Mem_Write_Generic_IT(i2c);
            break;
        case I2C_DMA:
            //status = I2C_Mem_Write_Generic_DMA(i2c);
            break;
        default:
            status = I2C_Mem_Write_Generic(i2c);
            break;
    }

    return status;
}


// ****************** Polling ****************


/*
 * Description: Receive data
 * 
 * 
 */
static int I2C_Master_Receive_Generic(I2C_GenericDef *i2c)
{
    int status = NO_ERROR;

    status = I2C_Master_Receive(i2c->i2c_base, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize, i2c->timeout);
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
static int I2C_Master_Transmit_Generic(I2C_GenericDef *i2c)
{
    int status = NO_ERROR;

    status = I2C_Master_Transmit(i2c->i2c_base, i2c->deviceAddr, i2c->dataPtr, i2c->dataSize, i2c->timeout);
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
static int I2C_Mem_Read_Generic(I2C_GenericDef *i2c)
{  
    int status = NO_ERROR;

    status = I2C_Master_ReadRegister(i2c->i2c_base, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->dataPtr, i2c->dataSize, i2c->timeout);
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
static int I2C_Mem_Write_Generic(I2C_GenericDef *i2c)
{
    int status = NO_ERROR;

    status = I2C_Master_WriteRegister(i2c->i2c_base, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->dataPtr, i2c->dataSize, i2c->timeout);
    if(status != 0)
    {
        status = I2C_ERROR_WRITE;
    }

    return status;
}


static int I2C_Mem_Read_Generic_IT(I2C_GenericDef *i2c)
{
    int status = NO_ERROR;

    status = I2C_Master_MemReadRegister(i2c->i2c_base, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->dataPtr, i2c->dataSize);
    if(status != 0)
    {
        status = I2C_ERROR_READ;
    }

    return status;
}

