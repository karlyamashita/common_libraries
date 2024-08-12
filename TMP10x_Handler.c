/*
 * TMP10x_Handler.c
 *
 *  Created on: Apr 24, 2024
 *      Author: karl.yamashita
 */

#include "main.h"

extern UART_DMA_QueueStruct uart2_msg;

/*
 * Description: initialize TMP10x sensor
 *
 * Input:
 * Output:
 */
int TMP10x_Init(I2C_GenericDef *i2c)
{
    int status = NO_ERROR;
    TMP10x_HandleTypeDef TMP10x_Data = {0};

    TMP10x_Data.config.Status.SD = 0;
    TMP10x_Data.config.Status.TM = 1; // interrupt mode
    TMP10x_Data.config.Status.POL = 0;
    TMP10x_Data.config.Status.F0_1 = 2;
    TMP10x_Data.config.Status.R0_1 = 3;
    TMP10x_Data.config.Status.OS_ALERT = 0;
    // = 0x70

    i2c->regSize = 1;
    i2c->registerAddr[0] = TMP10x_CONFIGURATION_REGISTER;
    i2c->dataSize = 2;
    i2c->dataPtr[0] = TMP10x_Data.config.Byte.byte0;
    i2c->dataPtr[1] = TMP10x_Data.config.Byte.byte1;

    status = I2C_Mem_Write_Generic_Method(i2c);
    if(status != NO_ERROR)
    {
        return status;
    }

    // 0x0F = 1�C;
    // c to f = (c * 1.8) + 32
    // f to c = (f -32) / 1.8

    return NO_ERROR;
}

/*
 * Description:
 *
 */
int TMP10x_GetRegisterHandle(I2C_GenericDef *i2c, char *msg, char *retStr)
{
	int status = NO_ERROR;
	char *token;
	char *rest = msg;
	char delim[] =":,\r";
	bool noAck = false;
	uint16_t reg;
	void (*callback) (struct __I2C_GenericDef_ *i2c);

	token = strtok(rest, delim);

	if(strncmp(token, "temp", strlen("temp")) == 0)
	{
		reg = TMP10x_TEMPERATURE_REGISTER;
		callback = TMP10x_TemperatureCallback;
		noAck = true;
	}
	else if(strncmp(token, "config", strlen("config")) == 0)
	{
		reg = TMP10x_CONFIGURATION_REGISTER;
		callback = TMP10x_ConfigCallback;
		noAck = true;
	}
	else if(strncmp(token, "low", strlen("low")) == 0)
	{
		reg = TMP10x_TEMP_LOW_REGISTER;
		callback = TMP10x_LowCallback;
		noAck = true;
	}
	else if(strncmp(token, "high", strlen("high")) == 0)
	{
		reg = TMP10x_TEMP_HIGH_REGISTER;
		callback = TMP10x_HighCallback;
		noAck = true;
	}
	else
	{
		return VALUE_NOT_VALID;
	}

	i2c->dataSize = 2;

	status = TMP10x_Read(i2c, reg, callback);
	if(status != NO_ERROR)
	{
		return status;
	}

	if(noAck)
	{
		return NO_ACK;
	}

	return status;
}

/*
 * Description: Uses I2C_Mem_Write_Generic so be sure to assign callback function
 *
 */
int TMP10x_SetRegisterHandle(I2C_GenericDef *i2c, char *msg)
{
	int status = NO_ERROR;
	char *token;
	char *token2;
	char *rest = msg;
	char delim[] =":,\r";
	float fValue;
	uint16_t regValue;
	uint16_t regAdd;
	bool noAck = false;
	void (*callback) (struct __I2C_GenericDef_ *i2c);

	token = strtok(rest, delim); // register
	token2 = strtok(NULL, delim); // data value

	if(strncmp(token, "temp", strlen("temp")) == 0)
	{
		regAdd = TMP10x_TEMPERATURE_REGISTER;
		callback = NULL;
	}
	else if(strncmp(token, "config", strlen("config")) == 0)
	{
		regAdd = TMP10x_CONFIGURATION_REGISTER;
		callback = NULL;
	}
	else if(strncmp(token, "low", strlen("low")) == 0)
	{
		regAdd = TMP10x_TEMP_LOW_REGISTER;
		callback = NULL;
	}
	else if(strncmp(token, "high", strlen("high")) == 0)
	{
		regAdd = TMP10x_TEMP_HIGH_REGISTER;
		callback = NULL;
	}
	else
	{
		return VALUE_NOT_VALID;
	}

	fValue = atof(token2); // string to float

	regValue = (uint16_t)(fValue * 16);

	i2c->dataSize = 2;
	i2c->dataPtr[0] = regValue >> 8;
	i2c->dataPtr[1] = regValue;

	status = TMP10x_Write(i2c, regAdd, callback);
	if(status != NO_ERROR)
	{
		return status;
	}

	if(noAck)
	{
		return NO_ACK;
	}

	return status;
}



/*
 * Description:
 *
 * Input:
 * Output:
 */
int TMP10x_SetThermostatMode(I2C_GenericDef *i2c, char *msg)
{
    int status = NO_ERROR;
    char *token;
    char *rest = msg;
    char delim[] = ":,\r";
    TMP10x_ConfigDef TMP10x_Config = {0};

    token = strtok(rest, delim);

    // set to read register
    i2c->regSize = 1;
    i2c->registerAddr[0] = TMP10x_CONFIGURATION_REGISTER;
    i2c->dataSize = 2;
    // Read register
    status = I2C_Mem_Read_Generic_Method(i2c);
    if (status != NO_ERROR)
    {
    	return status;
    }

    // modify bit
    TMP10x_Config.Byte.byte0 = i2c->dataPtr[0];
    TMP10x_Config.Status.TM = atoi(token);
    i2c->dataSize = 1;
    i2c->dataPtr[0] = TMP10x_Config.Byte.byte0;

    // set to write data
    i2c->regSize = 1;
    i2c->registerAddr[0] = TMP10x_CONFIGURATION_REGISTER;
    i2c->dataSize = 2;
    i2c->dataPtr[0] = TMP10x_Config.Byte.byte0;
    i2c->dataPtr[1] = TMP10x_Config.Byte.byte1;
	// write data
    status = I2C_Master_Transmit_Generic_Method(i2c);
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

    TMP10x_SwapBytes(tmp101Value); // need to first swap bytes

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

    sprintf(str2, "%.1f C", (float)(u16Value / 16));
    strcat(retStr, str2);

    if(isNeg)
    {
        strcat(retStr, " -");
    }
    else
    {
        strcat(retStr, ",");
    }

    sprintf(str2, "%.1f F", ((u16Value / 16) * 1.8) + 32 ); // get Fahrenheit
    strcat(retStr, str2);
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
void TMP10x_SwapBytes(uint8_t *data)
{
    uint8_t tmp;

    tmp = data[0];
    data[0] = data[1];
    data[1] = tmp;
}


/*
 * Description: USER can create callbacks in this section
 *
 */

void TMP10x_TemperatureCallback(I2C_GenericDef *i2c)
{
	char str[32] = {0};
	char str2[16] = {0};

	TMP101_GetTemperatureString(i2c->dataPtr, str2);
	sprintf(str, "=%s", str2); // TODO - make pointer to command string instead of manually writing the string
	strcat(i2c->cmdPtr, str);

	UART_DMA_NotifyUser(&uart2_msg, i2c->cmdPtr, strlen(i2c->cmdPtr), true);
}

void TMP10x_ConfigCallback(I2C_GenericDef *i2c)
{
	char str[32] = {0};
	TMP10x_ConfigDef config = {0};

	config.Byte.byte0 = i2c->dataPtr[0];
	config.Byte.byte1 = i2c->dataPtr[1];

	sprintf(str, "=CR0_1:%d, Fault:%d", config.Status.CR0_1, config.Status.F0_1);
	strcat(i2c->cmdPtr, str);

	UART_DMA_NotifyUser(&uart2_msg, i2c->cmdPtr, strlen(i2c->cmdPtr), true);
}

void TMP10x_LowCallback(I2C_GenericDef *i2c)
{
	char str[32] = {0};
	char str2[16] = {0};

	TMP101_GetTemperatureString(i2c->dataPtr, str2);
	sprintf(str, "=%s", str2);
	strcat(i2c->cmdPtr, str);

	UART_DMA_NotifyUser(&uart2_msg, i2c->cmdPtr, strlen(i2c->cmdPtr), true);
}

void TMP10x_HighCallback(I2C_GenericDef *i2c)
{
	char str[32] = {0};
	char str2[16] = {0};

	TMP101_GetTemperatureString(i2c->dataPtr, str2);
	sprintf(str, "=%s", str2);
	strcat(i2c->cmdPtr, str);

	UART_DMA_NotifyUser(&uart2_msg, i2c->cmdPtr, strlen(i2c->cmdPtr), true);
}


