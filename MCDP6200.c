/*
 * RegistersWrite.c
 *
 * Created: 3/1/2021 12:37:07 PM
 *  Author: karl.yamashita
 *
 *
 *  #include "MCDP6200_I2C_ReadWrite.h"
 *
 */ 

#include "main.h"
#include "MCDP6200.h"

//#define MCDP6200_PrintLine


/*
 *
 *
 *
 */
int MCDP6200_Init(void)
{

	return NO_ERROR;
}



/*
 * Description: Get the register data
 *
 *
 */
int GetMCDP6200Register(char *message, char *str){
	int status = NO_ERROR;
	char registerValue[4] = {0};
	uint8_t byteArray[4] = {0};
	int i = 0;
	char *ptr = (char*)message;
	char str2[16];
	uint32_t regAddr = 0;
	uint32_t regData = 0;

	// check to see if register address starts with 0x
	if(strncmp(ptr, "0x", strlen("0x")) != 0){
		return COMMAND_UNKNOWN;
	}
	ptr += strlen("0x"); // remove 0x

	// parse register address
	while(*ptr != '\r'){
		registerValue[i++] = *ptr++; // we should only be reading 2 characters max
	}

	status = Hex2Int(registerValue, &regAddr);
	if(status != 0){
		return COMMAND_UNKNOWN;
	}

	// parse register data
	status = MCDP6200_Read_Register_Data(MCDP6200_SLAVE_ID_14, (uint16_t)regAddr, (uint8_t*)&byteArray);
	if(status != 0){
		return I2C_ERROR;
	}

	// convert byte array to 32bit value
	regData = byteArray[0];
	regData |= byteArray[1] << 8;
	regData |= byteArray[2] << 16;
	regData |= byteArray[3] << 24;

	// TODO - do not use Standard C library.
	//sprintf(str, "0x%X8\r\n", regData);
	strcpy (str, "0x");

	itoa(regData, str2, BASE_16);

	strcat(str, str2);
	strcat(str, "\r\n");

	status = NO_ERROR;

	return status; // no ack/nack
}

/*
 * Description:
 *
 *
 */
int SetMCDP6200Register(char *message){
	int status;
	char registerAddr[4];
	char dataValue[8];
	char *ptr = (char*)message;
	uint8_t byteArray[4];
	int i = 0;
	uint32_t regAddr;
	uint32_t regData;

	// check to see if register address starts with 0x
	if(strncmp(ptr, "0x", strlen("0x")) != 0){
		return COMMAND_UNKNOWN;
	}
	ptr += strlen("0x");

	// parse register address
	while(*ptr != ','){
		registerAddr[i++] = *ptr++; // we should only be reading 4 characters max
	}
	status = Hex2Int(registerAddr, &regAddr);
	if(status != NO_ERROR){
		return COMMAND_UNKNOWN;
	}

	// check to see if comma with register data starts with 0x
	if(strncmp(ptr, ",0x", strlen(",0x")) == 0){
		ptr += strlen(",0x");
	}
	else
	{
		return COMMAND_UNKNOWN;
	}

	// parse register data
	i = 0;
	while(*ptr != '\r' && *ptr != '\0'){
		dataValue[i++] = *ptr++; // we should only be reading 8 characters max
	}
	regData = (uint32_t)strtoul((char*)dataValue, NULL, 16);

	// convert register data to byte array
	byteArray[0] = regData;
	byteArray[1] = regData >> 8;
	byteArray[2] = regData >> 16;
	byteArray[3] = regData >> 24;

	// write data to register
	status = MCDP6200_Write_Register(MCDP6200_SLAVE_ID_14, (uint16_t)regAddr, (uint8_t*)&byteArray);

	return status; // send ACK
}

/*
 *
 *
 *
 */
void GPIO_Pin_RTMR_DIS_N_StartPulse(void){
	GPIO_Pin_RTMR_DIS_N(1);
	TimerCallbackRegister(GPIO_Pin_RTMR_DIS_N_Disable,1000, NO_REPEAT_TIMER);
	TimerCallbackRegister(GPIO_Pin_RTMR_DIS_N_Enable,2000, NO_REPEAT_TIMER);
}

/*
 *
 *
 *
 */
void GPIO_Pin_RTMR_DIS_N_Enable(void){
	GPIO_Pin_RTMR_DIS_N(1);
}

/*
 *
 *
 *
 */
void GPIO_Pin_RTMR_DIS_N_Disable(void){
	GPIO_Pin_RTMR_DIS_N(0);
}


