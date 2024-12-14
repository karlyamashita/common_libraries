/*
 * LTM4675_Drv.c
 *
 *  Created on: Nov 12, 2021
 *      Author: karl.yamashita
 * 
 * 
 * Power Modules also supported: LTM4676
 * TODO - For LTM4686 this will need to be re-worked since page 0:3 is for quad module.
 * 
 * Note: There may be some extra or removed parameters in lookup table depending part# used. 
 * 
 */

#include "main.h"



#define TABLE_SIZE 6

/*
 * Description: Register, Page, Data Length, isNVM, Format ( none, L5-11, L16, Register), Read type
 *
 *
 */
const uint8_t LTM46xx_RegLookupTable[][TABLE_SIZE] =
{
    { PAGE, NO_PAGE, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE},
    { OPERATION_paged, IS_PAGED, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { ON_OFF_CONFIG_paged, IS_PAGED, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { CLEAR_FAULTS, NO_PAGE, LEN_0, NO_NVM, REG_FORMAT, WRITE_ONLY},
    { PAGE_PLUS_WRITE, NO_PAGE, LEN_0, NO_NVM, REG_FORMAT, WRITE_ONLY},
    { PAGE_PLUS_READ, NO_PAGE, LEN_0, NO_NVM, REG_FORMAT, WRITE_ONLY},
    { WRITE_PROTECT, NO_PAGE, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { STORE_USER_ALL, NO_PAGE, LEN_0, NO_NVM, REG_FORMAT, WRITE_ONLY},
    { RESTORE_USER_ALL, NO_PAGE, LEN_0, NO_NVM, REG_FORMAT, WRITE_ONLY}, // same as MFR_RESET (0xFD)
    { CAPABILITY, NO_PAGE, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE},
    { SMBALERT_MASK_paged, IS_PAGED, LEN_1, IS_NVM, READ_WRITE}, // TODO - need to read data sheet thoroughly
    { VOUT_MODE_paged, IS_PAGED, LEN_1, NO_NVM, REG_FORMAT, READ_ONLY},
    { VOUT_COMMAND_paged, IS_PAGED, LEN_2, NO_NVM, L16_FORMAT, READ_WRITE},
    { VOUT_MAX_paged, IS_PAGED, LEN_2, USER_NVM, L16_FORMAT, READ_WRITE},
    { VOUT_MARGIN_HIGH_paged, IS_PAGED, LEN_2, USER_NVM, L16_FORMAT, READ_WRITE},
    { VOUT_MARGIN_LOW_paged, IS_PAGED, LEN_2, USER_NVM, L16_FORMAT, READ_WRITE},
    { VOUT_TRANSITION_RATE_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { FREQUENCY_SWITCH, NO_PAGE, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { VIN_ON, NO_PAGE, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { VIN_OFF, NO_PAGE, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    // { IOUT_CAL_GAIN_paged, IS_PAGED, 1, 0, 0, READ_WRITE}, // write not recommended
    { VOUT_OV_FAULT_LIMIT_paged, IS_PAGED, LEN_2, USER_NVM, L16_FORMAT, READ_WRITE},
    { VOUT_OV_FAULT_RESPONSE, NO_PAGE, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { VOUT_OV_WARN_LIMIT_paged, IS_PAGED, LEN_2, USER_NVM, L16_FORMAT, READ_WRITE},
    { VOUT_UV_WARN_LIMIT_paged, IS_PAGED, LEN_2, USER_NVM, L16_FORMAT, READ_WRITE},
    { VOUT_UV_FAULT_LIMIT_paged, IS_PAGED, LEN_2, USER_NVM, L16_FORMAT, READ_WRITE},
    { VOUT_UV_FAULT_RESPONSE, NO_PAGE, LEN_2, USER_NVM, REG_FORMAT, READ_WRITE},
    { IOUT_OC_FAULT_LIMIT_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { IOUT_OC_FAULT_RESPONSE_paged, IS_PAGED, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { IOUT_OC_WARN_LIMIT_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { OT_FAULT_LIMIT_paged, IS_PAGED,LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { OT_FAULT_RESPONSE_paged, IS_PAGED, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { OT_WARN_LIMIT_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { UT_FAULT_LIMIT_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { UT_FAULT_RESPONSE_paged, IS_PAGED, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { VIN_OV_FAULT_LIMIT, NO_PAGE, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { VIN_OV_FAULT_RESPONSE, NO_PAGE, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { VIN_UV_WARN_LIMIT, NO_PAGE, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { IIN_OC_WARN_LIMIT, NO_PAGE, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { TON_DELAY_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { TON_RISE_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { TON_MAX_FAULT_LIMIT_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { TON_MAX_FAULT_RESPONSE, NO_PAGE, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { TOFF_DELAY_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { TOFF_FALL_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { TOFF_MAX_WARN_LIMIT_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { STATUS_BYTE_paged, IS_PAGED, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE},
    { STATUS_WORD_paged, IS_PAGED, LEN_2, NO_NVM, REG_FORMAT, READ_WRITE}, // Two-byte summary of the channel�s fault condition. The low byte of the STATUS_WORD is the same as the STATUS_BYTE command.
    { STATUS_VOUT_paged, IS_PAGED, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE},
    { STATUS_IOUT_paged, IS_PAGED, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE},
    { STATUS_INPUT, NO_PAGE, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE},  // 50
    { STATUS_TEMPERATURE_paged, IS_PAGED, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE},
    { STATUS_CML, NO_PAGE, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE},
    { STATUS_MFR_SPECIFIC_paged, IS_PAGED, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE}, // Each channel has a copy of the same information. Only bit 0 is page specific.
    { READ_VIN, NO_PAGE, LEN_2, NO_NVM, L5_11_FORMAT, READ_ONLY},
    { READ_IIN, NO_PAGE, LEN_2, NO_NVM, L5_11_FORMAT, READ_ONLY},
    { READ_VOUT_paged, IS_PAGED, LEN_2, NO_NVM, L16_FORMAT, READ_ONLY},
    { READ_IOUT_paged, IS_PAGED, LEN_2, NO_NVM, L5_11_FORMAT, READ_ONLY},
    { READ_TEMPERATURE1_paged, IS_PAGED, LEN_2, NO_NVM, L5_11_FORMAT, READ_ONLY},
    { READ_TEMPERATURE2, NO_PAGE, LEN_2, NO_NVM, L5_11_FORMAT, READ_ONLY},
    { READ_FREQUENCY, NO_PAGE, LEN_2, NO_NVM, L5_11_FORMAT, READ_ONLY},
    { READ_POUT_paged, IS_PAGED, LEN_2, NO_NVM, L5_11_FORMAT, READ_ONLY},
    { PMBUS_REVISION, NO_PAGE, LEN_1, NO_NVM, NO_FORMAT, READ_ONLY},
    // { MFR_ID, NO_PAGE, LEN_3, NO_NVM, ASCII_FORMAT, READ_ONLY}, // "LTC"
    // { MFR_MODEL, NO_PAGE, LEN_8, NO_NVM, ASCII_FORMAT, READ_ONLY}, // LTM4675
    // { MFR_SERIAL, NO_PAGE, LEN_9, NO_NVM, REG_FORMAT, READ_ONLY}, // 9 bytes
    { MFR_VOUT_MAX_paged, IS_PAGED, LEN_2, NO_NVM, L16_FORMAT, READ_ONLY},
    // Recommended against altering.
    // { USER_DATA_00, NO_PAGE, LEN_2, IS_NVM, REG_FORMAT, READ_WRITE},
    // { USER_DATA_01_paged, IS_PAGED, LEN_2, IS_NVM, REG_FORMAT, READ_WRITE},
    // { USER_DATA_02, NO_PAGE, LEN_2, IS_NVM, REG_FORMAT, READ_WRITE},
    { USER_DATA_03_paged, IS_PAGED, LEN_2, USER_NVM, REG_FORMAT, READ_WRITE},
    { USER_DATA_04, NO_PAGE, LEN_2, USER_NVM, REG_FORMAT, READ_WRITE},
    { MFR_INFO, NO_PAGE, LEN_2, NO_NVM, REG_FORMAT, READ_ONLY},
    // MFR_EE_XXXX commands are used to facilitate bulk programming of the internal EEPROM. Contact the factory for more details.
    //{ MFR_EE_UNLOCK, NO_PAGE, 1, 0, 0, READ_WRITE},
    //{ MFR_EE_ERASE, NO_PAGE, 1, 0, 0, READ_WRITE},
    //{ MFR_EE_DATA, NO_PAGE, 1, 0, 0, READ_WRITE},
    { MFR_CHAN_CONFIG_paged, IS_PAGED, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { MFR_CONFIG_ALL, NO_PAGE, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { MFR_GPIO_PROPAGATE_paged, IS_PAGED, LEN_2, USER_NVM, REG_FORMAT, READ_WRITE},
    { MFR_PWM_COMP_paged, IS_PAGED, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE}, // wss missing 3-13-2023
    { MFR_PWM_MODE_paged, IS_PAGED, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { MFR_GPIO_RESPONSE_paged, IS_PAGED, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},

    { MFR_OT_FAULT_RESPONSE, NO_PAGE, LEN_1, NO_NVM, REG_FORMAT, READ_ONLY},
    { MFR_IOUT_PEAK_paged, IS_PAGED, LEN_2, NO_NVM, L16_FORMAT, READ_ONLY},
    { MFR_ADC_CONTROL, NO_PAGE, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE},
    { MFR_IOUT_CAL_GAIN_paged, IS_PAGED, LEN_2, NO_NVM, L5_11_FORMAT, READ_ONLY}, // for 4676a it is MFR_ADC_TELEMETRY_STATUS
    { MFR_RETRY_DELAY_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { MFR_RESTART_DELAY_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { MFR_VOUT_PEAK_paged, IS_PAGED, LEN_2, NO_NVM, L16_FORMAT, READ_ONLY},
    { MFR_VIN_PEAK, NO_PAGE, LEN_2, NO_NVM, L16_FORMAT, READ_ONLY},
    { MFR_TEMPERATURE_1_PEAK_paged, IS_PAGED, LEN_2, NO_NVM, L16_FORMAT, READ_ONLY},
    { MFR_CLEAR_PEAKS, NO_PAGE, LEN_0, NO_NVM, NO_FORMAT, WRITE_ONLY},
    // page 43
    { MFR_PADS, NO_PAGE, LEN_2, NO_NVM, REG_FORMAT, READ_ONLY},
    { MFR_ADDRESS, NO_PAGE, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { MFR_SPECIAL_ID, NO_PAGE, LEN_2, NO_NVM, REG_FORMAT, READ_ONLY}, // 0x47A prefix denotes the part is an LTM4675, X is adjustable by the manufacturer.
    { MFR_IIN_CAL_GAIN, NO_PAGE, LEN_2, IS_NVM, L5_11_FORMAT, READ_WRITE},
    { MFR_IIN_OFFSET_paged, IS_PAGED, LEN_2, USER_NVM, L5_11_FORMAT, READ_WRITE},
    { MFR_FAULT_LOG_STORE, NO_PAGE, LEN_0, NO_NVM, NO_FORMAT, WRITE_ONLY},
    { MFR_FAULT_LOG_CLEAR, NO_PAGE, LEN_0, NO_NVM, NO_FORMAT, WRITE_ONLY},
    { MFR_READ_IIN_paged, IS_PAGED, LEN_2, NO_NVM, L5_11_FORMAT, READ_ONLY},
    // { MFR_FAULT_LOG, NO_PAGE, 1, NO_NVM, 0, READ_ONLY}, // see page 125
    { MFR_COMMON, NO_PAGE, LEN_1, LEN_1, NO_NVM, READ_ONLY},
    { MFR_COMPARE_USER_ALL, NO_PAGE, LEN_0, NO_NVM, NO_FORMAT, WRITE_ONLY},
    { MFR_TEMPERATURE_2_PEAK, NO_PAGE, LEN_2, NO_NVM, L5_11_FORMAT, READ_ONLY},
    { MFR_PWM_CONFIG, NO_PAGE, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { MFR_IOUT_CAL_GAIN_TC_paged, IS_PAGED, LEN_2, USER_NVM, L16_FORMAT, READ_WRITE},
    { MFR_ICHIP_CAL_GAIN_paged, NO_PAGE, LEN_2, USER_NVM, L5_11_FORMAT, READ_ONLY},
    { MFR_TEMP_1_GAIN_paged, IS_PAGED, LEN_2, USER_NVM, L16_FORMAT, READ_WRITE},
    { MFR_TEMP1_OFFSET_paged, IS_PAGED, LEN_2, IS_NVM, L5_11_FORMAT, READ_WRITE},
    { MFR_RAIL_ADDRESS_paged, IS_PAGED, LEN_1, IS_NVM, REG_FORMAT, READ_WRITE},
    { MFR_RESET, NO_PAGE, LEN_0, NO_NVM, NO_FORMAT, WRITE_ONLY}, // Identical to RESTORE_USER_ALL.
};


/*
 * Description:
 *
 */
int LTM46xx_ReadRegister(uint8_t slaveAddress, uint8_t regAddress, uint8_t *data_OUT, uint8_t readSize)
{
    int status = NO_ERROR;
    I2C_GenericDef i2c = {0};
    
    i2c.deviceAddr = slaveAddress;
    i2c.registerAddr[0] = regAddress;
    i2c.regSize = 1;
    i2c.dataSize = readSize;
    i2c.dataPtr = data_OUT;

    status = I2C_Mem_Read_Generic_Method(&i2c);
    
    return status;
}


/*
 * Description: Read MFR_Common register to see if LTM46xx is busy.
 *
 *
 *
 */
int LTM46xx_GetMFR_COMMON(I2C_GenericDef *i2c, uint8_t page)
{
    int status = NO_ERROR;
    uint32_t hal_error = 0;
    uint8_t slaveAddress = 0;

    status = LTM46xx_CheckSlaveAddressSet();
    if(status != NO_ERROR)
    {
        return status;
    }

    if(page == 0 || page == 1)
    {
        slaveAddress = slaveRailAddress.slave_1;
    }
    else if(page == 2 || page == 3)
    {
        slaveAddress = slaveRailAddress.slave_2;
    }
    else if(page == 4 || page == 5)
	{
		slaveAddress = slaveRailAddress.slave_3;
	}
	else if(page == 6 || page == 7)
	{
		slaveAddress = slaveRailAddress.slave_4;
	}
	else if(page == 8 || page == 9)
	{
		slaveAddress = slaveRailAddress.slave_5;
	}
	else if(page == 10 || page == 11)
	{
		slaveAddress = slaveRailAddress.slave_6;
	}
    else
    {
        return COMMAND_UNKNOWN;
    }
    
	i2c->deviceAddr = (slaveAddress << 1);
	i2c->registerAddr[0] = MFR_COMMON;
	i2c->dataSize = 1;

    status = I2C_Mem_Read_Generic_Method(i2c);
    if(status != NO_ERROR)
    {
    	hal_error = HAL_I2C_GetError(i2c->i2c_instance);
		if(hal_error == HAL_I2C_ERROR_TIMEOUT)
		{
			I2C_Reset();
			Nop();
		}
        return status;
    }
        
    return status;
}

/*
 * Description: Get LTM4675 register data
 * Input: pointer to string containing register value
 * Output: pointer to output string
 * Return: error status
 *
 */
int LTM46xx_GetRegisterData(LTM46xx_RegisterPageInfo_t *regPage, char *retStr)
{
    int status = NO_ERROR;
    LTM46xx_RegisterPageInfo_t info = {0};
    LTM46xx_RegLookUpType regLookUp = {0};
    uint16_t regDataValue = {0};
    uint8_t page = 0;
    bool isRailAddress = false;
    char str2[16] = {0};
    float result = 0;

    status = LTM46xx_CheckSlaveAddressSet();
    if(status != NO_ERROR)
    {
        return status;
    }

    //  check if register exist
    status = LTM46xx_RegisterLookp(regPage->Status.regAddress, &regLookUp);
    if(status != 0)
    {
        return COMMAND_UNKNOWN;
    }

    isRailAddress = LTM46xx_GetMFR_RailAddressFlag();
    if(isRailAddress)
    {
        return LTM46xx_RAIL_ADDR_NOT_ALLOWED;
    }
    else if(info.Status.page == 0 || info.Status.page == 1)
    {
    	i2c.deviceAddr = (slaveRailAddress.slave_1 << 1);
        page = info.Status.page;
    }
    else if(info.Status.page == 2 || info.Status.page == 3)
    {
    	i2c.deviceAddr = (slaveRailAddress.slave_2 << 1);
    	page = info.Status.page == 2 ? 0:1;
    }
    else
    {
        return COMMAND_UNKNOWN;
    }

    if(regLookUp.isPaged ) // Page Plus Read
    {
    	// init for page write
        i2c.registerAddr[0] = PAGE; // Page register
        i2c.dataPtr[0] = page; // page number
    	i2c.dataSize = 1; // send 1 byte
    	status = I2C_Mem_Write_Generic_Method(&i2c);
    	if(status != NO_ERROR)
    	{
    		return status;
    	}

    	// init for read
    	i2c.registerAddr[0] = regLookUp.regAddress;
    	i2c.dataSize = regLookUp.dataLen;

    	status = I2C_Mem_Read_Generic_Method(&i2c);
    	if(status != NO_ERROR)
		{
			return status;
		}

    	Nop();
    }
    else // regular I2C read
    {
    	// init for read
		i2c.registerAddr[0] = regLookUp.regAddress;
		i2c.dataSize = regLookUp.dataLen;

        status = I2C_Mem_Read_Generic_Method(&i2c);
        if(status != NO_ERROR)
		{
			return status;
		}
    }

    if(regLookUp.dataLen == 2)
	{
		regDataValue = i2c.dataPtr[1] << 8;
	}
	regDataValue |= i2c.dataPtr[0];

	// copy register value
    sprintf(retStr, "0x%04X", regDataValue);

    // convert to register value to L11 or L16 format and add to retStr
    if(regLookUp.fmt == L16_FORMAT)
    {
        result = L16_to_Float(0x14, regDataValue);
        sprintf(str2, "%ld", (uint32_t)((result * 1000) + .5));
        strcat(retStr,",");
        strcat(retStr, str2);
    }
    else if(regLookUp.fmt == L5_11_FORMAT)
    {
        result = L5_11_to_Float(regDataValue);
        sprintf(str2, "%ld", (uint32_t)((result * 1000) + .5));
        strcat(retStr,",");
        strcat(retStr, str2);
    }

    return status;
}

/*
 * Description: Write 16bit data to register.
 *
 *
 */
int LTM46xx_SetRegisterData(LTM46xx_RegisterPageInfo_t *regPage)
{
    int status = NO_ERROR;
    double result = 0;
    static uint32_t regVal = 0;
    uint8_t slaveAddress = 0;
    uint8_t page = 0;
    LTM46xx_RegLookUpType regLookUp = {0};

    status = LTM46xx_CheckSlaveAddressSet();
    if(status != NO_ERROR)
    {
        return status;
    }

    status = LTM46xx_RegisterLookp(regPage->Status.regAddress, &regLookUp);
	if(status != 0)
	{
		return COMMAND_UNKNOWN;
	}
    
    if(LTM46xx_GetMFR_RailAddressFlag())
    {
        slaveAddress = (slaveRailAddress.rail << 1);
    }
    else
    {
    	switch(slaveRailAddress.ltmCount)
    	{
    	case 1:
    		if(regPage->Status.page == 0 || regPage->Status.page == 1)
			{
				slaveAddress = (slaveRailAddress.slave_1 << 1);
				page = regPage->Status.page;
			}
			else if(regPage->Status.page == 2 || regPage->Status.page == 3)
			{
				slaveAddress = (slaveRailAddress.slave_2 << 1);
				page = regPage->Status.page == 2 ? 0:1;
			}
    		break;
    	case 2:
    		if(regPage->Status.page == 0 || regPage->Status.page == 1)
			{
				slaveAddress = (slaveRailAddress.slave_1 << 1);
				page = regPage->Status.page;
			}
			else if(regPage->Status.page == 2 || regPage->Status.page == 3)
			{
				slaveAddress = (slaveRailAddress.slave_2 << 1);
				page = regPage->Status.page == 2 ? 0:1;
			}
			else if(regPage->Status.page == 4 || regPage->Status.page == 5)
			{
				slaveAddress = (slaveRailAddress.slave_3 << 1);
				page = regPage->Status.page;
			}
			else if(regPage->Status.page == 6 || regPage->Status.page == 7)
			{
				slaveAddress = (slaveRailAddress.slave_4 << 1);
				page = regPage->Status.page == 2 ? 0:1;
			}
    		break;
    	case 3:
    		if(regPage->Status.page == 0 || regPage->Status.page == 1)
			{
				slaveAddress = (slaveRailAddress.slave_1 << 1);
				page = regPage->Status.page;
			}
			else if(regPage->Status.page == 2 || regPage->Status.page == 3)
			{
				slaveAddress = (slaveRailAddress.slave_2 << 1);
				page = regPage->Status.page == 2 ? 0:1;
			}
			else if(regPage->Status.page == 4 || regPage->Status.page == 5)
			{
				slaveAddress = (slaveRailAddress.slave_3 << 1);
				page = regPage->Status.page == 4 ? 0:1;
			}
			else if(regPage->Status.page == 6 || regPage->Status.page == 7)
			{
				slaveAddress = (slaveRailAddress.slave_4 << 1);
				page = regPage->Status.page == 6 ? 0:1;
			}
			else if(regPage->Status.page == 8 || regPage->Status.page == 8)
			{
				slaveAddress = (slaveRailAddress.slave_5 << 1);
				page = regPage->Status.page == 8 ? 0:1;
			}
			else if(regPage->Status.page == 10 || regPage->Status.page == 11)
			{
				slaveAddress = (slaveRailAddress.slave_6 << 1);
				page = regPage->Status.page == 10 ? 0:1;
			}
    		break;
    	default:
    		return COMMAND_UNKNOWN;
    		break;
    	}
    }

    i2c.deviceAddr = slaveAddress;
    
    regVal = regPage->Status.regData[0];
    regVal |= (regPage->Status.regData[1] << 8) & 0xFF00;
        
    result = (double)regVal;
    
    if(regLookUp.fmt == L16_FORMAT)
    {
        result *= 0.001; // convert from mV to V
        regVal = Float_to_L16(result);  
    }
    else if(regLookUp.fmt == L5_11_FORMAT)
    {    
        if(regLookUp.regAddress != FREQUENCY_SWITCH)
        {
            result *= 0.001;
        }
        regVal = Float_to_L11(result);
    }

    if(regLookUp.isPaged) // Page plus write. See data sheet, page 85
    {
    	// Write page number
    	i2c.registerAddr[0] = PAGE;  // register
    	i2c.dataPtr[0] = page;
    	i2c.dataSize = 1;

    	status = I2C_Mem_Write_Generic_Method(&i2c);
    	if(status != NO_ERROR)
		{
			return I2C_ERROR_WRITE;
		}

    	// write register and data
		i2c.registerAddr[0] = regLookUp.regAddress;
		i2c.dataPtr[0] = regVal;
		i2c.dataPtr[1] = regVal >> 8;
		i2c.dataSize = regLookUp.dataLen;

		status = I2C_Mem_Write_Generic_Method(&i2c);
    }
    else
    {
		if(regLookUp.fmt == REG_FORMAT)
		{
			i2c.dataPtr[0] = regLookUp.regAddress;
			i2c.dataPtr[1] = regVal >> 8;
			i2c.dataSize = regLookUp.dataLen + 1;

			status = I2C_Master_Transmit_Generic_Method(&i2c);
		}
		else
		{
			i2c.registerAddr[0] = regLookUp.regAddress;
			i2c.dataPtr[0] = regVal;
			i2c.dataPtr[1] = regVal >> 8;
			i2c.dataSize = regLookUp.dataLen;

			status = I2C_Mem_Write_Generic_Method(&i2c);
		}
    }

    return status;
}

/*
 * Description: Get parameters from table look up based off register address
 */
int LTM46xx_RegisterLookp(uint8_t regAddr, LTM46xx_RegLookUpType *regLookup)
{
    int status = NO_ERROR;
    int i;
    int found = 0;
    uint32_t tableSize = sizeof(LTM46xx_RegLookupTable) / TABLE_SIZE;

    for(i = 0; i < tableSize; i++)
    {
        if(regAddr == LTM46xx_RegLookupTable[i][0])
        {
            found = 1;
            break;
        }
    }

    if(!found)
    {
        return COMMAND_UNKNOWN;
    }

    regLookup->regAddress = LTM46xx_RegLookupTable[i][0];
    regLookup->isPaged = LTM46xx_RegLookupTable[i][1];
    regLookup->dataLen = LTM46xx_RegLookupTable[i][2];
    regLookup->nvm = LTM46xx_RegLookupTable[i][3];
    regLookup->fmt = LTM46xx_RegLookupTable[i][4];
    regLookup->readWriteType = LTM46xx_RegLookupTable[i][5];

    return status;
}

/*
 * Description: Set or clear flag
 */
int LTM46xx_SetMFR_RailAddressFlag(char *msg)
{
	int status = NO_ERROR;
	char *token;
	char *rest = msg;
	char delim[] = ":,\r";
	uint8_t mode = 0;

	token = strtok_r(rest, delim, &rest);

	mode = atoi(token);

	if(mode > 1)
	{
		return VALUE_OUT_OF_RANGE;
	}

	slaveRailAddress.useMfrRailAddressFlag = atoi(token);

	return status;
}

/*
 * Description: Get the rail address flag
 */
int LTM46xx_GetMFR_RailAddressFlag(void)
{
    return slaveRailAddress.useMfrRailAddressFlag;
}

/*
 * Description: Verify Slave/Rail address has been set
 */
int LTM46xx_CheckSlaveAddressSet(void)
{
    int status = NO_ERROR;

    if((slaveRailAddress.slave_1 | slaveRailAddress.slave_2 | slaveRailAddress.rail) == 0xFF)
    {
        return LTM46xx_SLAVE_ADDRESS_NOT_SET;
    }

    return status;
}


