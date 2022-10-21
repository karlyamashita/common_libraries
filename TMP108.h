/*
 * TMP108.h
 *
 *  Created on: September 27, 2022
 *      Author: karl.yamashita
 */

#ifndef TMP108_H_
#define TMP108_H_

#include "main.h"


//#define TMP108_ADDRESS 0b1001000 // 0x48, 0x90
#define TMP108_SLAVE_ADDRESS_00 0b1001000 // 0x48
#ifdef TRON
#define TMP108_SLAVE_ADDRESS_01 0b1001010 //0x4A, 0x94
#else
//#define TMP108_SLAVE_ADDRESS_01 0b1001001 //0x49 // avenger
#define TMP108_SLAVE_ADDRESS_01 0b1001010 // temporary
#endif

#ifndef TMP108_I2C_BASE_DEFAULT
#warning "TMP108 I2C base not defined"
#define TMP108_I2C_BASE_DEFAULT 1
#endif

#define TMP108_TEMPERATURE_REGISTER 0b00000000 // the temperature, 12 bit value
#define TMP108_CONFIGURATION_REGISTER 0b00000001 // 8 bit value
#define TMP108_TEMP_LOW_REGISTER 0b00000010 // low temperature, 12 bit temperature value. 4 LSB are zero
#define TMP108_TEMP_HIGH_REGISTER 0b00000011 // high temperature, 12 bit temperature value. 4 LSB are zero

#define I2C_TIMEOUT 125

typedef union {
    struct{
        uint8_t byte0;
        uint8_t byte1;
    }Byte;
    struct{
        unsigned M0_1 :2; // Functional modes. 0 = Shutdown, 1 = One-shot, 2 = Continuous conversion
        unsigned TM :1; // Thermostat mode. 0 = comparator mode, 1 = interrupt mode.
        unsigned FL :1; // flag bit, low temperature. Read configuration register to clear
        unsigned FH :1; // flag bit, high temperature. Read configuration register to clear
        unsigned CR0_1 :2; // Conversion rate. 0 = 0.25 Hz, 1 = 1 Hz, 2 = 4 Hz, 3 = 16 Hz
        unsigned ID :1; // no information available
        unsigned :4;
        unsigned HYS0_1:2; // Hysteresis control. 0 = 0 Celsius, 1 = 1 Celsius (Default), 2 = 2 Celsius, 3 = 4 Celsius
        unsigned :1;
        unsigned POL:1; // ALERT pin active state, 0 = active low (default), 1 = active high
    }Status;
}TMP108_Configuration;

typedef union {
    struct{
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte2;
    }Byte;
    struct{
        uint8_t pointerRegister;
        uint8_t tempLow;
        uint8_t tempHigh;
    }Status;
}TMP108_Pointer_Register;

int TMP108_Init(void);
int TMP108_SetLimitHigh_00(uint16_t temp);
int TMP108_SetLimitLow_00(uint16_t temp);
int TMP108_SetLimitHigh_01(uint16_t temp);
int TMP108_SetLimitLow_01(uint16_t temp);

int TMP108_ReadTempReg(uint8_t sensorSlaveAddress, uint8_t * value);
int TMP108_GetTemp_0(char *retStr);
int TMP108_GetTemp_1(char *retStr);


void TMP108_GetStringForTemp(uint16_t TMP108Value, char *str);
int TMP108_SetHighLimits(char *msg);
int TMP108_SetLowLimits(char *msg);
int TMP108_GetLimitHigh_00(char *retStr);
int TMP108_GetLimitHigh_01(char *retStr);
int TMP108_GetLimitLow_00(char *retStr);
int TMP108_GetLimitLow_01(char *retStr);
void TMP108_SwapBytes(uint8_t *data);

int TMP108_SetConfig(char *msg);
int TMP108_GetConfig(char *msg, char *retStr);
#endif /* TMP108_H_ */
