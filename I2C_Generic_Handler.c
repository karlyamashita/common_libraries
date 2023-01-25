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

    if(i2c_base == 0)
    {
       i2c_base = I2C0_BASE;
    }
    else if(i2c_base == 1)
    {
        i2c_base = I2C1_BASE;
    }
    // add more bases if needed
    
    status = I2C_Master_Receive(i2c_base, slaveAddress, data, readSize);

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

    if(i2c_base == 0)
    {
       i2c_base = I2C0_BASE;
    }
    else if(i2c_base == 1)
    {
        i2c_base = I2C1_BASE;
    }
    // add more bases if needed
        
    status = I2C_Master_Transmit(i2c_base, slaveAddress, data, writeSize);
       
    return status;    
}

/*
 * Description: Reads register data
 * 
 */
int I2C_Master_Read_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t regAddress, uint8_t *data_OUT, uint8_t readSize)
{  
    int status = NO_ERROR;
    uint16_t DevAddress = LTM46xx_SLAVE_ADDRESS_A;
    uint32_t regSize = 1;
    
    status = I2C_Master_ReadRegister(i2c_base, DevAddress, &regAddress, regSize, data_OUT, readSize);

    return status;
}

#endif // end css

/*************************************
 * Description: Microchip 
 *
 *************************************/   
//#ifdef _XC_H

#ifdef _I2C1_H  
static int GetI2C1_Status(I2C1_MESSAGE_STATUS i2c1_status);
static int GetI2C1_TimeoutStatus(uint32_t tickCountStart);
#endif
#ifdef _I2C2_H
static int GetI2C2_Status(I2C2_MESSAGE_STATUS i2c2_status);
static int GetI2C2_TimeoutStatus(uint32_t tickCountStart);
#endif

#ifdef _I2C1_H
    I2C1_MESSAGE_STATUS i2c1_status;
    I2C1_TRANSACTION_REQUEST_BLOCK readTRB1[2];
#endif // _I2C1_H    
#ifdef _I2C2_H    
    I2C2_MESSAGE_STATUS i2c2_status;
    I2C2_TRANSACTION_REQUEST_BLOCK readTRB2[2];
#endif // _I2C2_H  
    
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
            if((TMR1_GetTick() - tickCount) > SLAVE_I2C_GENERIC_SLAVE_TIMEOUT)
            {
                return I2C_ERROR_TIMEOUT;
            }
        }
    }
#endif // _I2C1_H
#ifdef _I2C2_H
    else if(i2c_base == 2)
    {
        while(i2c2_status == I2C2_MESSAGE_PENDING)
        {
            if((TMR1_GetTick() - tickCount) > SLAVE_I2C_GENERIC_SLAVE_TIMEOUT)
            {
                return I2C_ERROR_TIMEOUT;
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
        
#ifdef _I2C1_H    
    I2C1_MESSAGE_STATUS i2c1_status;
#endif // _I2C1_H
#ifdef _I2C2_H
    I2C2_MESSAGE_STATUS i2c2_status;
#endif // _I2C2_H
    uint32_t tickCount;
    
#ifdef _I2C1_H 
    if(i2c_base == 1)
    {
        I2C1_MasterWrite(data, writeSize, slaveAddress, &i2c1_status);
    }
#endif // _I2C1_H 
#ifdef _I2C2_H    
    else if(i2c_base == 2)
    {
        I2C2_MasterWrite(data, writeSize, slaveAddress, &i2c2_status);
    }
#endif // _I2C2_H 
    
    tickCount = TMR1_GetTick();
#ifdef _I2C1_H
    if(i2c_base == 1)
    {
        while(i2c1_status == I2C1_MESSAGE_PENDING)
        {
            if((TMR1_GetTick() - tickCount) > SLAVE_I2C_GENERIC_SLAVE_TIMEOUT)
            {
                return I2C_ERROR_TIMEOUT;
            }
        }
    }
#endif // _I2C1_H
#ifdef _I2C2_H
    else if(i2c_base == 2)
    {
        while(i2c2_status == I2C2_MESSAGE_PENDING)
        {
            if((TMR1_GetTick() - tickCount) > SLAVE_I2C_GENERIC_SLAVE_TIMEOUT)
            {
                Nop();
                return I2C_ERROR_TIMEOUT;
            }
        }
    }
#endif // _I2C2_H
       
    return status;    
}

/*
 * Description: Reads register data. This will allow of writing to register and then reading with a continue bit in between.
 * Note: Taken from Microchip example code in I2C1.h file
 */
uint8_t  regAddr[4];
int I2C_Master_Read_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *regAddress, uint8_t *data_OUT, uint8_t readSize)
{
    int status = NO_ERROR;
    uint32_t tickCountStart;
    uint32_t retries = 0;
    
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
    regAddr[0] = *regAddress;
    
    // we need to create the TRBs for a random read sequence to the EEPROM
    // Build TRB for sending address
    if(i2c_base == 1)
    {
#ifdef _I2C1_H      
        I2C1_MasterWriteTRBBuild(&readTRB1[0], regAddr, 1, slaveAddress);
        // Build TRB for receiving data
        I2C1_MasterReadTRBBuild(&readTRB1[1], data_OUT, readSize, slaveAddress);
#endif // _I2C1_H     
    }
    else if(i2c_base == 2)
    {
#ifdef _I2C2_H     
        I2C2_MasterWriteTRBBuild(&readTRB2[0], regAddr, 1, slaveAddress);
        // Build TRB for receiving data
        I2C2_MasterReadTRBBuild(&readTRB2[1], data_OUT, readSize, slaveAddress);
#endif // _I2C2_H
    }

    if(i2c_base == 1)
    {
#ifdef _I2C1_H  
        tickCountStart = TMR1_GetTick();
        while(i2c1_status != I2C1_MESSAGE_FAIL)
        {
            // now send the transactions
            I2C1_MasterTRBInsert(2, readTRB1, &i2c1_status);

            // wait for the message to be sent or status has changed.
            while(i2c1_status == I2C1_MESSAGE_PENDING)
            {
                status = GetI2C1_TimeoutStatus(tickCountStart);
                if(status != NO_ERROR)
                {
                    Nop();
                    return I2C_ERROR_TIMEOUT;
                }
            }

            if (i2c1_status == I2C1_MESSAGE_COMPLETE)
                break;

            // if status is  I2C2_MESSAGE_ADDRESS_NO_ACK,
            //               or I2C2_DATA_NO_ACK,
            // The device may be busy and needs more time for the last
            // write so we can retry writing the data, this is why we
            // use a while loop here

            // check for max retry and skip this byte
            if (retries == SLAVE_I2C_GENERIC_RETRY_MAX)
                return I2C_ERROR_RETRIES_EXCEEDED;
            else
                retries++;

        }
        
        status = GetI2C1_Status(i2c1_status);
#endif // _I2C1_H
    }
    else if(i2c_base == 2)
    {
#ifdef _I2C2_H     
        tickCountStart = TMR1_GetTick();
        while(i2c2_status != I2C2_MESSAGE_FAIL)
        {
            // now send the transactions
            I2C2_MasterTRBInsert(2, readTRB2, &i2c2_status);

            //wait for the message to be sent or status has changed.
            while(i2c2_status == I2C2_MESSAGE_PENDING)
            {
                status = GetI2C2_TimeoutStatus(tickCountStart);
                if(status != NO_ERROR)
                {
                    Nop();
                    return I2C_ERROR_TIMEOUT;
                }
            }

            if (i2c2_status == I2C2_MESSAGE_COMPLETE)
            {
                Nop();
                break;
            }

            // if status is  I2C2_MESSAGE_ADDRESS_NO_ACK,
            //               or I2C2_DATA_NO_ACK,
            // The device may be busy and needs more time for the last
            // write so we can retry writing the data, this is why we
            // use a while loop here

            // check for max retry and skip this byte
            if (retries == SLAVE_I2C_GENERIC_RETRY_MAX)
            {
                Nop();
                return I2C_ERROR_RETRIES_EXCEEDED;
            }
            else
                retries++;

        }
        Nop();
        status = GetI2C2_Status(i2c2_status);
#endif // _I2C2_H          
    }

    return status;
}

int I2C_Master_Write_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *regAddress, uint8_t *data_IN, uint8_t writeSize)
{
    int status = NO_ERROR;
    uint32_t tickCountStart;
    uint32_t retries = 0;
    
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
    regAddr[0] = *regAddress;
    regAddr[1] = data_IN[0];
    regAddr[2] = data_IN[1];
    
    // we need to create the TRBs for a random read sequence to the EEPROM
    // Build TRB for sending address
    if(i2c_base == 1)
    {
#ifdef _I2C1_H      
        I2C1_MasterWriteTRBBuild(&readTRB1[0], regAddr, 1 + writeSize, slaveAddress);
#endif // _I2C1_H     
    }
    else if(i2c_base == 2)
    {
#ifdef _I2C2_H     
        I2C2_MasterWriteTRBBuild(&readTRB2[0], regAddr, 1 + writeSize, slaveAddress);
#endif // _I2C2_H
    }

    if(i2c_base == 1)
    {
#ifdef _I2C1_H  
        tickCountStart = TMR1_GetTick();
        while(i2c1_status != I2C1_MESSAGE_FAIL)
        {
            // now send the transactions
            I2C1_MasterTRBInsert(1, readTRB1, &i2c1_status);

            // wait for the message to be sent or status has changed.
            while(i2c1_status == I2C1_MESSAGE_PENDING)
            {
                status = GetI2C1_TimeoutStatus(tickCountStart);
                if(status != NO_ERROR)
                {
                    Nop();
                    return I2C_ERROR_TIMEOUT;
                }
            }

            if (i2c1_status == I2C1_MESSAGE_COMPLETE)
                break;

            // if status is  I2C2_MESSAGE_ADDRESS_NO_ACK,
            //               or I2C2_DATA_NO_ACK,
            // The device may be busy and needs more time for the last
            // write so we can retry writing the data, this is why we
            // use a while loop here

            // check for max retry and skip this byte
            if (retries == SLAVE_I2C_GENERIC_RETRY_MAX)
                return I2C_ERROR_RETRIES_EXCEEDED;
            else
                retries++;

        }
        
        status = GetI2C1_Status(i2c1_status);
#endif // _I2C1_H
    }
    else if(i2c_base == 2)
    {
#ifdef _I2C2_H     
        tickCountStart = TMR1_GetTick();
        while(i2c2_status != I2C2_MESSAGE_FAIL)
        {
            // now send the transactions
            I2C2_MasterTRBInsert(1, readTRB2, &i2c2_status);

            //wait for the message to be sent or status has changed.
            while(i2c2_status == I2C2_MESSAGE_PENDING)
            {
                status = GetI2C2_TimeoutStatus(tickCountStart);
                if(status != NO_ERROR)
                {
                    Nop();
                    return I2C_ERROR_TIMEOUT;
                }
            }

            if (i2c2_status == I2C2_MESSAGE_COMPLETE)
            {
                Nop();
                break;
            }

            // if status is  I2C2_MESSAGE_ADDRESS_NO_ACK,
            //               or I2C2_DATA_NO_ACK,
            // The device may be busy and needs more time for the last
            // write so we can retry writing the data, this is why we
            // use a while loop here

            // check for max retry and skip this byte
            if (retries == SLAVE_I2C_GENERIC_RETRY_MAX)
            {
                Nop();
                return I2C_ERROR_RETRIES_EXCEEDED;
            }
            else
                retries++;

        }
        Nop();
        status = GetI2C2_Status(i2c2_status);
#endif // _I2C2_H          
    }

    return status;
}

/*
 * Description: use for reading a paged register
 */
int I2C_Master_Page_Plus_Read_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t page, uint8_t *regAddress, uint8_t *data_OUT, uint8_t readSize)
{
    int status = NO_ERROR;
    uint32_t tickCountStart;
    uint32_t retries = 0;    
    
    // this initial value is important
#ifdef _I2C1_H    
    i2c1_status = I2C1_MESSAGE_PENDING;
#endif // _I2C1_H  
#ifdef _I2C2_H    
    i2c2_status = I2C2_MESSAGE_PENDING;
#endif // _I2C2_H  
    
    regAddr[0] = PAGE_PLUS_READ;
    regAddr[1] = 2; // block count
    regAddr[2] = page; // page number
    regAddr[3] = *regAddress;
    
    
    // we need to create the TRBs for a random read sequence to the EEPROM
    // Build TRB for sending address
    if(i2c_base == 1)
    {
#ifdef _I2C1_H      
        I2C1_MasterWriteTRBBuild(&readTRB1[0], regAddr, 4, slaveAddress);
        // Build TRB for receiving data
        I2C1_MasterReadTRBBuild(&readTRB1[1], data_OUT, 1 + readSize, slaveAddress);
#endif // _I2C1_H     
    }
    else if(i2c_base == 2)
    {
#ifdef _I2C2_H     
        I2C2_MasterWriteTRBBuild(&readTRB2[0], regAddr, 4, slaveAddress);
        // Build TRB for receiving data
        I2C2_MasterReadTRBBuild(&readTRB2[1], data_OUT, 1 + readSize, slaveAddress);
#endif // _I2C2_H
    }    
    
    if(i2c_base == 1)
    {
#ifdef _I2C1_H  
        tickCountStart = TMR1_GetTick();
        while(i2c1_status != I2C1_MESSAGE_FAIL)
        {
            // now send the transactions
            I2C1_MasterTRBInsert(2, readTRB1, &i2c1_status);

            // wait for the message to be sent or status has changed.
            while(i2c1_status == I2C1_MESSAGE_PENDING)
            {
                status = GetI2C1_TimeoutStatus(tickCountStart);
                if(status != NO_ERROR)
                {
                    Nop();
                    return I2C_ERROR_TIMEOUT;
                }
            }

            if (i2c1_status == I2C1_MESSAGE_COMPLETE)
                break;

            // if status is  I2C2_MESSAGE_ADDRESS_NO_ACK,
            //               or I2C2_DATA_NO_ACK,
            // The device may be busy and needs more time for the last
            // write so we can retry writing the data, this is why we
            // use a while loop here

            // check for max retry and skip this byte
            if (retries == SLAVE_I2C_GENERIC_RETRY_MAX)
                return I2C_ERROR_RETRIES_EXCEEDED;
            else
                retries++;

        }
        
        status = GetI2C1_Status(i2c1_status);
#endif // _I2C1_H
    }
    else if(i2c_base == 2)
    {
#ifdef _I2C2_H     
        tickCountStart = TMR1_GetTick();
        while(i2c2_status != I2C2_MESSAGE_FAIL)
        {
            // now send the transactions
            I2C2_MasterTRBInsert(2, readTRB2, &i2c2_status);

            //wait for the message to be sent or status has changed.
            while(i2c2_status == I2C2_MESSAGE_PENDING)
            {
                status = GetI2C2_TimeoutStatus(tickCountStart);
                if(status != NO_ERROR)
                {
                    Nop();
                    return I2C_ERROR_TIMEOUT;
                }
            }

            if (i2c2_status == I2C2_MESSAGE_COMPLETE)
            {
                Nop();
                break;
            }

            // if status is  I2C2_MESSAGE_ADDRESS_NO_ACK,
            //               or I2C2_DATA_NO_ACK,
            // The device may be busy and needs more time for the last
            // write so we can retry writing the data, this is why we
            // use a while loop here

            // check for max retry and skip this byte
            if (retries == SLAVE_I2C_GENERIC_RETRY_MAX)
            {
                Nop();
                return I2C_ERROR_RETRIES_EXCEEDED;
            }
            else
                retries++;

        }
        Nop();
        status = GetI2C2_Status(i2c2_status);
#endif // _I2C2_H          
    }
    
    return status;
}

/*
 * Description: Used for writing to a paged register
 */
int I2C_Master_Page_Plus_Write_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t page, uint8_t *regAddress, uint8_t *dataIn, uint8_t dataSize)
{
    int status = NO_ERROR;
    uint32_t tickCountStart;
    uint32_t retries = 0;    
    
    // this initial value is important
#ifdef _I2C1_H    
    i2c1_status = I2C1_MESSAGE_PENDING;
#endif // _I2C1_H  
#ifdef _I2C2_H    
    i2c2_status = I2C2_MESSAGE_PENDING;
#endif // _I2C2_H  
    
    regAddr[0] = PAGE_PLUS_WRITE;
    regAddr[1] = 2 + dataSize; // block count
    regAddr[2] = page; // page number
    regAddr[3] = *regAddress;
    regAddr[4] = dataIn[0];
    regAddr[5] = dataIn[1];
    
    
    // we need to create the TRBs for a random read sequence to the EEPROM
    // Build TRB for sending address
    if(i2c_base == 1)
    {
#ifdef _I2C1_H      
        I2C1_MasterWriteTRBBuild(&readTRB1[0], regAddr, 4 + dataSize, slaveAddress);
        // Build TRB for receiving data
        //I2C1_MasterReadTRBBuild(&readTRB1[1], data_OUT, 1 + dataSize, slaveAddress);
#endif // _I2C1_H     
    }
    else if(i2c_base == 2)
    {
#ifdef _I2C2_H     
        I2C2_MasterWriteTRBBuild(&readTRB2[0], regAddr, 4 + dataSize, slaveAddress);
        // Build TRB for receiving data
       // I2C2_MasterReadTRBBuild(&readTRB2[1], data_OUT, 1 + dataSize, slaveAddress);
#endif // _I2C2_H
    }    
    
    if(i2c_base == 1)
    {
#ifdef _I2C1_H  
        tickCountStart = TMR1_GetTick();
        while(i2c1_status != I2C1_MESSAGE_FAIL)
        {
            // now send the transactions
            I2C1_MasterTRBInsert(1, readTRB1, &i2c1_status);

            // wait for the message to be sent or status has changed.
            while(i2c1_status == I2C1_MESSAGE_PENDING)
            {
                status = GetI2C1_TimeoutStatus(tickCountStart);
                if(status != NO_ERROR)
                {
                    Nop();
                    return I2C_ERROR_TIMEOUT;
                }
            }

            if (i2c1_status == I2C1_MESSAGE_COMPLETE)
                break;

            // if status is  I2C2_MESSAGE_ADDRESS_NO_ACK,
            //               or I2C2_DATA_NO_ACK,
            // The device may be busy and needs more time for the last
            // write so we can retry writing the data, this is why we
            // use a while loop here

            // check for max retry and skip this byte
            if (retries == SLAVE_I2C_GENERIC_RETRY_MAX)
                return I2C_ERROR_RETRIES_EXCEEDED;
            else
                retries++;

        }
        
        status = GetI2C1_Status(i2c1_status);
#endif // _I2C1_H
    }
    else if(i2c_base == 2)
    {
#ifdef _I2C2_H     
        tickCountStart = TMR1_GetTick();
        while(i2c2_status != I2C2_MESSAGE_FAIL)
        {
            // now send the transactions
            I2C2_MasterTRBInsert(1, readTRB2, &i2c2_status);

            //wait for the message to be sent or status has changed.
            while(i2c2_status == I2C2_MESSAGE_PENDING)
            {
                status = GetI2C2_TimeoutStatus(tickCountStart);
                if(status != NO_ERROR)
                {
                    Nop();
                    return I2C_ERROR_TIMEOUT;
                }
            }

            if (i2c2_status == I2C2_MESSAGE_COMPLETE)
            {
                Nop();
                break;
            }

            // if status is  I2C2_MESSAGE_ADDRESS_NO_ACK,
            //               or I2C2_DATA_NO_ACK,
            // The device may be busy and needs more time for the last
            // write so we can retry writing the data, this is why we
            // use a while loop here

            // check for max retry and skip this byte
            if (retries == SLAVE_I2C_GENERIC_RETRY_MAX)
            {
                Nop();
                return I2C_ERROR_RETRIES_EXCEEDED;
            }
            else
                retries++;

        }
        Nop();
        status = GetI2C2_Status(i2c2_status);
#endif // _I2C2_H          
    }
    
    return status;
}

#ifdef _I2C1_H 
static int GetI2C1_TimeoutStatus(uint32_t tickCountStart)
{
    int status = NO_ERROR;
    uint32_t tickCountNow = 0;
    
    tickCountNow = TMR1_GetTick();
    if(tickCountNow >= tickCountStart)
    {
        if((tickCountNow - tickCountStart) > SLAVE_I2C_GENERIC_SLAVE_TIMEOUT)
        {
            status = I2C_ERROR_TIMEOUT;
        }
    }
    else
    {
        if((tickCountStart - tickCountNow) > SLAVE_I2C_GENERIC_SLAVE_TIMEOUT)
        {
            status = I2C_ERROR_TIMEOUT;
        }     
    }
    
    return status;
}

static int GetI2C1_Status(I2C1_MESSAGE_STATUS i2c1_status)
{
    int status = NO_ERROR;
    
    switch(i2c1_status)
    {
       
        case I2C1_MESSAGE_FAIL:
            status = I2C_ERROR_MESSAGE_FAIL;
            break;
        case I2C1_MESSAGE_PENDING:
            
            break;
        case I2C1_STUCK_START:
            
            break;
        case I2C1_MESSAGE_ADDRESS_NO_ACK:
            
            break;
        case I2C1_DATA_NO_ACK:
            
            break;
        case I2C1_LOST_STATE:
            
            break;
        case I2C1_MESSAGE_COMPLETE:
            status = NO_ERROR;
            break;
        default:
            status = I2C_ERROR_MESSAGE_FAIL;
            break;
    }  
    
    return status;
}

#endif

#ifdef _I2C2_H 
static int GetI2C2_TimeoutStatus(uint32_t tickCountStart)
{
    int status = NO_ERROR;
    uint32_t tickCountNow = 0;
    
    tickCountNow = TMR1_GetTick();
    if(tickCountNow >= tickCountStart)
    {
        if((tickCountNow - tickCountStart) > SLAVE_I2C_GENERIC_SLAVE_TIMEOUT)
        {
            status = I2C_ERROR_TIMEOUT;
        }
    }
    else
    {
        if((tickCountStart - tickCountNow) > SLAVE_I2C_GENERIC_SLAVE_TIMEOUT)
        {
            status = I2C_ERROR_TIMEOUT;
        }     
    }    
    
    return status;
}

static int GetI2C2_Status(I2C2_MESSAGE_STATUS i2c2_status)
{
    int status = NO_ERROR;
    
    switch(i2c2_status)
    {
        case I2C2_MESSAGE_FAIL:
          status = I2C_ERROR_MESSAGE_FAIL;
            break;
        case I2C2_MESSAGE_PENDING:
            
            break;
        case I2C2_STUCK_START:
            status = I2C_ERROR_STUCK_START;
            break;
        case I2C2_MESSAGE_ADDRESS_NO_ACK:
            
            break;
        case I2C2_DATA_NO_ACK:
            
            break;
        case I2C2_LOST_STATE:
            
            break;
        case I2C2_MESSAGE_COMPLETE:
            status = NO_ERROR;
            break;
        default:
            status = I2C_ERROR_MESSAGE_FAIL;
            break;
    }  
    
    return status;
}

int I2C_2_ChangeBaudRate(uint32_t i2c_base, uint16_t baudRate)
{
    int status = NO_ERROR;

    I2C2BRG = baudRate;
    
    return status;
}

#endif

int I2C_ChangeBaudRate(uint32_t i2c_base, uint16_t baudRate)
{
    int status = NO_ERROR;

#ifdef _I2C1_H 
    if(i2c_base == 1)
    {
        I2C1BRG = baudRate;
    }
#endif
#ifdef _I2C2_H
    else if(i2c_base == 2)
    {
        I2C2BRG = baudRate;
    }
#endif
    return status;
}


//#endif // _XC_H  
