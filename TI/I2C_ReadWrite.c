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
#include "I2C_ReadWrite.h"



uint32_t slaveStatus;
bool interruptStatus;

// data buffer
uint8_t i2c1_dataBuffer[MAX_I2C_DATA_BUFFER];
RING_BUFF_INFO i2c1_RxDataBufferPointer;

// message buffer
I2C_DataStruct i2c1_messageBuffer[MAX_I2C_MESSAGE_BUFFER];
RING_BUFF_INFO i2c1_RxMsgBufferPointer;


/*
 * Description:
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef I2C_Master_Receive(uint32_t i2c_base, uint16_t DevAddress, uint8_t *pData, uint32_t size){
    HAL_StatusTypeDef errorCode;
    uint8_t i;
    if(I2CMasterBusy(i2c_base) == false){
        I2CMasterSlaveAddrSet(i2c_base, DevAddress, true);// // set slave address, read

        if(size == 1){ // single receive
            // read data
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_SINGLE_RECEIVE);
            while(I2CMasterBusy(i2c_base));
            pData[0] = I2CMasterDataGet(i2c_base);

            // check for error
            errorCode = (HAL_StatusTypeDef)I2CMasterErr(i2c_base);
            if(errorCode != I2C_MASTER_ERR_NONE){
                return HAL_ERROR;
            }
        }
        else // receive in burst
        {
            // receive first byte
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_START);
            // wait until mcu is done transferring
            while(I2CMasterBusy(i2c_base));

            pData[0] = I2CMasterDataGet(i2c_base);

            // check for error

            errorCode = (HAL_StatusTypeDef)I2CMasterErr(i2c_base);
            if(errorCode != I2C_MASTER_ERR_NONE){
                return HAL_ERROR;
            }

            i = 1;
            while((i + 1) < size)
            {
                //receive next data that was just placed into FIFO
                I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);

                // Wait until MCU is done transferring.
                while(I2CMasterBusy(i2c_base));

                // get data
                pData[i++] = I2CMasterDataGet(i2c_base);

                // check for error

                errorCode = (HAL_StatusTypeDef)I2CMasterErr(i2c_base);
                if(errorCode != I2C_MASTER_ERR_NONE){
                    return HAL_ERROR;
                }

            }

            //receive next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

            // Wait until MCU is done transferring.
            while(I2CMasterBusy(i2c_base));

            //get last piece of data into I2C FIFO
            pData[i] = I2CMasterDataGet(i2c_base);

            // check for error
            errorCode = (HAL_StatusTypeDef)I2CMasterErr(i2c_base);
            if(errorCode != I2C_MASTER_ERR_NONE){
                return HAL_ERROR;
            }

        }
    }
    else
    {
        return HAL_BUSY;
    }
    return HAL_OK;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef I2C_Master_Transmit(uint32_t i2c_base, uint16_t DevAddress, uint8_t *pData, uint32_t size){
    HAL_StatusTypeDef errorCode;
    uint8_t i;

    if (I2CMasterBusy(i2c_base) == false)
    {
        I2CMasterTimeoutSet(i2c_base, I2C_TIMEOUT); // set timeout
        I2CMasterSlaveAddrSet(i2c_base, DevAddress, false); // set slave address, write

        I2CMasterDataPut(i2c_base, pData[0]);

        if(size == 1){ // single send
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_SINGLE_SEND);

            // Wait until MCU is done transferring.
            while(I2CMasterBusy(i2c_base));

            // check for error
            errorCode = (HAL_StatusTypeDef)I2CMasterErr(i2c_base);
            if(errorCode != I2C_MASTER_ERR_NONE){
                return HAL_ERROR;
            }
        }
        else // burst send
        {
            // send first byte
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);
            // wait until mcu is done transferring
            while(I2CMasterBusy(i2c_base));

            // check for error
            errorCode = (HAL_StatusTypeDef)I2CMasterErr(i2c_base);
            if(errorCode != I2C_MASTER_ERR_NONE){
                return HAL_ERROR;
            }

            i = 1;
            while((i + 1) < size)
            {
                I2CMasterDataPut(i2c_base, pData[i++]);

                //send next data that was just placed into FIFO
                I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_CONT);

                // Wait until MCU is done transferring.
                while(I2CMasterBusy(i2c_base));

                // check for error
                errorCode = (HAL_StatusTypeDef)I2CMasterErr(i2c_base);
                if(errorCode != I2C_MASTER_ERR_NONE){
                    return HAL_ERROR;
                }
            }

            //put last piece of data into I2C FIFO
            I2CMasterDataPut(i2c_base, pData[i]);

            //send next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);

            // Wait until MCU is done transferring.
            while(I2CMasterBusy(i2c_base));

            // check for error
            errorCode = (HAL_StatusTypeDef)I2CMasterErr(i2c_base);
            if(errorCode != I2C_MASTER_ERR_NONE){
                return HAL_ERROR;
            }
        }
    }
    else
    {
        return HAL_BUSY;
    }
    return HAL_OK;
}

/*
 * Description: PMBus/SMBus Read data from an 8bit register. This will send register address without stop bit and get the register data.
 * Input: The I2C base, Slave Address, Register address, Size of Register address, pointer to received data, The amount of bytes to read
 * Output: Register data
 * Return: Error Status
 */
HAL_StatusTypeDef I2C_Master_ReadRegister(uint32_t i2c_base, uint16_t DevAddress, uint8_t *regAddress, uint32_t regSize, uint8_t *pData_OUT, uint32_t readSize){
    HAL_StatusTypeDef errorCode;
    uint8_t i;

    if (I2CMasterBusy(i2c_base) == false)
    {
        I2CMasterTimeoutSet(i2c_base, I2C_TIMEOUT); // set timeout
        I2CMasterSlaveAddrSet(i2c_base, DevAddress, false); // set slave address, write

        I2CMasterDataPut(i2c_base, regAddress[0]);

        if(regSize == 1)
        {
            // send first byte
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);
            // wait until mcu is done transferring
            while (I2CMasterBusy(i2c_base));

            // check for error
            errorCode = (HAL_StatusTypeDef) I2CMasterErr(i2c_base);
            if (errorCode != I2C_MASTER_ERR_NONE)
            {
                return HAL_ERROR;
            }
        }
        else // burst send
        {
            // send first byte
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);
            // wait until mcu is done transferring
            while(I2CMasterBusy(i2c_base));

            // check for error
            errorCode = (HAL_StatusTypeDef)I2CMasterErr(i2c_base);
            if(errorCode != I2C_MASTER_ERR_NONE){
                return HAL_ERROR;
            }

            i = 1;
            while ((i + 1) < regSize) // check to see if there is one more byte after this current byte, skip if not. The next step will send last byte instead.
            {
                I2CMasterDataPut(i2c_base, regAddress[i++]);

                //send next data that was just placed into FIFO
                I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_CONT);

                // Wait until MCU is done transferring.
                while (I2CMasterBusy(i2c_base));

                // check for error
                errorCode = (HAL_StatusTypeDef) I2CMasterErr(i2c_base);
                if (errorCode != I2C_MASTER_ERR_NONE)
                {
                    return HAL_ERROR;
                }
            }

            //put last piece of data into I2C FIFO
            I2CMasterDataPut(i2c_base, regAddress[i]);

            //send next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);

            // Wait until MCU is done transferring.
            while (I2CMasterBusy(i2c_base));

            // check for error
            errorCode = (HAL_StatusTypeDef) I2CMasterErr(i2c_base);
            if (errorCode != I2C_MASTER_ERR_NONE)
            {
                return HAL_ERROR;
            }
        }

        // Wait until MCU is done transferring.
        while (I2CMasterBusy(i2c_base));

        // check for error
        errorCode = (HAL_StatusTypeDef) I2CMasterErr(i2c_base);
        if (errorCode != HAL_OK)
        {
            return HAL_ERROR;
        }

        // now read data from register
        I2CMasterSlaveAddrSet(i2c_base, DevAddress, true); // // set slave address, read

        if (readSize == 1)
        { // single receive
            // read data
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_SINGLE_RECEIVE);
            while (I2CMasterBusy(i2c_base));

            pData_OUT[0] = I2CMasterDataGet(i2c_base);

            // check for error
            errorCode = (HAL_StatusTypeDef) I2CMasterErr(i2c_base);
            if (errorCode != I2C_MASTER_ERR_NONE)
            {
                return HAL_ERROR;
            }
        }
        else // receive in burst
        {
            // receive first byte
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_START);
            // wait until mcu is done transferring
            while (I2CMasterBusy(i2c_base));

            pData_OUT[0] = I2CMasterDataGet(i2c_base);

            // check for error
            errorCode = (HAL_StatusTypeDef) I2CMasterErr(i2c_base);
            if (errorCode != I2C_MASTER_ERR_NONE)
            {
                return errorCode;
            }

            i = 1;
            while ((i + 1) < readSize)
            {
                //receive next data that was just placed into FIFO
                I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);

                // Wait until MCU is done transferring.
                while (I2CMasterBusy(i2c_base));

                // get data
                pData_OUT[i++] = I2CMasterDataGet(i2c_base);

                // check for error
                errorCode = (HAL_StatusTypeDef) I2CMasterErr(i2c_base);
                if (errorCode != I2C_MASTER_ERR_NONE)
                {
                    return HAL_ERROR;
                }
            }

            //receive next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

            // Wait until MCU is done transferring.
            while (I2CMasterBusy(i2c_base));

            //get last piece of data into I2C FIFO
            pData_OUT[i] = I2CMasterDataGet(i2c_base);

            // check for error
            errorCode = (HAL_StatusTypeDef) I2CMasterErr(i2c_base);
            if (errorCode != I2C_MASTER_ERR_NONE)
            {
                return HAL_ERROR;
            }
        }
    }
    else
    {
        return HAL_BUSY;
    }
    return HAL_OK;
}


/*
 * Description: Saves the data to I2C data buffer. If the register and data bytes are complete then saves data to message buffer to be parsed from a polling routine.
 *
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef I2C_EV_IRQHandler(uint32_t i2c_base){
    bool bMasked = true;

    uint8_t i;

    if(i2c_base == I2C1_BASE){
        interruptStatus = I2CSlaveIntStatus(i2c_base, bMasked);
        slaveStatus = I2CSlaveStatus(i2c_base);

        i2c1_dataBuffer[i2c1_RxDataBufferPointer.iIndexIN] =  I2CSlaveDataGet(i2c_base);
        DRV_RingBuffPtr__Input(&i2c1_RxDataBufferPointer, MAX_I2C_DATA_BUFFER);
        if(i2c1_RxDataBufferPointer.iIndexIN == 0){ // data bytes complete, save to message buffer
            for(i = 0; i < MAX_I2C_DATA_BUFFER; i++){
                i2c1_messageBuffer[i2c1_RxMsgBufferPointer.iIndexIN].Data.data[i] = i2c1_dataBuffer[i];
            }
            DRV_RingBuffPtr__Input(&i2c1_RxMsgBufferPointer, MAX_I2C_MESSAGE_BUFFER);
        }
    }

    return HAL_OK;
}

