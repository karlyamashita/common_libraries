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

#ifdef _XC_H
#define    SLAVE_I2C_GENERIC_SLAVE_TIMEOUT 100
#define SLAVE_I2C_GENERIC_RETRY_MAX 3
#endif
    

#define I2C_BAUD_100_KHZ 0x1C0 // 100 kHz
#define I2C_BAUD_400_KHZ 0x6F // 400 kHz
    
    
int I2C_Master_Receive_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *data, uint8_t readSize);
int I2C_Master_Transmit_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *data, uint8_t writeSize);
int I2C_Master_Read_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *regAddress, uint8_t *data_OUT, uint8_t readSize);
int I2C_Master_Write_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t *regAddress, uint8_t *data_IN, uint8_t writeSize);
int I2C_Master_Page_Plus_Read_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t page, uint8_t *regAddress, uint8_t *data_OUT, uint8_t readSize);
int I2C_Master_Page_Plus_Write_Register_Generic(uint32_t i2c_base, uint8_t slaveAddress, uint8_t page, uint8_t *regAddress, uint8_t *data_OUT, uint8_t readSize);


int I2C_ChangeBaudRate(uint32_t i2c_base, uint16_t baudRate);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_GENERIC_HANDLER_H */

