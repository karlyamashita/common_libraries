/*
 * TMP102_Handler.h
 *
 *  Created on: Apr 24, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_TMP102_HANDLER_H_
#define INC_TMP102_HANDLER_H_

// HAL driver requires the slave address be shifted to the left by 1.
#define TMP102_SLAVE_ADDRESS_00 (0b1001000 << 1)// 0x48
#define TMP102_SLAVE_ADDRESS_01 (0b1001010 << 1)// 0x4A


#define TMP102_TEMPERATURE_REGISTER 0b00000000 // the temperature, 12 bit value
#define TMP102_CONFIGURATION_REGISTER 0b00000001 // 8 bit value
#define TMP102_TEMP_LOW_REGISTER 0b00000010 // low temperature, 12 bit temperature value. 4 LSB are zero.
#define TMP102_TEMP_HIGH_REGISTER 0b00000011 // high temperature, 12 bit temperature value. 4 LSB are zero

typedef union {
    struct{
        uint8_t byte0;
        uint8_t byte1;
    }Byte;
    struct{
        unsigned SD :1;
        unsigned TM :1;
        unsigned POL :1;
        unsigned F0 :1;
        unsigned F1 :1;
        unsigned R0 :1;
        unsigned R1 :1;
        unsigned OS_ALERT :1; // one shot
        unsigned :4;
        unsigned EM:1;
        unsigned AL:1;
        unsigned CR0:1;
        unsigned CR1:1;
    }Status;
}TMP102_Configuration;

typedef union {
    struct{
        uint8_t byte0;
    }Byte;
    struct{
        uint8_t registerPtr:2;
        uint8_t :6;
    }Status;
}TMP102_RegisterPtr_t;

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
}TMP102_TemperatureRegister; // temperature and high/low limit register

typedef struct
{
    TMP102_RegisterPtr_t ptr;
    TMP102_TemperatureRegister reg;
}TMP102_TemperatureSet;

#define TMP102_TemperatureGet TMP102_TemperatureSet


int TMP102_Init(I2C_DataStruct *msg);

int TMP102_GetRegisterHandle(char *msg, char *retStr);
int TMP102_SetRegisterHandle(char *msg);

int TMP102_SetThermostatMode(char *msg);

void TMP101_GetTemperatureString(uint8_t *tmp101Value, char *retStr);
void TMP102_SwapBytes(uint8_t *data);

#endif /* INC_TMP102_HANDLER_H_ */
