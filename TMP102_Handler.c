/*
 * TMP102_Handler.c
 *
 *  Created on: Apr 24, 2024
 *      Author: karl.yamashita
 */

#include "main.h"

extern I2C_HandleTypeDef hi2c1;

uint8_t tmp102_dataRx[3]; // REG and 2 data bytes
uint8_t tmp102_dataTx[3]; // REG and 2 data bytes

I2C_DataStruct tmp102_data_1 =
{
	.instance = &hi2c1,
	.slaveAddr = TMP102_SLAVE_ADDRESS_00,
	.Rx.data = tmp102_dataRx,
	.Tx.data = tmp102_dataTx,
	.Rx.timeout = 100,
	.Tx.timeout = 100
};

/*
 * Description: initialize TMP102 sensor
 *
 * Input:
 * Output:
 */
int TMP102_Init(I2C_DataStruct *msg)
{
    int status = NO_ERROR;
    TMP102_Configuration TMP102_Configuration = {0};

    TMP102_Configuration.Status.SD = 0;
    TMP102_Configuration.Status.TM = 1; // interrupt mode
    TMP102_Configuration.Status.POL = 0;
    TMP102_Configuration.Status.F0 = 0;
    TMP102_Configuration.Status.F1 = 1;
    TMP102_Configuration.Status.R0 = 1;
    TMP102_Configuration.Status.R1 = 1;
    TMP102_Configuration.Status.OS_ALERT = 0;
    // = 0x70

    msg->Tx.regSize = 1;
    msg->Tx.registerAddr = (uint16_t) TMP102_CONFIGURATION_REGISTER;
    msg->Tx.dataSize = 2;
    msg->Tx.data[0] = TMP102_Configuration.Byte.byte0;
    msg->Tx.data[1] = TMP102_Configuration.Byte.byte1;

    status = I2C_Mem_Write_Generic(msg);
    if(status != NO_ERROR){
        return status;
    }

    // 0x0F = 1�C;
    // c to f = (c * 1.8) + 32
    // f to c = (f -32) / 1.8

    return NO_ERROR;
}



/*
 * User should not need to modify anything below here.
 */

/*
 * Description:
 *
 */
int TMP102_GetRegisterHandle(char *msg, char *retStr)
{
	int status = NO_ERROR;
	char *token;
	char *rest = msg;
	char delim[] =",\r";

	token = strtok_r(rest, delim, &rest);

	if(strncmp(token, "temp", strlen("")) == 0)
	{
		tmp102_data_1.Rx.registerAddr = TMP102_TEMPERATURE_REGISTER;
	}
	else if(strncmp(token, "config", strlen("")) == 0)
	{
		tmp102_data_1.Rx.registerAddr = TMP102_CONFIGURATION_REGISTER;
	}
	else if(strncmp(token, "low", strlen("")) == 0)
	{
		tmp102_data_1.Rx.registerAddr = TMP102_TEMP_LOW_REGISTER;
	}
	else if(strncmp(token, "low", strlen("")) == 0)
	{
		tmp102_data_1.Rx.registerAddr = TMP102_TEMP_HIGH_REGISTER;
	}
	else
	{
		return VALUE_NOT_VALID;
	}

	tmp102_data_1.Rx.regSize = 1;
	tmp102_data_1.Rx.dataSize = 2;
	status = I2C_Mem_Read_Generic(&tmp102_data_1);
	if(status != NO_ERROR)
	{
		return status;
	}

	TMP101_GetTemperatureString(tmp102_data_1.Rx.data, retStr);

	return NO_ERROR;
}

/*
 * Description:
 *
 */
int TMP102_SetRegisterHandle(char *msg)
{
	int status = NO_ERROR;
	char *token;
	char *token2;
	char *rest = msg;
	char delim[] =",\r";
	float fValue;
	uint16_t regValue;

	token = strtok_r(rest, delim, &rest); // register
	token2 = strtok_r(rest, delim, &rest); // data value

	if(strncmp(token, "temp", strlen("")) == 0)
	{
		tmp102_data_1.Tx.registerAddr = TMP102_TEMPERATURE_REGISTER;
	}
	else if(strncmp(token, "config", strlen("")) == 0)
	{
		tmp102_data_1.Tx.registerAddr = TMP102_CONFIGURATION_REGISTER;
	}
	else if(strncmp(token, "low", strlen("")) == 0)
	{
		tmp102_data_1.Tx.registerAddr = TMP102_TEMP_LOW_REGISTER;
	}
	else if(strncmp(token, "low", strlen("")) == 0)
	{
		tmp102_data_1.Tx.registerAddr = TMP102_TEMP_HIGH_REGISTER;
	}
	else
	{
		return VALUE_NOT_VALID;
	}

	fValue = atof(token2); // string to float

	regValue = (uint16_t)(fValue * 16);

	tmp102_data_1.Tx.dataSize = 2;
	tmp102_data_1.Tx.data[0] = regValue >> 8;
	tmp102_data_1.Tx.data[1] = regValue;

	status = I2C_Mem_Write_Generic(&tmp102_data_1);

	return status;
}



/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP102_SetThermostatMode(char *msg)
{
    int status = NO_ERROR;
    char *token;
    char *rest = msg;
    char delim[] = ",\r";
    TMP102_Configuration TMP102_Configuration = {0};

    token = strtok_r(rest, delim, &rest);

    // set to read register
    tmp102_data_1.Rx.regSize = 1;
    tmp102_data_1.Rx.registerAddr = TMP102_CONFIGURATION_REGISTER;
    tmp102_data_1.Rx.dataSize = 2;
    status = I2C_Mem_Read_Generic(&tmp102_data_1);
    if (status != NO_ERROR)
    {
    	return status;
    }

    // modify bit
    TMP102_Configuration.Byte.byte0 = tmp102_data_1.Rx.data[0];
    TMP102_Configuration.Status.TM = atoi(token);

    // write data
    tmp102_data_1.Tx.regSize = 1;
    tmp102_data_1.Tx.registerAddr = TMP102_CONFIGURATION_REGISTER;
    tmp102_data_1.Tx.dataSize = 2;
    tmp102_data_1.Tx.data[0] = TMP102_Configuration.Byte.byte0;
    tmp102_data_1.Tx.data[1] = TMP102_Configuration.Byte.byte1;
    status = I2C_Mem_Write_Generic(&tmp102_data_1);
    if(status != NO_ERROR)
    {
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
void TMP101_GetTemperatureString(uint8_t *tmp101Value, char *retStr)
{
    char str2[32] = {0};
    uint16_t u16Value;
    bool isNeg = false;

    TMP102_SwapBytes(tmp101Value); // need to first swap bytes

    memcpy(&u16Value, tmp101Value, sizeof(uint16_t));

    u16Value = u16Value >> 4; // need to shift to right 4 bits.

    if(u16Value >= 0x800) // test for negative bit.
    {
    	u16Value ^= 0x800; // invert bit back to zero
        isNeg = true;
    }

    if(isNeg)
    {
        strcat(retStr, "-");
    }

    sprintf(str2, "%.1f", (float)(u16Value / 16));
    strcat(retStr, str2);

    if(isNeg)
    {
        strcat(retStr, " -");
    }
    else
    {
        strcat(retStr, ",");
    }

    sprintf(str2, "%.1f", ((u16Value / 16) * 1.8) + 32 ); // get Fahrenheit
    strcat(retStr, str2);
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
void TMP102_SwapBytes(uint8_t *data)
{
    uint8_t tmp;

    tmp = data[0];
    data[0] = data[1];
    data[1] = tmp;
}

