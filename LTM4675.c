/*
 * LTM4675.c
 *
 *  Created on: Nov 12, 2021
 *      Author: karl.yamashita
 * 
 * 
 * Power Modules also supported: LTM4676, LTM4681
 * 
 * Note: There may be some extra or removed parameters depending part# used. 
 * 
 */

#include "main.h"
#include "LTM4675.h"


uint8_t voutModeValue;


static int LTM46xx_ParseGetDirectRegisterInfo(char *msg, LTM46xx_DirectRegisterInfoType *info, LTM46xx_RegLookUpType *regLookUp);
static int LTM46xx_ParseSetDirectRegisterInfo(char *msg, LTM46xx_DirectRegisterInfoType *info, LTM46xx_RegLookUpType *regLookUpType);

extern UartTxBufferStruct uart1_txMsg;


/*
 * Description: Register, Page, Data Length, isNVM, Format ( none, L5-11, L16, Register), Read type
 *
 *
 */
const uint8_t LTM46xx_RegLookupTable[][6] =
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
    { STATUS_WORD_paged, IS_PAGED, LEN_2, NO_NVM, REG_FORMAT, READ_WRITE}, // Two-byte summary of the channel’s fault condition. The low byte of the STATUS_WORD is the same as the STATUS_BYTE command.
    { STATUS_VOUT_paged, IS_PAGED, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE},
    { STATUS_IOUT_paged, IS_PAGED, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE},
    { STATUS_INPUT, NO_PAGE, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE},
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
    // { MFR_MODEL, NO_PAGE, LEN_8, NO_NVM, ASCII_FORMAT, READ_ONLY}, // “LTM4675 ”
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
    { MFR_PWM_MODE_paged, IS_PAGED, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},
    { MFR_GPIO_RESPONSE_paged, IS_PAGED, LEN_1, USER_NVM, REG_FORMAT, READ_WRITE},

    { MFR_OT_FAULT_RESPONSE, NO_PAGE, LEN_1, NO_NVM, REG_FORMAT, READ_ONLY},
    { MFR_IOUT_PEAK_paged, IS_PAGED, LEN_2, NO_NVM, L16_FORMAT, READ_ONLY},
    { MFR_ADC_CONTROL, NO_PAGE, LEN_1, NO_NVM, REG_FORMAT, READ_WRITE},
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
    { MFR_TEMP_1_GAIN_paged, IS_PAGED, LEN_2, USER_NVM, L16_FORMAT, READ_WRITE},
    { MFR_TEMP1_OFFSET_paged, IS_PAGED, LEN_2, IS_NVM, L5_11_FORMAT, READ_WRITE},
    { MFR_RAIL_ADDRESS_paged, IS_PAGED, LEN_1, IS_NVM, REG_FORMAT, READ_WRITE},
    { MFR_RESET, NO_PAGE, LEN_0, NO_NVM, NO_FORMAT, WRITE_ONLY}, // Identical to RESTORE_USER_ALL.
};



/*
 * Description: Init LTM4675.
 *
 */
int LTM46xx_Init(void){
    int status = NO_ERROR;    

    return status;
}


/*
 *
 *
 *
 */
int LTM46xx_ReadRegister(uint8_t slaveAddress, uint8_t regAddress, uint8_t *data_OUT, uint8_t readSize)
{
    int status = NO_ERROR;
    
    status = I2C_Master_Read_Register_Generic(LTM4681_I2C_BASE, slaveAddress, &regAddress, data_OUT, readSize);   
    
    return status;
}


/*
 * Description:
 *
 *
 */
int LTM46xx_GetMFR_COMMON(uint8_t page, LTM46xx_MfrCommon *ltm4675_MfrCommon){
    int status = NO_ERROR;
    uint8_t regAddress = 0;
    uint8_t slaveAddress = 0;

    
    if(page == 0 || page == 1)
    {
        slaveAddress = LTM46xx_SLAVE_ADDRESS_A;
    }
    else if(page == 2 || page == 3)
    {
        slaveAddress = LTM46xx_SLAVE_ADDRESS_B;
    }
    else
    {
        return COMMAND_UNKNOWN;
    }
    
    regAddress = MFR_COMMON;
    status = I2C_Master_Read_Register_Generic(LTM4681_I2C_BASE, slaveAddress, &regAddress, &ltm4675_MfrCommon->Byte.data, 1);  
    if(status != NO_ERROR)
    {
        return status;
    }
        
    return status;
}

int LTM46xx_SetClockStretching(void)
{
    int status = NO_ERROR;
    
    return status;
}

/*
 * Description: Gets the LTM4675 info or register data.
 *
 *
 */
int LTM46xx_GetPwrMod(char *msg, char *retStr){
    int status = NO_ERROR;
    char str[32] = {0};
    char *token;
    char *token2;
    char *token3;
    char *rest = msg;
    
    token = strtok_r(rest, ":", &rest); // command
    token2 = strtok_r(rest, "\r", &rest); // page
    
    
    // start parsing
    if (strncmp(token, "reg", strlen("reg")) == 0)
    {
       token3 = strtok_r(rest, "\r", &rest); // register
       sprintf(str, "%s,%s", token2, token3);
       status = LTM46xx_GetRegisterData(str, retStr);
    }
    else if (strncmp(token, "status", strlen("status")) == 0)
    {
        token += strlen("status"); // remove status which leaves register
        sprintf(str, "%s,%s", token2, token);
        Nop();
        status = LTM46xx_GetStatus(str, retStr);
    }
    else if (strncmp(token, "mfrpads", strlen("mfrpads")) == 0)
    {
        sprintf(str, "%s,0x%X", token2, MFR_PADS);
        status = LTM46xx_GetRegisterData(str, retStr);
    }     
    else if(strncmp(token, "mfrspecialid", strlen("mfrspecialid")) == 0)
    {
        sprintf(str, "%s,0x%X", token2, MFR_SPECIAL_ID);
        status = LTM46xx_GetRegisterData(str, retStr);
    }
    else if(strncmp(token, "mfrcommon", strlen("mfrcommon")) == 0)
    {
        sprintf(str, "%s,0x%X", token2, MFR_COMMON);
        status = LTM46xx_GetRegisterData(str, retStr);
    }
    else if (strncmp(token, "readvin", strlen("readvin")) == 0)
    {
        sprintf(str, "%s,0x%X", token2, READ_VIN);
        status = LTM46xx_GetRegisterData(str, retStr);
    }
    else if (strncmp(token, "voutcommand", strlen("voutcommand")) == 0) // L16 format
    {
        sprintf(str, "%s,0x%X", token2, VOUT_COMMAND_paged);
        status = LTM46xx_GetRegisterData(str, retStr);
    }
    else if (strncmp(token, "readvout", strlen("readvout")) == 0) // L16 format
    {
        sprintf(str, "%s,0x%X", token2, READ_VOUT_paged);
        status = LTM46xx_GetRegisterData(str, retStr);
    }
    else if (strncmp(token, "readiin", strlen("readiin")) == 0)
    {
        sprintf(str, "%s,0x%X", token2, READ_IIN);
        status = LTM46xx_GetRegisterData(str, retStr);    
    }
    else if (strncmp(token, "readiout", strlen("readiout")) == 0)
    {
        sprintf(str, "%s,0x%X", token2, READ_IOUT_paged);
        status = LTM46xx_GetRegisterData(str, retStr);
    }
    else if (strncmp(token, "readtemperature_1", strlen("readtemperature_1")) == 0)
    {
        sprintf(str, "%s,0x%X", token2, READ_TEMPERATURE1_paged);
        status = LTM46xx_GetRegisterData(str, retStr); 
    }
    else if (strncmp(token, "readtemperature_2", strlen("readtemperature_2")) == 0) 
    {
        sprintf(str, "%s,0x%X", token2, READ_TEMPERATURE2);
        status = LTM46xx_GetRegisterData(str, retStr); 
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
int LTM46xx_SetPwrMod(char *msg){
    int status = NO_ERROR;
    char str[32] = {0};
    char *token;
    char *token2;
    char *token3;
    char *token4;
    char *rest = msg;
    
    LTM46xx_MfrCommon ltm4675_MfrCommon;

    token = strtok_r(rest, ":", &rest); // command
    token2 = strtok_r(rest, ",", &rest); // page
    token3 = strtok_r(rest, ",", &rest); // parameter or register
     
    status = LTM46xx_GetMFR_COMMON(atoi(token2), &ltm4675_MfrCommon);
    if(status != NO_ERROR){
        Nop();
        return status;
    }

    // Check if LTM46xx is busy
    if(ltm4675_MfrCommon.Status.moduleNotBusy != 1)
    {
        Nop();
        return LTM46xx_BUSY;
    }
    
    
    if (strncmp(token, (char*)"storeuserall", strlen("storeuserall")) == 0)
    {
        sprintf(str, "%s,0x%X", token2, STORE_USER_ALL);
        Nop();
        status = LTM46xx_SetRegisterData(str);
    }
    else if (strncmp(token, (char*)"clrfaults", strlen("clrfaults")) == 0)
    {
        sprintf(str, "%s,0x%X,%s", token2, CLEAR_FAULTS, token3);
        status = LTM46xx_SetRegisterData(str);
    }
    else if (strncmp(token, (char*)"reg", strlen("reg")) == 0)
    {
        token4 = strtok_r(rest, "\r", &rest); //if direct reg then this is the parameter 
        sprintf(str, "%s,%s,%s", token2, token3, token4);
        Nop();
        status = LTM46xx_SetRegisterData(str);
    }
    // vout0
    else if (strncmp(token, (char*)"voutcommand", strlen("voutcommand")) == 0)
    {          
        sprintf(str, "%s,0x%X,%s", token2, VOUT_COMMAND_paged, token3);
        Nop();
        status = LTM46xx_SetRegisterData(str);
    }
    // over voltage
    else if(strncmp(token, "voutovfaultlimit", strlen("voutovfaultlimit")) == 0)
    {
        sprintf(str, "%s,0x%X,%s", token2, VOUT_OV_FAULT_LIMIT_paged, token3);
        Nop();
        status = LTM46xx_SetRegisterData(str);
    }    
    // under voltage
    else if(strncmp(token, "voutuvfaultlimit", strlen("voutuvfaultlimit")) == 0)
    {
        sprintf(str, "%s,0x%X,%s", token2, VOUT_UV_FAULT_LIMIT_paged, token3);
        status = LTM46xx_SetRegisterData(str);
    }    
    // temperature limit
    else if(strncmp(token, "otfaultlimit", strlen("otfaultlimit")) == 0)
    {
        sprintf(str, "%s,0x%X,%s", token2, OT_FAULT_LIMIT_paged, token3);
        status = LTM46xx_SetRegisterData(str);
    }
    
    // over current   
    else if(strncmp(token, "ioutocfaultlimit", strlen("ioutocfaultlimit")) == 0)
    {
        sprintf(str, "%s,0x%X,%s", token2, IOUT_OC_FAULT_LIMIT_paged, token3);
        status = LTM46xx_SetRegisterData(str);
    }
    
    else if(strncmp(token, "mfrpwmmode", strlen("mfrpwmmode")) == 0)
    {
        sprintf(str, "%s,0x%X,%s", token2, MFR_PWM_MODE_paged, token3);
        Nop();
        status = LTM46xx_SetRegisterData(str);
    }
    else
    {
        status = COMMAND_UNKNOWN;
    }

    return status;
}

/*
 * Description: Write 16bit data to register.
 *
 *
 */
int LTM46xx_SetRegisterData(char *msg){
    int status = NO_ERROR;
    static uint8_t data[3] = {0};
    double result = 0;
    uint16_t regVal = 0;
    uint8_t slaveAddress = 0;
    uint8_t page = 0;
    char str[32] = {0};
    LTM46xx_DirectRegisterInfoType info;
    LTM46xx_RegLookUpType regLookUpType;

    status = LTM46xx_ParseSetDirectRegisterInfo(msg, &info, &regLookUpType); // extract msg string into the structure type (info)
    if(status != 0){
        return status;
    }

    if(info.Status.page == 0 || info.Status.page == 1)
    {
        slaveAddress = LTM46xx_SLAVE_ADDRESS_A;
        page = info.Status.page;
    }
    else if(info.Status.page == 2 || info.Status.page == 3)
    {
        slaveAddress = LTM46xx_SLAVE_ADDRESS_B;
        if(page == 2)
        {
            page = 0;
        }
        else if(page == 3)
        {
            page = 1;
        }
    }
    else
    {
        return COMMAND_UNKNOWN;
    }

    // enable NVM write if needed
    if(regLookUpType.nvm == USER_NVM)
    {
    //    status = LTM46xx_Init_NVM_Write(info.Status.page);
        if(status != NO_ERROR)
        {
            return LTM46xx_NVM;
        }
    }
    
    regVal = info.Status.regData[0];
    regVal |= (info.Status.regData[1] << 8) & 0xFF00;
    
    sprintf(str, "%d", regVal);
    
    result = atoi(str);
  
    if(regLookUpType.fmt == L16_FORMAT)
    {
        result *= 0.001;
        regVal = Float_to_L16(result);        
    }
    else if(regLookUpType.fmt == L5_11_FORMAT)
    {
        result *= 0.001;
        regVal = Float_to_L11(result);
    }
    else
    {
        regVal = atoi(str);
    }
    
    if(regLookUpType.dataLen == 1){
        data[0] = regVal;
    }
    else
    {
        data[0] = regVal;
        data[1] = regVal >> 8;
    }

    if(regLookUpType.isPaged)
    {
        status = I2C_Master_Page_Plus_Write_Register_Generic(LTM4681_I2C_BASE , slaveAddress, page, &regLookUpType.regAddress, data, regLookUpType.dataLen);
    }
    else
    {
        status = I2C_Master_Write_Register_Generic(LTM4681_I2C_BASE, slaveAddress, &regLookUpType.regAddress, data, regLookUpType.dataLen);
    }

    return status;
}

/*
 * Description: For setting Register data. Parse message for Register address, page, data length, data value
 * Input: Pointer to string containing Register, page, data length and data value
 * Output: The information in structured format
 * Return: error status
 *
 */
static int LTM46xx_ParseSetDirectRegisterInfo(char *msg, LTM46xx_DirectRegisterInfoType *info, LTM46xx_RegLookUpType *regLookUp){
    int status = NO_ERROR;
    uint32_t regVal = 0;
    char *token; 
    char *token2; 
    char *token3; 
    char *rest = msg;
    uint32_t regAddr = 0;

    token = strtok_r(rest, ",", &rest);// page
    token2 = strtok_r(rest, ",", &rest);// register
    token3 = strtok_r(rest, ",", &rest);// data
    
    // First check if starts as a hex number
    if(strncmp(token2, (char*)"0x", strlen("0x")) == 0){
        token2 += strlen("0x");
        Hex2Int(token2, &regAddr);
    }
    else
    {
        return COMMAND_UNKNOWN;
    }

    //  check if register exist
    status = LTM46xx_CheckIfRegisterValue(regAddr, regLookUp);
    if(status != 0){
        return COMMAND_UNKNOWN;
    }
    
    info->Status.page = atoi(token);

    // Test to be sure we have Write permission
    if(regLookUp->readWriteType == READ_ONLY)
    {
        return LTM46xx_READ_ONLY;
    }

    // save how many bytes we are writing
    //info->Status.dataLen = regLookUp->length;
    // writing zero bytes and no paging is needed.
    if(regLookUp->dataLen == 0)
    {
        return NO_ERROR;
    }

    regVal = 0;
    // now check if rest of characters is hex number
    if(strncmp(token3, (char*)"0x", strlen("0x")) == 0){
        token3 += strlen("0x");// skip chars
        Hex2Int(token3, &regVal);
    }
    else
    {
        regVal = atoi((char*)token3);
    }

    info->Status.regData[0] = regVal;
    info->Status.regData[1] = regVal >> 8;

    return status;
}

/*
 * Description: Get LTM4675 register data
 * Input: pointer to string containing register value
 * Output: pointer to output string
 * Return: error status
 *
 */
    uint8_t regDataArray[4] = {0}; // make global for debugger window
int LTM46xx_GetRegisterData(char *msg, char *retStr){
    int status = NO_ERROR;
    LTM46xx_DirectRegisterInfoType info;
    LTM46xx_RegLookUpType regLookUpType;
    //uint8_t regDataArray[4] = {0};
    uint8_t slaveAddress = 0;
    uint16_t regDataValue = {0};
    uint8_t page = 0;
    char str2[16] = {0};
    float result;

    status = LTM46xx_ParseGetDirectRegisterInfo(msg, &info, &regLookUpType); // extract msg string into the structure type (info)
    if(status != 0){
        return status;
    }
    
    if(info.Status.page == 0 || info.Status.page == 1)
    {
        slaveAddress = LTM46xx_SLAVE_ADDRESS_A;
        page = info.Status.page;
    }
    else if(info.Status.page == 2 || info.Status.page == 3)
    {
        slaveAddress = LTM46xx_SLAVE_ADDRESS_B;
        if(page == 2)
        {
            page = 0;
        }
        else if(page == 3)
        {
            page = 1;
        }
    }
    else
    {
        return COMMAND_UNKNOWN;
    }

    if(regLookUpType.isPaged)
    {
        status = I2C_Master_Page_Plus_Read_Register_Generic(LTM4681_I2C_BASE, slaveAddress, page, &regLookUpType.regAddress,  (uint8_t*)regDataArray, regLookUpType.dataLen);
    }
    else
    {
        status = I2C_Master_Read_Register_Generic(LTM4681_I2C_BASE, slaveAddress, &regLookUpType.regAddress, (uint8_t*)regDataArray, regLookUpType.dataLen);
    }
    if(status != NO_ERROR){
        return status;
    }

    //convert return data to string
    // with Page Plus Read, data[0] is byte count, so not needed
    if(regLookUpType.isPaged)
    {
        if(regLookUpType.dataLen == 2){
            regDataValue = regDataArray[2] << 8;
        }
        regDataValue |= regDataArray[1];
    }
    else
    {
        if(regLookUpType.dataLen == 2){
            regDataValue = regDataArray[1] << 8;
        }
        regDataValue |= regDataArray[0];
    }
    sprintf(retStr, "0x%X", regDataValue);

    if(regLookUpType.fmt == L16_FORMAT)
    {
        result = L16_to_Float(0x14, regDataValue);
        sprintf(str2, "%0.1f", (double)(result * 1000));
        strcat(retStr,",");
        strcat(retStr, str2);        
    }
    else if(regLookUpType.fmt == L5_11_FORMAT)
    {
        result = L5_11_to_Float(regDataValue);
        sprintf(str2, "%0.1f", (double)(result * 1000));
        strcat(retStr,",");
        strcat(retStr, str2);
    }

    return status;
}

/*
 * Description: Extracts message for register data and paging number.
 *
 *
 */
static int LTM46xx_ParseGetDirectRegisterInfo(char *msg, LTM46xx_DirectRegisterInfoType *info, LTM46xx_RegLookUpType *regLookUp){
    int status = NO_ERROR;
    char *token;
    char *token2;
    char *rest = msg;
    uint32_t regAddr = 0;
    
    token = strtok_r(rest, ",", &rest); // page
    token2 = strtok_r(rest, ",", &rest); // register
    
    // First check if starts as a hex number
    if(strncmp(token2, (char*)"0x", strlen("0x")) == 0){
        token2 += strlen("0x");
        Hex2Int(token2, &regAddr);
    }
    else
    {
        return COMMAND_UNKNOWN;
    }

    //  check if register exist
    status = LTM46xx_CheckIfRegisterValue(regAddr, regLookUp);
    if(status != 0){
        return COMMAND_UNKNOWN;
    }

    info->Status.page = atoi(token);
    
    return status;
}

/*
 * Description: Used for register stored in user-editable NVM memory. Need a sequence of write/reads
 *  • Write(0xD1) // MFR_CONFIG_ALL
 *  • Write(0x09)
 * 
 *  • Write(0x10) // WRITE_PROTECT
 *  • Write(0x00)
 * 
 *  • Write(0xEF) // MFR_COMMON
 *  • Read(0xFC)
 *
 *
 */
int LTM46xx_Init_NVM_Write(uint8_t page)
{
    int status = NO_ERROR;
    static uint8_t data[3] = {0};
    uint8_t regAddress = 0;
    uint8_t slaveAddress = 0;

    if(page == 0 || page == 1)
    {
        slaveAddress = LTM46xx_SLAVE_ADDRESS_A;
    }
    else if(page == 2 || page == 3)
    {
        slaveAddress = LTM46xx_SLAVE_ADDRESS_B;
    }
    else
    {
        return COMMAND_UNKNOWN;
    }
      
    data[0] = 0;
    regAddress = WRITE_PROTECT;
    status = I2C_Master_Write_Register_Generic(LTM4681_I2C_BASE, slaveAddress, &regAddress, data, 1);
    if(status != NO_ERROR){
        return status;
    }
    
    data[0] = 0x09;
    regAddress = MFR_CONFIG_ALL;
    status = I2C_Master_Write_Register_Generic(LTM4681_I2C_BASE, slaveAddress, &regAddress, data, 1);
    if(status != NO_ERROR){
        return status;
    }
        
    regAddress = MFR_COMMON;
    status = I2C_Master_Read_Register_Generic(LTM4681_I2C_BASE, slaveAddress, &regAddress, data, 1);

    return status;
}

int LTM46xx_CheckIfRegisterValue(uint8_t regAddr, LTM46xx_RegLookUpType *regLookup)
{
    int status = NO_ERROR;
    static int i;
    int found = 0;

    for(i = 0; i < sizeof(LTM46xx_RegLookupTable); i++){
        if(regAddr == LTM46xx_RegLookupTable[i][0]){
            found = 1;
            break;
        }
    }

    if(!found){
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
 * Description: Gets the status register
 *
 *
 */
int LTM46xx_GetStatus(char *msg, char * retStr)
{
    int status = NO_ERROR;
    uint8_t page = 0;
    uint8_t slaveAddress = 0;
    uint8_t regAddr = 0;
    uint8_t dataLen = 1;
    uint16_t value = 0;
    static uint8_t data[5];
    char *token;
    char *token2;
    char *rest = msg;
    
    token = strtok_r(rest, ",", &rest); // page
    token2 = strtok_r(rest, ",", &rest); // register
    
    page = atoi(token);
    
    if(page > 2)
    {
        return COMMAND_UNKNOWN;
    }
    
    // get register
    if(strncmp(token2, "word", strlen("word")) == 0){
        regAddr = STATUS_WORD_paged;
        dataLen = 2; // this is the only register that has two bytes
    }
    else if(strncmp(token2, "vout", strlen("vout")) == 0){
        regAddr = STATUS_VOUT_paged;
    }
    else if(strncmp(token2, "iout", strlen("iout")) == 0){
        regAddr = STATUS_IOUT_paged;
    }
    else if(strncmp(token2, "input", strlen("input")) == 0){
        regAddr = STATUS_INPUT;
    }
    else if(strncmp(token2, "temperature", strlen("temperature")) == 0){
        regAddr = STATUS_TEMPERATURE_paged;
    }
    else if(strncmp(token2, "cml", strlen("cml")) == 0){
        regAddr = STATUS_CML;
    }
    else
    {
        return COMMAND_UNKNOWN;
    }
    
    if(page == 0 || page == 1)
    {
        slaveAddress = LTM46xx_SLAVE_ADDRESS_A;
    }
    else if(page == 2 || page == 3)
    {
        slaveAddress = LTM46xx_SLAVE_ADDRESS_B;
        if(page == 2)
        {
            page = 0;
        }
        else if(page == 3)
        {
            page = 1;
        }
    }
    else
    {
        return COMMAND_UNKNOWN;
    }
    
    if(regAddr == STATUS_INPUT || STATUS_INPUT == STATUS_CML)
    {
        status = I2C_Master_Read_Register_Generic(LTM4681_I2C_BASE, slaveAddress, &regAddr, data, dataLen);
    }
    else
    {
        status = I2C_Master_Page_Plus_Read_Register_Generic(LTM4681_I2C_BASE, slaveAddress, page, &regAddr, data, dataLen);
    }
    if(status != NO_ERROR)
    {
        return status;
    }
    
    if(dataLen == 2)
    {
        value = data[2] << 8;
        value |= data[1];
    }
    else
    {
        value = data[1];
    }

    sprintf(retStr, "0x%X", value);

    return status;
}
