/*
 * Flash_Data_Handler.c
 *
 *  Created on: Oct 14, 2024
 *      Author: karl.yamashita
 *
 *
 *      Description: This driver is user configurable.
 *      			You'll need to edit the data structure when more data is needed to be saved.
 *
 */

#include "main.h"


// todo -combine serial and part as 1 function
int FLASH_DATA_SetSerial(char *msg)
{
	int status = 0;

	FlashWriteInit_t flashMsg = {0};
	FLASH_DataStruct_t flashData = {0};
	int i;

	const char* result = strchr(msg, '\r');
	uint32_t copyCount = result - msg;
	if(copyCount > FLASH_STRING_DATA_SIZE - 1)
	{
		copyCount = FLASH_STRING_DATA_SIZE - 1;
	}

	for(i = 0; i < FLASH_SIZEOF_DATA_STRUCT; i++) // read SerialPartNumber_t structure
	{
		flashData.Bytes.data[i] = FLASH_DATA_Read(FLASH_EEPROM_START_ADDRESS + i);
	}

	memset(&flashData.Status.serial, 0, FLASH_STRING_DATA_SIZE); // erase serial only
	snprintf((char*)flashData.Status.serial.data, copyCount + 1, "%s", msg); // copy only 128 bytes to serial data

	flashMsg.flashEraseInit.Banks = FLASH_BANK_1;
	flashMsg.flashEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	flashMsg.flashEraseInit.Page = (FLASH_EEPROM_START_ADDRESS/FLASH_PAGE_SIZE);
	flashMsg.flashEraseInit.NbPages = 1;

	flashMsg.address = FLASH_EEPROM_START_ADDRESS;
	flashMsg.data = flashData.Bytes.data;
	flashMsg.doubleWordAmount = FLASH_WRITE_DOUBLE_WORD_SIZE;

	status = FLASH_WriteDoubleWord(&flashMsg);

	return status;
}

int FLASH_DATA_SetPart(char *msg)
{
	int status = 0;
	FlashWriteInit_t flashMsg = {0};
	FLASH_DataStruct_t flashData = {0};
	int i;

	const char* result = strchr(msg, '\r');
	uint32_t copyCount = result - msg;
	if(copyCount > FLASH_STRING_DATA_SIZE - 1)
	{
		copyCount = FLASH_STRING_DATA_SIZE - 1;
	}

	for(i = 0; i < FLASH_SIZEOF_DATA_STRUCT; i++) // read SerialPartNumber_t structure
	{
		flashData.Bytes.data[i] = FLASH_DATA_Read(FLASH_EEPROM_START_ADDRESS + i);
	}

	memset(&flashData.Status.part, 0, FLASH_STRING_DATA_SIZE); // erase serial only
	snprintf((char*)flashData.Status.part.data, copyCount + 1, "%s", msg); // copy only 128 bytes to serial data

	flashMsg.flashEraseInit.Banks = FLASH_BANK_1;
	flashMsg.flashEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	flashMsg.flashEraseInit.Page = (FLASH_EEPROM_START_ADDRESS/FLASH_PAGE_SIZE);
	flashMsg.flashEraseInit.NbPages = 1;

	flashMsg.address = FLASH_EEPROM_START_ADDRESS;
	flashMsg.data = flashData.Bytes.data;
	flashMsg.doubleWordAmount = FLASH_WRITE_DOUBLE_WORD_SIZE;

	status = FLASH_WriteDoubleWord(&flashMsg);

	return status;
}

int FLASH_DATA_SetSlaveRail(char *msg)
{
	int status = 0;
	FlashWriteInit_t flashMsg = {0};
	FLASH_DataStruct_t flashData = {0};
	int i;

	const char* result = strchr(msg, '\r');
	uint32_t copyCount = result - msg;
	if(copyCount > FLASH_STRING_DATA_SIZE - 1)
	{
		copyCount = FLASH_STRING_DATA_SIZE - 1;
	}

	for(i = 0; i < FLASH_SIZEOF_DATA_STRUCT; i++) // read SerialPartNumber_t structure
	{
		flashData.Bytes.data[i] = FLASH_DATA_Read(FLASH_EEPROM_START_ADDRESS + i);
	}

	memset(&flashData.Status.slaveRail, 0, FLASH_STRING_DATA_SIZE); // erase serial only
	snprintf((char*)flashData.Status.slaveRail.data, copyCount + 1, "%s", msg); // copy only 128 bytes to serial data

	flashMsg.flashEraseInit.Banks = FLASH_BANK_1;
	flashMsg.flashEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	flashMsg.flashEraseInit.Page = (FLASH_EEPROM_START_ADDRESS/FLASH_PAGE_SIZE);
	flashMsg.flashEraseInit.NbPages = 1;

	flashMsg.address = FLASH_EEPROM_START_ADDRESS;
	flashMsg.data = flashData.Bytes.data;
	flashMsg.doubleWordAmount = FLASH_WRITE_DOUBLE_WORD_SIZE;

	status = FLASH_WriteDoubleWord(&flashMsg);

	LTM46xx_Init(); // update global variables

	return status;
}

/*
 * Description:
 *
 */
int FLASH_DATA_SetInfo(uint8_t ptr, char *msg)
{
	int status = 0;
	FlashWriteInit_t flashMsg = {0};
	FLASH_DataStruct_t flashData = {0};
	int i;

	const char* result = strchr(msg, '\r');
	uint32_t copyCount = result - msg;
	if(copyCount > FLASH_STRING_DATA_SIZE - 1)
	{
		copyCount = FLASH_STRING_DATA_SIZE - 1;
	}

	for(i = 0; i < FLASH_SIZEOF_DATA_STRUCT; i++) // read SerialPartNumber_t structure
	{
		flashData.Bytes.data[i] = FLASH_DATA_Read(FLASH_EEPROM_START_ADDRESS + i);
	}

	switch(ptr)
	{
	case 1:
		memset(&flashData.Status.info1, 0, FLASH_STRING_DATA_SIZE); // erase infox only
		snprintf((char*)flashData.Status.info1.data, copyCount + 1, "%s", msg); // copy only 128 bytes to infox data
		break;
	default:
		return VALUE_OUT_OF_RANGE;
		break;
	}

	flashMsg.flashEraseInit.Banks = FLASH_BANK_1;
	flashMsg.flashEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	flashMsg.flashEraseInit.Page = (FLASH_EEPROM_START_ADDRESS/FLASH_PAGE_SIZE);
	flashMsg.flashEraseInit.NbPages = 1;

	flashMsg.address = FLASH_EEPROM_START_ADDRESS;
	flashMsg.data = flashData.Bytes.data;
	flashMsg.doubleWordAmount = FLASH_WRITE_DOUBLE_WORD_SIZE;

	status = FLASH_WriteDoubleWord(&flashMsg);

	return status;
}

int FLASH_DATA_Set_Interpolation(Interpolation_t *interpol, char *msg)
{
	int status = 0;
	FlashWriteInit_t flashMsg = {0};
	FLASH_DataStruct_t flashData = {0};
	int i;

	const char* result = strchr(msg, '\r');
	uint32_t copyCount = result - msg;
	if(copyCount > FLASH_STRING_DATA_SIZE - 1)
	{
		copyCount = FLASH_STRING_DATA_SIZE - 1;
	}

	for(i = 0; i < FLASH_SIZEOF_DATA_STRUCT; i++) // read SerialPartNumber_t structure
	{
		flashData.Bytes.data[i] = FLASH_DATA_Read(FLASH_EEPROM_START_ADDRESS + i);
	}

	switch(interpol->type)
	{
	case INTERPOLATION_VOUT_COMMAND:
		memset(&flashData.Status.voutCommand, 0, FLASH_STRING_DATA_SIZE); // erase serial only
		snprintf((char*)flashData.Status.voutCommand.data, copyCount + 1, "%s", msg); // copy only 128 bytes to serial data
		break;
	case INTERPOLATION_LTM4681_SNS:
		memset(&flashData.Status.adcCalibration_1, 0, FLASH_STRING_DATA_SIZE); // erase serial only
		snprintf((char*)flashData.Status.adcCalibration_1.data, copyCount + 1, "%s", msg); // copy only 128 bytes to serial data
		break;
	case INTERPOLATION_OP_SMS:
		memset(&flashData.Status.adcCalibration_2, 0, FLASH_STRING_DATA_SIZE); // erase serial only
		snprintf((char*)flashData.Status.adcCalibration_2.data, copyCount + 1, "%s", msg); // copy only 128 bytes to serial data
		break;
	case INTERPOLATION_LTM_VOUT:
		memset(&flashData.Status.adcCalibration_2, 0, FLASH_STRING_DATA_SIZE); // erase serial only
		snprintf((char*)flashData.Status.adcCalibration_2.data, copyCount + 1, "%s", msg); // copy only 128 bytes to serial data
		break;
	}

	flashMsg.flashEraseInit.Banks = FLASH_BANK_1;
	flashMsg.flashEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	flashMsg.flashEraseInit.Page = (FLASH_EEPROM_START_ADDRESS/FLASH_PAGE_SIZE);
	flashMsg.flashEraseInit.NbPages = 1;

	flashMsg.address = FLASH_EEPROM_START_ADDRESS;
	flashMsg.data = flashData.Bytes.data;
	flashMsg.doubleWordAmount = FLASH_WRITE_DOUBLE_WORD_SIZE;

	status = FLASH_WriteDoubleWord(&flashMsg);

	LTM46xx_Init(); // update global variables

	return status;
}


/*
 * Description: Get selected string from flash memory
 */
int FLASH_DATA_Get_String(uint8_t type, char *retStr)
{
	int status = NO_ERROR;
	uint32_t address = FLASH_EEPROM_START_ADDRESS;
	FLASH_DataStruct_t flashData = {0};
	int i = 0;

	for(i = 0; i < FLASH_SIZEOF_DATA_STRUCT; i++)
	{
		flashData.Bytes.data[i] = FLASH_DATA_Read(address + i);
	}

	switch(type) // enum FLASH_Type
	{
	case FLASH_TYPE_VOUTCOMMAND:
		if(!isalpha(flashData.Status.voutCommand.data[0]) && !isdigit(flashData.Status.voutCommand.data[0]))
		{
			return NO_INTERPOLATION;
		}
		sprintf(retStr, "%s", (char*)flashData.Status.voutCommand.data);
		break;
	case FLASH_TYPE_ADC_CAL_1:
		if(!isalpha(flashData.Status.adcCalibration_1.data[0]) && !isdigit(flashData.Status.adcCalibration_1.data[0]))
		{
			return NO_INTERPOLATION;
		}
		sprintf(retStr, "%s", (char*)flashData.Status.adcCalibration_1.data);
		break;
	case FLASH_TYPE_ADC_CAL_2:
		if(!isalpha(flashData.Status.adcCalibration_2.data[0]) && !isdigit(flashData.Status.adcCalibration_2.data[0]))
		{
			return NO_INTERPOLATION;
		}
		sprintf(retStr, "%s", (char*)flashData.Status.adcCalibration_2.data);
		break;
	case FLASH_TYPE_ADC_CAL_3:
		if(!isalpha(flashData.Status.adcCalibration_3.data[0]) && !isdigit(flashData.Status.adcCalibration_3.data[0]))
		{
			return NO_INTERPOLATION;
		}
		sprintf(retStr, "%s", (char*)flashData.Status.adcCalibration_3.data);
		break;
	case FLASH_TYPE_SERIAL:
		if(!isalpha(flashData.Status.serial.data[0]) && !isdigit(flashData.Status.serial.data[0]))
		{
			return NO_SERIAL_PART;
		}
		sprintf(retStr, "%s", (char*)flashData.Status.serial.data);
		break;
	case FLASH_TYPE_PART:
		if(!isalpha(flashData.Status.part.data[0]) && !isdigit(flashData.Status.part.data[0]))
		{
			return NO_SERIAL_PART;
		}
		sprintf(retStr, "%s", (char*)flashData.Status.part.data);
		break;
	case FLASH_TYPE_SLAVERAIL:
		if(!isalpha(flashData.Status.slaveRail.data[0]) && !isdigit(flashData.Status.slaveRail.data[0]))
		{
			return LTM46xx_SLAVE_ADDRESS_NOT_SET;
		}
		sprintf(retStr, "%s", (char*)flashData.Status.slaveRail.data);
		break;
	case FLASH_TYPE_INFO_1:
		if(!isalpha(flashData.Status.info1.data[0]) && !isdigit(flashData.Status.info1.data[0]))
		{
			return NO_INFO_STRING;
		}
		sprintf(retStr, "%s", (char*)flashData.Status.info1.data);
		break;
	default:
		// should not reach here
		break;
	}

	return status;
}


