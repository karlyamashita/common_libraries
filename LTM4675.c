/*
 * LTM4675.c
 *
 *  Created on: Nov 12, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "LTM4675.h"



const uint8_t commandsPagedList[] = {OPERATION_paged, ON_OFF_CONFIG_paged};



uint8_t voutModeValue;


static int ParseSetDirectRegisterInfo(char *msg, DirectRegisterInfoType *info);
static int ParseGetDirectRegisterInfo(char *msg, DirectRegisterInfoType *info);
static int SetPage(uint8_t page);

/*
 * Description: Init LTM4675.
 *
 *  • VOUT0 max voltage 4.5V
 *  • VOUT0 min voltage 2.5V
 *  • VOUT0 min current 10A
 *  • VOUT1 max voltage 3.5V
 *  • VOUT1 min voltage 3.1V
 *  • VOUT1 min current 3A
 *
 *
 */
int LTM4675Init(void){
    HAL_StatusTypeDef hal_status = HAL_OK;



    return hal_status;
}

/*
 *
 *
 *
 */
HAL_StatusTypeDef LTM4675_Write(uint8_t *data, uint8_t writeSize){
    HAL_StatusTypeDef hal_status = HAL_OK;
    uint32_t i2c_base = I2C0_BASE;
    uint16_t DevAddress = LTM4675_SLAVE_ADDRESS;

    hal_status = I2C_Master_Transmit(i2c_base, DevAddress, data, writeSize);

    return hal_status;
}

/*
 *
 *
 *
 */
HAL_StatusTypeDef LTM4675_Read(uint8_t *data, uint8_t readSize){
    HAL_StatusTypeDef hal_status = HAL_OK;
    uint32_t i2c_base = I2C0_BASE;
    uint16_t DevAddress = LTM4675_SLAVE_ADDRESS;

    hal_status = I2C_Master_Receive(i2c_base, DevAddress, data, readSize);

    return hal_status;
}

/*
 *
 *
 *
 */
HAL_StatusTypeDef LTM4675_ReadRegister(uint8_t regAddress, uint8_t *data_OUT, uint8_t readSize){
    HAL_StatusTypeDef hal_status = HAL_OK;
    uint32_t i2c_base = I2C0_BASE;
    uint16_t DevAddress = LTM4675_SLAVE_ADDRESS;
    uint32_t regSize = 1;

    hal_status = I2C_Master_ReadRegister(i2c_base, DevAddress, &regAddress, regSize, data_OUT, readSize);

    return hal_status;
}

/*
 * Description: Sets the page
 * Input: the page number, 0 or 1
 *
 */
static int SetPage(uint8_t page){
    HAL_StatusTypeDef hal_status = HAL_OK;
    uint8_t data[3] = { 0 };
    int status = 0;

    data[0] = 0;
    data[1] = page;

    hal_status = LTM4675_Write(data, 2);
    if (hal_status != HAL_OK)
    {
        status = 1;
    }

    return status;
}


/*
 *
 *
 *
 */
int GetMFR_COMMON(LTM4675_MfrCommon *ltm4675_MfrCommon){
    HAL_StatusTypeDef hal_status = HAL_OK;
    uint8_t data[2] = {0};
    int status = 0;

    hal_status = LTM4675_ReadRegister(MFR_COMMON, data, 1);
    if(hal_status != HAL_OK){
        status = ERROR;
    }

    ltm4675_MfrCommon->Byte.data = data[0];

    return status;
}

/*
 * Description: Gets the LTM4675 info or register data.
 *
 *
 */
int GetPwrMod(char *msg, char *retStr){
    HAL_StatusTypeDef hal_status = HAL_OK;
    uint8_t data[2] = {0};
    int status = NO_ERROR;
    uint16_t value;
    //float result;
    char *ptr = (char*)msg;
    uint8_t page;
    LTM4675_MfrCommon ltm4675_MfrCommon;

    status = GetMFR_COMMON(&ltm4675_MfrCommon);
    if(status != NO_ERROR){
        return status;
    }

    if(ltm4675_MfrCommon.Status.moduleNotBusy != 1){
        return BUSY;
    }

    // start parsing
    if (strncmp(ptr, "reg:", strlen("reg:")) == 0)
    {
       ptr += strlen("reg:");
       status = GetLTM4675RegisterData(ptr, retStr);
    }
    else if (strncmp(ptr, "vin", strlen("vin")) == 0)
    {
        hal_status = LTM4675_ReadRegister(READ_VIN, data, 2);
        if(hal_status != HAL_OK){
            return (uint8_t)hal_status;
        }

        value = data[0];
        value |= data[1] << 8;
        //result = L5_11_to_Float(value);
        //sprintf(retStr, "%f", result);

        sprintf(retStr, "0x%X", value);
    }
    else if (strncmp(ptr, "vout:", strlen("vout:")) == 0)
    {
        ptr += strlen("vout:");
        page = atof(ptr);

        SetPage(page); // set page 0 first

        hal_status = LTM4675_ReadRegister(VOUT_MODE_paged, data, 1);
        if(hal_status != HAL_OK){
            return (uint8_t)hal_status;
        }

        voutModeValue = data[0];

        SetPage(page); // set page 0 first

        hal_status = LTM4675_ReadRegister(READ_VOUT_paged, data, 2);
        if(hal_status != HAL_OK){
            return (uint8_t)hal_status;
        }
        value = data[0];
        value |= data[1] << 8;
        //result = L16_to_Float(voutModeValue, value);
        //sprintf(retStr, "%f", result);

        sprintf(retStr, "0x%X", value);
    }
    else if (strncmp(ptr, "iin", strlen("iin")) == 0)
    {
        hal_status = LTM4675_ReadRegister(READ_IIN, data, 2);
        if(hal_status != HAL_OK){
            return (uint8_t)hal_status;
        }

        value = data[0];
        value |= data[1] << 8;
        //result = L5_11_to_Float(value);
        //sprintf(retStr, "%f", result);

        sprintf(retStr, "0x%X", value);
    }
    else if (strncmp(ptr, "iout:", strlen("iout:")) == 0)
    {
        ptr += strlen("iout:");
        page = atof(ptr);

        SetPage(page); // set page

        hal_status = LTM4675_ReadRegister(READ_IOUT_paged, data, 2);
        if(hal_status != HAL_OK){
            return (uint8_t)hal_status;
        }

        value = data[0];
        value |= data[1] << 8;
        //result = L5_11_to_Float(value);
        //sprintf(retStr, "%f", result);

        sprintf(retStr, "0x%X", value);
    }
    else if (strncmp(ptr, "temp:int", strlen("temp:int")) == 0) // check for internal first
    {
        hal_status = LTM4675_ReadRegister(READ_TEMPERATURE2, data, 2);
        if(hal_status != HAL_OK){
            return (uint8_t)hal_status;
        }

        value = data[0];
        value |= data[1] << 8;
        //result = L5_11_to_Float(value);
        //sprintf(retStr, "%f", result);

        sprintf(retStr, "0x%X", value);
    }
    else if (strncmp(ptr, "temp:", strlen("temp:")) == 0)
    {
        ptr += strlen("temp:");
        page = atof(ptr);

        SetPage(page); // set page

        hal_status = LTM4675_ReadRegister(READ_TEMPERATURE1_paged, data, 2);
        if(hal_status != HAL_OK){
            return (uint8_t)hal_status;
        }

        value = data[0];
        value |= data[1] << 8;
        //result = L5_11_to_Float(value);
        //sprintf(retStr, "%f", result);

        sprintf(retStr, "0x%X", value);
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
int SetPwrMod(char *msg){
    HAL_StatusTypeDef hal_status = HAL_OK;
    uint8_t data[3] = {0};
 //   uint16_t value;
 //   float result;
 //   char str[32] = {0};
    char *ptr = (char*)msg;
 //   uint8_t page;
 //   uint8_t regValue;
    int status = 0;

    if (strncmp(ptr, (char*)"storeuserall", strlen("storeuserall")) == 0)
    {
        data[0] = STORE_USER_ALL;

        hal_status = LTM4675_Write(data, 1);
        if(hal_status != HAL_OK){
            status = (uint8_t)hal_status;
        }
    }
    else if (strncmp(ptr, (char*)"reg:", strlen("reg:")) == 0)
    {
        ptr += strlen("reg:");

        status = SetLTM4675RegisterData(ptr);
        if(status != 0){
            return status;
        }
    }
    else
    {
        status = COMMAND_UNKNOWN;
    }

    return status;
}

/*
 * Description: Turns On/Off VBAT
 *
 *
 */
int SetVBAT_En(char *msg){
    char *ptr = (char*)msg;
    int status = NO_ERROR;
    char chr[8];
    int i = 0;
    int mode = 0;

    // get value
    while(*ptr != '\r' && *ptr != '\0'){
        if(IsDigit(*ptr) != true){
            return COMMAND_UNKNOWN;
        }
        chr[i++] = *ptr++;
    }
    mode = charArrayToInt(chr);

    if(mode){
        LTM4675_Enable();
    }
    else
    {
        LTM4675_Disable();
    }

    return status;
}

/*
 * Description: Write 16bit data to register.
 *
 *
 */
int SetLTM4675RegisterData(char *msg){
    HAL_StatusTypeDef hal_status = HAL_OK;
    int status = NO_ERROR;
    uint8_t data[3] = {0};
    DirectRegisterInfoType info;

    status = ParseSetDirectRegisterInfo(msg, &info); // extract msg string into the structure type (info)
    if(status != 0){
        return status;
    }

    if(info.Status.page == 0 || info.Status.page == 1){
        SetPage(info.Status.page);
    }

    // get data ready to send
    data[0] = info.Status.regAddress;
    data[1] = info.Status.regData; // lsb
    data[2] = info.Status.regData >> 8; // msb

    hal_status = LTM4675_Write(data, info.Status.dataLen);
    if(hal_status != HAL_OK){
        status = (uint8_t)hal_status;
    }

    return status;
}

/*
 * Description: For setting Register data. Parse message for Register address, data, page and data length
 * Input: Pointer to string containing Register, page, and data
 * Output: The information in structured format
 * Return: error status
 *
 */
static int ParseSetDirectRegisterInfo(char *msg, DirectRegisterInfoType *info){
    uint8_t page = 0;
    uint8_t i, n, p = 0;
    uint8_t shiftBy = 0;
    char *ptr = (char*)msg;
    int status;

    info->Status.dataLen = 3;

    // get register address. First check if starts as a hex number
    if(strncmp(ptr, (char*)"0x", strlen("0x")) == 0){
        ptr += strlen("0x");

        status = AscII2hex(*ptr++, &i);
        if(status != 0){
            return COMMAND_UNKNOWN;
        }
        info->Status.regAddress = i << 4;

        status = AscII2hex(*ptr++, &i);
        if(status != 0){
            return COMMAND_UNKNOWN;
        }
        info->Status.regAddress |= i;
    }
    else
    {
        return COMMAND_UNKNOWN;
    }

    // check for comma, then skip over comma
    if(*ptr++ != ','){
        return COMMAND_UNKNOWN;
    }

    // check if need to set page
    status = AscII2hex(*ptr++, &page);
    if(status != 0){
        return COMMAND_UNKNOWN;
    }
    info->Status.page = page;

    // now check if rest of characters is hex number
    if(strncmp(ptr, (char*)"=0x", strlen("=0x")) == 0){
        ptr += strlen("=0x");// skip chars
    }
    else
    {
        return COMMAND_UNKNOWN;
    }

    // now get register data
    for(i=0; i < 4; i++){
        p = *ptr++;
        if(p != '\r'){
            status = AscII2hex(p, &n);
            if(status == 0) {
                shiftBy = ((3 - i) * 4);
                info->Status.regData |= n << shiftBy;
            }
        }
        else
        {
            if(i == 2){
                info->Status.dataLen = 2; // command and 2 bytes
                info->Status.regData = info->Status.regData >> 8;// since one byte, move byte to lsb
            }
            break;
        }
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
int GetLTM4675RegisterData(char *msg, char *dataOut){
    int status = NO_ERROR;
    HAL_StatusTypeDef hal_status = HAL_OK;
    DirectRegisterInfoType info;
    uint8_t regDataArray[2];
    uint16_t regDataValue;

    status = ParseGetDirectRegisterInfo(msg, &info); // extract msg string into the structure type (info)
    if(status != 0){
        return status;
    }

    if(info.Status.page == 0 || info.Status.page == 1){
        SetPage(info.Status.page);
    }


    hal_status = LTM4675_ReadRegister(info.Status.regAddress, (uint8_t*)regDataArray, 2);
    if(hal_status != HAL_OK){
        status = (uint8_t)hal_status;
    }

    regDataValue = regDataArray[1] << 8;
    regDataValue |= regDataArray[0];

    itoa(regDataValue, dataOut, BASE_16);

    return status;
}

/*
 * Description: Extracts message for register data and paging number.
 *
 *
 */
static int ParseGetDirectRegisterInfo(char *msg, DirectRegisterInfoType *info){
    uint8_t page = 0;
    uint8_t i = 0;
    char *ptr = (char*)msg;
    int status = NO_ERROR;

    // get register address. First check if starts as a hex number
    if(strncmp(ptr, (char*)"0x", strlen("0x")) == 0){
        ptr += strlen("0x");

        status = AscII2hex(*ptr++, &i);
        if(status != 0){
            return COMMAND_UNKNOWN;
        }
        info->Status.regAddress = i << 4;

        status = AscII2hex(*ptr++, &i);
        if(status != 0){
            return COMMAND_UNKNOWN;
        }
        info->Status.regAddress |= i;
    }
    else
    {
        return COMMAND_UNKNOWN;
    }

    // check for comma, then skip over comma
    if(*ptr++ != ','){
        return COMMAND_UNKNOWN;
    }

    // check if need to set page
    status = AscII2hex(*ptr++, &page);
    if(status != 0){
        return COMMAND_UNKNOWN;
    }
    info->Status.page = page;

    return status;
}




