/* 
 * File:   I2C_Generic_Handler.h
 * Author: karl.yamashita
 *
 * Created on August 12, 2022, 12:30 PM
 */

#ifndef I2C_GENERIC_HANDLER_H
#define	I2C_GENERIC_HANDLER_H

#ifdef	__cplusplus
extern "C" {
#endif


#define I2C_PORT_0 0
#define I2C_PORT_1 1
#define I2C_PORT_2 2

    
int I2C_Master_Receive_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *data, uint8_t readSize);
int I2C_Master_Transmit_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *data, uint8_t writeSize);
int I2C_Master_Read_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *regAddress, uint32_t regSize, uint8_t *data_OUT, uint8_t readSize);
int I2C_Master_Write_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *regAddress, uint32_t regSize, uint8_t *data_OUT, uint8_t writeSize);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_GENERIC_HANDLER_H */

