/*
 * TM101.h
 *
 *  Created on: Mar 3, 2021
 *      Author: karl.yamashita
 */

#ifndef TMP101_H_
#define TMP101_H_

#include "main.h"


//#define TMP101_ADDRESS 0b1001000 // 0x48, 0x90
#define TMP101_SLAVE_ADDRESS_00 0b1001000 // 0x48
#define TMP101_SLAVE_ADDRESS_01 0b1001010 //0x4A, 0x94

#define TMP101_TEMPERATURE_REGISTER 0b00000000 // the temperature, 12 bit value
#define TMP101_CONFIGURATION_REGISTER 0b00000001 // 8 bit value
#define TMP101_TEMP_LOW_REGISTER 0b00000010 // low temperature, 12 bit temperature value. 4 LSB are zero
#define TMP101_TEMP_HIGH_REGISTER 0b00000011 // high temperature, 12 bit temperature value. 4 LSB are zero

#define I2C_TIMEOUT 125

typedef union {
    struct{
        uint8_t byte0;
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
    }Status;
}TMP101_Configuration;

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
}TMP101_Pointer_Register;

HAL_StatusTypeDef InitTMP101(void);
HAL_StatusTypeDef SetTMP101_00_High(uint16_t temp);
HAL_StatusTypeDef SetTMP101_01_High(uint16_t temp);
HAL_StatusTypeDef SetTMP101_00_Low(uint16_t temp);
HAL_StatusTypeDef SetTMP101_01_Low(uint16_t temp);
HAL_StatusTypeDef GetTMP101Limit00_High(void);
HAL_StatusTypeDef GetTMP101Limit01_High(void);
HAL_StatusTypeDef GetTMP101Limit00_Low(void);
HAL_StatusTypeDef GetTMP101Limit01_Low(void);
HAL_StatusTypeDef TMP101_ReadTempReg(uint8_t sensorSlaveAddress, uint8_t * value);

HAL_StatusTypeDef TMP101_WriteRegister(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *data, uint8_t size);
HAL_StatusTypeDef TMP101_ReadRegister(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *data, uint8_t size);



#endif /* TMP101_H_ */
