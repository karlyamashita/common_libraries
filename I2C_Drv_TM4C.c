/*
 * i2c_hal.c
 *
 *  Created on: Mar 5, 2021
 *      Author: karl.yamashita
 *
 *      For TI's I2C driver
 *
 */


#include "main.h"


/*
 * Description: Polling. Re-did I2CMasterControl sequence but has not been tested!!!!!!!!!!!!!!
 *
 * Input:
 * Output:
 */
int I2C_Master_Receive(uint32_t i2c_base, uint16_t DevAddress, uint8_t *pData, uint32_t size, uint32_t timeout)
{
    int status = NO_ERROR;
    uint8_t i;
    uint32_t currentTick = 0;

    if(I2CMasterBusy(i2c_base) == false)
    {
        I2CMasterTimeoutSet(i2c_base, timeout);
        I2CMasterSlaveAddrSet(i2c_base, DevAddress, false);// // set slave address, read

        if(size == 1){ // single receive
            // read data
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_SINGLE_RECEIVE);

            currentTick = SystemTickGet();
            while(I2CMasterBusy(i2c_base))
            {
                if((SystemTickGet() - currentTick) > timeout)
                {
                    return I2C_TIMEOUT;
                }
            }

            pData[0] = I2CMasterDataGet(i2c_base);
        }
        else // receive in burst
        {
            // receive first byte
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_START);

            // wait until mcu is done transferring
            currentTick = SystemTickGet();
            while(I2CMasterBusy(i2c_base))
            {
                if((SystemTickGet() - currentTick) > timeout)
                {
                    return I2C_TIMEOUT;
                }
            }

            pData[0] = I2CMasterDataGet(i2c_base);

            i = 1;
            while((i + 1) < size)
            {
                //receive next data that was just placed into FIFO
                I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);

                // Wait until MCU is done transferring.
                currentTick = SystemTickGet();
                while(I2CMasterBusy(i2c_base))
                {
                    if((SystemTickGet() - currentTick) > timeout)
                    {
                        return I2C_TIMEOUT;
                    }
                }

                // get data
                pData[i++] = I2CMasterDataGet(i2c_base);
            }

            //receive next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

            // Wait until MCU is done transferring.
            currentTick = SystemTickGet();
            while(I2CMasterBusy(i2c_base))
            {
                if((SystemTickGet() - currentTick) > timeout)
                {
                    return I2C_TIMEOUT;
                }
            }

            //get last piece of data into I2C FIFO
            pData[i] = I2CMasterDataGet(i2c_base);
        }
    }
    else
    {
        return I2C_BUSY;
    }
    return status;
}

/*
 * Description: Polling. Re-did I2CMasterControl sequence but has not been tested!!!!!!!!!!!!!!
 *
 * Input:
 * Output:
 */
int I2C_Master_Transmit(uint32_t i2c_base, uint16_t DevAddress, uint8_t *pData, uint32_t size, uint32_t timeout)
{
    int status = NO_ERROR;
    uint8_t i;
    uint32_t currentTick = 0;

    if (I2CMasterBusy(i2c_base) == false)
    {
        I2CMasterTimeoutSet(i2c_base, timeout);
        I2CMasterSlaveAddrSet(i2c_base, DevAddress, false); // set slave address, write

        I2CMasterDataPut(i2c_base, pData[0]);

        if(size == 1){ // single send
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_SINGLE_SEND);

            // Wait until MCU is done transferring.
            currentTick = SystemTickGet();
            while(I2CMasterBusy(i2c_base))
            {
                if((SystemTickGet() - currentTick) > timeout)
                {
                    return I2C_TIMEOUT;
                }
            }
        }
        else // burst send
        {
            // send first byte
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);
            // wait until mcu is done transferring
            currentTick = SystemTickGet();
            while(I2CMasterBusy(i2c_base))
            {
                if((SystemTickGet() - currentTick) > timeout)
                {
                    return I2C_TIMEOUT;
                }
            }

            i = 1;
            while((i + 1) < size)
            {
                I2CMasterDataPut(i2c_base, pData[i++]);

                //send next data that was just placed into FIFO
                I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_CONT);

                // Wait until MCU is done transferring.
                currentTick = SystemTickGet();
                while(I2CMasterBusy(i2c_base))
                {
                    if((SystemTickGet() - currentTick) > timeout)
                    {
                        return I2C_TIMEOUT;
                    }
                }
            }

            //put last piece of data into I2C FIFO
            I2CMasterDataPut(i2c_base, pData[i]);

            //send next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);

            // Wait until MCU is done transferring.
            currentTick = SystemTickGet();
            while(I2CMasterBusy(i2c_base))
            {
                if((SystemTickGet() - currentTick) > timeout)
                {
                    return I2C_TIMEOUT;
                }
            }
        }
    }
    else
    {
        return I2C_BUSY;
    }
    return status;
}

/*
 * Description: Polling. PMBus/SMBus Read data from an 8/16 bit register. This will send register address without stop bit and get the register data.
 * Input: The I2C base, Slave Address, Register address, Size of Register address, pointer to received data, The amount of bytes to read
 * Output: Register data
 * Return: Error Status
 *
 * TODO add timeout
 */
int I2C_Master_Mem_Read(uint32_t i2c_base, uint16_t DevAddress, uint16_t regAddress, uint32_t regSize, uint8_t *pData_OUT, uint32_t readSize, uint32_t timeout)
{
    int status = NO_ERROR;
    uint8_t i = 0;
    uint32_t currentTick = 0;

    if (I2CMasterBusy(i2c_base) == false)
    {
        I2CMasterTimeoutSet(i2c_base, timeout);
        I2CMasterSlaveAddrSet(i2c_base, DevAddress, false); // set slave address, write

        // put 1st byte
        I2CMasterDataPut(i2c_base, (uint8_t)regAddress);

        // send first byte
        I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);
        // wait until mcu is done transferring
        currentTick = SystemTickGet();
        while(I2CMasterBusy(i2c_base))
        {
            if((SystemTickGet() - currentTick) > timeout)
            {
                return I2C_TIMEOUT;
            }
        }

        if(regSize == 2) // burst send
        {
            //put last piece of data into I2C FIFO
            I2CMasterDataPut(i2c_base, (uint8_t)(regAddress >> 8));

            //send next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);

            // Wait until MCU is done transferring.
            currentTick = SystemTickGet();
            while(I2CMasterBusy(i2c_base))
            {
                if((SystemTickGet() - currentTick) > timeout)
                {
                    return I2C_TIMEOUT;
                }
            }
        }

        if(readSize == 1)
        {
            // set to read from slave
            I2CMasterSlaveAddrSet(i2c_base, DevAddress, true); // set slave address, read

            // receive first byte
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_SINGLE_RECEIVE);
            // wait until mcu is done transferring
            currentTick = SystemTickGet();
            while(I2CMasterBusy(i2c_base))
            {
                if((SystemTickGet() - currentTick) > timeout)
                {
                    return I2C_TIMEOUT;
                }
            }

            pData_OUT[0] = I2CMasterDataGet(i2c_base);
        }
        else if(readSize > 1)// receive in burst
        {
            // set to read from slave
            I2CMasterSlaveAddrSet(i2c_base, DevAddress, true); // set slave address, read

            // receive first byte
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_START);
            // wait until mcu is done transferring
            currentTick = SystemTickGet();
            while(I2CMasterBusy(i2c_base))
            {
                if((SystemTickGet() - currentTick) > timeout)
                {
                    return I2C_TIMEOUT;
                }
            }

            pData_OUT[0] = I2CMasterDataGet(i2c_base);

            i = 1;
            while ((i + 1) < readSize)
            {
                //receive next data that was just placed into FIFO
                I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);

                // Wait until MCU is done transferring.
                currentTick = SystemTickGet();
                while(I2CMasterBusy(i2c_base))
                {
                    if((SystemTickGet() - currentTick) > timeout)
                    {
                        return I2C_TIMEOUT;
                    }
                }

                // get data
                pData_OUT[i++] = I2CMasterDataGet(i2c_base);
            }

            //receive next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

            // Wait until MCU is done transferring.
            currentTick = SystemTickGet();
            while(I2CMasterBusy(i2c_base))
            {
                if((SystemTickGet() - currentTick) > timeout)
                {
                    return I2C_TIMEOUT;
                }
            }

            //get last piece of data into I2C FIFO
            pData_OUT[i] = I2CMasterDataGet(i2c_base);
        }
    }
    else
    {
        return I2C_BUSY;
    }

    return status;
}

/*
 * Description: TODO add timeout
 */
int I2C_Master_Mem_Write(uint32_t i2c_base, uint16_t DevAddress, uint16_t regAddress, uint32_t regSize, uint8_t *pData_OUT, uint32_t writeSize, uint32_t timeout)
{
    int status = NO_ERROR;
    uint8_t i = 0;
    uint32_t currentTick = 0;

    if (I2CMasterBusy(i2c_base) == false)
    {
        I2CMasterTimeoutSet(i2c_base, timeout);
        I2CMasterSlaveAddrSet(i2c_base, DevAddress, false); // set slave address, write

        I2CMasterDataPut(i2c_base, (uint8_t)regAddress);

        // send first byte
        I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);

        // wait until mcu is done transferring
        currentTick = SystemTickGet();
        while(I2CMasterBusy(i2c_base))
        {
            if((SystemTickGet() - currentTick) > timeout)
            {
                return I2C_TIMEOUT;
            }
        }

        if(regSize == 2) // send 2nd register byte
        {
            //put last piece of data into I2C FIFO
            I2CMasterDataPut(i2c_base, (uint8_t)(regAddress >> 8));

            //send next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_CONT); // do we need I2C_MASTER_CMD_BURST_SEND_FINISH?

            // Wait until MCU is done transferring.
            currentTick = SystemTickGet();
            while(I2CMasterBusy(i2c_base))
            {
                if((SystemTickGet() - currentTick) > timeout)
                {
                    return I2C_TIMEOUT;
                }
            }
        }

        if(writeSize == 1)
        {
            // send data
            I2CMasterDataPut(i2c_base, pData_OUT[i++]);

            //transmit next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_SINGLE_SEND);

            // Wait until MCU is done transferring.
            currentTick = SystemTickGet();
            while(I2CMasterBusy(i2c_base))
            {
                if((SystemTickGet() - currentTick) > timeout)
                {
                    return I2C_TIMEOUT;
                }
            }
        }
        else if(writeSize > 1)
        {
            while ((i + 1) < writeSize)
            {
                // send data
                I2CMasterDataPut(i2c_base, pData_OUT[i++]);

                //transmit next data that was just placed into FIFO
                I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_CONT);

                // Wait until MCU is done transferring.
                currentTick = SystemTickGet();
                while(I2CMasterBusy(i2c_base))
                {
                    if((SystemTickGet() - currentTick) > timeout)
                    {
                        return I2C_TIMEOUT;
                    }
                }
            }

            //send last piece of data
            I2CMasterDataPut(i2c_base, pData_OUT[i]);

            //transmit last data
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);

            // Wait until MCU is done transferring.
            currentTick = SystemTickGet();
            while(I2CMasterBusy(i2c_base))
            {
                if((SystemTickGet() - currentTick) > timeout)
                {
                    return I2C_TIMEOUT;
                }
            }
        }
    }
    else
    {
        return I2C_BUSY;
    }
    return status;
}

/*
 * Description: Interrupt.
 */
int I2C_Master_Mem_Read_Generic_IT(uint32_t i2c_base, uint16_t DevAddress, uint16_t regAddress, uint32_t regSize, uint8_t *pData_OUT, uint32_t readSize)
{
    int status = NO_ERROR;




    // enable master interrupt
    I2CMasterIntEnable(I2C0_BASE);

    return status;
}

/*
 * Description: Interrupt.
 */
int I2C_Master_Mem_Write_Generic_IT(uint32_t i2c_base, uint16_t DevAddress, uint16_t regAddress, uint32_t regSize, uint8_t *pData, uint32_t writeSize)
{
    int status = NO_ERROR;

    return status;
}




