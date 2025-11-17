/*
 * XDPE192C3D_Drv.c
 *
 *  Created on: Sep 24, 2025
 *      Author: karl.yamashita
 */

#include "main.h"


// define registers
#define page DECLARE_XDPE192C3D_REG("page", PAGE, NO_PAGE, LEN_1, REG_FORMAT, WRITE_ONLY)
#define operation DECLARE_XDPE192C3D_REG("operation", OPERATION_paged, IS_PAGED, LEN_1, REG_FORMAT, READ_WRITE)
#define onoffconfig DECLARE_XDPE192C3D_REG("onoffconfig", ON_OFF_CONFIG_paged, IS_PAGED, LEN_1, REG_FORMAT, READ_WRITE)
#define clrfaults DECLARE_XDPE192C3D_REG("clearfaults", CLEAR_FAULTS, NO_PAGE, LEN_0, REG_FORMAT, WRITE_ONLY)
#define writeprotect DECLARE_XDPE192C3D_REG("writeprotect", WRITE_PROTECT_paged, IS_PAGED, LEN_1, REG_FORMAT, WRITE_ONLY)
#define storeuserall DECLARE_XDPE192C3D_REG("storeuserall", STORE_USER_ALL, NO_PAGE, LEN_0, REG_FORMAT, WRITE_ONLY)
#define capability DECLARE_XDPE192C3D_REG("capability", CAPABILITY_paged, IS_PAGED, LEN_1, REG_FORMAT, READ_WRITE)
#define smbalertmask DECLARE_XDPE192C3D_REG("smbalertmask", SMBALERT_MASK_paged, IS_PAGED, LEN_8, REG_FORMAT, READ_WRITE)
#define voutmode DECLARE_XDPE192C3D_REG("voutmode", VOUT_MODE_paged, IS_PAGED, LEN_1, REG_FORMAT, READ_ONLY)
#define voutcommand DECLARE_XDPE192C3D_REG("voutcommand", VOUT_COMMAND_paged, IS_PAGED, LEN_2, L16_FORMAT, READ_WRITE)
#define voutmax DECLARE_XDPE192C3D_REG("voutmax", VOUT_MAX_paged, IS_PAGED, LEN_2, L16_FORMAT, READ_WRITE)
#define voutmarginhigh DECLARE_XDPE192C3D_REG("voutmarginhigh", VOUT_MARGIN_HIGH_paged, IS_PAGED, LEN_2, L16_FORMAT, READ_WRITE)
#define voutmarginlow DECLARE_XDPE192C3D_REG("voutmarginlow", VOUT_MARGIN_LOW_paged, IS_PAGED, LEN_2, L16_FORMAT, READ_WRITE)
#define vouttransitionrate DECLARE_XDPE192C3D_REG("vouttransitionrate", VOUT_TRANSITION_RATE_paged, IS_PAGED, LEN_2, MS_FORMAT, READ_WRITE)
#define voutdroop DECLARE_XDPE192C3D_REG("voutdroop", VOUT_DROOP_paged, IS_PAGED, LEN_2, OHM_FORMAT, READ_WRITE)
#define voutmin DECLARE_XDPE192C3D_REG("voutmin", VOUT_MIN_paged, IS_PAGED, LEN_2, L5_11_FORMAT, READ_WRITE)
#define maxduty DECLARE_XDPE192C3D_REG("maxduty", MAX_DUTY_paged, IS_PAGED, LEN_2, DUTY_CYCLE_FORMAT, READ_WRITE)
#define frequencyswitch DECLARE_XDPE192C3D_REG("frequencyswitch", FREQUENCY_SWITCH_paged, IS_PAGED, LEN_2, FREQUENCY_FORMAT, READ_WRITE)
#define powermode DECLARE_XDPE192C3D_REG("powermode", POWER_MODE_paged, IS_PAGED, LEN_1, L5_11_FORMAT, READ_WRITE)
#define vinon DECLARE_XDPE192C3D_REG("vinon", VIN_ON_paged, IS_PAGED, LEN_2, L5_11_FORMAT, READ_WRITE)
#define vinoff DECLARE_XDPE192C3D_REG("vinoff", VIN_OFF_paged, IS_PAGED, LEN_2, L5_11_FORMAT, READ_WRITE)
#define interleaved DECLARE_XDPE192C3D_REG("interleaved", INTERLEAVE_paged, IS_PAGED, LEN_2, REG_FORMAT, READ_WRITE)
#define ioutcalgain DECLARE_XDPE192C3D_REG("ioutcalgain", IOUT_CAL_GAIN_paged, IS_PAGED, LEN_2, L5_11_FORMAT, READ_WRITE)
#define ioutcaloffset DECLARE_XDPE192C3D_REG("ioutcaloffset", IOUT_CAL_OFFSET_paged, IS_PAGED, LEN_2, AMP_PORMAT, READ_WRITE)
#define voutovfaultlimit DECLARE_XDPE192C3D_REG("voutovfaultlimit", VOUT_OV_FAULT_LIMIT_paged, IS_PAGED, LEN_2, L16_FORMAT, READ_WRITE)
#define voutovfaultresponse DECLARE_XDPE192C3D_REG("voutovfaultresponse", VOUT_OV_FAULT_RESPONSE_paged, IS_PAGED, LEN_1, REG_FORMAT, READ_WRITE)
#define voutuvfaultlimit DECLARE_XDPE192C3D_REG("voutuvfaultlimit", VOUT_UV_FAULT_LIMIT_paged, IS_PAGED, LEN_2, L16_FORMAT, READ_WRITE)
#define ioutocfaultlimit DECLARE_XDPE192C3D_REG("ioutocfaultlimit", IOUT_OC_FAULT_LIMIT_paged, IS_PAGED, LEN_2, AMP_PORMAT, READ_WRITE)
#define vinovfaultlimit DECLARE_XDPE192C3D_REG("vinovfaultlimit", VIN_OV_FAULT_LIMIT_paged, IS_PAGED, LEN_2, L5_11_FORMAT, READ_WRITE)
#define otfaultlimit DECLARE_XDPE192C3D_REG("otfaultlimit", OT_FAULT_LIMIT_paged, IS_PAGED,LEN_2, TEMPERATURE_FORMAT, READ_WRITE)
#define vinovfaultlimit DECLARE_XDPE192C3D_REG("vinovfaultlimit", VIN_OV_FAULT_LIMIT_paged, IS_PAGED, LEN_2, L5_11_FORMAT, READ_WRITE)
#define iinocfaultlimit DECLARE_XDPE192C3D_REG("iinocfaultlimit", IIN_OC_FAULT_LIMIT_paged, IS_PAGED, LEN_2, AMP_PORMAT, READ_WRITE)
#define powergoodon DECLARE_XDPE192C3D_REG("powergoodon", POWER_GOOD_ON_paged, IS_PAGED, LEN_2, L16_FORMAT, READ_WRITE)
#define powergoodoff DECLARE_XDPE192C3D_REG("powergoodoff", POWER_GOOD_OFF_paged, IS_PAGED, LEN_2, L16_FORMAT, READ_WRITE)
#define tondelay DECLARE_XDPE192C3D_REG("tondelay", TON_DELAY_paged, IS_PAGED, LEN_2, MS_FORMAT, READ_WRITE)
#define tonrise DECLARE_XDPE192C3D_REG("tonrise", TON_RISE_paged, IS_PAGED, LEN_2, MS_FORMAT, READ_WRITE)
#define tonmaxfaultlimit DECLARE_XDPE192C3D_REG("tonmaxfaultlimit", TON_MAX_FAULT_LIMIT_paged, IS_PAGED, LEN_2, MS_FORMAT, READ_WRITE)
#define toffdelay DECLARE_XDPE192C3D_REG("toffdelay", TOFF_DELAY_paged, IS_PAGED, LEN_2, MS_FORMAT, READ_WRITE)
#define tofffall DECLARE_XDPE192C3D_REG("tofffall", TOFF_FALL_paged, IS_PAGED, LEN_2, MS_FORMAT, READ_WRITE)
#define poutopwarnlimit DECLARE_XDPE192C3D_REG("poutopwarnlimit", POUT_OP_WARN_LIMIT_paged, IS_PAGED, LEN_2, POWER_FORMAT, READ_WRITE)
#define pinopwarnlimit DECLARE_XDPE192C3D_REG("pinopwarnlimit", PIN_OP_WARN_LIMIT_paged, IS_PAGED, LEN_2, POWER_FORMAT, READ_WRITE)
#define statusbyte DECLARE_XDPE192C3D_REG("statusbyte", STATUS_BYTE_paged, IS_PAGED, LEN_1, REG_FORMAT, READ_ONLY)
#define statusword DECLARE_XDPE192C3D_REG("statusword", STATUS_WORD_paged, IS_PAGED, LEN_2, REG_FORMAT, READ_ONLY)
#define statusvout DECLARE_XDPE192C3D_REG("statusvout", STATUS_VOUT_paged, IS_PAGED, LEN_1, REG_FORMAT, READ_ONLY)
#define statusiout DECLARE_XDPE192C3D_REG("statusiout", STATUS_IOUT_paged, IS_PAGED, LEN_1, REG_FORMAT, READ_ONLY)
#define statusinput DECLARE_XDPE192C3D_REG("statusinput", STATUS_INPUT_paged, IS_PAGED, LEN_1, REG_FORMAT, READ_ONLY)
#define statustemperature DECLARE_XDPE192C3D_REG("statustemperature", STATUS_TEMPERATURE_paged, IS_PAGED, LEN_1, REG_FORMAT, READ_ONLY)
#define statuscml DECLARE_XDPE192C3D_REG("statuscml", STATUS_CML_paged, IS_PAGED, LEN_1, REG_FORMAT, READ_ONLY)
#define statusmfrfans12 DECLARE_XDPE192C3D_REG("statusmfrfans12", STATUS_MFR_FANS_1_2_paged, NO_PAGE, LEN_1, REG_FORMAT, READ_ONLY)
#define readein DECLARE_XDPE192C3D_REG("readein", READ_EIN_paged, IS_PAGED, LEN_6, AMP_PORMAT, READ_ONLY)
#define readeout DECLARE_XDPE192C3D_REG("readeout", READ_EOUT_paged, IS_PAGED, LEN_6, L5_11_FORMAT, READ_ONLY)
#define readvin DECLARE_XDPE192C3D_REG("readvin", READ_VIN_paged, IS_PAGED, LEN_2, L5_11_FORMAT, READ_ONLY)
#define readiin DECLARE_XDPE192C3D_REG("readiin", READ_IIN_paged, IS_PAGED, LEN_2, AMP_PORMAT, READ_ONLY)
#define readvout DECLARE_XDPE192C3D_REG("readvout", READ_VOUT_paged, IS_PAGED, LEN_2, L16_FORMAT, READ_ONLY)
#define readiout DECLARE_XDPE192C3D_REG("readiout", READ_IOUT_paged, IS_PAGED, LEN_2, AMP_PORMAT, READ_ONLY)
#define readtemperature1 DECLARE_XDPE192C3D_REG("readtemperature1", READ_TEMPERATURE1_paged, IS_PAGED, LEN_2, TEMPERATURE_FORMAT, READ_ONLY)
#define readtemperature2 DECLARE_XDPE192C3D_REG("readtemperature2", READ_TEMPERATURE2_paged, IS_PAGED, LEN_2, TEMPERATURE_FORMAT, READ_ONLY)
#define readdutycycle DECLARE_XDPE192C3D_REG("readdutycycle", READ_DUTY_CYCLE_paged, NO_PAGE, LEN_2, DUTY_CYCLE_FORMAT, READ_ONLY)
#define readpout DECLARE_XDPE192C3D_REG("readpout", READ_POUT_paged, IS_PAGED, LEN_2, POWER_FORMAT, READ_ONLY)
#define readpin DECLARE_XDPE192C3D_REG("readpin", READ_PIN_paged, IS_PAGED, LEN_2, POWER_FORMAT, READ_ONLY)
#define pmbusrevision DECLARE_XDPE192C3D_REG("pmbusrevision", PMBUS_REVISION_paged, NO_PAGE, LEN_1, REG_FORMAT, READ_ONLY)

// custom, read VOUT and IOUT
#define readvoutiout DECLARE_XDPE192C3D_REG("readvoutiout", READ_VOUT_paged, IS_PAGED, LEN_4, L16_FORMAT, READ_ONLY)


// add new instances to table
const XDP_StringReg_t XDP_StrReg[] =
{
	page,
	operation,
	onoffconfig,
	clrfaults,
	writeprotect,
	storeuserall,
	capability,
	smbalertmask,
	voutmode,
	voutcommand,
	voutmax,
	voutmarginhigh,
	voutmarginlow,
	vouttransitionrate,
	voutdroop,
	voutmin,
	maxduty,
	frequencyswitch,
	powermode,
	vinon,
	vinoff,
	interleaved,
	ioutcalgain,
	ioutcaloffset,
	voutovfaultlimit,
	voutovfaultresponse,
	voutuvfaultlimit,
	ioutocfaultlimit,
	vinovfaultlimit,
	otfaultlimit,
	vinovfaultlimit,
	iinocfaultlimit,
	powergoodon,
	powergoodoff,
	tondelay,
	tonrise,
	tonmaxfaultlimit,
	toffdelay,
	tofffall,
	poutopwarnlimit,
	pinopwarnlimit,
	statusbyte,
	statusword,
	statusvout,
	statusiout,
	statusinput,
	statustemperature,
	statuscml,
	statusmfrfans12,
	readein,
	readeout,
	readvin,
	readiin,
	readvout,
	readiout,
	readtemperature1,
	readtemperature2,
	readdutycycle,
	readpout,
	readpin,
	pmbusrevision
};

int XDP_StrReg_size = sizeof(XDP_StrReg); // extern int XDP_StrReg_size


int XDPE192C3D_Write(I2C_GenericDef *i2c)
{
	int status = NO_ERROR;

	status = I2C_Master_Transmit_Generic_Method(i2c);

	return status;
}

int XDPE192C3D_Read(I2C_GenericDef *i2c)
{
	int status = NO_ERROR;

	status = I2C_Master_Receive_Generic_Method(i2c);

	return status;
}

// Mem functions
int XDPE192C3D_Mem_Write(I2C_GenericDef *i2c)
{
	int status = NO_ERROR;

	status = I2C_Mem_Write_Generic_Method(i2c);

	return status;
}

int XDPE192C3D_Mem_Read(I2C_GenericDef *i2c)
{
	int status = NO_ERROR;

	status = I2C_Mem_Read_Generic_Method(i2c);

	return status;
}





/*
 * Description: Get VOUT_MODE. 1 byte. Should be 0x18 = Linear16 exponent -8
 */
int XDPE192C3D_GetMode(I2C_GenericDef *i2c, int *exponent)
{
	int status = NO_ERROR;
	XDP_VOUT_Mode_t mode = {0};
	uint32_t lastI2C_Transfer_type = 0;

	// read register
	i2c->registerAddr[0] = VOUT_MODE_paged;
	i2c->dataSize = 1; // use table look up value
	lastI2C_Transfer_type = i2c->transferType;
	i2c->transferType = I2C_POLLING;
	status = I2C_Mem_Read_Generic_Method(i2c);
	i2c->transferType = lastI2C_Transfer_type;
	if(status != HAL_OK)
	{
		return status;
	}

	// copy data to XDP_VOUT_Mode_t data structure
	mode.data = i2c->dataPtr[0];

	*exponent = (int) mode.Status.exponent;
	if(mode.Status.sign)
	{
		*exponent *= -1;
	}

	status = NO_ERROR;

	return status;
}


