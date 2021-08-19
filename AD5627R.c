/*
 * AD5627R.c
 *
 *  Created on: Jul 1, 2021
 *      Author: karl.yamashita
 *
 *	12 bit resolution: 5v / 4096 = 0.001220703125v
 *
 *
 *
 */


#include "main.h"
#include "AD5627R.h"

//#define AD5627R_PrintLine

extern XIic Iic;

AD5627R_Data_Struct ad5627R_Data_Struct = {0};

/*
 *
 *
 *
 *
 */
int AD5627R_Init(void){
	int errorCode = 0;
	memset(&ad5627R_Data_Struct, 0, sizeof(ad5627R_Data_Struct));

	ad5627R_Data_Struct.ReferenceSetup_Struct.command = INTERNAL_REF_SETUP;
	ad5627R_Data_Struct.ReferenceSetup_Struct.refMode = 1; // internal reference On

	errorCode = AD5627R_Write(ad5627R_Data_Struct.Bytes.data, 3); // write 3 bytes
	if(errorCode != 0){
		return errorCode;
	}

	return errorCode;
}

int AD5627R_ClearRef(void){
	int errorCode = 0;
	memset(&ad5627R_Data_Struct, 0, sizeof(ad5627R_Data_Struct));

	ad5627R_Data_Struct.ReferenceSetup_Struct.command = INTERNAL_REF_SETUP;
	ad5627R_Data_Struct.ReferenceSetup_Struct.refMode = 0; // internal reference On

	errorCode = AD5627R_Write(ad5627R_Data_Struct.Bytes.data, 3); // write 3 bytes
	if(errorCode != 0){
		return errorCode;
	}

	return errorCode;
}

/*
 * Description:
 *
 *
 */
int AD5627R_SetDAC_Value(uint8_t command, float voltage){
	int errorCode = 0;
	uint16_t value;
#ifdef AD5627R_PrintLine
	char str[24] = {0};
	char str2[8];

	PrintLine("AD5627R_SetDAC_Value()");
#endif
	memset(&ad5627R_Data_Struct, 0, sizeof(ad5627R_Data_Struct));

	// Init AD5627R
	ad5627R_Data_Struct.InputShift_Struct.command = command; // UPDATE_DAC_REG; // WRITE_TO_INPUT_REG_UPDATE_ALL;
	ad5627R_Data_Struct.InputShift_Struct.dacAddr = DAC_A;
	ad5627R_Data_Struct.InputShift_Struct.byteSelection = 0;

	value = (uint16_t) (voltage / AD_RESOLUTION);

#ifdef AD5627R_PrintLine
	strcpy(str, "12 bit Value: ");
	itoa(value, str2, 10);
	strcat(str, str2);
	PrintLine(str);
#endif
	value = (value << 4);
	ad5627R_Data_Struct.InputShift_Struct.bytes.dacHighByte = (value & 0x00ff) << 8;
	ad5627R_Data_Struct.InputShift_Struct.bytes.dacLowByte = (value >> 8);
	errorCode = AD5627R_Write(ad5627R_Data_Struct.Bytes.data, 3); // write 3 bytes
	if(errorCode != 0){
		return errorCode;
	}
#ifdef AD5627R_PrintLine
//	PrintLine("AD5627R_Init successful!");
#endif

	return errorCode;
}


/*
 * Description: Three bytes of data must therefore be written to the DAC,
 * 				 the command byte followed by the most significant data byte and the least significant data byte
 *
 * Input:
 * Output:
 */
int AD5627R_Write(uint8_t *data_IN, uint8_t byteCount)
{
	int errorCode = 0;
	int bytesSent = 0;
	uint8_t data[3] = {0};
	uint8_t *ptr = data_IN;
	int i;
#ifdef AD5627R_PrintLine
	char str[32];

//	PrintLine("AD5627R_Write()");
#endif
	//memcpy(&data, ptr, 3);
	for(i = 0; i < 3; i++){
		data[i] = *ptr++;
	}
	bytesSent = I2C_Send_Data(I2C_PORT_0, AD5627R_SLAVE_ADDRESS, data, byteCount, XIIC_STOP);
	if(bytesSent != byteCount){
#ifdef AD5627R_PrintLine
	//	sprintf(str, "AD5627R_Write busy. Bytes sent: %d", bytesSent);
	//	PrintLine(str);
#endif
		return XST_IIC_BUS_BUSY;
	}
#ifdef AD5627R_PrintLine
//	PrintLine("AD5627R_Write success");
#endif
	return errorCode;
}

/*
 * Description: Read 3 bytes of data which is the command byte and the 2 bytes of data.
 *
 * Input: the pointer to the data array and how many bytes to read.
 * Output: errorCode
 *
 */
int AD5627R_Read(uint8_t *data_OUT, uint8_t byteCount)
{
	int errorCode = 0;
	int bytesSent = 0;
#ifdef AD5627R_PrintLine
//	PrintLine("AD5627R_Read()");
#endif
	bytesSent = I2C_Get_Data(I2C_PORT_0, AD5627R_SLAVE_ADDRESS, data_OUT, byteCount,  XIIC_STOP);
	if(bytesSent != byteCount){
		return XST_IIC_BUS_BUSY;
	}
#ifdef AD5627R_PrintLine
//	PrintLine("AD5627R_Read success");
#endif
	return errorCode;
}

void GPIO_Pin_CLR_Set(bool status){
	uint8_t pins;

	XIic_GetGpOutput(&Iic, &pins);

	if(status){
		pins |= 1;
	}
	else
	{
		pins &= 0xFE;
	}

	XIic_SetGpOutput(&Iic, pins);
}

void GPIO_Pin_LDAC_Set(bool status){
	uint8_t pins;

	XIic_GetGpOutput(&Iic, &pins);

	if(status){
		pins |= 1 << 1;
	}
	else
	{
		pins &= 0xFD;
	}

	XIic_SetGpOutput(&Iic, pins);
}





