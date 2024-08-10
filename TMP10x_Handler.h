/*
 * TMP10x_Handler.h
 *
 *  Created on: Apr 24, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_TMP10X_HANDLER_H_
#define INC_TMP10X_HANDLER_H_

// HAL driver requires the slave address be shifted to the left by 1.
#define TMP10x_SLAVE_ADDRESS_00 0b1001000 // 0x48
#define TMP10x_SLAVE_ADDRESS_01 0b1001010 // 0x4A


int TMP10x_Init(I2C_GenericDef *i2c);

int TMP10x_GetRegisterHandle(I2C_GenericDef *i2c, char *msg, char *retStr);
int TMP10x_SetRegisterHandle(I2C_GenericDef *i2c, char *msg);

int TMP10x_SetThermostatMode(I2C_GenericDef *i2c, char *msg);

void TMP101_GetTemperatureString(uint8_t *tmp101Value, char *retStr);
void TMP10x_SwapBytes(uint8_t *data);

// USER can create callbacks prototypes here
void TMP10x_TemperatureCallback(I2C_GenericDef *i2c);
void TMP10x_ConfigCallback(I2C_GenericDef *i2c);
void TMP10x_LowCallback(I2C_GenericDef *i2c);
void TMP10x_HighCallback(I2C_GenericDef *i2c);

#endif /* INC_TMP10X_HANDLER_H_ */
