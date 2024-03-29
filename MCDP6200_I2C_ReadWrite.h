/* 
 * File:   MCDP6200.h
 * Author: karl.yamashita
 *
 * Created on January 14, 2021, 1:12 PM
 */

#ifndef MCDP6200_H
#define	MCDP6200_H

#ifdef	__cplusplus
extern "C" {
#endif


#define MCDP6200_I2C_PORT I2C_PORT_1
#define REG_SIZE 2 // register is 2 bytes

enum{
	READ,
	WRITE
};

// end structures

typedef enum {
	I2C_NOERR, // The message was sent.
	I2C_FAIL, // Message was NOT sent, bus failure
	I2C_BUSY  // Message was NOT sent, bus was busy.
	           // If you are interested in the failure reason,
	           // Sit on the event call-backs.
} i2c_error_t;

// prototypes

int MCDP6200_Write_Register(uint8_t devAddr, uint32_t _register, uint8_t *data);
int MCDP6200_Read_Register_Data(uint8_t devAddr, uint32_t _register, uint8_t *data);

	
#ifdef	__cplusplus
}
#endif

#endif	/* MCDP6200_H */

