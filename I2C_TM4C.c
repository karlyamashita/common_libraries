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


extern UartBufferStruct uart0_msg;


I2C_HandleTypeDef ti2c1 =
{
     .i2c_base = I2C1_BASE,
     .ErrorCode = TM4C_I2C_ERROR_NONE,
     .State = TM4C_I2C_STATE_READY,
     .PreviousState = TM4C_I2C_MODE_NONE,
     .Mode = TM4C_I2C_MODE_NONE

};

static TM4C_StatusTypeDef I2C_Mem_ISR_IT(struct __I2C_HandleTypeDef *ti2c, uint32_t ITFlags, uint32_t ITSources);
static void I2C_TransferConfig(I2C_HandleTypeDef *ti2c, uint16_t DevAddress, uint32_t Request);
static void I2C_ITMasterCplt(I2C_HandleTypeDef *hi2c, uint32_t ITFlags);




/*
 * Description:
 *
 * Input:
 * Output:
 */
int I2C_Master_Receive(uint32_t i2c_base, uint16_t DevAddress, uint8_t *pData, uint32_t size)
{
    int status = NO_ERROR;
    uint8_t i;

    if(I2CMasterBusy(i2c_base) == false){
        I2CMasterSlaveAddrSet(i2c_base, DevAddress, true);// // set slave address, read

        if(size == 1){ // single receive
            // read data
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_SINGLE_RECEIVE);
            while(I2CMasterBusy(i2c_base));
            pData[0] = I2CMasterDataGet(i2c_base);

            // check for error
            status = I2CMasterErr(i2c_base);
            if(status != I2C_MASTER_ERR_NONE){
                return I2C_MCS_ERROR;
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

            status = I2CMasterErr(i2c_base);
            if(status != I2C_MASTER_ERR_NONE){
                return I2C_MCS_ERROR;
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

                status = I2CMasterErr(i2c_base);
                if(status != I2C_MASTER_ERR_NONE){
                    return I2C_MCS_ERROR;
                }

            }

            //receive next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

            // Wait until MCU is done transferring.
            while(I2CMasterBusy(i2c_base));

            //get last piece of data into I2C FIFO
            pData[i] = I2CMasterDataGet(i2c_base);

            // check for error
            status = I2CMasterErr(i2c_base);
            if(status != I2C_MASTER_ERR_NONE){
                return I2C_MCS_ERROR;
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
 * Description:
 *
 * Input:
 * Output:
 */
int I2C_Master_Transmit(uint32_t i2c_base, uint16_t DevAddress, uint8_t *pData, uint32_t size)
{
    int status = NO_ERROR;
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
            status = I2CMasterErr(i2c_base);
            if(status != I2C_MASTER_ERR_NONE){
                return I2C_MCS_ERROR;
            }
        }
        else // burst send
        {
            // send first byte
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);
            // wait until mcu is done transferring
            while(I2CMasterBusy(i2c_base));

            // check for error
            status = I2CMasterErr(i2c_base);
            if(status != I2C_MASTER_ERR_NONE){
                return I2C_MCS_ERROR;
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
                status = I2CMasterErr(i2c_base);
                if(status != I2C_MASTER_ERR_NONE){
                    return I2C_MCS_ERROR;
                }
            }

            //put last piece of data into I2C FIFO
            I2CMasterDataPut(i2c_base, pData[i]);

            //send next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);

            // Wait until MCU is done transferring.
            while(I2CMasterBusy(i2c_base));

            // check for error
            status = I2CMasterErr(i2c_base);
            if(status != I2C_MASTER_ERR_NONE){
                return I2C_MCS_ERROR;
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
 * Description: PMBus/SMBus Read data from an 8bit register. This will send register address without stop bit and get the register data.
 * Input: The I2C base, Slave Address, Register address, Size of Register address, pointer to received data, The amount of bytes to read
 * Output: Register data
 * Return: Error Status
 */
int I2C_Master_ReadRegister(uint32_t i2c_base, uint16_t DevAddress, uint16_t regAddress, uint32_t regSize, uint8_t *pData_OUT, uint32_t readSize, uint32_t timeout)
{
    int status = NO_ERROR;
    uint8_t i;

    if (I2CMasterBusy(i2c_base) == false)
    {
        I2CMasterTimeoutSet(i2c_base, timeout); // set timeout
        I2CMasterSlaveAddrSet(i2c_base, DevAddress, false); // set slave address, write

        I2CMasterDataPut(i2c_base, (uint8_t)regAddress);

        if(regSize == 1)
        {
            // send first byte
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);
            // wait until mcu is done transferring
            while (I2CMasterBusy(i2c_base));

            // check for error
            status = I2CMasterErr(i2c_base);
            if (status != I2C_MASTER_ERR_NONE)
            {
                return I2C_MCS_ERROR;
            }
        }
        else // burst send
        {
            // send first byte
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);
            // wait until mcu is done transferring
            while(I2CMasterBusy(i2c_base));

            // check for error
            status = I2CMasterErr(i2c_base);
            if(status != I2C_MASTER_ERR_NONE){
                return I2C_MCS_ERROR;
            }

            //put last piece of data into I2C FIFO
            I2CMasterDataPut(i2c_base, (uint8_t)(regAddress >> 8));

            //send next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);

            // Wait until MCU is done transferring.
            while (I2CMasterBusy(i2c_base));

            // check for error
            status = I2CMasterErr(i2c_base);
            if (status != I2C_MASTER_ERR_NONE)
            {
                return I2C_MCS_ERROR;
            }
        }

        // Wait until MCU is done transferring.
        while (I2CMasterBusy(i2c_base));

        // check for error
        status = I2CMasterErr(i2c_base);
        if (status != I2C_MASTER_ERR_NONE)
        {
            return I2C_MCS_ERROR;
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
            status = I2CMasterErr(i2c_base);
            if (status != I2C_MASTER_ERR_NONE)
            {
                return I2C_MCS_ERROR;
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
            status = I2CMasterErr(i2c_base);
            if (status != I2C_MASTER_ERR_NONE)
            {
                return status;
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
                status = I2CMasterErr(i2c_base);
                if (status != I2C_MASTER_ERR_NONE)
                {
                    return I2C_MCS_ERROR;
                }
            }

            //receive next data that was just placed into FIFO
            I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

            // Wait until MCU is done transferring.
            while (I2CMasterBusy(i2c_base));

            //get last piece of data into I2C FIFO
            pData_OUT[i] = I2CMasterDataGet(i2c_base);

            // check for error
            status = I2CMasterErr(i2c_base);
            if (status != I2C_MASTER_ERR_NONE)
            {
                return I2C_MCS_ERROR;
            }
        }
    }
    else
    {
        return I2C_BUSY;
    }
    return status;
}

TM4C_StatusTypeDef TM4C_I2C_Mem_Read_IT(I2C_HandleTypeDef *ti2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
    if (ti2c->State == TM4C_I2C_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
          ti2c->ErrorCode = TM4C_I2C_ERROR_INVALID_PARAM;
          return  TM4C_ERROR;
        }

        if (I2CMasterBusy(ti2c->i2c_base) == true)
        {
            return TM4C_BUSY;
        }

        /* Process Locked */
        __TM4C_LOCK(ti2c);

        ti2c->State = TM4C_I2C_STATE_BUSY_RX;
        ti2c->Mode = TM4C_I2C_MODE_MEM;
        ti2c->ErrorCode = TM4C_I2C_ERROR_NONE;

        /* Prepare transfer parameters */
        ti2c->pBuffPtr = pData;
        ti2c->XferSize = Size;
        ti2c->XferISR = I2C_Mem_ISR_IT;
        ti2c->Devaddress = DevAddress;

        /* If Memory address size is 8Bit */
        if (MemAddSize == I2C_MEMADD_SIZE_8BIT)
        {
             /* Prefetch Memory Address */
             ti2c->MemaddressMSB = I2C_MEM_ADD_LSB(MemAddress);

             /* Reset Memaddress content */
             ti2c->MemaddressLSB = 0xFFFFFFFFU;
        }
        /* If Memory address size is 16Bit */
        else
        {
             /* Prefetch Memory Address (MSB part, LSB will be manage through interrupt) */
             ti2c->MemaddressMSB = I2C_MEM_ADD_MSB(MemAddress);

             /* Prepare Memaddress buffer for LSB part */
             ti2c->MemaddressLSB = I2C_MEM_ADD_LSB(MemAddress);
        }

        I2C_TransferConfig(ti2c, DevAddress, I2C_MASTER_CMD_BURST_SEND_START);

        I2CMasterTimeoutSet(ti2c->i2c_base, I2C_TIMEOUT); // set timeout

        // enable I2C interrupt
        I2CMasterIntEnableEx(ti2c->i2c_base, I2C_MASTER_INT_DATA);

        __TM4C_UNLOCK(ti2c);

        I2CMasterDataPut(ti2c->i2c_base, ti2c->MemaddressMSB);

        return TM4C_OK;
    }
    else
    {
        return TM4C_BUSY;
    }
}


static TM4C_StatusTypeDef I2C_Mem_ISR_IT(struct __I2C_HandleTypeDef *ti2c, uint32_t ITFlags, uint32_t ITSources)
{
    uint32_t direction = I2C_MASTER_CMD_BURST_SEND_START;
    uint32_t tmpITFlags = ITFlags;

    /* Process Locked */
    __TM4C_LOCK(ti2c);

    // Clear Interrupt
    I2CMasterIntClear(ti2c->i2c_base);

    // Master interrupt
    if((tmpITFlags & I2C_MMIS_MIS) == I2C_MMIS_MIS)
    {
        if(ITSources != 0)
        {
            return TM4C_ERROR;
        }

        if(ti2c->Mode == TM4C_I2C_MODE_MEM)
        {
              /* Call I2C Master complete process */
              I2C_ITMasterCplt(ti2c, tmpITFlags);
        }
    }


//
//  /* Process Locked */
//  __TM4C_LOCK(ti2c);
//
//  if ((I2C_CHECK_FLAG(tmpITFlags, I2C_FLAG_AF) != RESET) && \
//      (I2C_CHECK_IT_SOURCE(ITSources, I2C_IT_NACKI) != RESET))
//  {
//    /* Clear NACK Flag */
//    __TM4C_I2C_CLEAR_FLAG(ti2c, I2C_FLAG_AF);
//
//    /* Set corresponding Error Code */
//    /* No need to generate STOP, it is automatically done */
//    /* Error callback will be send during stop flag treatment */
//    ti2c->ErrorCode |= TM4C_I2C_ERROR_AF;
//
//    /* Flush TX register */
//    I2C_Flush_TXDR(ti2c);
//  }
//  else if ((I2C_CHECK_FLAG(tmpITFlags, I2C_FLAG_RXNE) != RESET) && \
//           (I2C_CHECK_IT_SOURCE(ITSources, I2C_IT_RXI) != RESET))
//  {
//    /* Remove RXNE flag on temporary variable as read done */
//    tmpITFlags &= ~I2C_FLAG_RXNE;
//
//    /* Read data from RXDR */
//    *ti2c->pBuffPtr = (uint8_t)ti2c->Instance->RXDR;
//
//    /* Increment Buffer pointer */
//    ti2c->pBuffPtr++;
//
//    ti2c->XferSize--;
//    ti2c->XferCount--;
//  }
//  else if ((I2C_CHECK_FLAG(tmpITFlags, I2C_FLAG_TXIS) != RESET) && \
//           (I2C_CHECK_IT_SOURCE(ITSources, I2C_IT_TXI) != RESET))
//  {
//    if (ti2c->Memaddress == 0xFFFFFFFFU)
//    {
//      /* Write data to TXDR */
//      ti2c->Instance->TXDR = *ti2c->pBuffPtr;
//
//      /* Increment Buffer pointer */
//      ti2c->pBuffPtr++;
//
//      ti2c->XferSize--;
//      ti2c->XferCount--;
//    }
//    else
//    {
//      /* Write LSB part of Memory Address */
//      ti2c->Instance->TXDR = ti2c->Memaddress;
//
//      /* Reset Memaddress content */
//      ti2c->Memaddress = 0xFFFFFFFFU;
//    }
//  }
//  else if ((I2C_CHECK_FLAG(tmpITFlags, I2C_FLAG_TCR) != RESET) && \
//           (I2C_CHECK_IT_SOURCE(ITSources, I2C_IT_TCI) != RESET))
//  {
//    if ((ti2c->XferCount != 0U) && (ti2c->XferSize == 0U))
//    {
//      if (ti2c->XferCount > MAX_NBYTE_SIZE)
//      {
//        ti2c->XferSize = MAX_NBYTE_SIZE;
//        I2C_TransferConfig(ti2c, (uint16_t)ti2c->Devaddress, (uint8_t)ti2c->XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
//      }
//      else
//      {
//        ti2c->XferSize = ti2c->XferCount;
//        I2C_TransferConfig(ti2c, (uint16_t)ti2c->Devaddress, (uint8_t)ti2c->XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
//      }
//    }
//    else
//    {
//      /* Wrong size Status regarding TCR flag event */
//      /* Call the corresponding callback to inform upper layer of End of Transfer */
//      I2C_ITError(ti2c, TM4C_I2C_ERROR_SIZE);
//    }
//  }
//  else if ((I2C_CHECK_FLAG(tmpITFlags, I2C_FLAG_TC) != RESET) && \
//           (I2C_CHECK_IT_SOURCE(ITSources, I2C_IT_TCI) != RESET))
//  {
//    /* Disable Interrupt related to address step */
//    I2C_Disable_IRQ(ti2c, I2C_XFER_TX_IT);
//
//    /* Enable ERR, TC, STOP, NACK and RXI interrupts */
//    I2C_Enable_IRQ(ti2c, I2C_XFER_RX_IT);
//
//    if (ti2c->State == TM4C_I2C_STATE_BUSY_RX)
//    {
//      direction = I2C_GENERATE_START_READ;
//    }
//
//    if (ti2c->XferCount > MAX_NBYTE_SIZE)
//    {
//      ti2c->XferSize = MAX_NBYTE_SIZE;
//
//      /* Set NBYTES to write and reload if ti2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
//      I2C_TransferConfig(ti2c, (uint16_t)ti2c->Devaddress, (uint8_t)ti2c->XferSize, I2C_RELOAD_MODE, direction);
//    }
//    else
//    {
//      ti2c->XferSize = ti2c->XferCount;
//
//      /* Set NBYTES to write and generate RESTART */
//      I2C_TransferConfig(ti2c, (uint16_t)ti2c->Devaddress, (uint8_t)ti2c->XferSize, I2C_AUTOEND_MODE, direction);
//    }
//  }
//  else
//  {
//    /* Nothing to do */
//  }
//
//  if ((I2C_CHECK_FLAG(tmpITFlags, I2C_FLAG_STOPF) != RESET) && \
//      (I2C_CHECK_IT_SOURCE(ITSources, I2C_IT_STOPI) != RESET))
//  {
//    /* Call I2C Master complete process */
//    I2C_ITMasterCplt(ti2c, tmpITFlags);
//  }

  /* Process Unlocked */
  __TM4C_UNLOCK(ti2c);

  return TM4C_OK;
}

static void I2C_ITMasterCplt(I2C_HandleTypeDef *ti2c, uint32_t ITFlags)
{
  uint32_t tmperror;
  uint32_t tmpITFlags = ITFlags;
  volatile uint32_t tmpreg;

  /* Clear STOP Flag */
  //__TM4C_I2C_CLEAR_FLAG(ti2c, I2C_FLAG_STOPF);

  /* Disable Interrupts and Store Previous state */
  if (ti2c->State == TM4C_I2C_STATE_BUSY_TX)
  {
    I2CMasterIntDisableEx(ti2c->i2c_base, I2C_MASTER_INT_DATA);
  //  ti2c->PreviousState = I2C_STATE_MASTER_BUSY_TX;
  }
  else
  {
    /* Do nothing */
  }

  /* Clear Configuration Register 2 */
  //I2C_RESET_CR2(ti2c);

  /* Reset handle parameters */
  ti2c->XferISR       = NULL;
 //ti2c->XferOptions   = I2C_NO_OPTION_FRAME;


  //ti2c->pBuffPtr = (uint8_t) I2CMasterDataGet(ti2c);


  /* Flush TX register */
  //I2C_Flush_TXDR(ti2c);

  /* Store current volatile ti2c->ErrorCode, misra rule */
  tmperror = ti2c->ErrorCode;

  /* Call the corresponding callback to inform upper layer of End of Transfer */
  if (ti2c->State == TM4C_I2C_STATE_BUSY_TX)
  {
    ti2c->State = TM4C_I2C_STATE_READY;
  //  ti2c->PreviousState = I2C_STATE_NONE;

    if (ti2c->Mode == TM4C_I2C_MODE_MEM)
    {
      ti2c->Mode = TM4C_I2C_MODE_NONE;

      /* Process Unlocked */
      __TM4C_UNLOCK(ti2c);

      /* Call the corresponding callback to inform upper layer of End of Transfer */
    //  TM4C_I2C_MemTxCpltCallback(ti2c);
    }
  }
  /* ti2c->State == TM4C_I2C_STATE_BUSY_RX */
  else if (ti2c->State == TM4C_I2C_STATE_BUSY_RX)
  {
    ti2c->State = TM4C_I2C_STATE_READY;
  //  ti2c->PreviousState = I2C_STATE_NONE;

    if (ti2c->Mode == TM4C_I2C_MODE_MEM)
    {
      ti2c->Mode = TM4C_I2C_MODE_NONE;

      /* Process Unlocked */
      __TM4C_UNLOCK(ti2c);

      /* Call the corresponding callback to inform upper layer of End of Transfer */
    //  TM4C_I2C_MemRxCpltCallback(ti2c);
    }
  }
  else
  {
    /* Nothing to do */
  }
}


static void I2C_TransferConfig(I2C_HandleTypeDef *ti2c, uint16_t DevAddress, uint32_t Request)
{
    /* Check the parameters */
   // assert_param(IS_I2C_ALL_INSTANCE(ti2c->Instance));
   // assert_param(IS_TRANSFER_MODE(Mode));
   // assert_param(IS_TRANSFER_REQUEST(Request));

    I2CMasterSlaveAddrSet(ti2c->i2c_base, DevAddress, Request); // set slave address and read/write request

    I2CMasterControl(ti2c->i2c_base, I2C_MASTER_CMD_BURST_SEND_START);

}


/*
 * Description:
 *
 * Input:
 * Output:
 */
void TM4C_I2C_EV_IRQHandler(I2C_HandleTypeDef *ti2c)
{
    bool bMasked = false;
    uint32_t interruptStatus = I2CMasterIntStatus(ti2c->i2c_base, bMasked);
    uint32_t errorStatus = I2CMasterErr(ti2c->i2c_base);;

    if(ti2c->XferISR != NULL)
    {
        ti2c->XferISR(ti2c, interruptStatus, errorStatus);
    }
}

int i2c_mem_read(char *retStr)
{
    int status = NO_ERROR;
    uint8_t TMP108_00_Value[2];
    int32_t TMP108Value_0;

    // read temp #1
    status = TMP108_mem_read((uint16_t)TMP108_SLAVE_ADDRESS_00, TMP108_00_Value);
    if(status != NO_ERROR){
        return status;
    }

    TMP108Value_0 = (uint16_t)( (TMP108_00_Value[0] >> 4) & 0x0F) << 8;
    TMP108Value_0 |= (uint16_t) ( (TMP108_00_Value[0] & 0x0F) << 4 | (TMP108_00_Value[1] & 0xF0) >> 4 );
    TMP108_GetStringForTemp(TMP108Value_0, retStr);

    return status;
}

int TMP108_mem_read(uint16_t slaveAddress, uint8_t *value)
{
    int status = NO_ERROR;

    if(TM4C_I2C_Mem_Read_IT(&ti2c1, slaveAddress, (uint16_t)TMP108_TEMPERATURE_REGISTER, 1, value, 2) != TM4C_OK)
    {
        return I2C_ERROR_READ;
    }

    return status;
}


__weak void TM4C_I2C_MemTxCpltCallback(I2C_HandleTypeDef *ti2c)
{
    UNUSED(ti2c);
}

__weak void TM4C_I2C_MemRxCpltCallback(I2C_HandleTypeDef *ti2c)
{

}

