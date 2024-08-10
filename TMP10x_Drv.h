/*
 * TMP10x_Drv.h
 *
 *  Created on: Jun 17, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_TMP10x_DRV_H_
#define INC_TMP10x_DRV_H_


// currently have only the TMP101, TMP102 and TMP108 configuration structure
//#define USE_TMP101
#define USE_TMP102
//#define USE_TMP108


#define TMP10x_TEMPERATURE_REGISTER 0x0 // the temperature, 12 bit value
#define TMP10x_CONFIGURATION_REGISTER 0x01 // 8 bit value
#define TMP10x_TEMP_LOW_REGISTER 0x02 // low temperature, 12 bit temperature value. 4 LSB are zero.
#define TMP10x_TEMP_HIGH_REGISTER 0x03 // high temperature, 12 bit temperature value. 4 LSB are zero

#define TMP10x_RESOLUTION .0625


typedef union {
    struct{
        uint8_t byte0;
    }Byte;
    struct{
        uint8_t registerPtr:2;
        uint8_t :6;
    }Status;
}TMP10x_RegisterPtrDef;

typedef union
{
	struct
	{
		uint8_t data[2];
	}Bytes;
	struct
	{
		uint16_t temp;
	}u16;
}TMP10x_TemperatureRegDef; // temperature and high/low limit register

#ifdef USE_TMP101
typedef union {
    struct{
        uint8_t byte0;
        uint8_t byte1;
    }Byte;
    struct{
        unsigned SD :1;
        unsigned TM :1;
        unsigned POL :1;
        unsigned F0_1 :2;
        unsigned R0_1 :2;
        unsigned OS_ALERT :1; // one shot
    }Status;
}TMP10x_ConfigDef;
#endif

#ifdef USE_TMP102
typedef union {
    struct{
        uint8_t byte0;
        uint8_t byte1;
    }Byte;
    struct{
        unsigned SD :1;
        unsigned TM :1;
        unsigned POL :1;
        unsigned F0_1 :2;
        unsigned R0_1 :2;
        unsigned OS_ALERT :1; // one shot
        unsigned :4;
        unsigned EM:1;
        unsigned AL:1;
        unsigned CR0_1:2;
    }Status;
}TMP10x_ConfigDef;
#endif

#ifdef USE_TMP108
enum TMP108_Hysteresis
{
    TMP108_HYS_0_C,
    TMP108_HYS_1_C, // default
    TMP108_HYS_2_C,
    TMP108_HYS_4_C
};

enum TMP108_ConverionRate
{
	TMP108_ConvRate_0_25,
	TMP108_ConvRate_1,
	TMP108_ConvRate_4,
	TMP108_ConvRate_16
};

typedef union {
    struct{
        uint8_t byte0;
        uint8_t byte1;
    }Byte;
    struct{
        unsigned :4;
        unsigned HYS1_0:2;
        unsigned :1;
        unsigned POL :1;
        unsigned M1_0 :2;
        unsigned TM:1;
        unsigned FH_L:2;
        unsigned CR0_1:2;
        unsigned ID:1;
    }Status;
}TMP10x_ConfigDef;
#endif


typedef struct __TMP10x_HandleTypeDef
{
    TMP10x_RegisterPtrDef regPtr;
    TMP10x_TemperatureRegDef tempReg;
    TMP10x_ConfigDef config;
    char *commandCopy;
}TMP10x_HandleTypeDef;


#endif /* INC_TMP10x_DRV_H_ */
