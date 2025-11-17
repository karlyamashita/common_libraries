/*
 * XDPE192C3D_Handler.c
 *
 *  Created on: Sep 25, 2025
 *      Author: karl.yamashita
 */


#include "main.h"


XDPE192C3D_Configuration_t xdp_config =
{
	.railCount = 2,
	.read_vout_iout_enable = false,
	.exponent = 0 // init to zero
};

volatile XDPE192C3D_I2C_DataRdy_t xdp_i2c_dataRdy = {0};


/*
 * Description: Initialize rail count from flash memory
 */
int XDPE192C3D_Configuration_Init(XDPE192C3D_Configuration_t *config)
{
	int status = NO_ERROR;
	uint32_t railCount = 0;

	status = FLASH_MEMORY_ReadData(&flash_memory_data);
	if(status != NO_ERROR)
	{
		return status;
	}

	if(flash_memory_data.Status.railCount.data[0] == 0xFF)
	{
		return NO_STRING_INFO;
	}

	if(strlen((char*)flash_memory_data.Status.railCount.data) != 0)
	{
		railCount = atoi((char*)flash_memory_data.Status.railCount.data);
		config->railCount = railCount;
	}
	else
	{
		return NO_STRING_INFO;
	}

	return status;
}

uint8_t pData[8] = {0};

/*
 * Description: Parse Command UART messages
 */
int XDPE192C3D_Handler(char *msg, char *retStr)
{
	int status = NO_ERROR;
	char *result;
	char getChar[] = "?"; // the char that indicates a get command

	RemoveSpaces(msg);

	// check if get command (?)
	result = strstr(msg, getChar);
	if(result)
	{
		status = XDPE192C3D_RegisterGet(msg, retStr);

		return status;
	}
	// no ?, so is a set command
	status = XDPE192C3D_RegisterSet(msg); //set

	return status;
}

/*
 * Description: Read register
 */
int XDPE192C3D_RegisterGet(char *msg, char *retStr)
{
	int status = NO_ERROR;
	char *token; // command
	char *token2; // page
	char *rest = msg;
	char delim[] = "?:,\r";
	float value = 0;
	float valueConverted = 0;
	uint16_t regValue = 0;
	uint32_t page;
	//int exponent = 0;
	uint8_t cmdPtr = 0;
	uint32_t lastI2C_Transfer_type = 0;

	token = strtok_r(rest, delim, &rest); // command
	token2 = strtok_r(rest, delim, &rest); // page

	// find index of command in table lookup
	status = XDPE192C3D_GetCommandPtr(token, &cmdPtr);
	if(status != NO_ERROR)
	{
		return status;
	}

	// check if write only
	if(XDP_StrReg[cmdPtr].rwType == WRITE_ONLY)
	{
		return WRITE_ONLY_ERROR;
	}

	// extract the page
	IsHex(token2, &page);

	// validate page
	status = XDPE192C3D_Validate_Page(&xdp_config, page);
	if(status != NO_ERROR)
	{
		return XDP_PAGE_SELECTION_ERROR;
	}

	// set page 0
	status = XDPE192C3D_SetPage(&xdp_i2c, (uint8_t)page);
	if(status != NO_ERROR)
	{
		return status;
	}

	// read register
	xdp_i2c.registerAddr[0] = XDP_StrReg[cmdPtr].reg;
	xdp_i2c.dataSize = XDP_StrReg[cmdPtr].datalen;
	lastI2C_Transfer_type = xdp_i2c.transferType;
	xdp_i2c.transferType = I2C_POLLING;
	status = XDPE192C3D_Mem_Read(&xdp_i2c);
	xdp_i2c.transferType = lastI2C_Transfer_type;
	if(status != NO_ERROR)
	{
		return status;
	}

	// move data to regValue before getting VOUT_MODE exponent.
	if(XDP_StrReg[cmdPtr].datalen == 1)
	{
		regValue = xdp_i2c.dataPtr[0];
	}
	else if(XDP_StrReg[cmdPtr].datalen == 2 || XDP_StrReg[cmdPtr].datalen == 4)
	{
		regValue = xdp_i2c.dataPtr[0];
		regValue |= (xdp_i2c.dataPtr[1] << 8);
	}

	// Convert data to string. A handful of registers returned are very similar, but we'll keep each format type
	// separate in case we need to return a custom string format.
	if(XDP_StrReg[cmdPtr].format == REG_FORMAT)
	{
		sprintf(retStr, "0x%02X", regValue);
	}
	else if(XDP_StrReg[cmdPtr].format == L16_FORMAT)
	{
		// get VOUT_MODE exponent if still init to zero. We should only need to do this once after POR.
		if(xdp_config.exponent == 0)
		{
			status = XDPE192C3D_GetMode(&xdp_i2c, &xdp_config.exponent);
			if(status != NO_ERROR)
			{
				return status;
			}
		}

		value = L16_to_Float(xdp_config.exponent, regValue);
		if(verbose.enable_1)
		{
			sprintf(retStr, "0x%X, %d", regValue, (uint16_t)(value * 1000));
		}
		else
		{
			sprintf(retStr, "%d", (uint16_t)(value * 1000));
		}
	}
	else if(XDP_StrReg[cmdPtr].format == L5_11_FORMAT)
	{
		value = L5_11_to_Float(regValue);
		if(verbose.enable_1)
		{
			sprintf(retStr, "0x%X, %d", regValue, (uint16_t)(value * 1000));
		}
		else
		{
			sprintf(retStr, "%d", (uint16_t)(value * 1000));
		}
	}
	else if(XDP_StrReg[cmdPtr].format == MS_FORMAT)
	{
		value = L5_11_to_Float(regValue);
		sprintf(retStr, "%0.3f", (value));
	}
	else if(XDP_StrReg[cmdPtr].format == TEMPERATURE_FORMAT)
	{
		value = L5_11_to_Float(regValue);
		if(verbose.enable_1)
		{
			Celsius_Fahrenheit(&value, &valueConverted);
			sprintf(retStr, "%0.1fF, %0.1fC", valueConverted, (value));
		}
		else
		{
			sprintf(retStr, "%0.1f", (value));
		}
	}
	else if(XDP_StrReg[cmdPtr].format == AMP_PORMAT)
	{
		value = L5_11_to_Float(regValue);
		if(verbose.enable_1)
		{
			sprintf(retStr, "0x%x, %0.2f", regValue, value);
		}
		else
		{
			sprintf(retStr, "%0.2f", value);
		}
	}
	else if(XDP_StrReg[cmdPtr].format == POWER_FORMAT)
	{
		value = L5_11_to_Float(regValue);
		sprintf(retStr, "%0.2f", value);
	}
	else if(XDP_StrReg[cmdPtr].format == DUTY_CYCLE_FORMAT)
	{
		value = L5_11_to_Float(regValue);
		sprintf(retStr, "%0.2f", value);
	}
	else if(XDP_StrReg[cmdPtr].format == FREQUENCY_FORMAT)
	{
		value = L5_11_to_Float(regValue);
		sprintf(retStr, "%0.0f", value);
	}
	else if(XDP_StrReg[cmdPtr].format == OHM_FORMAT)
	{
		value = L5_11_to_Float(regValue);
		sprintf(retStr, "%0.4f", value);
	}
	else
	{
		return NO_FORMAT_ERROR;
	}

	return status;
}

/*
 * Description: Write register
 */
int XDPE192C3D_RegisterSet(char *msg)
{
	int status = NO_ERROR;
	char *token; // command
	char *token2; // page
	char *token3; // for command reg, this is the register address
	char *rest = msg;
	char delim[] = "?:,\r";
	float floatValue = 0;
	uint32_t regValue[1] = {0};
	uint32_t page; // rail
	//int exponent = 0;
	uint8_t cmdPtr = 0;

	token = strtok_r(rest, delim, &rest); // command
	token2 = strtok_r(rest, delim, &rest); // page
	token3 = strtok_r(rest, delim, &rest); // floatValue

	// find index of command in table lookup
	status = XDPE192C3D_GetCommandPtr(token, &cmdPtr);
	if(status != NO_ERROR)
	{
		return status;
	}

	// check if read only
	if(XDP_StrReg[cmdPtr].rwType == READ_ONLY)
	{
		return READ_ONLY_ERROR;
	}

	// extract the page
	IsHex(token2, &page);

	// validate page
	status = XDPE192C3D_Validate_Page(&xdp_config, page);
	if(status != NO_ERROR)
	{
		return XDP_PAGE_SELECTION_ERROR;
	}

	// check if no data to send, just register
	if(XDP_StrReg[cmdPtr].datalen != LEN_0)
	{
		// set page 0
		status = XDPE192C3D_SetPage(&xdp_i2c, (uint8_t)page);
		if(status != NO_ERROR)
		{
			return status;
		}

		// convert floatValue
		switch(XDP_StrReg[cmdPtr].format)
		{
		case REG_FORMAT:
			if(token3 != NULL)
			{
				IsHex(token3, regValue);
			}
			xdp_i2c.dataSize = LEN_2; // register + 1 byte
			break;
		case L16_FORMAT:
			// get VOUT_MODE exponent if still init to zero. We should only need to do this once after POR.
			if(xdp_config.exponent == 0)
			{
				status = XDPE192C3D_GetMode(&xdp_i2c, &xdp_config.exponent);
				if(status != NO_ERROR)
				{
					return status;
				}
			}
			floatValue = (atoi(token3) * 0.001); // convert mV to V
			regValue[0] = Float_to_L16(xdp_config.exponent, floatValue);
			xdp_i2c.dataSize = XDP_StrReg[cmdPtr].datalen; // register + 2 byte
			break;
		case L5_11_FORMAT:
			floatValue = (atoi(token3) * 0.001); // convert mV to V
			regValue[0] = Float_to_L11(floatValue);
			xdp_i2c.dataSize = XDP_StrReg[cmdPtr].datalen; // register + 2 byte
			break;
		default:
			floatValue = atof(token3);
			regValue[0] = Float_to_L11(floatValue);
			xdp_i2c.dataSize = XDP_StrReg[cmdPtr].datalen; // register + 2 byte
			break;
		}

		// set register value
		xdp_i2c.registerAddr[0] = XDP_StrReg[cmdPtr].reg;

		// get data ready
		xdp_i2c.dataPtr[0] = (uint8_t)regValue[0];
		xdp_i2c.dataPtr[1] = (uint8_t)(regValue[0] >> 8);
		// write to device
		status = XDPE192C3D_Mem_Write(&xdp_i2c);
	}
	else
	{
		xdp_i2c.dataPtr[0] = XDP_StrReg[cmdPtr].reg;
		xdp_i2c.dataSize = LEN_1;
		// write register value only
		status = XDPE192C3D_Write(&xdp_i2c);
	}

	return status;
}

/*
 * Description: Find index of command in table.
 */
int XDPE192C3D_GetCommandPtr(char *msg, uint8_t *ptr)
{
	int status = NO_ERROR;
	int i = 0;
	char *result;
	int element_size = sizeof(XDP_StrReg[0]);
	int totalElements = XDP_StrReg_size / element_size; // XDP_StrReg_size defined in XDPE192C3D_Drv.c and extern
	bool found = false;

	for(i = 0 ; i < totalElements; i++)
	{
		result = strstr(XDP_StrReg[i].str, msg);
		if(result)
		{
			found = true;
			break;
		}
	}

	if(!found) return COMMAND_UNKNOWN;

	*ptr = i;

	return status;
}

/*
 * Description: Set the page
 */
int XDPE192C3D_SetPage(I2C_GenericDef *i2c, uint8_t page)
{
	int status = NO_ERROR;
	uint32_t temp = i2c->transferType;

	i2c->registerAddr[0] = PAGE;
	i2c->dataPtr[0] = page; // page
	i2c->dataSize = 1;
	i2c->transferType = I2C_POLLING;

	status = I2C_Mem_Write_Generic_Method(i2c);

	i2c->transferType = temp;

	return status;
}

/*
 * Description: Validate user page selection is valid
 */
int XDPE192C3D_Validate_Page(XDPE192C3D_Configuration_t *config, uint32_t page)
{
	int status = NO_ERROR;

	if(page < 1 && page > 2) // should be 1 or 2
	{
		return XDP_PAGE_SELECTION_ERROR;
	}

	// if rail count is 1, make sure page is not greater.
	if(config->railCount == 1)
	{
		if(page == 2)
		{
			status = XDP_PAGE_SELECTION_ERROR;
		}
	}

	return status;
}

/*
 * Description: Check for flag and get READ_VOUT and READ_IOUT value.
 * 				Convert and save data to telemetry data structure.
 * 				Call from polling routine
 * 				read_vout_iout_enable flag is enabled when Telemetry data is in DMA queue and being sent.
 */
uint16_t count = 0;
void XDPE192C3D_Poll_READ_VOUT_IOUT(void)
{
	if(xdp_config.read_vout_iout_enable)
	{
		xdp_config.read_vout_iout_enable = false;

		xdp_i2c.dataSize = 2;
		xdp_i2c.readWriteType = MEM_READ;

		// vout 1
		xdp_i2c.registerAddr[0] = READ_VOUT_paged;
		xdp_i2c.RxISR = XDPE192C3D_Voltage_1_Callback;
		I2C_Mem_Read_Generic_Method(&xdp_i2c);

		// vout 2
		xdp_i2c.registerAddr[0] = READ_VOUT_paged;
		xdp_i2c.RxISR = XDPE192C3D_Voltage_2_Callback;
		I2C_Mem_Read_Generic_Method(&xdp_i2c);

		// iout 1
		xdp_i2c.registerAddr[0] = READ_IOUT_paged;
		xdp_i2c.RxISR = XDPE192C3D_Current_1_Callback;
		I2C_Mem_Read_Generic_Method(&xdp_i2c);

		// iout 2
		xdp_i2c.registerAddr[0] = READ_IOUT_paged;
		xdp_i2c.RxISR = XDPE192C3D_Current_2_Callback;
		I2C_Mem_Read_Generic_Method(&xdp_i2c);

		// temperature2_1
		xdp_i2c.registerAddr[0] = READ_TEMPERATURE2_paged;
		xdp_i2c.RxISR = XDPE192C3D_Temperature2_Callback_1;
		I2C_Mem_Read_Generic_Method(&xdp_i2c);

		// temperature2_2
		xdp_i2c.registerAddr[0] = READ_TEMPERATURE2_paged;
		xdp_i2c.RxISR = XDPE192C3D_Temperature2_Callback_2;
		I2C_Mem_Read_Generic_Method(&xdp_i2c);
	}
}

void XDPE192C3D_Voltage_1_Callback(I2C_GenericDef *i2c)
{
	uint16_t regValue = 0;
	float voltage = 0;

	regValue = i2c->dataPtr[0];
	regValue |= i2c->dataPtr[1] << 8;

	xdp_i2c_dataRdy.Flags_Voltages.voltage_1 = regValue;
	xdp_i2c_dataRdy.Flags_Voltages.v1 = true;
#ifdef ADD_TO_TELEMETRY
	regValue = xdp_i2c_dataRdy.Flags_Voltages.voltage_1;
	voltage = L16_to_Float(xdp_config.exponent, regValue);
	Telemetry_Update_Voltage(&telemData, voltage, 1);
	xdp_i2c_dataRdy.Flags_Voltages.f_voltage_1 = voltage;
	LED_Debug_Toggle();
#endif
	// update DC_OK flag in telemetry
	DC_OK_Evaluate_VOUT(&dc_ok_1, voltage);
}

void XDPE192C3D_Voltage_2_Callback(I2C_GenericDef *i2c)
{
	uint16_t regValue = 0;
	float voltage = 0;

	regValue = i2c->dataPtr[0];
	regValue |= i2c->dataPtr[1] << 8;

	xdp_i2c_dataRdy.Flags_Voltages.voltage_2 = regValue;
	xdp_i2c_dataRdy.Flags_Voltages.v2 = true;
#ifdef ADD_TO_TELEMETRY
	regValue = xdp_i2c_dataRdy.Flags_Voltages.voltage_2;
	voltage = L16_to_Float(xdp_config.exponent, regValue);
	Telemetry_Update_Voltage(&telemData, voltage, 2);
	xdp_i2c_dataRdy.Flags_Voltages.f_voltage_2 = voltage;
	LED_Debug_Toggle();
#endif
	// update DC_OK flag in telemetry
	DC_OK_Evaluate_VOUT(&dc_ok_2, voltage);
}

void XDPE192C3D_Current_1_Callback(I2C_GenericDef *i2c)
{
	uint16_t regValue = 0;
	float voltage = 0;

	regValue = i2c->dataPtr[0];
	regValue |= i2c->dataPtr[1] << 8;

	xdp_i2c_dataRdy.Flags_Voltages.current_1 = regValue;
	xdp_i2c_dataRdy.Flags_Voltages.i1 = true;
#ifdef ADD_TO_TELEMETRY
	regValue = xdp_i2c_dataRdy.Flags_Voltages.current_1;
	voltage = L5_11_to_Float(regValue);
	Telemetry_Update_Current(&telemData, voltage, 1);
	xdp_i2c_dataRdy.Flags_Voltages.f_current_1 = voltage;
	LED_Debug_Toggle();
#endif
}

void XDPE192C3D_Current_2_Callback(I2C_GenericDef *i2c)
{
	uint16_t regValue = 0;
	float voltage = 0;

	regValue = i2c->dataPtr[0];
	regValue |= i2c->dataPtr[1] << 8;

	xdp_i2c_dataRdy.Flags_Voltages.current_2 = regValue;
	xdp_i2c_dataRdy.Flags_Voltages.i2 = true;
#ifdef ADD_TO_TELEMETRY
	regValue = xdp_i2c_dataRdy.Flags_Voltages.current_2;
	voltage = L5_11_to_Float(regValue);
	Telemetry_Update_Current(&telemData, voltage, 2);
	xdp_i2c_dataRdy.Flags_Voltages.f_current_2 = voltage;
	LED_Debug_Toggle();
#endif
}

void XDPE192C3D_Temperature2_Callback_1(I2C_GenericDef *i2c)
{
	uint16_t regValue = 0;
	float voltage = 0;

	regValue = i2c->dataPtr[0];
	regValue |= i2c->dataPtr[1] << 8;

	xdp_i2c_dataRdy.Flags_Voltages.temperature2_1 = regValue;
	xdp_i2c_dataRdy.Flags_Voltages.t1 = true;
#ifdef ADD_TO_TELEMETRY
	regValue = xdp_i2c_dataRdy.Flags_Voltages.temperature2_1;
	voltage = L5_11_to_Float(regValue);
	Telemetry_Update_Temperature(&telemData, voltage, 1);
	xdp_i2c_dataRdy.Flags_Voltages.f_temperature2_1 = voltage;
	LED_Debug_Toggle();
#endif
}

void XDPE192C3D_Temperature2_Callback_2(I2C_GenericDef *i2c)
{
	uint16_t regValue = 0;
	float voltage  = 0;

	regValue = i2c->dataPtr[0];
	regValue |= i2c->dataPtr[1] << 8;

	xdp_i2c_dataRdy.Flags_Voltages.temperature2_2 = regValue;
	xdp_i2c_dataRdy.Flags_Voltages.t2 = true;
#ifdef ADD_TO_TELEMETRY
	regValue = xdp_i2c_dataRdy.Flags_Voltages.temperature2_2;
	voltage = L5_11_to_Float(regValue);
	Telemetry_Update_Temperature(&telemData, voltage, 2);
	xdp_i2c_dataRdy.Flags_Voltages.f_temperature2_2 = voltage;
	LED_Debug_Toggle();
#endif
}

#ifndef ADD_TO_TELEMETRY
void XDPE192C3D_CheckVoltagesCurrentTemperature(void)
{
	uint16_t regValue = 0;
	float voltage = 0;
	char str[64] = {0};

	if(xdp_i2c_dataRdy.Bytes.flags) // at least 1 bit has been set
	{
		if(xdp_i2c_dataRdy.Flags_Voltages.v1)
		{
			xdp_i2c_dataRdy.Flags_Voltages.v1 = false;

			regValue = xdp_i2c_dataRdy.Flags_Voltages.voltage_1;
			voltage = L16_to_Float(xdp_config.exponent, regValue);
			Telemetry_Update_Voltage(&telemData, voltage, 1);
			xdp_i2c_dataRdy.Flags_Voltages.f_voltage_1 = voltage;
			LED_Debug_Toggle();
		}
		if(xdp_i2c_dataRdy.Flags_Voltages.v2)
		{
			xdp_i2c_dataRdy.Flags_Voltages.v2 = false;

			regValue = xdp_i2c_dataRdy.Flags_Voltages.voltage_2;
			voltage = L16_to_Float(xdp_config.exponent, regValue);
			Telemetry_Update_Voltage(&telemData, voltage, 2);
			xdp_i2c_dataRdy.Flags_Voltages.f_voltage_2 = voltage;
			LED_Debug_Toggle();
		}
		if(xdp_i2c_dataRdy.Flags_Voltages.i1)
		{
			xdp_i2c_dataRdy.Flags_Voltages.i1 = false;

			regValue = xdp_i2c_dataRdy.Flags_Voltages.current_1;
			voltage = L5_11_to_Float(regValue);
			Telemetry_Update_Current(&telemData, voltage, 1);
			xdp_i2c_dataRdy.Flags_Voltages.f_current_1 = voltage;
			LED_Debug_Toggle();
		}
		if(xdp_i2c_dataRdy.Flags_Voltages.i2)
		{
			xdp_i2c_dataRdy.Flags_Voltages.i2 = false;

			regValue = xdp_i2c_dataRdy.Flags_Voltages.current_2;
			voltage = L5_11_to_Float(regValue);
			Telemetry_Update_Current(&telemData, voltage, 2);
			xdp_i2c_dataRdy.Flags_Voltages.f_current_2 = voltage;
			LED_Debug_Toggle();
		}
		if(xdp_i2c_dataRdy.Flags_Voltages.t1)
		{
			xdp_i2c_dataRdy.Flags_Voltages.t1 = false;

			regValue = xdp_i2c_dataRdy.Flags_Voltages.temperature2_1;
			voltage = L5_11_to_Float(regValue);
			Telemetry_Update_Temperature(&telemData, voltage, 1);
			xdp_i2c_dataRdy.Flags_Voltages.f_temperature2_1 = voltage;
			LED_Debug_Toggle();
		}
		if(xdp_i2c_dataRdy.Flags_Voltages.t2)
		{
			xdp_i2c_dataRdy.Flags_Voltages.t2 = false;

			regValue = xdp_i2c_dataRdy.Flags_Voltages.temperature2_2;
			voltage = L5_11_to_Float(regValue);
			Telemetry_Update_Temperature(&telemData, voltage, 2);
			xdp_i2c_dataRdy.Flags_Voltages.f_temperature2_2 = voltage;
			LED_Debug_Toggle();
		}
	}
}
#endif

int XDPE192C3D_Configuration_Handler(char *msg, char *retStr)
{
	int status = NO_ERROR;
	char *result;
	char getChar[] = "?"; // the char that indicates a get command

	RemoveSpaces(msg);

	// check if get command (?)
	result = strstr(msg, getChar);
	if(result)
	{
		status = XDPE192C3D_Configuration_Get(msg, retStr);

		return status;
	}
	// no ?, so is a set command
	status = XDPE192C3D_Configuration_Set(msg); //set

	return status;
}

int XDPE192C3D_Configuration_Get(char *msg, char *retStr)
{
	int status = NO_ERROR;

	sprintf(retStr, "%ld", xdp_config.railCount);

	return status;
}

int XDPE192C3D_Configuration_Set(char *msg)
{
	int status = NO_ERROR;
	char *token;
	char *rest = msg;
	char delim[] = "?:,\r";
	uint32_t railCount = 0;

	// validate string has count:
	if(strncmp(rest, "count:", strlen("count:")) == 0)
	{
		rest += strlen("count:"); // remove string count:
	}

	token = strtok_r(rest, delim, &rest); // rail count

	railCount = atoi(token);
	if(railCount < 1 && railCount > 2)
	{
		return VALUE_OUT_OF_RANGE;
	}

	xdp_config.railCount = railCount;

	status = FLASH_MEMORY_ReadData(&flash_memory_data);
	if(status != NO_ERROR)
	{
		return status;
	}

	memset(&flash_memory_data.Status.railCount.data, 0, FLASH_STRING_DATA_SIZE); // clear
	sprintf((char*)flash_memory_data.Status.railCount.data, "%s", token);
	status = FLASH_MEMORY_WriteData(&flash_memory_data);

	return status;
}

