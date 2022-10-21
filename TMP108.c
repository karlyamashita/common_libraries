/*
 * TM101.c
 *
 *  Created on: September 27, , 2022
 *      Author: karl.yamashita
 *
 *      Texas Instruments TMP108 temperature sensor
 *
 *      include UartCharBuffer.h
 */

#include "main.h"
#include "TMP108.h"


/*
 * Description: initialize TMP108 sensors
 *
 * Input:
 * Output:
 */
int TMP108_Init(void){
    int status;
    TMP108_Configuration config = {0};
    int16_t tempHigh, tempLow;
    uint8_t pui8DataTx[3]= {0};
    uint16_t size = 3;

    config.Status.M0_1 = 2;
    config.Status.TM = 1; // interrupt mode
    config.Status.CR0_1 = 1;
    config.Status.HYS0_1 = 1;
    config.Status.POL = 0;


    pui8DataTx[0] = TMP108_CONFIGURATION_REGISTER;
    pui8DataTx[1] = config.Byte.byte0;
    pui8DataTx[2] = config.Byte.byte1;

    // #1
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_00, pui8DataTx, size);
    if(status != NO_ERROR){
        return status;
    }

    // #2
    status = I2C_Master_Receive_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_01, pui8DataTx, size);
    if(status != NO_ERROR){
        return status;
    }

    // 0x0F = 1°C;
    // c to f = (c * 1.8) + 32
    // f to c = (f -32) / 1.8
    tempLow = 0x1E0; // 30C = 0x1E0
    tempHigh = 0x460; // 70 Celsius = 0x460

    status = TMP108_SetLimitLow_00(tempLow);
    if(status != NO_ERROR){
        return status;
    }
    status = TMP108_SetLimitLow_01(tempLow);
    if(status != NO_ERROR){
        return status;
    }
    status = TMP108_SetLimitHigh_00(tempHigh);
    if(status != NO_ERROR){
        return status;
    }
    status = TMP108_SetLimitHigh_01(tempHigh);
    if(status != NO_ERROR){
        return status;
    }
    return status;
}

/*
 * Description: Set configuration register. 5 parameters
 *
 *
 */
int TMP108_SetConfig(char *msg)
{
    int status = NO_ERROR;
    TMP108_Configuration config = {0};
    const uint8_t size = 3;
    uint8_t data[3] = {0};
    uint32_t i2c_base = TMP108_I2C_BASE_DEFAULT;
    uint8_t slaveAddress = 0;


    //extract parameters
    if (*msg < 0x30 || *msg > 0x31)
    {
        return VALUE_OUT_OF_RANGE;
    }
    slaveAddress = atoi(msg);
    msg += 2; // skip comma

    if(slaveAddress == 0)
    {
        slaveAddress = TMP108_SLAVE_ADDRESS_00;
    }
    else if(slaveAddress == 1)
    {
        slaveAddress = TMP108_SLAVE_ADDRESS_01;
    }

    if (*msg < 0x30 || *msg > 0x31)
    {
        return VALUE_OUT_OF_RANGE;
    }
    config.Status.POL = atoi(msg);
    msg += 2; // skip comma

    if (*msg < 0x30 || *msg > 0x33)
    {
        return VALUE_OUT_OF_RANGE;
    }
    config.Status.HYS0_1 = atoi(msg);
    msg += 2; // skip comma

    if (*msg < 0x30 || *msg > 0x33)
    {
        return VALUE_OUT_OF_RANGE;
    }
    config.Status.CR0_1 = atoi(msg);
    msg += 2; // skip comma

    if (*msg < 0x30 || *msg > 0x31)
    {
        return VALUE_OUT_OF_RANGE;
    }
    config.Status.TM = atoi(msg);
    msg += 2; // skip comma

    if (*msg < 0x30 || *msg > 0x32)
    {
        return VALUE_OUT_OF_RANGE;
    }
    config.Status.M0_1 = atoi(msg);



    data[0] = TMP108_CONFIGURATION_REGISTER;
    data[1] = config.Byte.byte1;
    data[2] = config.Byte.byte0;

    status = I2C_Master_Transmit_Generic(i2c_base, slaveAddress, data, size);

    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP108_SetLimitHigh_00(uint16_t temp){
    int status;
    const uint8_t size = 3;
    uint8_t pui8DataTx[3];

    temp = temp << 4;

    // ****** High Temp ********
    pui8DataTx[0] = TMP108_TEMP_HIGH_REGISTER;
    pui8DataTx[1] = temp >> 8;
    pui8DataTx[2] = temp;

    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_00, pui8DataTx, size);

    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP108_SetLimitHigh_01(uint16_t temp){
    int status;
    const uint8_t size = 3;
    uint8_t pui8DataTx[3];

    temp = temp << 4;

    // ****** High Temp ********
    pui8DataTx[0] = TMP108_TEMP_HIGH_REGISTER;
    pui8DataTx[1] = temp >> 8;
    pui8DataTx[2] = temp;

    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_01, pui8DataTx, size);

    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP108_SetLimitLow_00(uint16_t temp){
    int status;
    const uint8_t size = 3;
    uint8_t pui8DataTx[3];

    temp = temp << 4;

    // ****** Low Temp ********
    pui8DataTx[0] = TMP108_TEMP_LOW_REGISTER;
    pui8DataTx[1] = temp >> 8;
    pui8DataTx[2] = temp;

    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_00, pui8DataTx, size);

    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP108_SetLimitLow_01(uint16_t temp){
    int status;
    const uint8_t size = 3;
    uint8_t pui8DataTx[3];

    temp = temp << 4;

    // ****** Low Temp ********
    pui8DataTx[0] = TMP108_TEMP_LOW_REGISTER;
    pui8DataTx[1] = temp >> 8;
    pui8DataTx[2] = temp;

    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_01, pui8DataTx, size);

    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP108_GetLimitHigh_00(char *retStr){
    int status;
    uint8_t pui8DataTx[3];
    uint8_t TMP108_LimitValue[2];
    uint16_t TMP108Value;

    pui8DataTx[0] = TMP108_TEMP_HIGH_REGISTER;
    // set to read TMP108 slave 00
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_00, pui8DataTx, 1);
    if (status != NO_ERROR)
    {
        return status;
    }

    // read temp sensor 1
    status = I2C_Master_Receive_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_00, TMP108_LimitValue, 2);
    if(status != NO_ERROR){
       return status;
    }

    TMP108_SwapBytes(TMP108_LimitValue);
    memcpy(&TMP108Value, TMP108_LimitValue, sizeof(uint16_t));

    TMP108_GetStringForTemp(TMP108Value >> 4, retStr);

    return status;
}

void TMP108_SwapBytes(uint8_t *data){
    uint8_t tmp;

    tmp = data[0];
    data[0] = data[1];
    data[1] = tmp;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP108_GetLimitHigh_01(char *retStr){
    int status;
    uint8_t pui8DataTx[3];
    uint8_t TMP108_LimitValue[2];
    uint16_t TMP108Value;

    pui8DataTx[0] = TMP108_TEMP_HIGH_REGISTER;
    // set to read TMP108 slave 01
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_01, pui8DataTx, 1);
    if (status != NO_ERROR)
    {
        return status;
    }

    // read temp sensor 1
    status = I2C_Master_Receive_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_01, TMP108_LimitValue, 2);
    if(status != NO_ERROR){
       return status;
    }

    TMP108_SwapBytes(TMP108_LimitValue);
    memcpy(&TMP108Value, TMP108_LimitValue, sizeof(uint16_t));

    TMP108_GetStringForTemp(TMP108Value >> 4, retStr);
    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP108_GetLimitLow_00(char *retStr){
    int status;
    uint8_t pui8DataTx[3];
    uint8_t TMP108_LimitValue[2];
    uint16_t TMP108Value;

    pui8DataTx[0] = TMP108_TEMP_LOW_REGISTER;
    // set to read TMP108 slave 00
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_00, pui8DataTx, 1);
    if (status != NO_ERROR)
    {
        return status;
    }

    // read temp sensor 1
    status = I2C_Master_Receive_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_00, TMP108_LimitValue, 2);
    if(status != NO_ERROR){
       return status;
    }

    TMP108_SwapBytes(TMP108_LimitValue);
    memcpy(&TMP108Value, TMP108_LimitValue, sizeof(uint16_t));

    TMP108_GetStringForTemp(TMP108Value >> 4, retStr);

    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP108_GetLimitLow_01(char *retStr){
    int status;
    uint8_t pui8DataTx[3];
    uint8_t TMP108_LimitValue[2];
    uint16_t TMP108Value;

    pui8DataTx[0] = TMP108_TEMP_LOW_REGISTER;
    // set to read TMP108 slave 01
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_01, pui8DataTx, 1);
    if (status != NO_ERROR)
    {
        return status;
    }

    // read temp sensor 1
    status = I2C_Master_Receive_Generic(I2C_PORT_1, TMP108_SLAVE_ADDRESS_01, TMP108_LimitValue, 2);
    if(status != NO_ERROR){
       return status;
    }

    TMP108_SwapBytes(TMP108_LimitValue);
    memcpy(&TMP108Value, TMP108_LimitValue, sizeof(uint16_t));

    TMP108_GetStringForTemp(TMP108Value >> 4, retStr);

    return status;
}

/*
 *
 *
 *
 */
int TMP108_ReadTempReg(uint8_t slaveAddress, uint8_t *value){
    uint8_t pui8DataTx[3];
    int status;

    pui8DataTx[0] = TMP108_TEMPERATURE_REGISTER;
    // set to read TMP108 slave 01
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, slaveAddress, pui8DataTx, 1);
    if (status != NO_ERROR){
        return status;
    }
    // read temp sensor 2
    status = I2C_Master_Receive_Generic(I2C_PORT_1, slaveAddress, value, 2);
    if(status != NO_ERROR){
        return status;
    }
    return status;
}



/*
 * Description: Get string format of temperature Celsius/Fahrenheit values
 *
 * Input: The leading string and the temperature value to convert to string
 * Output: The full char string
 */
void TMP108_GetStringForTemp(uint16_t TMP108Value, char *str){
    char str2[32] = {0};
    uint16_t backupValue;
    bool isNeg = false;

    memcpy(&backupValue, &TMP108Value, sizeof(TMP108Value));
    if(TMP108Value >= 0x800){
        TMP108Value = 0x1000 - TMP108Value;
        isNeg = true;
    }

    if(isNeg){
        strcat(str, "-");
    }

    Float_to_string(((float)TMP108Value / 16), 1, str2);
    //sprintf(str2, "%.1f", ((float)TMP108Value / 16));

    strcat(str, str2);

    if(isNeg){
        strcat(str, " -");
    }
    else
    {
        strcat(str, ",");
    }

    Float_to_string( ((TMP108Value / 16) * 1.8) + 32, 1, str2); // get Fahrenheit
    //sprintf(str2, "%.1f", (((float)TMP108Value / 16) * 1.8) + 32);

    strcat(str, str2);

}

int TMP108_GetTemp_0(char *retStr)
{
    int status = NO_ERROR;
    uint8_t TMP108_00_Value[2];
    int32_t TMP108Value_0;

    // read temp #1
    status = TMP108_ReadTempReg((uint8_t)TMP108_SLAVE_ADDRESS_00, TMP108_00_Value);
    if(status != NO_ERROR){
        return status;
    }

    TMP108Value_0 = (uint16_t)( (TMP108_00_Value[0] >> 4) & 0x0F) << 8;
    TMP108Value_0 |= (uint16_t) ( (TMP108_00_Value[0] & 0x0F) << 4 | (TMP108_00_Value[1] & 0xF0) >> 4 );
    TMP108_GetStringForTemp(TMP108Value_0, retStr);

    return status;
}

int TMP108_GetTemp_1(char *retStr)
{
    int status = NO_ERROR;
    uint8_t TMP108_01_Value[2];
    int32_t TMP108Value_1;

    // read temp #2
    status = TMP108_ReadTempReg((uint8_t)TMP108_SLAVE_ADDRESS_01, TMP108_01_Value);
    if(status != NO_ERROR){
        return status;
    }

    TMP108Value_1 = (uint16_t)( (TMP108_01_Value[0] >> 4) & 0x0F) << 8;
    TMP108Value_1 |= (uint16_t) ( (TMP108_01_Value[0] & 0x0F) << 4 | (TMP108_01_Value[1] & 0xF0) >> 4 );
    TMP108_GetStringForTemp(TMP108Value_1, retStr);

    return status;
}

/*
 * Description: Parse message to set High Limi
 *
 *
 */
int TMP108_SetHighLimits(char *msg)
{
    int status = NO_ERROR;
    char dataBytes[16] = {0};
    uint16_t regVal = 0;
    uint8_t deviceSel = 0;
    float tempValue = 0;
    uint8_t isNeg = 0;
    int i;

    // get device number
    i = 0;
    while(*msg != ',')
    {
        dataBytes[i++] = *msg++;
    }
    deviceSel = atoi(dataBytes);
    msg++; // skip comma
    memset(&dataBytes, 0, sizeof(dataBytes));

    // check if negative symbol
    if(*msg == '-'){
        isNeg = 1;
        msg++; // skip '-'
    }

    // get Celsius value
    i = 0;
    while(*msg != '\r')
    {
        dataBytes[i++] = *msg++;
    }
    tempValue = atof(dataBytes);

    // convert to byte value;
    // 0x0F = 1°C = 16;
    // c to f = (c * 1.8) + 32
    // f to c = (f -32) / 1.8
    regVal = (uint16_t)(tempValue * 16);

    // update TMP108
    if(deviceSel == 0)
    {
        TMP108_SetLimitHigh_00(regVal);
    }
    else if(deviceSel == 1)
    {
        TMP108_SetLimitHigh_01(regVal);
    }
    else
    {
        return VALUE_OUT_OF_RANGE;
    }

    return status;
}

/*
 * Description: Parse message to set High Limi
 *
 *
 */
int TMP108_SetLowLimits(char *msg)
{
    int status = NO_ERROR;
    char dataBytes[16] = {0};
    uint16_t regVal = 0;
    uint8_t deviceSel = 0;
    float tempValue = 0;
    uint8_t isNeg = 0;
    int i;

    // get device number
    i = 0;
    while(*msg != ',')
    {
        dataBytes[i++] = *msg++;
    }
    deviceSel = atoi(dataBytes);
    msg++; // skip comma
    memset(&dataBytes, 0, sizeof(dataBytes));

    // check if negative symbol
    if(*msg == '-'){
        isNeg = 1;
        msg++; // skip '-'
    }

    // get Celsius value
    i = 0;
    while(*msg != '\r')
    {
        dataBytes[i++] = *msg++;
    }
    tempValue = atof(dataBytes);

    // convert to byte value;
    // 0x0F = 1°C;
    // c to f = (c * 1.8) + 32
    // f to c = (f -32) / 1.8
    regVal = (uint16_t)(tempValue * 16);

    // update TMP108
    if(deviceSel == 0)
    {
        TMP108_SetLimitLow_00(regVal);
    }
    else if(deviceSel == 1)
    {
        TMP108_SetLimitLow_01(regVal);
    }
    else
    {
        return VALUE_NOT_VALID;
    }

    return status;
}


/*
 * Description: Get configuration register. 16 bit value is read from register as MSB>LSB
 *
 *
 */
int TMP108_GetConfig(char *msg, char *retStr)
{
    int status = NO_ERROR;
    uint8_t pui8DataTx[3];
    uint8_t slaveAddr;
    TMP108_Configuration config = {0};
    char str2[4] = {0};
    int i;

    i = 0;
    while(*msg != '\r')
    {
        pui8DataTx[i++] = *msg++;
    }

    slaveAddr = atoi((char*)pui8DataTx);

    if(slaveAddr == 0)
    {
        slaveAddr = (uint8_t)TMP108_SLAVE_ADDRESS_00;
    }
    else if(slaveAddr ==  1)
    {
        slaveAddr = (uint8_t)TMP108_SLAVE_ADDRESS_01;
    }
    else
    {
        return VALUE_NOT_VALID;
    }

    pui8DataTx[0] = TMP108_CONFIGURATION_REGISTER;

    status = I2C_Master_Transmit_Generic(I2C_PORT_1, slaveAddr, pui8DataTx, 1);
    if (status != NO_ERROR){
        return status;
    }
    memset(pui8DataTx, 0 , sizeof(pui8DataTx));

    // read config
    status = I2C_Master_Receive_Generic(I2C_PORT_1, slaveAddr, pui8DataTx, 2);
    if(status != NO_ERROR){
        return status;
    }

    config.Byte.byte0 = pui8DataTx[1];
    config.Byte.byte1 = pui8DataTx[0];

    strcpy(retStr, "0x");

    sprintf(str2, "%X%X, ", pui8DataTx[0], pui8DataTx[1]);
    strcat(retStr, str2);

    sprintf(str2, "POL %d, ", config.Status.POL);
    strcat(retStr, str2);

    sprintf(str2, "HYS %d, ", config.Status.HYS0_1);
    strcat(retStr, str2);

    sprintf(str2, "CR %d, ", config.Status.CR0_1);
    strcat(retStr, str2);

    sprintf(str2, "FH %d, ", config.Status.FH);
    strcat(retStr, str2);

    sprintf(str2, "FL %d, ", config.Status.FL);
    strcat(retStr, str2);

    sprintf(str2, "TM %d, ", config.Status.TM);
    strcat(retStr, str2);

    sprintf(str2, "M %d, ", config.Status.M0_1);
    strcat(retStr, str2);

    return status;
}
