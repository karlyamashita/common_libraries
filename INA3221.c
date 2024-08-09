/*
 * INA3221.c
 *
 *  Created on: Jun 28, 2021
 *      Author: karl.yamashita
 *
 *  Description: INA3221 Triple-Channel, High-Side Measurement, Shunt and Bus Voltage Monitor
 *  			Note: Unlike most I2C devices, INA3221 requires that the register address be sent first with a stop bit,
 *  					before sending data.
 *
 *
 */

#include "main.h"
#include "INA3221.h"

extern UartBufferStruct uart0_msg;


static int INA3221_WriteConfigReg(uint32_t i2c_base, uint8_t slaveAddress, INA3221_ConfigRegStruct *config);

//static void OnesCompliment(uint16_t *value);

INA3221_Voltage ina3221_Voltage;

/*
 * Description: write values to configuration register
 *
 *
 */
int INA3221_Init(void){
	int status = NO_ERROR;
	char str[32];
	static INA3221_ConfigRegStruct ina3221_ConfigRegStruct = {0};

	ina3221_ConfigRegStruct.Status.ch3_1_En = 7;
	ina3221_ConfigRegStruct.Status.vBus_CT2_0 = VBUS_CT2_0_TIME_1_1ms;
	ina3221_ConfigRegStruct.Status.vShCT2_0 = VBUS_CT2_0_TIME_1_1ms;
	ina3221_ConfigRegStruct.Status.mode3_1 = MODE3_1_MODE_SHUNT_BUS_CONT;

	status = INA3221_WriteConfigReg(INA3221_I2C_PORT_DEFAULT, INA3221_SLAVE_ADDRESS, &ina3221_ConfigRegStruct);
	if(status != NO_ERROR){
	    GetErrorString(status, str);
	    NotifyUser(&uart0_msg, str, true);
	}

	return status;
}


int INA3221_SetConfigReg(char *msg)
{
    int status = NO_ERROR;
    INA3221_ConfigRegStruct config;

    //extract parameters

    if(*msg < 0x30 || *msg > 0x31)
    {
        return VALUE_OUT_OF_RANGE;
    }
    config.Status.rst = atoi(msg);
    msg += 2; // skip comma

    if(*msg < 0x30 || *msg > 0x37)
        {
            return VALUE_OUT_OF_RANGE;
        }
    config.Status.ch3_1_En = atoi(msg);
    msg += 2; // skip comma

    if(*msg < 0x30 || *msg > 0x37)
        {
            return VALUE_OUT_OF_RANGE;
        }
    config.Status.avg2_0 = atoi(msg);
    msg += 2; // skip comma

    if(*msg < 0x30 || *msg > 0x37)
        {
            return VALUE_OUT_OF_RANGE;
        }
    config.Status.vBus_CT2_0 = atoi(msg);
    msg += 2; // skip comma

    if(*msg < 0x30 || *msg > 0x37)
        {
            return VALUE_OUT_OF_RANGE;
        }
    config.Status.vShCT2_0 = atoi(msg);
    msg += 2; // skip comma

    if(*msg < 0x30 || *msg > 0x37)
        {
            return VALUE_OUT_OF_RANGE;
        }
    config.Status.mode3_1 = atoi(msg);


    status = INA3221_WriteConfigReg(INA3221_I2C_PORT_DEFAULT, INA3221_SLAVE_ADDRESS, &config);

    return status;
}


/*
 * Description: Sets up the Configuration register. No stop bit
 *
 * Input:
 * Output: HAL status
 *
 */
static int INA3221_WriteConfigReg(uint32_t i2c_base, uint8_t slaveAddress, INA3221_ConfigRegStruct *config)
{
	int status = NO_ERROR;
	uint32_t writeSize = 0;
	uint8_t data[3] = {0};

	data[0] = CONFIG_REG_ADDR;
	data[1] = config->Bytes.bytes[1];// Note that register bytes are sent most-significant byte first, followed by the least significant byte
	data[2] = config->Bytes.bytes[0];

	writeSize = 3;
	status = I2C_Master_Transmit_Generic(i2c_base, slaveAddress, data, writeSize);
	if(status != NO_ERROR){
		return status;
	}

	return status;
}

/*
 * Description: Read shunt or bus voltage from specified channel.
 *
 *
 */
int INA3221_ReadVoltage(uint32_t i2c_base, uint8_t slaveAddress, ShuntBusVoltage_struct *shuntBusVoltage, uint16_t *value_OUT)
{
	int status = NO_ERROR;
    uint32_t readWriteSize = 0;
	uint8_t data[3] = {0};
	uint16_t value;
	*value_OUT = 0;

	// set register to read from
	switch(shuntBusVoltage->channel){
	case 1:
		if(shuntBusVoltage->shuntOrBus == SHUNT_SEL){
			data[0] = CHAN1_SHUNT_VOLTAGE_ADDR;
		}
		else // Bus voltage
		{
			data[0] = CHAN1_BUS_VOLTAGE_ADDR;
		}
		break;
	case 2:
		if(shuntBusVoltage->shuntOrBus == SHUNT_SEL){
			data[0] = CHAN2_SHUNT_VOLTAGE_ADDR;
		}
		else // Bus voltage
		{
			data[0] = CHAN2_BUS_VOLTAGE_ADDR;
		}
		break;
	case 3:
		if(shuntBusVoltage->shuntOrBus == SHUNT_SEL){
			data[0] = CHAN3_SHUNT_VOLTAGE_ADDR;
		}
		else // Bus voltage
		{
			data[0] = CHAN3_BUS_VOLTAGE_ADDR;
		}
	default:
		break;
	}

	// send register value to read from
	readWriteSize = 1;
	status = I2C_Master_Transmit_Generic(i2c_base, slaveAddress, data, readWriteSize);
	if(status != NO_ERROR){
		return status;
	}

	// read data
	readWriteSize = 2;
	status = I2C_Master_Receive_Generic(i2c_base, slaveAddress, data, readWriteSize);
	if(status != NO_ERROR){
		return status;
	}
	//value = (data[0] << 8) | (data[1]); // return (int)rx[0]<<8|(int)rx[1];
	//value = (data[0] << 8) | (data[1] >> 3); // return (int)rx[0]<<8|(int)rx[1]; // 1.2.11
	value = ((data[0] << 8) | (data[1])) >> 3; // 1.2.12
	*value_OUT = value;

	return status;
}

/*
 * Description: Gets the manufacturer ID or DIE ID. The INA3221 will return "TI" or 0x3220
 *
 *
 */
int INA3221_GetManufacturerID(uint32_t i2c_base, uint8_t slaveAddress, uint8_t id, uint8_t *value_OUT){
	int status = NO_ERROR;
    uint32_t readWriteSize = 0;
	uint8_t data[3] = {0};
    uint16_t value;
    *value_OUT = 0;

	if(id == 1){
		data[0] = MANUFACTURER_ID_ADDR;
	}
	else
	{
		data[0] = DIE_ID_ADDR;
	}

	// send register value to read from
	readWriteSize = 1;
	status = I2C_Master_Receive_Generic(i2c_base, slaveAddress, data, readWriteSize);
	if(status != NO_ERROR){
        return status;
    }

	// read data
	readWriteSize = 2;
	status = I2C_Master_Receive_Generic(i2c_base, slaveAddress, data, readWriteSize);
	if(status != NO_ERROR){
        return status;
    }

	value = (data[0] << 8) | (data[1]); // return (int)rx[0]<<8|(int)rx[1];
	*value_OUT = value;

	return status;
}

/*
 * not used, incomplete
 *
 */
void INA3221_Reset(uint32_t i2c_base, uint8_t slaveAddress){
	INA3221_ConfigRegStruct ina3221_Config;

	ina3221_Config.Status.rst = 1;

	INA3221_WriteConfigReg(i2c_base, slaveAddress, &ina3221_Config);
}


/*
 * Description:
 *
 */
int INA3221_ReadConfigData(char *retStr)
{
	int status = NO_ERROR;
	uint8_t data[3] = {0};
	uint32_t readWriteSize = 0;
	uint32_t i2c_base = INA3221_I2C_PORT_DEFAULT;
	uint8_t slaveAddress = INA3221_SLAVE_ADDRESS;
	char str2[16] = {0};
	INA3221_ConfigRegStruct config = {0};

	data[0] = CONFIG_REG_ADDR;
	// send register value to read from
	readWriteSize  = 1;
	status = I2C_Master_Transmit_Generic(i2c_base, slaveAddress, data, readWriteSize);
	if(status != NO_ERROR){
        return status;
    }

	// read data
	readWriteSize = 2;
	status = I2C_Master_Receive_Generic(i2c_base, slaveAddress, data, readWriteSize);
	if(status != NO_ERROR){
		return status;
	}

    config.Bytes.bytes[1] = data[0];// Note that register bytes are sent most-significant byte first, followed by the least significant byte
    config.Bytes.bytes[0] = data[1];

	strcpy(retStr, "0x");

	sprintf(str2, "%X%X, ", data[0], data[1]);
	strcat(retStr, str2);

	sprintf(str2, "rst %d, ", config.Status.rst);
	strcat(retStr, str2);

	sprintf(str2, "ch %d, ", config.Status.ch3_1_En);
	strcat(retStr, str2);

	sprintf(str2, "avg %d, ", config.Status.avg2_0);
	strcat(retStr, str2);

	sprintf(str2, "vBus %d, ", config.Status.vBus_CT2_0);
	strcat(retStr, str2);

	sprintf(str2, "vSh %d, ", config.Status.vShCT2_0);
	strcat(retStr, str2);

	sprintf(str2, "mode %d, ", config.Status.mode3_1);
	strcat(retStr, str2);


	return status;
}

/*
 * Description: Gets Bus voltage on a channel
 *  Input: pointer to message received
 *
 *
 * 0.1 = 24590
 * 1 = 4110
 * 2.5 = 36877
 * 3.3 = 18445
 * 5 = 0
 *
 * 	wNumber = (uint16_t) ((fabs(shuntBusVoltage->voltage) / lsbValue) + 1); // translate to whole decimal
	wNumber = wNumber << 3; // shift left 3, last 3 bits are cleared to 0
	OnesCompliment(&wNumber);
	wNumber &= 0x7FFF; // clear sign bit if it was set.
	wNumber += 1; // two's compliment
	if(shuntBusVoltage->voltage < 0){ // add 1 to msb to indicate negative number
		wNumber |= 0x8000;
	}
 */

int GetINA3221Bus(char *message, char *retStr){
	int status;
	int i = 0;
	char str2[8];
	ShuntBusVoltage_struct shuntBusVoltage;
    uint32_t i2c_base = INA3221_I2C_PORT_DEFAULT;
    uint8_t slaveAddress = INA3221_SLAVE_ADDRESS;

	i = atoi(message);

	if(i < 1 || i > 3){
		return VALUE_OUT_OF_RANGE; // NACK
	}

	shuntBusVoltage.channel = i;
	shuntBusVoltage.shuntOrBus = BUS_SEL;

	// read data
	status = INA3221_ReadVoltage(i2c_base, slaveAddress, &shuntBusVoltage, &ina3221_Voltage.voltage);
	if(status != NO_ERROR){
        return status;
    }

	// remember A/D voltage value is left shifted by 3
	ina3221_Voltage.channel = i;
	//ina3221_Voltage.voltage = ((ina3221_Voltage.voltage >> 3) & 0x1FFF); // * 0.001;

	strcpy(retStr, "0x");
	itoa(ina3221_Voltage.voltage, str2, BASE_16);
	strcat(retStr, str2);
	strcat(retStr, "\r\n");

	return NO_ERROR;
}

/*
 * Description: Gets Shunt voltage on a channel
 * Input: pointer to message received
 *
 */
int GetINA3221Shunt(char *message, char *retStr){
	int status;
	int i = 0;
	char str2[8];
	ShuntBusVoltage_struct shuntBusVoltage;
    uint32_t i2c_base = INA3221_I2C_PORT_DEFAULT;
    uint8_t slaveAddress = INA3221_SLAVE_ADDRESS;

	i = atoi(message);

	if(i < 1 || i > 3){
		return VALUE_OUT_OF_RANGE; // NACK
	}

	shuntBusVoltage.channel = i;
	shuntBusVoltage.shuntOrBus = SHUNT_SEL;

	status = INA3221_ReadVoltage(i2c_base, slaveAddress, &shuntBusVoltage, &ina3221_Voltage.voltage);
	if(status != NO_ERROR){
        return status;
    }

	// save voltage value
	ina3221_Voltage.channel = i;
	//ina3221_Voltage.voltage = ((ina3221_Voltage.voltage >> 3) & 0x1FFF);// * 0.001;

	strcpy(retStr, "0x");
	itoa(ina3221_Voltage.voltage, str2, BASE_16);
	strcat(retStr, str2);
	strcat(retStr, "\r\n");

	return NO_ERROR;
}



