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

#define MCDP6200_PrintLine


/*
	uint8_t retimerTxRegister[what side][site][register values]
	register values; {Main_EN, Pre_EN, Post_EN, Pre, Post, Tune(TX_DLDO_TUNE)}
*/
const RetimerTxRegister retimerTxRegister[4][24] = {
	{	// Gen 2 host
		{20, 5, 15, 5, 0, 6}, // site 1
		{20, 5, 15, 5, 0, 6}, // site 2
		{20, 5, 15, 4, 0, 5}, // ...
		{20, 5, 15, 4, 0, 2},
		{20, 5, 15, 5, 0, 6},
		{20, 5, 15, 5, 0, 6},
		{20, 5, 15, 3, 0, 3},
		{20, 5, 15, 3, 0, 3},
		{20, 5, 15, 2, 0, 2},
		{20, 5, 15, 2, 0, 0},
		{20, 5, 15, 4, 0, 3},
		{20, 5, 15, 5, 0, 4},
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 0, 0, 0}, // site 14
		{20, 5, 15, 1, 0, 0},
		{20, 5, 15, 4, 0, 5},
		{20, 5, 15, 3, 0, 0},
		{20, 5, 15, 4, 0, 0},
		{20, 5, 15, 1, 0, 0},
		{20, 5, 15, 1, 0, 0},
		{20, 5, 15, 1, 0, 0},
		{20, 5, 15, 2, 0, 0},
		{20, 5, 15, 2, 0, 0}, // site 23
		{20, 5, 15, 2, 0, 0} // done	
	},	
	{	// Gen 2 device
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6}, // site 10
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6}, // site 14
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6}, // site 20
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6},
		{20, 5, 15, 1, 9, 6} // done
	},
	{	// Gen 1 host
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 0, 0, 0}, // site 5
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 0, 0, 0}, // site 10
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 1, 0, 0},
		{20, 5, 15, 0, 0, 0}, // site 15
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 0, 0, 0},
		{20, 5, 15, 1, 0, 0},
		{20, 5, 15, 1, 0, 0}, // site 20
		{20, 5, 15, 1, 0, 0},
		{20, 5, 15, 1, 0, 0},
		{20, 5, 15, 1, 0, 0},
		{20, 5, 15, 1, 0, 0} // done
	},
	{	// Gen 1 device
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6},
		{20, 5, 15, 0, 7, 6} // done
	}	
};

/*
	uint8_t retimerRxRegister[what side][site][register values]
	// register values; {Main_EN, Pre_EN, Post_EN, Pre, Post, Tune(TX_DLDO_TUNE)}
*/
const RetimerRxRegister retimerRxRegister[2][24] = {
	{	// Gen 2 host
		{3, 3, 0, 4, 1, 35, 1}, // site 1
		{3, 3, 0, 4, 1, 33, 1}, // site 2
		{3, 3, 0, 4, 1, 40, 1}, // ...
		{3, 3, 0, 1, 1, 26, 1},
		{3, 2, 0, 6, 1, 48, 1},
		{3, 4, 0, 5, 1, 55, 1}, 
		{3, 2, 0, 2, 1, 45, 1}, // 7
		{3, 3, 0, 2, 1, 35, 1},
		{3, 3, 0, 0, 1, 35, 1},
		{3, 3, 0, 3, 1, 25, 1},
		{3, 3, 0, 2, 1, 30, 1},
		{3, 3, 0, 2, 1, 27, 1}, // 12
		{3, 0, 0, 0, 1, 35, 1},
		{3, 1, 0, 1, 1, 30, 1},
		{3, 3, 0, 1, 1, 33, 1},
		{3, 3, 0, 2, 1, 25, 1},
		{3, 3, 0, 2, 1, 22, 1}, // 17
		{3, 2, 0, 1, 1, 27, 1},
		{3, 0, 0, 1, 1, 20, 1},
		{3, 1, 0, 1, 1, 20, 1}, // 20
		{3, 3, 0, 2, 1, 24, 1},
		{3, 3, 0, 0, 1, 25, 1},
		{3, 2, 0, 1, 1, 27, 1},
		{3, 0, 0, 1, 1, 22, 1},  // site 24
	},	
	{	// Gen 2 device
		{5, 4, 0, 10, 1, 40, 1}, // site 1
		{5, 4, 0, 10, 1, 40, 1},
		{5, 4, 0, 10, 1, 40, 1},
		{5, 4, 0, 10, 1, 40, 1},
		{5, 4, 0, 10, 1, 40, 1}, // site 5
		{5, 4, 0, 10, 1, 40, 1},
		{5, 4, 0, 10, 1, 40, 1},
		{5, 4, 0, 10, 1, 40, 1},
		{5, 4, 0, 10, 1, 40, 1},
		{5, 4, 0, 10, 1, 40, 1}, // site 10
		{5, 4, 0, 10, 1, 40, 1},
		{5, 4, 0, 10, 1, 40, 1},
		{5, 5, 0, 12, 1, 40, 1},
		{5, 5, 0, 12, 1, 40, 1},
		{5, 5, 0, 12, 1, 40, 1}, // site 15
		{5, 5, 0, 12, 1, 40, 1},
		{5, 5, 0, 12, 1, 40, 1},
		{5, 5, 0, 12, 1, 40, 1},
		{5, 5, 0, 12, 1, 40, 1},
		{5, 5, 0, 12, 1, 40, 1}, // site 20
		{5, 5, 0, 12, 1, 40, 1},
		{5, 5, 0, 12, 1, 40, 1},
		{5, 5, 0, 12, 1, 40, 1},
		{5, 5, 0, 12, 1, 40, 1},											
	}	
};

static int RetimerWriteRxRegisterData(Retimer *retimer, RetimerStatus *retimerStatus);
static int RetimerWriteTxRegisterData(Retimer *retimer, RetimerStatus *retimerStatus);

/*
 *
 *
 *
 */
void MCDP6200_Init(void)
{

}

/*
* Description: Sets the TX_CONFIG_WINDOW_x for Read or Write, and for what Gen
* Input:
*  Output: none
*/
void MCDP6200_SetTxConfigWindow(uint8_t retimerAddress, uint16_t _txRegister, uint8_t rdWr, uint8_t gen){

}

// main registers write functions
/*
* Description: Write to rx register(s)
* Input: retimer register data
* Output: I2C error code
*/
static int RetimerWriteRxRegisterData(Retimer *retimer, RetimerStatus *retimerStatus){
	int errorCode = 0;
	uint8_t retimerAddress = 0;
	uint16_t registerAddress = 0; // register address
	uint8_t size = retimer->Status.rxSize;
	uint8_t i = 0;
	uint8_t tempRegisterData[4] = {0};
	uint8_t *ptr = retimer->Status.rx_RegisterWrite[i].registerData;
	
	for(i = 0; i < size; i++){
		retimerAddress = retimer->Status.retimerAddress;
		registerAddress = retimer->Status.rx_RegisterWrite[i].registerAddress;
		errorCode = MCDP6200_I2C_Write_Register(retimerAddress, registerAddress, ptr);
		if(errorCode != I2C_NOERR){
			return errorCode;
		}
		// TODO - verify data
		errorCode = MCDP6200_I2C_Read_Register_Data(retimerAddress, registerAddress, tempRegisterData);
		if(errorCode != I2C_NOERR){
			return errorCode;
		}

		if(memcmp(ptr, tempRegisterData, 4) != 0){
			retimerStatus->Status.regCompareError = 1;
			return I2C_FAIL;
		}
	}
	return I2C_NOERR;
}

/*
* Description: Write to tx register(s)
* Input: retimer register data
* Output: I2C error code
*/
static int RetimerWriteTxRegisterData(Retimer *retimer, RetimerStatus *retimerStatus){
	int errorCode = 0;
	uint8_t retimerAddress = 0;
	uint16_t registerAddress = 0; // register address
	uint8_t size = retimer->Status.txSize;
	uint8_t i = 0;
	uint8_t tempRegisterData[4] = {0};
	uint8_t *ptr = retimer->Status.tx_RegisterWrite[i].registerData;
	
	for(i = 0; i < size; i++){
		retimerAddress = retimer->Status.retimerAddress;
		registerAddress = retimer->Status.tx_RegisterWrite[i].registerAddress;
		errorCode = MCDP6200_I2C_Write_Register(retimerAddress, registerAddress, ptr);
		if(errorCode != I2C_NOERR){
			return errorCode;
		}
		// TODO - verify data
		errorCode = MCDP6200_I2C_Read_Register_Data(retimerAddress, registerAddress, tempRegisterData);
		if(errorCode != I2C_NOERR){
			return errorCode;
		}

		if(memcmp(ptr, tempRegisterData, 4) != 0){
			retimerStatus->Status.regCompareError = 1;
			return I2C_FAIL;
		}
	}
	return I2C_NOERR;
}


/*
* Description: 
* Input: 
* Output: 
*/
int RetimerWriteRxRegister(uint8_t address, uint8_t siteNumber, RetimerStatus *retimerStatus){
	Retimer retimer;
	uint8_t errorCode;
	uint8_t Rx_CTRL_def[4]; // 0x84
	uint8_t tap_wts_def[4]; // 0x24
	uint8_t dfe_ctrl_def[4]; // 0x28
	uint8_t rx_DFE_Adapt_def[4]; // 0x88
	const uint16_t readAddressDeviceBase = 0x2000; // device 
	const uint16_t writeAddressDeviceBase = 0x2000; // device
	const uint16_t readAddressHostBase = 0x1100; // host
	const uint16_t writeAddressHostBase = 0x1100; // host
	//const uint32_t DFE_WTS_EN_DG2 = 0x3f;
	//const uint32_t DFE_TAP_EN_DG2 = 0xffffffff;
	//const uint32_t CTLE_MN_CTRL_EN_DG2 = 0xffffffff;
	//const uint32_t RX_ADAPT_EN_DG2 = 0xFFFFFFFF;
	
	RX_Gen2_RX_Manual_EN_84_RW rx_Gen2_RX_Manual_EN_84_RW = {0};
	RX_DFE_TAP_CTRL_24_RW rx_DFE_TAP_CTRL_24_RW = {0};
	RX_DFE_CTRL_28_RW rx_DFE_CTRL_28_RW = {0};
	RX_DFE_ADAPT_CTRL_88_RW rx_DFE_ADAPT_CTRL_88_RW = {0};
	
	siteNumber -= 1; // defines arguments passed start at 1 but we start indexing at zero

	// ***** Init array *****
	retimer.Status.retimerAddress = address;
	retimer.Status.rxSize = 4;

	// ***************************************************************** write DEVICE side ***********************************************************			
	// init register address 0x84
	retimer.Status.rx_RegisterWrite[0].registerAddress = writeAddressDeviceBase+0x84;
	// read register address
	errorCode = MCDP6200_I2C_Read_Register_Data(address, readAddressDeviceBase+0x84, Rx_CTRL_def);
	if(errorCode != I2C_NOERR) return errorCode;
	// init data
	//rx_Gen2_RX_Manual_EN_84_RW.RegisterData.data = Rx_CTRL_def;
	memcpy(&rx_Gen2_RX_Manual_EN_84_RW.RegDataArray.data, &Rx_CTRL_def, 4);
	rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CDR_IGAIN = retimerRxRegister[GEN2_DEVICE][siteNumber].GEN2_CDR_IGAIN;
	rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CDR_PGAIN = retimerRxRegister[GEN2_DEVICE][siteNumber].GEN2_CDR_PGAIN;		
	rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CLK_TUNE = 0;
	//rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CTLE_ALGO
	rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CTLE_CAP = retimerRxRegister[GEN2_DEVICE][siteNumber].GEN2_CTLE_CAP;
	rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CTLE_RES = retimerRxRegister[GEN2_DEVICE][siteNumber].GEN2_CTLE_RES;
	rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CTLER_OVR = retimerRxRegister[GEN2_DEVICE][siteNumber].GEN2_CTLER_OVR;
	//rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_PI_CLOAD = retimerRxRegister[GEN2_DEVICE][siteNumber]
	memcpy(&retimer.Status.rx_RegisterWrite[0].registerData, &rx_Gen2_RX_Manual_EN_84_RW.RegDataArray.data, 4);
				
	// init register address 0x24
	retimer.Status.rx_RegisterWrite[1].registerAddress = writeAddressDeviceBase+0x24;
	// read register address
	errorCode = MCDP6200_I2C_Read_Register_Data(address, readAddressDeviceBase+0x24, tap_wts_def);
	if(errorCode != I2C_NOERR) return errorCode;
	// init data
	//rx_DFE_TAP_CTRL_24_RW.RegDataArray.data = tap_wts_def & DFE_TAP_EN_DG2;
	memcpy(&rx_DFE_TAP_CTRL_24_RW.RegDataArray.data, &tap_wts_def, 4);
	rx_DFE_TAP_CTRL_24_RW.Status.RX_SUM_TAP1_SPI = retimerRxRegister[GEN2_DEVICE][siteNumber].RX_SUM_TAP1_SPI;
	//rx_DFE_TAP_CTRL_24_RW.Status.RX_SUM_TAP2_SPI = 22;
	//rx_DFE_TAP_CTRL_24_RW.Status.RX_SUM_TAP3_SPI = 18;
	//rx_DFE_TAP_CTRL_24_RW.Status.RX_SUM_TAP4_SPI = 23;
	memcpy(&retimer.Status.rx_RegisterWrite[1].registerData, &rx_DFE_TAP_CTRL_24_RW.RegDataArray.data, 4);
	
	// init register address 0x28
	retimer.Status.rx_RegisterWrite[2].registerAddress = writeAddressDeviceBase+0x28;
	// read register address
	errorCode = MCDP6200_I2C_Read_Register_Data(address, readAddressDeviceBase+0x28, dfe_ctrl_def);
	if(errorCode != I2C_NOERR) return errorCode;
	// init data
	//rx_DFE_CTRL_28_RW.RegDataArray.data = dfe_ctrl_def & CTLE_MN_CTRL_EN_DG2;
	memcpy(&rx_DFE_CTRL_28_RW.RegDataArray.data, &dfe_ctrl_def, 4);
	rx_DFE_CTRL_28_RW.Status.RX_SUM_TAPWEIGHTSEL1 = retimerRxRegister[GEN2_DEVICE][siteNumber].RX_SUM_TAPWEIGHTSEL1;
	//rx_DFE_CTRL_28_RW.Status.RX_SUM_TAPWEIGHTSEL2
	//rx_DFE_CTRL_28_RW.Status.RX_SUM_TAPWEIGHTSEL3
	//rx_DFE_CTRL_28_RW.Status.RX_SUM_TAPWEIGHTSEL4
	memcpy(&retimer.Status.rx_RegisterWrite[2].registerData, &rx_DFE_CTRL_28_RW.RegDataArray.data, 4);

	// init register address 0x88
	retimer.Status.rx_RegisterWrite[3].registerAddress = writeAddressDeviceBase+0x88;
	// read register address
	errorCode = MCDP6200_I2C_Read_Register_Data(address, readAddressDeviceBase+0x88, rx_DFE_Adapt_def);
	if(errorCode != I2C_NOERR) return errorCode;
	// init data
	//rx_DFE_ADAPT_CTRL_88_RW.RegDataArray.data = rx_DFE_Adapt_def & RX_ADAPT_EN_DG2;
	memcpy(&rx_DFE_ADAPT_CTRL_88_RW.RegDataArray.data, &rx_DFE_Adapt_def, 4);
	//rx_DFE_ADAPT_CTRL_88_RW.Status.GEN1_EQ_MODE_TAP
	//rx_DFE_ADAPT_CTRL_88_RW.Status.GEN1_EQ_WAIT_TAP
	//rx_DFE_ADAPT_CTRL_88_RW.Status.GEN2_EQ_MODE_TAP = 2;
	//rx_DFE_ADAPT_CTRL_88_RW.Status.GEN2_EQ_WAIT_TAP = 0x0C;
	memcpy(&retimer.Status.rx_RegisterWrite[3].registerData, &rx_DFE_ADAPT_CTRL_88_RW.RegDataArray.data, 4);
	
	// write register data
	errorCode = RetimerWriteRxRegisterData(&retimer, retimerStatus);
	if(errorCode != I2C_NOERR) return errorCode;
	
	
	// ************************************************************ write HOST side ****************************************************************
	// init register address 0x84
	retimer.Status.rx_RegisterWrite[0].registerAddress = writeAddressHostBase+0x84;
	// read register address
	errorCode = MCDP6200_I2C_Read_Register_Data(address, readAddressHostBase+0x84, Rx_CTRL_def);
	if(errorCode != I2C_NOERR) return errorCode;
	// init data
	//rx_Gen2_RX_Manual_EN_84_RW.RegDataArray.data = Rx_CTRL_def;
	memcpy(&rx_Gen2_RX_Manual_EN_84_RW.RegDataArray.data, &Rx_CTRL_def, 4);
	rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CDR_IGAIN = retimerRxRegister[GEN2_HOST][siteNumber].GEN2_CDR_IGAIN;
	rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CDR_PGAIN = retimerRxRegister[GEN2_HOST][siteNumber].GEN2_CDR_PGAIN;
	rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CLK_TUNE = 0;
	//rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CTLE_ALGO
	rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CTLE_CAP = retimerRxRegister[GEN2_HOST][siteNumber].GEN2_CTLE_CAP;
	rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CTLE_RES = retimerRxRegister[GEN2_HOST][siteNumber].GEN2_CTLE_RES;
	rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_CTLER_OVR = retimerRxRegister[GEN2_HOST][siteNumber].GEN2_CTLER_OVR;
	//rx_Gen2_RX_Manual_EN_84_RW.Status.GEN2_PI_CLOAD = retimerRxRegister[GEN2_HOST][siteNumber]
	memcpy(&retimer.Status.rx_RegisterWrite[0].registerData, &rx_Gen2_RX_Manual_EN_84_RW.RegDataArray.data, 4);

	
	// init register address 0x24
	retimer.Status.rx_RegisterWrite[1].registerAddress = writeAddressHostBase+0x24;
	// read register address
	errorCode = MCDP6200_I2C_Read_Register_Data(address, readAddressHostBase+0x24, tap_wts_def);
	if(errorCode != I2C_NOERR) return errorCode;
	// init data
	//rx_DFE_TAP_CTRL_24_RW.RegDataArray.data = tap_wts_def & DFE_TAP_EN_DG2;
	memcpy(&rx_DFE_TAP_CTRL_24_RW.RegDataArray.data, &tap_wts_def, 4);
	rx_DFE_TAP_CTRL_24_RW.Status.RX_SUM_TAP1_SPI = retimerRxRegister[GEN2_HOST][siteNumber].RX_SUM_TAP1_SPI;
	//rx_DFE_TAP_CTRL_24_RW.Status.RX_SUM_TAP2_SPI = 22;
	//rx_DFE_TAP_CTRL_24_RW.Status.RX_SUM_TAP3_SPI = 18;
	//rx_DFE_TAP_CTRL_24_RW.Status.RX_SUM_TAP4_SPI = 23;
	memcpy(&retimer.Status.rx_RegisterWrite[1].registerData, &rx_DFE_TAP_CTRL_24_RW.RegDataArray.data, 4);
	
	// init register address 0x28
	retimer.Status.rx_RegisterWrite[2].registerAddress = writeAddressHostBase+0x28;
	// read register address
	errorCode = MCDP6200_I2C_Read_Register_Data(address, readAddressHostBase+0x28, dfe_ctrl_def);
	if(errorCode != I2C_NOERR) return errorCode;
	// init data
	//rx_DFE_CTRL_28_RW.RegDataArray.data = dfe_ctrl_def & CTLE_MN_CTRL_EN_DG2;
	memcpy(&rx_DFE_CTRL_28_RW.RegDataArray.data, &dfe_ctrl_def, 4);
	rx_DFE_CTRL_28_RW.Status.RX_SUM_TAPWEIGHTSEL1 = retimerRxRegister[GEN2_HOST][siteNumber].RX_SUM_TAPWEIGHTSEL1;
	//rx_DFE_CTRL_28_RW.Status.RX_SUM_TAPWEIGHTSEL2
	//rx_DFE_CTRL_28_RW.Status.RX_SUM_TAPWEIGHTSEL3
	//rx_DFE_CTRL_28_RW.Status.RX_SUM_TAPWEIGHTSEL4
	memcpy(&retimer.Status.rx_RegisterWrite[2].registerData, &rx_DFE_CTRL_28_RW.RegDataArray.data, 4);
	
	// init register address 0x88
	retimer.Status.rx_RegisterWrite[3].registerAddress = writeAddressHostBase+0x88;
	// read register address
	errorCode = MCDP6200_I2C_Read_Register_Data(address, readAddressHostBase+0x88, rx_DFE_Adapt_def);
	if(errorCode != I2C_NOERR) return errorCode;
	// init data
	//rx_DFE_ADAPT_CTRL_88_RW.RegDataArray.data = rx_DFE_Adapt_def & RX_ADAPT_EN_DG2;
	memcpy(&rx_DFE_ADAPT_CTRL_88_RW.RegDataArray.data, &rx_DFE_Adapt_def, 4);
	//rx_DFE_ADAPT_CTRL_88_RW.Status.GEN1_EQ_MODE_TAP
	//rx_DFE_ADAPT_CTRL_88_RW.Status.GEN1_EQ_WAIT_TAP
	//rx_DFE_ADAPT_CTRL_88_RW.Status.GEN2_EQ_MODE_TAP = 2;
	//rx_DFE_ADAPT_CTRL_88_RW.Status.GEN2_EQ_WAIT_TAP = 0x0C;
	memcpy(&retimer.Status.rx_RegisterWrite[3].registerData, &rx_DFE_ADAPT_CTRL_88_RW.RegDataArray.data, 4);
		
	// write register data
	return RetimerWriteRxRegisterData(&retimer, retimerStatus);
	
}

/*
* Description:
*
*
*/
int RetimerWriteTxRegister(uint8_t address, uint8_t siteNumber, RetimerStatus *retimerStatus){
	int errorCode = 0;
	//uint32_t windows_0_r;// not used in python script
	uint8_t windows_1_r[4];
	uint8_t windows_0_w[4];
	uint8_t windows_1_w[4];
	uint8_t tempData[4];
	TX_CONF_WINDOW_0_54_RW tx_CONF_WINDOW_0_54_RW = {0};
	TX_CONF_WINDOW_1_58_RW tx_CONF_WINDOW_1_58_RW = {0};
	const uint16_t readAddressDeviceBase = 0x2400; // device
	const uint16_t writeAddressDeviceBase = 0x2400; // device
	const uint16_t readAddressHostBase = 0x1000; // host
	const uint16_t writeAddressHostBase = 0x1000; // host
	Retimer retimer;

	siteNumber -= 1; // defines arguments passed start at 1 but we start indexing at zero

	// ***** Init array *****
	retimer.Status.retimerAddress = address;
	retimer.Status.txSize = 8;
	
	
	
	// ************************************************* setup GEN 1 ********************************************************
	
	// ***************************************************** DEVICE side *************************************************
	retimer.Status.txSize = 7; // We don't need to update Compliance Mode Parameter, so last index is not sent
	
	retimer.Status.tx_RegisterWrite[0].registerAddress = writeAddressDeviceBase+0x54; // window 0
	retimer.Status.tx_RegisterWrite[1].registerAddress = writeAddressDeviceBase+0x58; // window 1
	// update Gen1
	retimer.Status.tx_RegisterWrite[2].registerAddress = writeAddressDeviceBase+0x5C; // Update Gen1 Parameter
	retimer.Status.tx_RegisterWrite[2].registerData[0] = 0x80; // gen 1 = 0x80, gen 2 = 0x81
	retimer.Status.tx_RegisterWrite[3].registerAddress = writeAddressDeviceBase+0x5C; // Update Compliance Mode Parameter
	retimer.Status.tx_RegisterWrite[3].registerData[0] = 0x9A;

	// Host side
	retimer.Status.tx_RegisterWrite[4].registerAddress = writeAddressHostBase+0x54; // window 0
	retimer.Status.tx_RegisterWrite[5].registerAddress = writeAddressHostBase+0x58; // window 1
	// update Gen1
	retimer.Status.tx_RegisterWrite[6].registerAddress = writeAddressHostBase+0x5C; // Update Gen1 Parameter
	retimer.Status.tx_RegisterWrite[6].registerData[0] = 0x80; // gen 1 = 0x80, gen 2 = 0x81
	retimer.Status.tx_RegisterWrite[7].registerAddress = writeAddressHostBase+0x5C; // Update Compliance Mode Parameter
	retimer.Status.tx_RegisterWrite[7].registerData[0] = 0x9A;
		
	
	// device side
	tempData[0] = 0x00; // gen 1
	errorCode = MCDP6200_I2C_Write_Register(address,  writeAddressHostBase+0x5C, tempData);	// Read parameter of USBGEN1
	if(errorCode != I2C_NOERR) return errorCode;

	//errorCode = I2C_0_read4ByteRegister(address, readAddressDeviceBase+0x60, &windows_0_r); // python script doesn't use this else where
	//if(errorCode != I2C_NOERR) return errorCode;

	errorCode = MCDP6200_I2C_Read_Register_Data(address, readAddressDeviceBase+0x64, windows_1_r); // Read WINDOWS1_MON
	if(errorCode != I2C_NOERR) return errorCode;

	// init windows_0
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_MAIN_EN = retimerTxRegister[GEN1_DEVICE][siteNumber].mainEN;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_PRE_EN = retimerTxRegister[GEN1_DEVICE][siteNumber].preEN;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_POST_EN = retimerTxRegister[GEN1_DEVICE][siteNumber].postEN;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_PRE = retimerTxRegister[GEN1_DEVICE][siteNumber].pre;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_POST = retimerTxRegister[GEN1_DEVICE][siteNumber].post;
	//windows_0_w = tx_CONF_WINDOW_0_54_RW.RegDataArray.data;
	memcpy(&windows_0_w, &tx_CONF_WINDOW_0_54_RW.RegDataArray.data, 4);
	//retimer.Status.tx_RegisterWrite[0].registerData[0] = windows_0_w;
	memcpy(&retimer.Status.tx_RegisterWrite[0].registerData, &windows_0_w, 4);

	// init windows_1
	//tx_CONF_WINDOW_1_58_RW.RegDataArray.data = windows_1_r;
	memcpy(&tx_CONF_WINDOW_1_58_RW.RegDataArray.data, &windows_1_r, 4);
	tx_CONF_WINDOW_1_58_RW.Status.TX_DLDO_TUNE = retimerTxRegister[GEN1_DEVICE][siteNumber].dldoTune; // but we can use our structure to change the value
	//windows_1_w = tx_CONF_WINDOW_1_58_RW.RegDataArray.data;
	memcpy(&windows_1_w, &tx_CONF_WINDOW_1_58_RW.RegDataArray.data, 4);
	//retimer.Status.tx_RegisterWrite[1].registerData[0] = windows_1_w;
	memcpy(&retimer.Status.tx_RegisterWrite[1].registerData, &windows_1_w, 4);

	// ****************************************************** HOST side *********************************************

	tempData[0] = 0x00; // gen 1
	errorCode = MCDP6200_I2C_Write_Register(address, writeAddressHostBase+0x5C, tempData);	// Read parameter of USBGEN1
	if(errorCode != I2C_NOERR) return errorCode;

	//errorCode = I2C_0_read4ByteRegister(address, readAddressHostBase+0x60, &windows_0_r); // python script doesn't use this else where
	//if(errorCode != I2C_NOERR) return errorCode;

	errorCode = MCDP6200_I2C_Read_Register_Data(address, readAddressHostBase+0x64, windows_1_r); // Read WINDOWS1_MON
	if(errorCode != I2C_NOERR) return errorCode;
	
	// init windows_0
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_MAIN_EN = retimerTxRegister[GEN1_HOST][siteNumber].mainEN;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_PRE_EN = retimerTxRegister[GEN1_HOST][siteNumber].preEN;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_POST_EN = retimerTxRegister[GEN1_HOST][siteNumber].postEN;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_PRE = retimerTxRegister[GEN1_HOST][siteNumber].pre;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_POST = retimerTxRegister[GEN1_HOST][siteNumber].post;
	//windows_0_w = tx_CONF_WINDOW_0_54_RW.RegDataArray.data;
	memcpy(&windows_0_w, &tx_CONF_WINDOW_0_54_RW.RegDataArray.data, 4);
	//retimer.Status.tx_RegisterWrite[4].registerData[0] = windows_0_w;
	memcpy(&retimer.Status.tx_RegisterWrite[4].registerData, &windows_0_w, 4);

	// init windows_1
	//tx_CONF_WINDOW_1_58_RW.RegDataArray.data = windows_1_r;
	memcpy(&tx_CONF_WINDOW_1_58_RW.RegDataArray.data, &windows_1_r, 4);
	tx_CONF_WINDOW_1_58_RW.Status.TX_DLDO_TUNE = retimerTxRegister[GEN1_HOST][siteNumber].dldoTune; // but we can use our structure to change the value
	//windows_1_w = tx_CONF_WINDOW_1_58_RW.RegDataArray.data;
	memcpy(&windows_1_w, &tx_CONF_WINDOW_1_58_RW.RegDataArray.data, 4);
	//retimer.Status.tx_RegisterWrite[5].registerData[0] = windows_1_w;
	memcpy(&retimer.Status.tx_RegisterWrite[5].registerData, &windows_1_w, 4);

	// write data
	errorCode = RetimerWriteTxRegisterData(&retimer, retimerStatus);
	if(errorCode != I2C_NOERR) return errorCode;
	// end gen 1
	
	
	
	// ********************************************* setup GEN 2 **************************************************

	// ********************************************** DEVICE side *************************************************
	retimer.Status.tx_RegisterWrite[0].registerAddress = writeAddressDeviceBase+0x54; // window 0
	retimer.Status.tx_RegisterWrite[1].registerAddress = writeAddressDeviceBase+0x58; // window 1
	// update Gen2
	retimer.Status.tx_RegisterWrite[2].registerAddress = writeAddressDeviceBase+0x5C; // Update Gen2 Parameter
	retimer.Status.tx_RegisterWrite[2].registerData[0] = 0x81; // gen 1 = 0x80, gen 2 = 0x81
	retimer.Status.tx_RegisterWrite[3].registerAddress = writeAddressDeviceBase+0x5C; // Update Compliance Mode Parameter
	retimer.Status.tx_RegisterWrite[3].registerData[0] = 0x9A;

	// Host side
	retimer.Status.tx_RegisterWrite[4].registerAddress = writeAddressHostBase+0x54; // window 0
	retimer.Status.tx_RegisterWrite[5].registerAddress = writeAddressHostBase+0x58; // window 1
	// update Gen2
	retimer.Status.tx_RegisterWrite[6].registerAddress = writeAddressHostBase+0x5C; // Update Gen2 Parameter
	retimer.Status.tx_RegisterWrite[6].registerData[0] = 0x81; // gen 1 = 0x80, gen 2 = 0x81
	retimer.Status.tx_RegisterWrite[7].registerAddress = writeAddressHostBase+0x5C; // Update Compliance Mode Parameter
	retimer.Status.tx_RegisterWrite[7].registerData[0] = 0x9A;
	
	// device side
	tempData[0] = 0x01; // gen 2
	errorCode = MCDP6200_I2C_Write_Register(address, writeAddressDeviceBase+0x24, tempData);	// Read parameter of USBGEN2
	if(errorCode != I2C_NOERR) return errorCode;

	//errorCode = I2C_0_read4ByteRegister(address, readAddressDeviceBase+0x60, &windows_0_r); // python script doesn't use this else where
	//if(errorCode != I2C_NOERR) return errorCode;

	errorCode = MCDP6200_I2C_Read_Register_Data(address, readAddressDeviceBase+0x64, windows_1_r); // Read WINDOWS1_MON
	if(errorCode != I2C_NOERR) return errorCode;

	// get values from retimerTxRegister array
	
	// device side
	// init windows_0
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_SWING_EN = 0;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_MAIN_EN = retimerTxRegister[GEN2_DEVICE][siteNumber].mainEN;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_PRE_EN = retimerTxRegister[GEN2_DEVICE][siteNumber].preEN;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_POST_EN = retimerTxRegister[GEN2_DEVICE][siteNumber].postEN;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_PRE = retimerTxRegister[GEN2_DEVICE][siteNumber].pre;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_POST = retimerTxRegister[GEN2_DEVICE][siteNumber].post;
	//windows_0_w = tx_CONF_WINDOW_0_54_RW.RegDataArray.data;
	memcpy(&windows_0_w, &tx_CONF_WINDOW_0_54_RW.RegDataArray.data, 4);
	//retimer.Status.tx_RegisterWrite[0].registerData[0] = windows_0_w;
	memcpy(&retimer.Status.tx_RegisterWrite[0].registerData, &windows_0_w, 4);

	// init windows_1
	//tx_CONF_WINDOW_1_58_RW.RegDataArray.data = windows_1_r;
	memcpy(&tx_CONF_WINDOW_1_58_RW.RegDataArray.data, &windows_1_r, 4);
	tx_CONF_WINDOW_1_58_RW.Status.TX_DLDO_TUNE = retimerTxRegister[GEN2_DEVICE][siteNumber].dldoTune; // but we can use our structure to change the value
	//windows_1_w = tx_CONF_WINDOW_1_58_RW.RegDataArray.data;
	memcpy(&windows_1_w, &tx_CONF_WINDOW_1_58_RW.RegDataArray.data, 4);
	//retimer.Status.tx_RegisterWrite[1].registerData[0] = windows_1_w;
	memcpy(&retimer.Status.tx_RegisterWrite[1].registerData, &windows_1_w, 4);


	// ********************************************** HOST side *********************************************************
	tempData[0] = 0x01; // gen 2
	errorCode = MCDP6200_I2C_Write_Register(address, writeAddressHostBase+0x5C, tempData);	// Read parameter of USBGEN2
	if(errorCode != I2C_NOERR) return errorCode;

	//errorCode = I2C_0_read4ByteRegister(address, readAddressHostBase+0x60, &windows_0_r); // python script doesn't use this else where
	//if(errorCode != I2C_NOERR) return errorCode;

	errorCode = MCDP6200_I2C_Read_Register_Data(address, readAddressHostBase+0x64, windows_1_r); // Read WINDOWS1_MON
	if(errorCode != I2C_NOERR) return errorCode;

	// init windows_0
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_SWING_EN = 0;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_MAIN_EN = retimerTxRegister[GEN2_HOST][siteNumber].mainEN;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_PRE_EN = retimerTxRegister[GEN2_HOST][siteNumber].preEN;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_POST_EN = retimerTxRegister[GEN2_HOST][siteNumber].postEN;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_PRE = retimerTxRegister[GEN2_HOST][siteNumber].pre;
	tx_CONF_WINDOW_0_54_RW.Status.TX_VML_POST = retimerTxRegister[GEN2_HOST][siteNumber].post;
	//windows_0_w = tx_CONF_WINDOW_0_54_RW.RegDataArray.data;
	memcpy(&windows_0_w, &tx_CONF_WINDOW_0_54_RW.RegDataArray.data, 4);
	//retimer.Status.tx_RegisterWrite[4].registerData[0] = windows_0_w;
	memcpy(&retimer.Status.tx_RegisterWrite[4].registerData, &windows_0_w, 4);

	// init windows_1
	//tx_CONF_WINDOW_1_58_RW.RegDataArray.data = windows_1_r;
	memcpy(&tx_CONF_WINDOW_1_58_RW.RegDataArray.data, &windows_1_r, 4);
	tx_CONF_WINDOW_1_58_RW.Status.TX_DLDO_TUNE = retimerTxRegister[GEN2_HOST][siteNumber].dldoTune; // but we can use our structure to change the value
	//windows_1_w = tx_CONF_WINDOW_1_58_RW.RegDataArray.data;
	memcpy(&windows_1_w, &tx_CONF_WINDOW_1_58_RW.RegDataArray.data, 4);
	//retimer.Status.tx_RegisterWrite[5].registerData[0] = windows_1_w;
	memcpy(&retimer.Status.tx_RegisterWrite[5].registerData, &windows_1_w, 4);

	// write data
	return RetimerWriteTxRegisterData(&retimer, retimerStatus);
	// end gen 2
}


