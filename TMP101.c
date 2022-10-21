/*
 * TM101.c
 *
 *  Created on: Mar 3, 2021
 *      Author: karl.yamashita
 *
 *      Texas Instruments TMP101 temperature sensor
 *
 *      include UartCharBuffer.h
 */

#include "main.h"
#include "TMP101.h"


/*
 * Description: initialize TMP101 sensors
 *
 * Input:
 * Output:
 */
int TMP101_Init(void){
    int status = NO_ERROR;
    TMP101_Configuration TMP101_Configuration;
    int16_t tempHigh, tempLow;
    uint8_t pui8DataTx[3]= {0};
    uint16_t size = 2;

    TMP101_Configuration.Status.SD = 0;
    TMP101_Configuration.Status.TM = 1; // interrupt mode
    TMP101_Configuration.Status.POL = 0;
    TMP101_Configuration.Status.F0 = 0;
    TMP101_Configuration.Status.F1 = 1;
    TMP101_Configuration.Status.R0 = 1;
    TMP101_Configuration.Status.R1 = 1;
    TMP101_Configuration.Status.OS_ALERT = 0;
    // = 0x70

    pui8DataTx[0] = TMP101_CONFIGURATION_REGISTER;
    pui8DataTx[1] = TMP101_Configuration.Byte.byte0;

    // #1
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_00, pui8DataTx, size);
    if(status != NO_ERROR){
        return status;
    }

    // #2
    status = I2C_Master_Receive_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_01, pui8DataTx, size);
    if(status != NO_ERROR){
        return status;
    }

    // 0x0F = 1°C;
    // c to f = (c * 1.8) + 32
    // f to c = (f -32) / 1.8
    tempLow = 0x1E0; // 30C = 0x1E0
    tempHigh = 0x460; // 70 Celsius = 0x460

    status = TMP101_SetLimitLow_00(tempLow);
    if(status != NO_ERROR){
        return status;
    }
    status = TMP101_SetLimitLow_01(tempLow);
    if(status != NO_ERROR){
        return status;
    }
    status = TMP101_SetLimitHigh_00(tempHigh);
    if(status != NO_ERROR){
        return status;
    }
    status = TMP101_SetLimitHigh_01(tempHigh);
    if(status != NO_ERROR){
        return status;
    }
    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP101_SetLimitHigh_00(uint16_t temp){
    int status = NO_ERROR;
    const uint8_t size = 3;
    uint8_t pui8DataTx[3];

    temp = temp << 4;

    // ****** High Temp ********
    pui8DataTx[0] = TMP101_TEMP_HIGH_REGISTER;
    pui8DataTx[1] = temp >> 8;
    pui8DataTx[2] = temp;

    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_00, pui8DataTx, size);
    if (status != NO_ERROR)
    {

    }
    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP101_SetLimitHigh_01(uint16_t temp){
    int status = NO_ERROR;
    const uint8_t size = 3;
    uint8_t pui8DataTx[3];

    temp = temp << 4;

    // ****** High Temp ********
    pui8DataTx[0] = TMP101_TEMP_HIGH_REGISTER;
    pui8DataTx[1] = temp >> 8;
    pui8DataTx[2] = temp;

    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_01, pui8DataTx, size);
    if (status != NO_ERROR)
    {

    }
    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP101_SetLimitLow_00(uint16_t temp){
    int status = NO_ERROR;
    const uint8_t size = 3;
    uint8_t pui8DataTx[3];

    temp = temp << 4;

    // ****** Low Temp ********
    pui8DataTx[0] = TMP101_TEMP_LOW_REGISTER;
    pui8DataTx[1] = temp >> 8;
    pui8DataTx[2] = temp;

    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_00, pui8DataTx, size);
    if (status != NO_ERROR)
    {

    }
    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP101_SetLimitLow_01(uint16_t temp){
    int status = NO_ERROR;
    const uint8_t size = 3;
    uint8_t pui8DataTx[3];

    temp = temp << 4;

    // ****** Low Temp ********
    pui8DataTx[0] = TMP101_TEMP_LOW_REGISTER;
    pui8DataTx[1] = temp >> 8;
    pui8DataTx[2] = temp;

    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_01, pui8DataTx, size);
    if (status != NO_ERROR)
    {

    }
    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP101_GetLimitHigh_00(char *retStr){
    int status = NO_ERROR;
    uint8_t pui8DataTx[3];
    uint8_t tmp101_LimitValue[2];
    uint16_t tmp101Value;

    pui8DataTx[0] = TMP101_TEMP_HIGH_REGISTER;
    // set to read tmp101 slave 00
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_00, pui8DataTx, 1);
    if (status != NO_ERROR)
    {
        return status;
    }

    // read temp sensor 1
    status = I2C_Master_Receive_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_00, tmp101_LimitValue, 2);
    if(status != NO_ERROR){
       return status;
    }

    TMP101_SwapBytes(tmp101_LimitValue);
    memcpy(&tmp101Value, tmp101_LimitValue, sizeof(uint16_t));

    TMP101_GetStringForTemp(tmp101Value >> 4, retStr);

    return status;
}

void TMP101_SwapBytes(uint8_t *data){
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
int TMP101_GetLimitHigh_01(char *retStr){
    int status = NO_ERROR;
    uint8_t pui8DataTx[3];
    uint8_t tmp101_LimitValue[2];
    uint16_t tmp101Value;

    pui8DataTx[0] = TMP101_TEMP_HIGH_REGISTER;
    // set to read tmp101 slave 01
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_01, pui8DataTx, 1);
    if (status != NO_ERROR)
    {
        return status;
    }

    // read temp sensor 1
    status = I2C_Master_Receive_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_01, tmp101_LimitValue, 2);
    if(status != NO_ERROR){
       return status;
    }

    TMP101_SwapBytes(tmp101_LimitValue);
    memcpy(&tmp101Value, tmp101_LimitValue, sizeof(uint16_t));

    TMP101_GetStringForTemp(tmp101Value >> 4, retStr);
    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP101_GetLimitLow_00(char *retStr){
    int status;
    uint8_t pui8DataTx[3];
    uint8_t tmp101_LimitValue[2];
    uint16_t tmp101Value;

    pui8DataTx[0] = TMP101_TEMP_LOW_REGISTER;
    // set to read tmp101 slave 00
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_00, pui8DataTx, 1);
    if (status != NO_ERROR)
    {
        return status;
    }

    // read temp sensor 1
    status = I2C_Master_Receive_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_00, tmp101_LimitValue, 2);
    if(status != NO_ERROR){
       return status;
    }

    TMP101_SwapBytes(tmp101_LimitValue);
    memcpy(&tmp101Value, tmp101_LimitValue, sizeof(uint16_t));

    TMP101_GetStringForTemp(tmp101Value >> 4, retStr);

    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP101_GetLimitLow_01(char *retStr){
    int status;
    uint8_t pui8DataTx[3];
    uint8_t tmp101_LimitValue[2];
    uint16_t tmp101Value;

    pui8DataTx[0] = TMP101_TEMP_LOW_REGISTER;
    // set to read tmp101 slave 01
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_01, pui8DataTx, 1);
    if (status != NO_ERROR)
    {
        return status;
    }

    // read temp sensor 1
    status = I2C_Master_Receive_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_01, tmp101_LimitValue, 2);
    if(status != NO_ERROR){
       return status;
    }

    TMP101_SwapBytes(tmp101_LimitValue);
    memcpy(&tmp101Value, tmp101_LimitValue, sizeof(uint16_t));

    TMP101_GetStringForTemp(tmp101Value >> 4, retStr);

    return status;
}

/*
 *
 *
 *
 */
int TMP101_ReadTempReg(uint8_t sensorSlaveAddress, uint8_t *value){
    uint8_t pui8DataTx[3];
    int status = NO_ERROR;

    pui8DataTx[0] = TMP101_TEMPERATURE_REGISTER;
    // set to read tmp101 slave 01
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, sensorSlaveAddress, pui8DataTx, 1);
    if (status != NO_ERROR){
        return status;
    }
    // read temp sensor 2
    status = I2C_Master_Receive_Generic(I2C_PORT_1, sensorSlaveAddress, value, 2);
    if(status != NO_ERROR){
        return status;
    }
    return NO_ERROR;
}



/*
 * Description: Get string format of temperature Celsius/Fahrenheit values
 *
 * Input: The leading string and the temperature value to convert to string
 * Output: The full char string
 */
void TMP101_GetStringForTemp(uint16_t tmp101Value, char *str){
    char str2[32] = {0};
    uint16_t backupValue;
    bool isNeg = false;

    memcpy(&backupValue, &tmp101Value, sizeof(tmp101Value));
    if(tmp101Value >= 0x800){
        tmp101Value = 0x1000 - tmp101Value;
        isNeg = true;
    }

    if(isNeg){
        strcat(str, "-");
    }

    Float_to_string(((float)tmp101Value / 16), 1, str2);
    strcat(str, str2);

    if(isNeg){
        strcat(str, " -");
    }
    else
    {
        strcat(str, ",");
    }
    Float_to_string( ((tmp101Value / 16) * 1.8) + 32, 1, str2); // get Fahrenheit
    strcat(str, str2);

}

int TMP101_GetTemp_0(char *retStr)
{
    int status = NO_ERROR;
    uint8_t tmp101_00_Value[2];
    int32_t tmp101Value_0;

    // read temp #1
    status = TMP101_ReadTempReg((uint8_t)TMP101_SLAVE_ADDRESS_00, tmp101_00_Value);
    if(status != NO_ERROR){
        return status;
    }

    tmp101Value_0 = (uint16_t)( (tmp101_00_Value[0] >> 4) & 0x0F) << 8;
    tmp101Value_0 |= (uint16_t) ( (tmp101_00_Value[0] & 0x0F) << 4 | (tmp101_00_Value[1] & 0xF0) >> 4 );
    TMP101_GetStringForTemp(tmp101Value_0, retStr);

    return status;
}

int TMP101_GetTemp_1(char *retStr)
{
    int status = NO_ERROR;
    uint8_t tmp101_01_Value[2];
    int32_t tmp101Value_1;

    // read temp #2
    status = TMP101_ReadTempReg((uint8_t)TMP101_SLAVE_ADDRESS_01, tmp101_01_Value);
    if(status != NO_ERROR){
        return status;
    }

    tmp101Value_1 = (uint16_t)( (tmp101_01_Value[0] >> 4) & 0x0F) << 8;
    tmp101Value_1 |= (uint16_t) ( (tmp101_01_Value[0] & 0x0F) << 4 | (tmp101_01_Value[1] & 0xF0) >> 4 );
    TMP101_GetStringForTemp(tmp101Value_1, retStr);

    return status;
}

/*
 * Description: Parse message to set High Limi
 *
 *
 */
int TMP101_SetHighLimits(char *msg)
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

    // update tmp101
    if(deviceSel == 0)
    {
        TMP101_SetLimitHigh_00(regVal);
    }
    else if(deviceSel == 1)
    {
        TMP101_SetLimitHigh_01(regVal);
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
int TMP101_SetLowLimits(char *msg)
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

    // update tmp101
    if(deviceSel == 0)
    {
        TMP101_SetLimitLow_00(regVal);
    }
    else if(deviceSel == 1)
    {
        TMP101_SetLimitLow_01(regVal);
    }
    else
    {
        return VALUE_NOT_VALID;
    }

    return status;
}

int TMP101_SetComparatorMode(char *msg)
{
    int status = NO_ERROR;
    char dataBytes[4] = {0};
    uint8_t deviceSel = 0;
    uint8_t data[4];
    uint8_t slaveAddr;
    int i;
    TMP101_Configuration TMP101_Configuration;

    // get device number
    i = 0;
    while(*msg != '\r')
    {
        dataBytes[i++] = *msg++;
    }
    deviceSel = atoi(dataBytes);

    if(deviceSel == 0){
        slaveAddr = (uint8_t)TMP101_SLAVE_ADDRESS_00;
    }
    else if(deviceSel == 1)
    {
        slaveAddr = (uint8_t)TMP101_SLAVE_ADDRESS_01;
    }
    else
    {
        return VALUE_NOT_VALID;
    }

    data[0] = (uint8_t)TMP101_CONFIGURATION_REGISTER;
    // set to read tmp101 slave 00
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, slaveAddr, data, 1);
    if (status == NO_ERROR)
    {
        // read temp sensor 0
        status = I2C_Master_Receive_Generic(I2C_PORT_1, slaveAddr, data, 1);
        if(status != NO_ERROR){
           return status;
        }
    }

    // modify bit
    TMP101_Configuration.Byte.byte0 = data[0];
    TMP101_Configuration.Status.TM = 0;
    data[0] = (uint8_t)TMP101_CONFIGURATION_REGISTER;
    data[1] = TMP101_Configuration.Byte.byte0;

    // write data
    status = I2C_Master_Receive_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_00, data, 2);
    if(status != NO_ERROR){
        return status;
    }

    return status;
}

int TMP101_SetInterruptMode(char *msg)
{
    int status = NO_ERROR;
    char dataBytes[4] = {0};
    uint8_t deviceSel = 0;
    uint8_t data[4];
    uint8_t slaveAddr;
    TMP101_Configuration TMP101_Configuration;
    int i;

    // get device number
    i = 0;
    while(*msg != '\r')
    {
        dataBytes[i++] = *msg++;
    }
    deviceSel = atoi(dataBytes);

    if(deviceSel == 0){
        slaveAddr = (uint8_t)TMP101_SLAVE_ADDRESS_00;
    }
    else if(deviceSel == 1)
    {
        slaveAddr = (uint8_t)TMP101_SLAVE_ADDRESS_01;
    }
    else
    {
        return VALUE_NOT_VALID;
    }

    data[0] = (uint8_t)TMP101_CONFIGURATION_REGISTER;
    // set to read tmp101 slave 01
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, slaveAddr, data, 1);
    if (status == NO_ERROR)
    {
        // read temp sensor 1
        status = I2C_Master_Receive_Generic(I2C_PORT_1, slaveAddr, data, 1);
        if(status != NO_ERROR){
           return status;
        }
    }

    // modify bit
    TMP101_Configuration.Byte.byte0 = data[0];
    TMP101_Configuration.Status.TM = 0;
    data[0] = (uint8_t)TMP101_CONFIGURATION_REGISTER;
    data[1] = TMP101_Configuration.Byte.byte0;

    // write data
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, TMP101_SLAVE_ADDRESS_00, data, 2);
    if(status != NO_ERROR){
        return status;
    }

    return status;
}

int TMP101_GetConfig(char *msg, char *retStr)
{
    int status = NO_ERROR;
    uint8_t pui8DataTx[3];
    uint8_t slaveAddr;
    uint16_t tmp101Value;
    int i;

    i = 0;
    while(*msg != '\r')
    {
        pui8DataTx[i++] = *msg++;
    }

    slaveAddr = atoi((char*)pui8DataTx);

    if(slaveAddr == 0)
    {
        slaveAddr = (uint8_t)TMP101_SLAVE_ADDRESS_00;
    }
    else if(slaveAddr ==  1)
    {
        slaveAddr = (uint8_t)TMP101_SLAVE_ADDRESS_01;
    }
    else
    {
        return VALUE_NOT_VALID;
    }

    pui8DataTx[0] = TMP101_CONFIGURATION_REGISTER;
    // set to read tmp101 slave 01
    status = I2C_Master_Transmit_Generic(I2C_PORT_1, slaveAddr, pui8DataTx, 1);
    if (status != NO_ERROR){
        return status;
    }
    memset(pui8DataTx, 0 , sizeof(pui8DataTx));

    // read config
    status = I2C_Master_Receive_Generic(I2C_PORT_1, slaveAddr, pui8DataTx, 1);
    if(status != NO_ERROR){
        return status;
    }

    memcpy(&tmp101Value, pui8DataTx, sizeof(uint8_t));
    sprintf(retStr, "0x%X", tmp101Value);

    return status;
}
