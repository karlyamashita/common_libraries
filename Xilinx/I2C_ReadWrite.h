/*
 * I2C_ReadWrite.h
 *
 *  Created on: Jul 1, 2021
 *      Author: karl.yamashita
 */

#ifndef SRC_I2C_READWRITE_H_
#define SRC_I2C_READWRITE_H_

#define I2C_TIMEOUT 100

#define I2C_PORT_0 0
#define I2C_PORT_1 1
#define I2C_PORT_2 2

int I2C_Send_Data(uint8_t i2c_port, uint8_t devAddr, uint8_t *TxMsgPtr, int ByteCount, uint8_t option);
int I2C_Get_Data(uint8_t i2c_port, uint8_t devAddr, uint8_t *RxMsgPtr, int ByteCount, uint8_t option);
int I2C_Get_Register_Data(uint8_t i2c_port, uint8_t devAddr, uint8_t regAddr, uint8_t *RxMsgPtr, int ByteCount);

/*
 * These below don't work. XIic_MasterSend & XIic_MasterRecv don't interrupt when done. SCL stays low. Seems to be an issue people are running into.
 *
 *
 */
int I2C_Write_Data(uint8_t i2c_port, uint8_t devAddr, u8 *TxMsgPtr, int ByteCount);
int I2C_Read_Data(uint8_t i2c_port, uint8_t devAddr, uint8_t regAddr, uint8_t *RxMsgPtr, int ByteCount);

#endif /* SRC_I2C_READWRITE_H_ */
