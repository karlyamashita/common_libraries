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


//static void OnesCompliment(uint16_t *value);

INA3221_Voltage ina3221_Voltage;

/*
 * Description: write values to configuration register
 *
 *
 */
int INA3221_Init(void){
	int errorCode = 0;
	static INA3221_ConfigRegStruct ina3221_ConfigRegStruct = {0};

	ina3221_ConfigRegStruct.Status.ch1_En = 1;
	ina3221_ConfigRegStruct.Status.ch2_En = 1;
	ina3221_ConfigRegStruct.Status.ch3_En = 1;
	ina3221_ConfigRegStruct.Status.vBus_CT2_0 = VBUS_CT2_0_TIME_1_1ms;
	ina3221_ConfigRegStruct.Status.vShCT2_0 = VBUS_CT2_0_TIME_1_1ms;
	ina3221_ConfigRegStruct.Status.mode3_1 = MODE3_1_MODE_SHUNT_BUS_CONT;

	errorCode = INA3221_SetConfigReg(&ina3221_ConfigRegStruct);
	if(errorCode != 0){
		return errorCode;
	}

	return errorCode;
}

/*
 * Description: Sets up the Configuration register.
 *
 * Input:
 * Output: HAL status
 *
 */
int INA3221_SetConfigReg(INA3221_ConfigRegStruct *config){
	int status = NO_ERROR;
	uint8_t data[3] = {0};

	// TODO - define a generic i2c_Write_Register function for whatever uC being used. In that function, call the uC I2C write driver.
	data[0] = CONFIG_REG_ADDR;
	status = I2C_Send_Data(INA3221_SLAVE_ADDRESS, data, 1, XIIC_STOP);
	if(status != NO_ERROR){
		return status;
	}

	data[0] = config->Bytes.bytes[1];// Note that register bytes are sent most-significant byte first, followed by the least significant byte
	data[1] = config->Bytes.bytes[0];

	status = I2C_Send_Data(INA3221_SLAVE_ADDRESS, data, 2, XIIC_STOP);
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
int INA3221_ReadVoltage(ShuntBusVoltage_struct *shuntBusVoltage, uint16_t *value_OUT){
	int status = NO_ERROR;
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
	status = I2C_Send_Data(INA3221_SLAVE_ADDRESS, data, 1, XIIC_STOP);
	if(status != NO_ERROR){
		return status;
	}

	// read data
	status = I2C_Get_Data(INA3221_SLAVE_ADDRESS, data, 2, XIIC_STOP);
	if(status != NO_ERROR){
		return status;
	}
	value = (data[0] << 8) | (data[1]); // return (int)rx[0]<<8|(int)rx[1];
	*value_OUT = value;

	return status;
}

/*
 * Description: Gets the manufacturer ID or DIE ID. The INA3221 will return "TI" or 0x3220
 *
 *
 */
int INA3221_GetManufacturerID(uint8_t id, uint8_t *value_OUT){
	int status = NO_ERROR;
	int sentCount = 0; // the number of bytes sent
	uint8_t data[3] = {0};

	if(id == 1){
		data[0] = MANUFACTURER_ID_ADDR;
	}
	else
	{
		data[0] = DIE_ID_ADDR;
	}

	// send register value to read from
	sentCount = I2C_Send_Data(INA3221_SLAVE_ADDRESS, data, 1, XIIC_STOP);
	if(sentCount != 1){
		return XST_FAILURE;
	}

	// read data
	status = I2C_Get_Data(INA3221_SLAVE_ADDRESS, value_OUT, 2, XIIC_STOP);
	if(status != NO_ERROR){
		return status;
	}

	return status;
}

void INA3221_Reset(void){
	INA3221_ConfigRegStruct ina3221_Config;

	ina3221_Config.Status.rst = 1;

	INA3221_SetConfigReg(&ina3221_Config);
}

int INA3221_ReadConfigData(void){
	int status = NO_ERROR;
	int sentCount = 0; // the number of bytes sent
	uint8_t data[3] = {0};

	data[0] = CONFIG_REG_ADDR;
	// send register value to read from
	sentCount = I2C_Send_Data(INA3221_SLAVE_ADDRESS, data, 1, XIIC_STOP);
	if(sentCount != 1){
		return XST_FAILURE;
	}

	// read data
	status = I2C_Get_Data(INA3221_SLAVE_ADDRESS, data, 2, XIIC_STOP);
	if(status != NO_ERROR){
		return status;
	}

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

int GetINA3221Bus(char *message, char *str){
	int errorCode;
	char *ptr = (char*)message;
	int i = 0;
	char str2[8];
	ShuntBusVoltage_struct shuntBusVoltage;

	i = GetCharToByteValue(*ptr);

	if(i < 1 || i > 3){
		return VALUE_OUT_OF_RANGE; // NACK
	}

	shuntBusVoltage.channel = i;
	shuntBusVoltage.shuntOrBus = BUS_SEL;

	// read data
	errorCode = INA3221_ReadVoltage(&shuntBusVoltage, &ina3221_Voltage.voltage);
	if(errorCode != XST_SUCCESS){
		return I2C_ERROR_READ;
	}

	// remember A/D voltage value is left shifted by 3
	ina3221_Voltage.channel = i;
	//ina3221_Voltage.voltage = ((ina3221_Voltage.voltage >> 3) & 0x1FFF); // * 0.001;

	strcpy(str, "0x");
	itoa(ina3221_Voltage.voltage, str2, BASE_16);
	strcat(str, str2);
	strcat(str, "\r\n");

	return NO_ERROR;
}

/*
 * Description: Gets Shunt voltage on a channel
 * Input: pointer to message received
 *
 */
int GetINA3221Shunt(char *message, char *str){
	int errorCode;
	char *ptr = (char*)message;
	int i = 0;
	char str2[8];
	ShuntBusVoltage_struct shuntBusVoltage;

	i = GetCharToByteValue(*ptr);

	if(i < 1 || i > 3){
		return VALUE_OUT_OF_RANGE; // NACK
	}

	shuntBusVoltage.channel = i;
	shuntBusVoltage.shuntOrBus = SHUNT_SEL;

	errorCode = INA3221_ReadVoltage(&shuntBusVoltage, &ina3221_Voltage.voltage);
	if(errorCode != XST_SUCCESS){
		return I2C_ERROR_READ;
	}

	// save voltage value
	ina3221_Voltage.channel = i;
	//ina3221_Voltage.voltage = ((ina3221_Voltage.voltage >> 3) & 0x1FFF);// * 0.001;

	strcpy(str, "0x");
	itoa(ina3221_Voltage.voltage, str2, BASE_16);
	strcat(str, str2);
	strcat(str, "\r\n");

	return NO_ERROR;
}

/*
 * Description: Converts voltage value to hex number with sign bit if negative and sends to IC.
 *
 * Input: ShuntBusVoltage struct
 * Output:
 *
 *
 * notes: 	Full-scale range = 163.8 mV (decimal = 7FF8); LSB (SD0): 40 microV
  			Example: For a value of VSHUNT = -80 mV:
  			1. Take the absolute value: 80 mV
			2. Translate this number to a whole decimal number (80 mV / 40 microV) = 2000
			3. Convert this number to binary = 011 1110 1000 0_ _ _ (last three bits are set to 0)
			4. Complement the binary result = 100 0001 0111 1111
			5. Add 1 to the complement to create the twos complement result = 100 0001 1000 0000
			6. Extend the sign and create the 16-bit word: 1100 0001 1000 0000 = C180h
 *
 *
 */
/*
int INA3221_ChannelShuntBusVoltage(ShuntBusVoltage *shuntBusVoltage){
	int errorCode = 0;
	uint16_t wNumber; // whole number
	float lsbValue;
	uint8_t data[4];

	// make sure values are within range
	if(shuntBusVoltage->shuntOrBus == SHUNT_SEL){// shunt
		if(fabs(shuntBusVoltage->voltage > SHUNT_VOLTAGE_FULL) || fabs(shuntBusVoltage->voltage) < SHUNT_VOLTAGE_LSB){
			// out of range
			return 1;
		}
		lsbValue = SHUNT_VOLTAGE_LSB;
	}
	else if(shuntBusVoltage->shuntOrBus == BUS_SEL)// bus
	{
		if(fabs(shuntBusVoltage->voltage) > BUS_VOLTAGE_FULL || fabs(shuntBusVoltage->voltage) < BUS_VOLTAGE_LSB){
			// out of range
			return 1;
		}
		lsbValue = BUS_VOLTAGE_LSB;
	}
	else
	{
		// out of range
		return 1;
	}

	wNumber = (uint16_t) ((fabs(shuntBusVoltage->voltage) / lsbValue) + 1); // translate to whole decimal

	wNumber = wNumber << 3; // shift left 3, last 3 bits are cleared to 0

	OnesCompliment(&wNumber);

	wNumber &= 0x7FFF; // clear sign bit if it was set.

	wNumber += 1; // two's compliment

	if(shuntBusVoltage->voltage < 0){ // add 1 to msb to indicate negative number
		wNumber |= 0x8000;
	}

	// set data[0] value depending which channel and if Shunt or Bus Voltage
	if(shuntBusVoltage->channel == 1 && shuntBusVoltage->shuntOrBus == SHUNT_SEL){
		data[0] = CHAN1_SHUNT_VOLTAGE_ADDR;
	}
	else if(shuntBusVoltage->channel == 1 && shuntBusVoltage->shuntOrBus == BUS_SEL){
		data[0] = CHAN1_BUS_VOLTAGE_ADDR;
	}
	else if(shuntBusVoltage->channel == 2 && shuntBusVoltage->shuntOrBus == SHUNT_SEL){
		data[0] = CHAN2_SHUNT_VOLTAGE_ADDR;
	}
	else if(shuntBusVoltage->channel == 2 && shuntBusVoltage->shuntOrBus == BUS_SEL){
		data[0] = CHAN2_BUS_VOLTAGE_ADDR;
	}
	else if(shuntBusVoltage->channel == 3 && shuntBusVoltage->shuntOrBus == SHUNT_SEL){
		data[0] = CHAN3_SHUNT_VOLTAGE_ADDR;
	}
	else if(shuntBusVoltage->channel == 3 && shuntBusVoltage->shuntOrBus == BUS_SEL){
		data[0] = CHAN3_BUS_VOLTAGE_ADDR;
	}

	data[1] = wNumber >> 8;
	data[2] = wNumber & 0x0f;
	errorCode = I2C_Send_Data(0, INA3221_SLAVE_ADDRESS, data, 3, XIIC_STOP);

	return errorCode;
}
*/

/*
 * Description: 1's compliment value
 *
 *
 */

/*
static void OnesCompliment(uint16_t *value){
	uint8_t i;
	uint16_t tempValue;
	uint8_t bit;

	for(i = 0; i < 16; i++)
	{
		bit = ((*value >> i) & 0x1) ^ 1;
		tempValue |= bit << i;
	}
	*value = tempValue;
}
*/

