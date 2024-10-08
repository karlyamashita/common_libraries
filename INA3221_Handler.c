/*
 * INA3221_Handler.c
 *
 *  Created on: Jun 17, 2024
 *      Author: karl.yamashita
 */

#include "main.h"

extern UART_DMA_QueueStruct uart2_msg;

int INA3221_SetConfigReg(I2C_GenericDef *i2c, char *msg)
{
    int status = NO_ERROR;
    char *rst;
    char *ch_En;
    char *avg2_0;
    char *vBus_CT2_0;
    char *vShCT2_0;
    char *mode3_1;

    INA3221_HandleTypeDef ina = {0};

    rst = strtok(msg, ",");
    ch_En = strtok(NULL, ",");
    avg2_0 = strtok(NULL, ",");
    vBus_CT2_0 = strtok(NULL, ",");
    vShCT2_0 = strtok(NULL, ",");
    mode3_1 = strtok(NULL, ",");

    //extract parameters

    if(*rst < 0x30 || *rst > 0x31)
    {
        return VALUE_OUT_OF_RANGE;
    }
    ina.config.Status.rst = atoi(rst);

    if(*ch_En < 0x30 || *ch_En > 0x37)
    {
        return VALUE_OUT_OF_RANGE;
    }
    ina.config.Status.ch_En = atoi(ch_En);

    if(*avg2_0 < 0x30 || *avg2_0 > 0x37)
    {
        return VALUE_OUT_OF_RANGE;
    }
    ina.config.Status.avg2_0 = atoi(avg2_0);

    if(*vBus_CT2_0 < 0x30 || *vBus_CT2_0 > 0x37)
    {
        return VALUE_OUT_OF_RANGE;
    }
    ina.config.Status.vBus_CT2_0 = atoi(vBus_CT2_0);

    if(*vShCT2_0 < 0x30 || *vShCT2_0 > 0x37)
    {
        return VALUE_OUT_OF_RANGE;
    }
    ina.config.Status.vShCT2_0 = atoi(vShCT2_0);

    if(*mode3_1 < 0x30 || *mode3_1 > 0x37)
    {
        return VALUE_OUT_OF_RANGE;
    }
    ina.config.Status.mode3_1 = atoi(mode3_1);

    i2c->dataPtr[0] = ina.config.Bytes.data[1]; // swap bytes when copying
    i2c->dataPtr[1] = ina.config.Bytes.data[0];

    status = INA3221_Write(i2c, INA3221_CONFIG, NULL);

    return status;
}

int INA3221_GetConfigReg(I2C_GenericDef *i2c, char *retStr)
{
    int status = NO_ERROR;

    i2c->dataSize = 2;

    status = INA3221_Read(i2c, INA3221_CONFIG, INA3221_ConfigCallback);
    if(status != NO_ERROR)
    {
    	return status;
    }

    status = NO_ACK;

    return status;
}

int INA3221_GetBusShunt(I2C_GenericDef *i2c, char *msg, char *retStr)
{
    int status = NO_ERROR;
    char *token; // channel
    char *token2; // bus or shunt
    char *rest = msg;
    char delim[] = ":,\r";
    uint32_t chan;
    uint32_t busShunt; // 0 or 1
    uint32_t busShuntReg; // register values
    INA3221_ShuntBusVoltDef busShunt_voltage = {0};

    token = strtok(rest, delim); // TM4C12x does not support strtok_r library
    token2 = strtok(NULL, delim);

    chan = atoi(token);
    if(chan < 1 || chan > 3)
    {
        return VALUE_OUT_OF_RANGE;
    }

    if(strncmp(token2, "bus", strlen("bus")) == 0)
	{
    	busShunt = 0;
	}
    else if(strncmp(token2, "shunt", strlen("shunt")) == 0)
    {
    	busShunt = 1;
    }
    else
    {
    	return COMMAND_UNKNOWN;
    }

    switch(chan)
    {
    case 1:
    	if(busShunt) // shunt
    	{
    		busShuntReg = INA3221_CH1_SHUNT;
    	}
    	else // bus
    	{
    		busShuntReg = INA3221_CH1_BUS;
    	}
    	break;
    case 2:
    	if(busShunt) // shunt
    	{
    		busShuntReg = INA3221_CH2_SHUNT;
    	}
    	else // bus
    	{
    		busShuntReg = INA3221_CH2_BUS;
    	}
    	break;
    case 3:
    	if(busShunt) // shunt
    	{
    		busShuntReg = INA3221_CH3_SHUNT;
    	}
    	else // bus
    	{
    		busShuntReg = INA3221_CH3_BUS;
    	}
    	break;
    }

    i2c->dataSize = 2;
#ifdef USING_CALLBACK
    status = INA3221_Read(i2c, busShuntReg, INA3221_VoltageCallback);
    if(status != NO_ERROR)
    {
    	return status;
    }

    status = NO_ACK;
#else
    status = INA3221_Read(i2c, busShuntReg, NULL);
	if(status != NO_ERROR)
	{
		return status;
	}

    busShunt_voltage.Bytes.data[0] = i2c->dataPtr[1]; // swap bytes
	busShunt_voltage.Bytes.data[1] = i2c->dataPtr[0];

	if(busShunt == 0) // bus
	{
		sprintf(retStr, "%d", (uint16_t) ((busShunt_voltage.Status.shuntBusVoltage * BUS_VOLTAGE_LSB) * 1000));
	}
	else if(busShunt == 1) // shunt
	{
		sprintf(retStr, "%d", (uint16_t) ((busShunt_voltage.Status.shuntBusVoltage * SHUNT_VOLTAGE_LSB) * 1000000)); // current = (voltage * SHUNT_VOLTAGE_LSB) * 1M
	}
#endif

    return status;
}

// ************ CALLBACK *****************
void INA3221_ConfigCallback(I2C_GenericDef *i2c)
{
	char str2[64] = {0};
	INA3221_ConfigRegDef config = {0};

	// reading INA3221 registers start with MSB then LSB. So we need to swap bytes when copying to config struct.
	config.Bytes.data[0] = i2c->dataPtr[1];
	config.Bytes.data[1] = i2c->dataPtr[0];

	sprintf(str2, "= data[1]=0x%X,data[0]=0x%X, ch1-3= %d, mode3_1= %d", config.Bytes.data[1], config.Bytes.data[0], config.Status.ch_En, config.Status.mode3_1);
	strcat(i2c->cmdPtr, str2);

	UART_DMA_NotifyUser(&uart2_msg, i2c->cmdPtr, strlen(i2c->cmdPtr), true);
}

void INA3221_VoltageCallback(I2C_GenericDef *i2c)
{
	char str2[32] = {0};
	INA3221_ShuntBusVoltDef voltage = {0};

	voltage.Bytes.data[0] = i2c->dataPtr[1]; // swap bytes when copying
	voltage.Bytes.data[1] = i2c->dataPtr[0];

	if(i2c->deviceAddr == INA3221_CH1_BUS || INA3221_CH2_BUS || INA3221_CH3_BUS)
	{
		sprintf(str2, "= %d", (uint16_t) ((voltage.Status.shuntBusVoltage * BUS_VOLTAGE_LSB) * 1000));
	}
	else if(i2c->deviceAddr == INA3221_CH1_SHUNT || INA3221_CH2_SHUNT || INA3221_CH3_SHUNT)
	{
		sprintf(str2, "= %d", (uint16_t) ((voltage.Status.shuntBusVoltage * SHUNT_VOLTAGE_LSB) * 1000000)); // current = (voltage * SHUNT_VOLTAGE_LSB) * 1M
	}

	strcat(i2c->cmdPtr, str2);

	UART_DMA_NotifyUser(&uart2_msg, i2c->cmdPtr, strlen(i2c->cmdPtr), true);
}


