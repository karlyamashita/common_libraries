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

extern TimerCallbackStruct timerCallbackInstance;


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
int GetMCDP6200Register(char *msg, char *str){
	int status = NO_ERROR;
	char registerValue[8] = {0};
	uint8_t byteArray[4] = {0};
	int i = 0;
	char str2[16];
	uint32_t regAddr = 0;
	uint32_t regData = 0;

	// check to see if register address starts with 0x
	if(strncmp(msg, "0x", strlen("0x")) != 0){
		return COMMAND_UNKNOWN;
	}
	msg += strlen("0x"); // remove 0x

	// parse register address
	while(*msg != '\r'){
		registerValue[i++] = *msg++; // we should only be reading 2 characters max
	}

	status = Hex2Int(registerValue, &regAddr);
	if(status != 0){
		return COMMAND_UNKNOWN;
	}

	// parse register data
	status = MCDP6200_Read_Register_Data(MCDP6200_SLAVE_ID_14, (uint16_t)regAddr, (uint8_t*)&byteArray);
	if(status != 0){
		return I2C_ERROR_READ;
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
	//strcat(str, "\r\n");

	status = NO_ERROR;

	return status; // no ack/nack
}

/*
 * Description:
 *
 *
 */
int SetMCDP6200Register(char *msg){
	int status;
	char registerAddr[8];
	char dataValue[8];
	uint8_t byteArray[4];
	int i = 0;
	uint32_t regAddr;
	uint32_t regData;

	// check to see if register address starts with 0x
	if(strncmp(msg, "0x", strlen("0x")) != 0){
		return COMMAND_UNKNOWN;
	}
	msg += strlen("0x");

	// parse register address
	while(*msg != ','){
		registerAddr[i++] = *msg++;
	}
	status = Hex2Int(registerAddr, &regAddr);
	if(status != NO_ERROR){
		return COMMAND_UNKNOWN;
	}

	// check to see if comma with register data starts with 0x
	if(strncmp(msg, ",0x", strlen(",0x")) == 0){
	    msg += strlen(",0x");
	}
	else
	{
		return COMMAND_UNKNOWN;
	}

	// parse register data
	i = 0;
	while(*msg != '\r' && *msg != '\0'){
		dataValue[i++] = *msg++; // we should only be reading 8 characters max
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
int SetMCDP6200_RTMR_P_POL(char *msg){
    int status = NO_ERROR;

    if(strncmp(msg, "0", strlen("0")) == 0){
        MCDP6200_GPIO_Pin_RTMR_P_POL_Disable();
    }
    else if(strncmp(msg, "1", strlen("1")) == 0){
        MCDP6200_GPIO_Pin_RTMR_P_POL_Enable();
    }
    else
    {
        status = COMMAND_UNKNOWN;
    }

    return status;
}

/*
 *
 *
 *
 */
int SetMCDP6200_RTMR_DIS_N(char *msg){
    int status = NO_ERROR;

    if(strncmp(msg, "0", strlen("0")) == 0){
        MCDP6200_GPIO_Pin_RTMR_DIS_N_Disable();
    }
    else if(strncmp(msg, "1", strlen("1")) == 0){
        MCDP6200_GPIO_Pin_RTMR_DIS_N_Enable();
    }
    else
    {
        status = COMMAND_UNKNOWN;
    }

    return status;
}

/*
 *
 *
 *
 */
void MCDP6200_GPIO_Pin_RTMR_DIS_N_StartPulse(void){
    MCDP6200_GPIO_Pin_RTMR_DIS_N_Enable();
	TimerCallbackRegister(&timerCallbackInstance, MCDP6200_GPIO_Pin_RTMR_DIS_N_Disable,1000, TIMER_NO_REPEAT);
	TimerCallbackRegister(&timerCallbackInstance, MCDP6200_GPIO_Pin_RTMR_DIS_N_Enable,2000, TIMER_NO_REPEAT);
}

/*
 *
 *
 *
 */
void MCDP6200_GPIO_Pin_RTMR_DIS_N_Enable(void){
	GPIO_Pin_RTMR_DIS_N(1);
}

/*
 *
 *
 *
 */
void MCDP6200_GPIO_Pin_RTMR_DIS_N_Disable(void){
	GPIO_Pin_RTMR_DIS_N(0);
}

/*
 *
 *
 *
 */
void MCDP6200_GPIO_Pin_RTMR_P_POL_Enable(void){
    GPIO_Pin_RTMR_P_POL(1);
}

/*
 *
 *
 *
 */
void MCDP6200_GPIO_Pin_RTMR_P_POL_Disable(void){
    GPIO_Pin_RTMR_P_POL(0);
}


