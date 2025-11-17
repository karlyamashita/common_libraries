/*
 * I2C_Bus_Reset.h
 *
 *  Created on: Oct 20, 2025
 *      Author: karl.yamashita
 */

#ifndef INC_I2C_BUS_RESET_H_
#define INC_I2C_BUS_RESET_H_



#define CMD_I2C_BUS_RESET DECLARE_CMD("i2c bus reset", 0, 0, 0, I2C_Handler, "Resets I2C bus", "<W>i2c bus reset")


int I2C_Handler(char *msg, char *retStr);

int I2C_Set(char *msg, char *retStr);

#endif /* INC_I2C_BUS_RESET_H_ */
