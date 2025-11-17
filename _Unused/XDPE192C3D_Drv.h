/*
 * XDPE192C3D_Drv.h
 *
 *  Created on: Sep 23, 2025
 *      Author: karl.yamashita
 */

#ifndef INC_XDPE192C3D_DRV_H_
#define INC_XDPE192C3D_DRV_H_


enum XDPE192C3D_Commands
{
    PAGE,
    OPERATION_paged,
    ON_OFF_CONFIG_paged,
    CLEAR_FAULTS,
    WRITE_PROTECT_paged  = 0x10,
	STORE_DEFAULT_ALL,
	RESTORE_DEFAULT_ALL,
    STORE_USER_ALL = 0x15,
    RESTORE_USER_ALL,
	STORE_USER_CODE,
	RESTORE_USER_CODE,
    CAPABILITY_paged = 0x19,
    SMBALERT_MASK_paged = 0x1B,
    VOUT_MODE_paged = 0x20,
    VOUT_COMMAND_paged,
	VOUT_TRIM_paged,
	VOUT_CAL_OFFSET_paged,
    VOUT_MAX_paged = 0x24,
    VOUT_MARGIN_HIGH_paged,
    VOUT_MARGIN_LOW_paged,
    VOUT_TRANSITION_RATE_paged,
	VOUT_DROOP_paged,
	VOUT_MIN_paged = 0x2B,
	MFR_OVP_RELATIVE_THRESH,
	MFR_UVP_RELATIVE_THRESH,
	MAX_DUTY_paged = 0x32,
    FREQUENCY_SWITCH_paged = 0x33,
	POWER_MODE_paged,
    VIN_ON_paged = 0x35,
    VIN_OFF_paged,
	INTERLEAVE_paged,
    IOUT_CAL_GAIN_paged = 0x38,
	IOUT_CAL_OFFSET_paged,
    VOUT_OV_FAULT_LIMIT_paged = 0x40,
    VOUT_OV_FAULT_RESPONSE_paged,
    VOUT_OV_WARN_LIMIT_paged,
    VOUT_UV_WARN_LIMIT_paged,
    VOUT_UV_FAULT_LIMIT_paged,
    VOUT_UV_FAULT_RESPONSE_paged,
    IOUT_OC_FAULT_LIMIT_paged,
    IOUT_OC_FAULT_RESPONSE_paged,
    IOUT_OC_WARN_LIMIT_paged = 0x4A,
    OT_FAULT_LIMIT_paged = 0x4F,
    OT_FAULT_RESPONSE_paged,
    OT_WARN_LIMIT_paged,

    VIN_OV_FAULT_LIMIT_paged = 0x55,//
    VIN_OV_FAULT_RESPONSE_paged,
	VIN_OV_WARN_LIMIT_paged,
    VIN_UV_WARN_LIMIT_paged = 0x58,
	IIN_OC_FAULT_LIMIT_paged = 0x5B,
	IIN_OC_FAULT_RESPONSE_paged,
	IIN_OC_WARN_LIMIT_paged,

	POWER_GOOD_ON_paged = 0x5E,
	POWER_GOOD_OFF_paged,

    TON_DELAY_paged = 0x60,
    TON_RISE_paged,
    TON_MAX_FAULT_LIMIT_paged,
    TON_MAX_FAULT_RESPONSE_paged,

    TOFF_DELAY_paged,
    TOFF_FALL_paged,
    TOFF_MAX_WARN_LIMIT_paged,
	POUT_OP_WARN_LIMIT_paged = 0x6A,
	PIN_OP_WARN_LIMIT_paged,

    STATUS_BYTE_paged = 0x78,
    STATUS_WORD_paged,
    STATUS_VOUT_paged,
    STATUS_IOUT_paged,
    STATUS_INPUT_paged,
    STATUS_TEMPERATURE_paged,
    STATUS_CML_paged,
	STATUS_OTHER_paged,
    STATUS_MFR_SPECIFIC_paged = 0x80,
	STATUS_MFR_FANS_1_2_paged,
	READ_EIN_paged = 0x86,
	READ_EOUT_paged,
    READ_VIN_paged = 0x88,
    READ_IIN_paged,
    READ_VOUT_paged = 0x8B,
    READ_IOUT_paged,
    READ_TEMPERATURE1_paged, // External sensor
    READ_TEMPERATURE2_paged, // internal sensor

    READ_DUTY_CYCLE_paged = 0x94,
    READ_POUT_paged = 0x96,
	READ_PIN_paged,
    PMBUS_REVISION_paged = 0x98,

    MFR_ID_paged = 0x99,
    MFR_MODEL_paged,
	MFR_REVISION_paged = 0x9b,
	MFR_DATE = 0x9D,

	MFR_PIN_MAX_paged = 0xA3,
	MFR_VOUT_MIN_paged = 0xA4,
    MFR_VOUT_MAX_paged = 0xA5,

	MFR_IOUT_MAX_paged,
	MFR_POUT_MAX_paged,
	MFR_TAMBIENT_MAX_paged,
	IC_DEVICE_ID_paged = 0xAD,
	IC_DEVICE_REV_paged,
	MFR_IOUT_MIN_paged,

    USER_DATA_00_paged = 0xB0,
    USER_DATA_01_paged,

	FW_CONFIG_REGULATION_paged = 0xC5,
	FW_CONFIG_TELEMETRY_paged,
	MFR_COMMON_FAULT_STATUS1_paged,
	FW_CONFIG_FAULTS_paged,
	MFR_COMMON_FAULT_STATUS2_paged,
	MFR_SETUP_PASSWORD_paged = 0xCA,
	MFR_DISABLE_SECURITY_ONCE_paged,
	MFR_SELECT_TEMPERATURE_SENSOR_paged,
	MFR_GAMER_paged,
	MFR_AHB_ADDRESS,
	MFR_DEBUG_BUFF_paged,

	MFR_SECURITY_BIT_MASK_LOW_paged = 0xD2,
	MFR_SECURITY_BIT_MASK_HIGH_paged,
	MFR_REG_WRITE = 0xDE,
	MFR_REG_READ,

	MFR_PID_paged = 0xF2,
	MFR_COUT_CONFIG_paged = 0xF4, // reserved
	MFR_READ_COUT_paged, // reserved
	MFR_FIRMWARE_COMMAND_DATA = 0xFD,
	MFR_FIRMWARE_COMMAND
};

enum XDPE192C3D_Page
{
    NO_PAGE,
    IS_PAGED
};

enum Length
{
    LEN_0,
    LEN_1,
    LEN_2,
    LEN_3,
	LEN_4,
	LEN_6 = 6,
    LEN_8 = 8,
    LEN_9,
	LEN_14 = 14,
	LEN_16 = 16,
	LEN_31 = 31
};

enum NVM
{
    NO_NVM,
    USER_NVM,
    IS_NVM
};

enum Format
{
    NO_FORMAT,
    REG_FORMAT,
    L5_11_FORMAT,
    L16_FORMAT,
	MS_FORMAT, // millisecond
	TEMPERATURE_FORMAT,
	AMP_PORMAT,
	POWER_FORMAT,
	DUTY_CYCLE_FORMAT,
	FREQUENCY_FORMAT,
	OHM_FORMAT,
    ASCII_FORMAT
};

enum ReadWriteType
{
    READ_WRITE,
    READ_ONLY,
    WRITE_ONLY
};

enum Page
{
	PAGE_0,
	PAGE_1
};

enum PEC_Format
{
	NO_PEC,
	PEC
};


typedef union
{
	uint8_t data;
	struct
	{
		uint8_t exponent:4;
		uint8_t sign:1;
		uint8_t mode:3;
	}Status;
}XDP_VOUT_Mode_t;

/*
typedef struct
{
    uint8_t regAddress;
    uint8_t isPaged;
    uint8_t dataLen;
    uint8_t nvm;
    uint8_t fmt; // data format. Reg, L16, L11, CF, ASCII
    uint8_t readWriteType;
}XDP_RegLookUp_t;
*/

// Description: Register, Page, Data Length, isNVM, Format ( none, L5-11, L16, Register), Read type
#define DECLARE_XDPE192C3D_REG(name, reg, page, datalen, format, rwtype) { name, reg, page, datalen, format, rwtype }

typedef struct
{
	char str[20];
	uint8_t reg;
	uint8_t page;
	uint8_t datalen;
	uint8_t format;
	uint8_t rwType;
}XDP_StringReg_t;


int XDPE192C3D_Write(I2C_GenericDef *i2c);
int XDPE192C3D_Read(I2C_GenericDef *i2c);

int XDPE192C3D_Mem_Write(I2C_GenericDef *i2c);
int XDPE192C3D_Mem_Read(I2C_GenericDef *i2c);

int XDPE192C3D_GetMode(I2C_GenericDef *i2c, int *exponent);

#endif /* INC_XDPE192C3D_DRV_H_ */
