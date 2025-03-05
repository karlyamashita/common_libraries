/*
 * Flash_Data_Handler.h
 *
 *  Created on: Oct 14, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_SERIALPARTPROGRAMMING_H_
#define INC_SERIALPARTPROGRAMMING_H_


#define FLASH_DATA_STRUCT_AMOUNT 8 // The amount of FLASH_Data_t used in FLASH_SerialPartNumber_t


enum {
	FLASH_TYPE_SERIAL,
	FLASH_TYPE_PART,
	FLASH_TYPE_SLAVERAIL,
	FLASH_TYPE_VOUTCOMMAND,
	FLASH_TYPE_ADC_CAL_1,
	FLASH_TYPE_ADC_CAL_2,
	FLASH_TYPE_ADC_CAL_3,
	FLASH_TYPE_INFO_1,
};

// data structure for saving serial/part number.
typedef struct
{
	uint8_t data[FLASH_STRING_DATA_SIZE];
}FLASH_Data_t;

typedef union
{
	struct
	{
		uint8_t data[FLASH_WRITE_DOUBLE_WORD_SIZE];
	}Bytes;
	struct
	{
		FLASH_Data_t serial;
		FLASH_Data_t part;
		FLASH_Data_t slaveRail;
		FLASH_Data_t voutCommand;
		FLASH_Data_t adcCalibration_1; // LTM4681_SNS
		FLASH_Data_t adcCalibration_2; // OP_CRNT_SNS
		FLASH_Data_t adcCalibration_3; // LTM Vout
		FLASH_Data_t info1;// be sure to update #define FLASH_DATA_STRUCT_AMOUNT for every FLASH_Data_t added to this structure
	}Status;
}FLASH_DataStruct_t;


int FLASH_DATA_SetSerial(char *msg);
int FLASH_DATA_SetPart(char *msg);
int FLASH_DATA_GetSerial(char *retStr);
int FLASH_DATA_GetPart(char *retStr);
int FLASH_DATA_SetSlaveRail(char *msg);
int FLASH_DATA_GetSlaveRail(char *retStr);
int FLASH_DATA_SetInfo(uint8_t ptr, char *retStr);
int FLASH_DATA_GetInfo(uint8_t ptr, char *retStr);

int FLASH_DATA_Set_Interpolation(Interpolation_t *interpol, char *msg);
int FLASH_DATA_Get_String(uint8_t type, char *retStr);

#endif /* INC_SERIALPARTPROGRAMMING_H_ */
