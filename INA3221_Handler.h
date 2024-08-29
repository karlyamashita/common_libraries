/*
 * INA3221_Handler.h
 *
 *  Created on: Jun 17, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_INA3221_HANDLER_H_
#define INC_INA3221_HANDLER_H_


#define USING_CALLBACK


#define INA3221_SLAVE_ADDRESS_0 0x40


int INA3221_SetConfigReg(I2C_GenericDef *i2c, char *msg);
int INA3221_GetConfigReg(I2C_GenericDef *i2c, char *retStr);
int INA3221_GetBusShunt(I2C_GenericDef *i2c, char *msg, char *retStr);

void INA3221_ConfigCallback(I2C_GenericDef *i2c);
void INA3221_VoltageCallback(I2C_GenericDef *i2c);

#endif /* INC_INA3221_HANDLER_H_ */
