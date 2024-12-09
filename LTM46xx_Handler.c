/*
 * Description: Each project may have different command string names so it is up to the user to adjust code.
 */


#include "main.h"
#include "LTM46xx_Handler.h"



uint8_t regDataArray[4] = {0};

I2C_GenericDef i2c =
{
	.i2c_instance = &hi2c2,
	.regSize = 1, // always 1
	.dataPtr = regDataArray, // point to array
	.timeout = 100 // typical timeout
};


LTM_SlaveRail_t slaveRailAddress = {0};

// prototype
static int LTM46xx_AddrInit(LTM_SlaveRail_t *addr);

/*
 * Description: Called from PollingInit
 */
void LTM46xx_Init(void)
{
	LTM46xx_AddrInit(&slaveRailAddress);
}

/*
 * Description: Read the slave and rail address from Flash Memory and populate slaveRailAddress data structure
 */
static int LTM46xx_AddrInit(LTM_SlaveRail_t *addr)
{
    int status = NO_ERROR;
    char retStr[FLASH_STRING_DATA_SIZE] = {0};
    char *token; // device count
    char *token2; // rail address
    char *token3; // should be 0x40
    char *token4; // should be 0x41
    char *token5; // should be 0x42
    char *token6; // should be 0x43
    char *token7; // should be 0x44
    char *token8; // should be 0x45
    char *rest;
    char delim[] = ",\r";
    uint32_t value;
    uint32_t deviceCount = 0;

    FLASH_DATA_GetSlaveRail(retStr);
    rest = retStr;

    token = strtok_r(rest, delim, &rest); // LTM46xx device count
    deviceCount = atoi(token);
    addr->ltmCount = deviceCount;

    token2 = strtok_r(rest, delim, &rest);
    // save rail address
    IsHex(token2, &value);
    addr->rail = value;

    switch(deviceCount)
    {
    case 1:
    	token3 = strtok_r(rest, delim, &rest);
    	token4 = strtok_r(rest, delim, &rest);

        IsHex(token3, &value);
        addr->slave_1 = value;
        IsHex(token4, &value);
        addr->slave_2 = value;
    	break;
    case 2:
    	token3 = strtok_r(rest, delim, &rest);
    	token4 = strtok_r(rest, delim, &rest);
    	token5 = strtok_r(rest, delim, &rest);
    	token6 = strtok_r(rest, delim, &rest);

    	IsHex(token3, &value);
    	addr->slave_1 = value;
		IsHex(token4, &value);
		addr->slave_2 = value;

		IsHex(token5, &value);
		addr->slave_3 = value;
		IsHex(token6, &value);
		addr->slave_4 = value;
    	break;
    case 3:
    	token3 = strtok_r(rest, delim, &rest);
    	token4 = strtok_r(rest, delim, &rest);
    	token5 = strtok_r(rest, delim, &rest);
    	token6 = strtok_r(rest, delim, &rest);
    	token7 = strtok_r(rest, delim, &rest);
    	token8 = strtok_r(rest, delim, &rest);

    	IsHex(token3, &value);
    	addr->slave_1 = value;
		IsHex(token4, &value);
		addr->slave_2 = value;

		IsHex(token5, &value);
		addr->slave_3 = value;
		IsHex(token6, &value);
		addr->slave_4 = value;

		IsHex(token7, &value);
		addr->slave_5 = value;
		IsHex(token8, &value);
		addr->slave_6 = value;
    	break;
    default:
    	break;
    }

    return status;
}


/*
 * Description: Gets the LTM4675 info or register data.
 *
 *
 */
int LTM46xx_GetPwrMod(char *msg, char *retStr)
{
    int status = NO_ERROR;
    char *token; // command
    char *token2; // page
    char *token3; // for command reg, this is the register address
    char *rest = msg;
    char delim[] = ":,\r";
    LTM46xx_RegisterPageInfo_t regPage = {0};
    
    // get strings
    token = strtok_r(rest, delim, &rest); // command
    token2 = strtok_r(rest, delim, &rest); // page
    
    // update data structure
    regPage.Status.page = atoi(token2);
    
    // start parsing
    if(strncmp(token, "reg", strlen("reg")) == 0)
    {
    	token3 = strtok_r(rest, delim, &rest);
    	regPage.Status.regAddress = atoi(token3);
    	status = LTM46xx_GetRegisterData(&regPage, retStr);
    }
    else if(strncmp(token, "statusword", strlen("statusword")) == 0)
    {
    	regPage.Status.regAddress = STATUS_WORD_paged;
        status = LTM46xx_GetRegisterData(&regPage, retStr);
    }
    else if(strncmp(token, "statusvout", strlen("statusvout")) == 0)
	{
    	regPage.Status.regAddress = STATUS_VOUT_paged;
		status = LTM46xx_GetRegisterData(&regPage, retStr);
	}
    else if(strncmp(token, "statusiout", strlen("statusiout")) == 0)
	{
    	regPage.Status.regAddress = STATUS_IOUT_paged;
		status = LTM46xx_GetRegisterData(&regPage, retStr);
	}
    else if(strncmp(token, "statusinput", strlen("statusinput")) == 0)
	{
    	regPage.Status.regAddress = STATUS_INPUT;
		status = LTM46xx_GetRegisterData(&regPage, retStr);
	}
    else if(strncmp(token, "statustemperature", strlen("statustemperature")) == 0)
	{
    	regPage.Status.regAddress = STATUS_TEMPERATURE_paged;
		status = LTM46xx_GetRegisterData(&regPage, retStr);
	}
    else if(strncmp(token, "statuscml", strlen("statuscml")) == 0)
	{
    	regPage.Status.regAddress = STATUS_CML;
		status = LTM46xx_GetRegisterData(&regPage, retStr);
	}
    else if(strncmp(token, "statusmfrspecific", strlen("statusmfrspecific")) == 0)
	{
    	regPage.Status.regAddress = STATUS_MFR_SPECIFIC_paged;
		status = LTM46xx_GetRegisterData(&regPage, retStr);
	}

    else if(strncmp(token, "mfrpads", strlen("mfrpads")) == 0)
    {
    	regPage.Status.regAddress = MFR_PADS;
        status = LTM46xx_GetRegisterData(&regPage, retStr);
    }     
    else if(strncmp(token, "mfrspecialid", strlen("mfrspecialid")) == 0)
    {
    	regPage.Status.regAddress = MFR_SPECIAL_ID;
        status = LTM46xx_GetRegisterData(&regPage, retStr);
    }
    else if(strncmp(token, "mfrcommon", strlen("mfrcommon")) == 0)
    {
    	regPage.Status.regAddress = MFR_COMMON;
        status = LTM46xx_GetRegisterData(&regPage, retStr);
    }
    else if (strncmp(token, "readvin", strlen("readvin")) == 0)
    {
    	regPage.Status.regAddress = READ_VIN;
        status = LTM46xx_GetRegisterData(&regPage, retStr);
    }
    else if (strncmp(token, "voutcommand", strlen("voutcommand")) == 0) // L16 format
    {
    	// todo - interpolation if needed
    	regPage.Status.regAddress = VOUT_COMMAND_paged;
        status = LTM46xx_GetRegisterData(&regPage, retStr);
    }
    else if (strncmp(token, "voutovfaultlimit", strlen("voutovfaultlimit")) == 0) // L16 format
    {
    	regPage.Status.regAddress = VOUT_OV_FAULT_LIMIT_paged;
        status = LTM46xx_GetRegisterData(&regPage, retStr);
    }
    else if (strncmp(token, "voutuvfaultlimit", strlen("voutuvfaultlimit")) == 0) // L16 format
	{
    	regPage.Status.regAddress = VOUT_UV_FAULT_LIMIT_paged;
		status = LTM46xx_GetRegisterData(&regPage, retStr);
	}
    else if (strncmp(token, "readvout", strlen("readvout")) == 0) // L16 format
    {
    	regPage.Status.regAddress = READ_VOUT_paged;
        status = LTM46xx_GetRegisterData(&regPage, retStr);
    }
    else if (strncmp(token, "readiin", strlen("readiin")) == 0)
    {
    	regPage.Status.regAddress = READ_IIN;
        status = LTM46xx_GetRegisterData(&regPage, retStr);
    }
    else if (strncmp(token, "readiout", strlen("readiout")) == 0)
    {
    	regPage.Status.regAddress = READ_IOUT_paged;
        status = LTM46xx_GetRegisterData(&regPage, retStr);
    }
    else if (strncmp(token, "readpout", strlen("readpout")) == 0)
	{
    	regPage.Status.regAddress = READ_POUT_paged;
	   status = LTM46xx_GetRegisterData(&regPage, retStr);
	}

    else if (strncmp(token, "readtemperature_1", strlen("readtemperature_1")) == 0)
    {
    	regPage.Status.regAddress = READ_TEMPERATURE1_paged;
        status = LTM46xx_GetRegisterData(&regPage, retStr);
    }
    else if (strncmp(token, "readtemperature_2", strlen("readtemperature_2")) == 0) 
    {
    	regPage.Status.regAddress = READ_TEMPERATURE2;
        status = LTM46xx_GetRegisterData(&regPage, retStr);
    }
    else if (strncmp(token, "operation", strlen("operation")) == 0) 
    {
    	regPage.Status.regAddress = OPERATION_paged;
        status = LTM46xx_GetRegisterData(&regPage, retStr);
    }
    else if (strncmp(token, "slaveaddress0", strlen("slaveaddress0")) == 0)
	{
    	regPage.Status.regAddress = MFR_ADDRESS;
		status = LTM46xx_GetRegisterData(&regPage, retStr);
	}
    else
    {
        status = COMMAND_UNKNOWN;
    }

    return status;
}


/*
 * Description: LTM4675 Set commands
 *
 *
 */
int LTM46xx_SetPwrMod(char *msg)
{
    int status = NO_ERROR;
    char *token;
    char *token2;
    char *token3;
    char *token4; // only used for direct register
    char *rest = msg;
    char delim[] = ":,\r";
    LTM46xx_RegisterPageInfo_t regPage = {0};
    LTM46xx_MfrCommon ltm4675_MfrCommon = {0};
    uint32_t regData = 0;
    
    // get strings
    token = strtok_r(rest, delim, &rest); // command
    token2 = strtok_r(rest, delim, &rest); // page
    token3 = strtok_r(rest, delim, &rest); // parameter or register

    // update data structure
    regPage.Status.page = atoi(token2);
    
    // The rest of commands continue
    status = LTM46xx_GetMFR_COMMON(&i2c, atoi(token2));
    if(status != NO_ERROR)
    {
        return status;
    }

    // copy to structure in order to get moduleNotBusy flag
    ltm4675_MfrCommon.Byte.data = i2c.dataPtr[0];

    // Check if LTM46xx is busy
    if(ltm4675_MfrCommon.Status.moduleNotBusy != 1)
    {
        return LTM46xx_BUSY;
    }
    
    // copy register data to data structure
    IsHex(token3, &regData); // convert string to uint32_t
    memcpy(&regPage.Status.regData, &regData, 2);

    if (strncmp(token, (char*)"reg", strlen("reg")) == 0) // direct register programming
	{
		// register address
		IsHex(token3, &regData); // convert string to uint32_t
		regPage.Status.regAddress = (uint8_t)regData;
		// register data
		token4 = strtok_r(rest, delim, &rest); // parameter
		IsHex(token4, &regData); // convert string to uint32_t
		memcpy(&regPage.Status.regData, &regData, 2);
		status = LTM46xx_SetRegisterData(&regPage);
	}
    else if (strncmp(token, (char*)"storeuserall", strlen("storeuserall")) == 0)
    {
    	regPage.Status.regAddress = STORE_USER_ALL;
        status = LTM46xx_SetRegisterData(&regPage);
    }
    else if (strncmp(token, (char*)"clrfaults", strlen("clrfaults")) == 0)
    {
    	regPage.Status.regAddress = CLEAR_FAULTS;
        status = LTM46xx_SetRegisterData(&regPage);
    }
    else if (strncmp(token, (char*)"freqswitch", strlen("freqswitch")) == 0) // frequency of 350 can't be passed as 350,000 so we need special command
    {
    	regPage.Status.regAddress = FREQUENCY_SWITCH;
        status = LTM46xx_SetRegisterData(&regPage);
    }
    // vout0
    else if (strncmp(token, (char*)"voutcommand", strlen("voutcommand")) == 0)
    {          
    	// May need interpolation?
        if(status != NO_ERROR)
        {
            return status;
        }
        regPage.Status.regAddress = VOUT_COMMAND_paged;
        status = LTM46xx_SetRegisterData(&regPage);
    }
    // over voltage
    else if(strncmp(token, "voutovfaultlimit", strlen("voutovfaultlimit")) == 0)
    {
    	regPage.Status.regAddress = VOUT_OV_FAULT_LIMIT_paged;
        status = LTM46xx_SetRegisterData(&regPage);
    }    
    // under voltage
    else if(strncmp(token, "voutuvfaultlimit", strlen("voutuvfaultlimit")) == 0)
    {
    	regPage.Status.regAddress = VOUT_UV_FAULT_LIMIT_paged;
        status = LTM46xx_SetRegisterData(&regPage);
    }    
    // temperature limit
    else if(strncmp(token, "otfaultlimit", strlen("otfaultlimit")) == 0)
    {
    	regPage.Status.regAddress = OT_FAULT_LIMIT_paged;
        status = LTM46xx_SetRegisterData(&regPage);
    }
    // over current   
    else if(strncmp(token, "ioutocfaultlimit", strlen("ioutocfaultlimit")) == 0)
    {
    	regPage.Status.regAddress = IOUT_OC_FAULT_LIMIT_paged;
        status = LTM46xx_SetRegisterData(&regPage);
    }
    else if(strncmp(token, "mfrpwmmode", strlen("mfrpwmmode")) == 0)
    {
    	regPage.Status.regAddress = MFR_PWM_MODE_paged;
        status = LTM46xx_SetRegisterData(&regPage);
    }
    else if(strncmp(token, "mfrreset", strlen("mfrreset")) == 0)
    {
    	regPage.Status.regAddress = MFR_RESET;
        status = LTM46xx_SetRegisterData(&regPage);
    }
    else if(strncmp(token, "operationonoff", strlen("operationonoff")) == 0)
    {
        if(atoi(token3) == 0)
        {
            regPage.Status.regData[0] = 0;
        }
        else if(atoi(token3) == 1)
        {
            regPage.Status.regData[0] = 0x80;
        }
        regPage.Status.regAddress = OPERATION_paged;
        status = LTM46xx_SetRegisterData(&regPage);
    }
    else
    {
        status = COMMAND_UNKNOWN;
    }

    return status;
}

