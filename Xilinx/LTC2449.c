/*
 * LTC2449.c
 *
 *  Created on: Jun 28, 2021
 *      Author: karl.yamashita
 *
 *
 *
 *
 *
 */


#include "main.h"
#include "LTC2449.h"

//#define LTC2449_PrintLine

LTC2449_SDI_Data ltc2449_SDI_Data;

uint8_t LTC2449_WriteBuffer[BUFFER_SIZE];
uint8_t LTC2449_ReadBuffer[BUFFER_SIZE];


/*
 * Description: 16 channels with 4 bit mux address for ODD, A2, A1, A0
 * 				See datasheet for LTC2449 page 14 table 3.
 *
 */
const uint8_t LTC2449_ChannelSelection[16][4] = {
		{0, 0, 0, 0}, // 0
		{1, 0, 0, 0},
		{0, 0, 0, 1}, // 2
		{1, 0, 0, 1},
		{0, 0, 1, 0},
		{1, 0, 1, 0},
		{0, 0, 1, 1}, // 6
		{1, 0, 1, 1},
		{0, 1, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 0, 1}, // 10
		{1, 1, 0, 1},
		{0, 1, 1, 0},
		{1, 1, 1, 0},
		{0, 1, 1, 1},
		{1, 1, 1, 1} // 15
};

/*
 * Description: Check for BUSY I/O
 * 				BUSY = 0 // Sleep, can read data now
 * 				BUSY = 1 // Conversion in progress
 *
 * 	Read register for ADC_BUSY pin
 *
 */
bool LTC2449_GetBusyState(void){
	uint32_t adcBusyReg;
#ifdef LTC2449_PrintLine
	char str[32];
	char str2[16];
#endif

	PDB_5038_ConfigReg_ReadValue32LE(ADC_BUSY_ADDR, &adcBusyReg);
#ifdef LTC2449_PrintLine
	strcpy(str, "BusyState: ");
	itoa(adcBusyReg, str2, 10);
	strcat(str, str2);
//	PrintLine(str);
#endif
	return (adcBusyReg != 0) ? true:false; // looks like bit 3 is set if busy.
}


/*
 *
 *
 *
 */
int LTC2449_SetChannel(uint8_t channel){
	int status = 0;
#ifdef LTC2449_PrintLine
	char str[32];
	char str2[2];
#endif
	uint8_t ByteCount = 4;
	memset(&ltc2449_SDI_Data, 0, sizeof(ltc2449_SDI_Data));
#ifdef LTC2449_PrintLine
//	PrintLine("LTC2449_SetChannel()");
#endif
	if(LTC2449_GetBusyState() == true){

	}
	ltc2449_SDI_Data.ChannelSpdRes.Conversion = 0x5;
	ltc2449_SDI_Data.ChannelSpdRes.SGL = 1; // always 1
	ltc2449_SDI_Data.ChannelSpdRes.OSR = 1;
	ltc2449_SDI_Data.ChannelSpdRes.TWOX = 1;

	ltc2449_SDI_Data.ChannelSpdRes.ODD = LTC2449_ChannelSelection[channel][0];
	ltc2449_SDI_Data.ChannelSpdRes.A2 = LTC2449_ChannelSelection[channel][1];
	ltc2449_SDI_Data.ChannelSpdRes.A1 = LTC2449_ChannelSelection[channel][2];
	ltc2449_SDI_Data.ChannelSpdRes.A0 = LTC2449_ChannelSelection[channel][3];

	LTC2449_WriteBuffer[0] = ltc2449_SDI_Data.Bytes.byte0;
	LTC2449_WriteBuffer[1] = ltc2449_SDI_Data.Bytes.byte1;
	LTC2449_WriteBuffer[2] = ltc2449_SDI_Data.Bytes.byte2;
	LTC2449_WriteBuffer[3] = ltc2449_SDI_Data.Bytes.byte3;

	status = SPI_WriteData(LTC2449_WriteBuffer, ByteCount);
	if(status != XST_SUCCESS){
		return status;
	}

#ifdef LTC2449_PrintLine
	strcpy(str, "LTC2449_SetChannel ");
	itoa(channel, str2, 10);
	strcat(str, str2);
	strcat(str, " ok");
//	PrintLine(str);
#endif

	return status;
}

int LTC2449_GetChannelValue(uint8_t channel, LTC2449_SDO_Data *data_OUT) {
	int status = 0;
#ifdef LTC2449_PrintLine
	char str[32];
	char str2[16];
#endif
	uint8_t ByteCount = 4;
	memset(&ltc2449_SDI_Data, 0, sizeof(ltc2449_SDI_Data));
#ifdef LTC2449_PrintLine
//	PrintLine("LTC2449_GetChannelValue()");
#endif
	if(LTC2449_GetBusyState() == true){

	}
	ltc2449_SDI_Data.ChannelSpdRes.Conversion = 0x0;
	ltc2449_SDI_Data.ChannelSpdRes.SGL = 1; // always 1
	ltc2449_SDI_Data.ChannelSpdRes.OSR = 1;
	ltc2449_SDI_Data.ChannelSpdRes.TWOX = 1;

	ltc2449_SDI_Data.ChannelSpdRes.ODD = LTC2449_ChannelSelection[channel][0];
	ltc2449_SDI_Data.ChannelSpdRes.A2 = LTC2449_ChannelSelection[channel][1];
	ltc2449_SDI_Data.ChannelSpdRes.A1 = LTC2449_ChannelSelection[channel][2];
	ltc2449_SDI_Data.ChannelSpdRes.A0 = LTC2449_ChannelSelection[channel][3];

	LTC2449_WriteBuffer[0] = ltc2449_SDI_Data.Bytes.byte0;
	LTC2449_WriteBuffer[1] = ltc2449_SDI_Data.Bytes.byte1;
	LTC2449_WriteBuffer[2] = ltc2449_SDI_Data.Bytes.byte2;
	LTC2449_WriteBuffer[3] = ltc2449_SDI_Data.Bytes.byte3;

	memset(&LTC2449_ReadBuffer, 0, sizeof(LTC2449_ReadBuffer));

	status = SPI_TransferData(LTC2449_WriteBuffer, LTC2449_ReadBuffer, ByteCount);
	if(status != XST_SUCCESS){
		return status;
	}

	// shuffle bytes
	data_OUT->Bytes.data[3] = LTC2449_ReadBuffer[0];
	data_OUT->Bytes.data[2] = LTC2449_ReadBuffer[1];
	data_OUT->Bytes.data[1] = LTC2449_ReadBuffer[2];
	data_OUT->Bytes.data[0] = LTC2449_ReadBuffer[3];

#ifdef LTC2449_PrintLine
	strcpy(str, "Range: ");
	itoa(data_OUT->InputRange.range, str2, 10);
	strcat(str,str2);
	strcat(str, "\r\n");

	strcat(str, "ADC Result: ");
	itoa(data_OUT->Status.result, str2, 16);
	strcat(str, str2);
	PrintLine(str);
#endif

	return status;
}

