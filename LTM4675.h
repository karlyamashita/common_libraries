/*
 * LTM4675.h
 *
 *  Created on: Nov 12, 2021
 *      Author: karl.yamashita
 */

#ifndef LTM4675_H_
#define LTM4675_H_

// user changeable values
// Need to define which I2C peripheral is being used.
#define LTM4681_I2C_BASE 2 // I2C2 for Microchip
// end user changeable values

//#define LTM46xx_SLAVE_ADDRESS_A 0x40 // see page 22 and 26 of data sheet
//#define LTM46xx_SLAVE_ADDRESS_B 0x41 // see page 22 and 26 of data sheet
// MFR_RAIL_ADDRESS
//#define LTM46xx_RAIL_SLAVE_ADDRESS 0x30 // see page 22 and 26 of data sheet

// Global address
//#define LTM46xx_SLAVE_ADDRESS_A 0x5A // see page 41 of data sheet
//#define LTM46xx_SLAVE_ADDRESS_B 0x5B // see page 41 of data sheet

typedef struct
{
	uint8_t slave_1; // 1 LTM46xx connected to STM32
	uint8_t slave_2;
	uint8_t slave_3; // for 2 LTM46xx connected to STM32
	uint8_t slave_4;
	uint8_t slave_5; // for 3 LTM46xx connected to STM32
	uint8_t slave_6;
	uint8_t rail;
	bool useMfrRailAddressFlag;
	uint8_t ltmCount;
}LTM_SlaveRail_t;


// commands register address
enum LTM46xx_Commands{
    PAGE,
    OPERATION_paged, // paged
    ON_OFF_CONFIG_paged, // paged
    CLEAR_FAULTS,
    PAGE_PLUS_WRITE = 0x05,
    PAGE_PLUS_READ,
    WRITE_PROTECT  = 0x10,
    STORE_USER_ALL = 0x15,
    RESTORE_USER_ALL,
    CAPABILITY = 0x19,
    SMBALERT_MASK_paged = 0x1B, // paged
    VOUT_MODE_paged = 0x20, // paged
    VOUT_COMMAND_paged, // paged
    VOUT_MAX_paged = 0x24, // paged
    VOUT_MARGIN_HIGH_paged, // paged
    VOUT_MARGIN_LOW_paged, // paged
    VOUT_TRANSITION_RATE_paged, // paged
    FREQUENCY_SWITCH = 0x33,
    VIN_ON = 0x35,
    VIN_OFF,
    IOUT_CAL_GAIN_paged = 0x3B, // paged
    VOUT_OV_FAULT_LIMIT_paged = 0x40, // paged
    VOUT_OV_FAULT_RESPONSE,
    VOUT_OV_WARN_LIMIT_paged, // paged
    VOUT_UV_WARN_LIMIT_paged, // paged
    VOUT_UV_FAULT_LIMIT_paged, // paged
    VOUT_UV_FAULT_RESPONSE,
    IOUT_OC_FAULT_LIMIT_paged, // paged
    IOUT_OC_FAULT_RESPONSE_paged, // paged
    IOUT_OC_WARN_LIMIT_paged = 0x4A, // paged
    OT_FAULT_LIMIT_paged = 0x4F, // paged
    OT_FAULT_RESPONSE_paged, // paged
    OT_WARN_LIMIT_paged, // paged
    UT_FAULT_LIMIT_paged = 0x53, // paged
    UT_FAULT_RESPONSE_paged, // paged
    VIN_OV_FAULT_LIMIT,
    VIN_OV_FAULT_RESPONSE,
    VIN_UV_WARN_LIMIT = 0x58,
    IIN_OC_WARN_LIMIT = 0x5D,
    TON_DELAY_paged = 0x60, // paged
    TON_RISE_paged, // paged
    TON_MAX_FAULT_LIMIT_paged, // paged
    TON_MAX_FAULT_RESPONSE,
    TOFF_DELAY_paged, // paged
    TOFF_FALL_paged, // paged
    TOFF_MAX_WARN_LIMIT_paged, // paged
    STATUS_BYTE_paged = 0x78, // paged
    STATUS_WORD_paged, // paged
    STATUS_VOUT_paged, // paged
    STATUS_IOUT_paged, // paged
    STATUS_INPUT,
    STATUS_TEMPERATURE_paged, // paged
    STATUS_CML,
    STATUS_MFR_SPECIFIC_paged = 0x80, // paged
    READ_VIN = 0x88,
    READ_IIN,
    READ_VOUT_paged = 0x8B, // paged
    READ_IOUT_paged, // paged
    READ_TEMPERATURE1_paged, // paged. External sensor
    READ_TEMPERATURE2, // internal sensor
    READ_DUTY_CYCLE_paged = 0x94, // paged
    READ_FREQUENCY, // not in data sheet but is in LTPowerPlay software
    READ_POUT_paged = 0x96, // paged
    PMBUS_REVISION = 0x98,
    MFR_ID = 0x99,
    MFR_MODEL,
    MFR_SERIAL = 0x9E,
    MFR_VOUT_MAX_paged = 0xA5, // paged
    USER_DATA_00 = 0xB0,
    USER_DATA_01_paged, // paged
    USER_DATA_02,
    USER_DATA_03_paged, // paged
    USER_DATA_04,
    MFR_INFO = 0xB6,
    MFR_EE_UNLOCK = 0xBD,
    MFR_EE_ERASE,
    MFR_EE_DATA,
    MFR_CHAN_CONFIG_paged = 0xD0, // paged
    MFR_CONFIG_ALL,
    MFR_GPIO_PROPAGATE_paged, // paged
    MFR_PWM_COMP_paged, // paged 
    MFR_PWM_MODE_paged = 0xD4, // paged
    MFR_GPIO_RESPONSE_paged, // paged
    MFR_OT_FAULT_RESPONSE,
    MFR_IOUT_PEAK_paged, // paged
    MFR_ADC_CONTROL,
    //MFR_IOUT_CAL_GAIN 0xDA = 0xDA, // 4676a
    MFR_IOUT_CAL_GAIN_paged = 0xDA, // 4681
    MFR_RETRY_DELAY_paged, // paged
    MFR_RESTART_DELAY_paged, // paged
    MFR_VOUT_PEAK_paged, // paged
    MFR_VIN_PEAK,
    MFR_TEMPERATURE_1_PEAK_paged, // paged
    MFR_CLEAR_PEAKS = 0xE3,
    MFR_PADS = 0xE5,
    MFR_ADDRESS,
    MFR_SPECIAL_ID,
    MFR_IIN_CAL_GAIN,
    MFR_IIN_OFFSET_paged = 0xE9, // paged
    MFR_FAULT_LOG_STORE,
    MFR_FAULT_LOG_CLEAR = 0xEC,
    MFR_READ_IIN_paged, // paged
    MFR_FAULT_LOG,
    MFR_COMMON,
    MFR_COMPARE_USER_ALL = 0xF0,
    MFR_TEMPERATURE_2_PEAK = 0xF4,
    MFR_PWM_CONFIG,
    MFR_IOUT_CAL_GAIN_TC_paged, // paged
    MFR_ICHIP_CAL_GAIN_paged,// paged 
    MFR_TEMP_1_GAIN_paged = 0xF8, // paged
    MFR_TEMP1_OFFSET_paged, // paged
    MFR_RAIL_ADDRESS_paged, // paged
    MFR_RESET = 0xFD
};

enum LTM46xx_Page{
    NO_PAGE,
    IS_PAGED
};

enum Length{
    LEN_0,
    LEN_1,
    LEN_2,
    LEN_3,
    LEN_8,
    LEN_9,
    LEN_147
};

enum NVM{
    NO_NVM,
    USER_NVM,
    IS_NVM
};

enum Format{
    NO_FORMAT,
    REG_FORMAT,
    L5_11_FORMAT,
    L16_FORMAT,
    ASCII_FORMAT
} ;

enum ReadWriteType{
    READ_WRITE,
    READ_ONLY,
    WRITE_ONLY
} ;


typedef struct{
    uint8_t regAddress;
    uint8_t isPaged;
    uint8_t dataLen;
    uint8_t nvm;
    uint8_t fmt; // data format. Reg, L16, L11, CF, ASCII
    uint8_t readWriteType;
} LTM46xx_RegLookUpType;


/*
 * Description: Use for parsing message to get Register address, data, paging and data length
 *
 *
 */
typedef union{
    struct{
        uint8_t data[3];
    }Byte;
    struct{
        unsigned page:2;
        unsigned :6;
        uint8_t regData[2];
    }Status;
}LTM46xx_DirectRegisterInfoType;

// default 0x1f
typedef union{
    struct
    {
        uint8_t data;
    } Byte;
    struct
    {
        unsigned voutDisable :1;
        unsigned gpioAlert :1;
        unsigned shareClkControl :1;
        unsigned shortCycle :1;
        unsigned runDisable :1;
        unsigned :3;
    } Status;
}LTM46xx_MfrChanConfig;

// default 0x09
typedef union{
    struct
    {
        uint8_t data;
    }Byte;
    struct
    {
        unsigned runPinClearFaults :1;
        unsigned enablePMBusClkStretching :1;
        unsigned validPEC :1;
        unsigned enable255msTimeout :1;
        unsigned disableSyncOut :1;
        unsigned disableCmlFault :1;
        unsigned ignoreResistorConfigPins :1;
        unsigned enableFaultLogging :1;
    } Status;
}LTM46xx_MfrConfigAll;

// ON/OFF/MARGIN
enum OnOffMargin{
    ON_OFF_CONFIG_OPERATION_Immediate = 0x1F,
    ON_OFF_CONFIG_OPERATION_Toff = 0x1E,
    ON_OFF_CONFIG_RUN_Immediate = 0x17,
    ON_OFF_CONFIG_RUN_Toff = 0x16
};

enum OperationCommand{
    TURN_OFF_IMMEDIATELY,
    TURN_ON = 0x80,
    //MARGIN_LOW = 0x98,
    //MARGIN_HIGH = 0xA8,
    SEQUENCE_OFF = 0x40
};

// default 0xC1
typedef union{
    struct
    {
        uint8_t data;
    } Byte;
    struct
    {
        unsigned enableContinuousMode :1;
        unsigned voltageRange :1;
        unsigned :2;
        unsigned tempSensedExternal :1;
        unsigned :1;
        unsigned enableServoMode:1;
        unsigned rangeOfCurrentLimit:1;
    } Status;
}LTM46xx_MFR_PWM_mode;

// default 0x10
typedef union{
    struct
    {
        uint8_t data;
    } Byte;
    struct
    {
        unsigned degreesCH0Ch1 :3;
        unsigned :1;
        unsigned enableShareClk:1;
        unsigned :2;
        unsigned eaConnection:1;
    } Status;
}LTM46xx_MfrPWM_Config;

typedef union{
    struct
    {
        uint8_t data;
    } Byte;
    struct
    {
        unsigned delayTime :3;
        unsigned retrySetting:3;
        unsigned response:2;
    } Status;
}LTM46xx_IOUT_OC_FAULT_RESPONSE;

typedef union {
    struct
    {
        uint8_t data;
    } Byte;
    struct
    {
        unsigned wpPinStatus :1;
        unsigned shareClkTimeout:1;
        unsigned :1;
        unsigned nvmInitialized:1;
        unsigned outputNotInTransistion:1;
        unsigned calculationsNotPending:1;
        unsigned moduleNotBusy:1;
        unsigned moduleNotDrivingAlertLow:1;
    } Status;
}LTM46xx_MfrCommon;

enum NumOfBytes
{
    NUM_OF_BYTES_1 = 1,
    NUM_OF_BYTES_2,
    NUM_OF_BYTES_3
};


enum DataType
{
    L16_L5_CONVERTED,
    L16_L5_NOT_CONVERTED
};



// prototypes
int LTM46xx_SetSlaveAddress(char *page, char *slaveAddr);
int LTM46xx_GetSlaveAddress(char *page, char *slaveAddr);

int LTM46xx_GetMFR_COMMON(I2C_GenericDef *i2c, uint8_t page);

int LTM46xx_GetPwrMod(char *msg, char *retStr);
int LTM46xx_SetPwrMod(char *msg);

int LTM46xx_SetRegisterData(char *msg, uint8_t dataType);
int LTM46xx_GetRegisterData(char *msg, char *dataOut);

int LTM46xx_RegisterLookp(uint8_t regAddr, LTM46xx_RegLookUpType *regLookup);

int LTM46xx_SetEnableRailAddress(int mode);
int LTM46xx_GetMFR_RailAddressFlag(void);
int LTM46xx_CheckSlaveAddressSet(void);


#endif /* LTM4675_H_ */
