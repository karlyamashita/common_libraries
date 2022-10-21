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
#include "I2C_Generic_Handler.h"


/**************************************
 * Description: TI 
 *
 **************************************/ 
#ifdef css
/*
 * Description: Receive data
 * 
 * 
 */
int I2C_Master_Receive_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *data, uint8_t readSize)
{
    int status = NO_ERROR;

    switch(i2c_base)
    {
    case 0:
        i2c_base = I2C0_BASE;
        break;
    case 1:
        i2c_base = I2C1_BASE;
        break;
    default:
        return I2C_BASE_NOT_VALID;
    }
    
    status = I2C_Master_Receive(i2c_base, slaveAddress, data, readSize);
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
int I2C_Master_Transmit_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *data, uint8_t writeSize)
{
    int status = NO_ERROR;

    switch(i2c_base)
    {
    case 0:
        i2c_base = I2C0_BASE;
        break;
    case 1:
        i2c_base = I2C1_BASE;
        break;
    default:
        return I2C_BASE_NOT_VALID;
    }

    status = I2C_Master_Transmit(i2c_base, slaveAddress, data, writeSize);
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
int I2C_Master_Read_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *regAddress, uint32_t regSize, uint8_t *data_OUT, uint8_t readSize)
{  
    int status = NO_ERROR;

    switch(i2c_base)
    {
    case 0:
        i2c_base = I2C0_BASE;
        break;
    case 1:
        i2c_base = I2C1_BASE;
        break;
    default:
        return I2C_BASE_NOT_VALID;
    }

    status = I2C_Master_ReadRegister(i2c_base, slaveAddress, regAddress, regSize, data_OUT, readSize);
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
int I2C_Master_Write_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *regAddress, uint32_t regSize, uint8_t *data, uint8_t writeSize)
{
    int status = NO_ERROR;
    uint8_t regData[6] = {0};

    switch(i2c_base)
    {
    case 0:
        i2c_base = I2C0_BASE;
        break;
    case 1:
        i2c_base = I2C1_BASE;
        break;
    default:
        return I2C_BASE_NOT_VALID;
    }

    if(regSize == 2)
    {
        regData[0] = (uint8_t) regAddress[0]; // lower address
        regData[1] = (uint8_t) regAddress[1]; // upper address
        regData[2] = (uint8_t) (*data++);
        regData[3] = (uint8_t) (*data++);
        regData[4] = (uint8_t) (*data++);
        regData[5] = (uint8_t) (*data);
    }
    else
    {
        regData[0] = (uint8_t) regAddress[0]; // lower address
        regData[1] = (uint8_t) (*data++);
        regData[2] = (uint8_t) (*data++);
        regData[3] = (uint8_t) (*data++);
        regData[4] = (uint8_t) (*data);
    }

    status = I2C_Master_Transmit(i2c_base, slaveAddress, regData, regSize + writeSize);
    if(status != 0)
    {
        status = I2C_ERROR_WRITE;
    }

    return status;
}



#endif // end css

/*************************************
 * Description: Microchip 
 *
 *************************************/   
#ifdef _XC_H
/*
 * Description: Receive data
 * 
 * 
 */
int I2C_Master_Receive_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *data, uint8_t readSize)
{
    int status = NO_ERROR;
    uint32_t tickCount;
    
#ifdef _I2C1_H    
    I2C1_MESSAGE_STATUS i2c1_status;
#endif // _I2C1_H
#ifdef _I2C2_H
    I2C2_MESSAGE_STATUS i2c2_status;
#endif // _I2C2_H

#ifdef _I2C1_H 
    if(i2c_base == 1)
    {
        I2C1_MasterRead(data, readSize, slaveAddress, &i2c1_status);
    }
#endif // _I2C1_H 
#ifdef _I2C2_H    
    else if(i2c_base == 2)
    {
        I2C2_MasterRead(data, readSize, slaveAddress, &i2c2_status);
    }
#endif // _I2C2_H    
    tickCount = TMR1_GetTick();
    
#ifdef _I2C1_H
    if(i2c_base == 1)
    {
        while(i2c1_status == I2C1_MESSAGE_PENDING)
        {
            if((TMR1_GetTick() - tickCount) > 100)
            {
                return TIMEOUT;
            }
        }
    }
#endif // _I2C1_H
#ifdef _I2C2_H
    else if(i2c_base == 2)
    {
        while(i2c2_status == I2C2_MESSAGE_PENDING)
        {
            if((TMR1_GetTick() - tickCount) > 100)
            {
                return TIMEOUT;
            }
        }
    }
#endif // _I2C2_H
    return status;
}
 

/*
 * Description: Transmit data
 * 
 * 
 */
int I2C_Master_Transmit_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *data, uint8_t writeSize)
{
    int status = NO_ERROR;
    I2C2_MESSAGE_STATUS i2c_status;
    uint32_t tickCount;
    
    I2C2_MasterWrite(data, writeSize, slaveAddress, &i2c_status);
    
    tickCount = TMR1_GetTick();
    while(i2c_status == I2C2_MESSAGE_PENDING)
    {
        if((TMR1_GetTick() - tickCount) > 100)
        {
            return TIMEOUT;
        }
    }
       
    return status;    
}

/*
 * Description: Reads register data. This will allow of writing to register and then reading with a continue bit in between.
 * Note: Taken from Microchip example code in I2C1.h file
 */
int I2C_Master_Read_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t regAddress, uint8_t *data_OUT, uint8_t readSize)
{
    int status = NO_ERROR;
#ifdef _I2C1_H
    I2C1_MESSAGE_STATUS i2c1_status;
    I2C1_TRANSACTION_REQUEST_BLOCK readTRB1[2];
#endif // _I2C1_H    
#ifdef _I2C2_H    
    I2C2_MESSAGE_STATUS i2c2_status;
    I2C2_TRANSACTION_REQUEST_BLOCK readTRB2[2];
#endif // _I2C2_H     
    uint8_t     writeBuffer[3];
    uint16_t    timeOut, slaveTimeOut;
    
    // this initial value is important
#ifdef _I2C1_H    
    i2c1_status = I2C1_MESSAGE_PENDING;
#endif // _I2C1_H  
#ifdef _I2C2_H    
    i2c2_status = I2C2_MESSAGE_PENDING;
#endif // _I2C2_H     
    
    // build the write buffer first
    // starting address of the EEPROM memory
    //writeBuffer[0] = (regAddress >> 8);                        // high address
    //writeBuffer[1] = (uint8_t)(regAddress);                    // low low address
    writeBuffer[0] = regAddress;
    
    // we need to create the TRBs for a random read sequence to the EEPROM
    // Build TRB for sending address
    if(i2c_base == 1)
    {
#ifdef _I2C1_H      
        I2C1_MasterWriteTRBBuild(&readTRB1[0], writeBuffer, 1, slaveAddress);
        // Build TRB for receiving data
        I2C1_MasterReadTRBBuild(&readTRB1[1], data_OUT, readSize, slaveAddress);
    }
    else if(i2c_base == 2)
    {
#endif // _I2C1_H    
#ifdef _I2C2_H     
        I2C2_MasterWriteTRBBuild(&readTRB2[0], writeBuffer, 1, slaveAddress);
        // Build TRB for receiving data
        I2C2_MasterReadTRBBuild(&readTRB2[1], data_OUT, readSize, slaveAddress);
    }
#endif // _I2C2_H
    timeOut = 0;
    slaveTimeOut = 0;

#ifdef _I2C2_H    
    if(i2c_base == 1)
    {
        while(i2c1_status != I2C1_MESSAGE_FAIL)
        {
            // now send the transactions
            I2C1_MasterTRBInsert(2, readTRB1, &i2c1_status);

            // wait for the message to be sent or status has changed.
            while(i2c1_status == I2C1_MESSAGE_PENDING)
            {
                // add some delay here

                // timeout checking
                // check for max retry and skip this byte
                if (slaveTimeOut == SLAVE_I2C_GENERIC_SLAVE_TIMEOUT)
                    return (0);
                else
                    slaveTimeOut++;
            }

            if (i2c1_status == I2C1_MESSAGE_COMPLETE)
                break;

            // if status is  I2C2_MESSAGE_ADDRESS_NO_ACK,
            //               or I2C2_DATA_NO_ACK,
            // The device may be busy and needs more time for the last
            // write so we can retry writing the data, this is why we
            // use a while loop here

            // check for max retry and skip this byte
            if (timeOut == SLAVE_I2C_GENERIC_RETRY_MAX)
                return (0);
            else
                timeOut++;

        }
    }
    else if(i2c_base == 2)
    {
#endif // _I2C1_H

#ifdef _I2C2_H     
        while(i2c2_status != I2C2_MESSAGE_FAIL)
        {
            // now send the transactions
            I2C2_MasterTRBInsert(2, readTRB2, &i2c2_status);

            // wait for the message to be sent or status has changed.
            while(i2c2_status == I2C2_MESSAGE_PENDING)
            {
                // add some delay here

                // timeout checking
                // check for max retry and skip this byte
                if (slaveTimeOut == SLAVE_I2C_GENERIC_SLAVE_TIMEOUT)
                    return (0);
                else
                    slaveTimeOut++;
            }

            if (i2c2_status == I2C2_MESSAGE_COMPLETE)
                break;

            // if status is  I2C2_MESSAGE_ADDRESS_NO_ACK,
            //               or I2C2_DATA_NO_ACK,
            // The device may be busy and needs more time for the last
            // write so we can retry writing the data, this is why we
            // use a while loop here

            // check for max retry and skip this byte
            if (timeOut == SLAVE_I2C_GENERIC_RETRY_MAX)
                return (0);
            else
                timeOut++;

        }
    }
#endif // _I2C2_H  
    return status;
}
#endif // _XC_H  
