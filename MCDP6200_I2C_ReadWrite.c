/*
 * Description: Kenetic MCDP6200 USB Type-C Switching Retimer
 * 
 * The MCDP6200 operating mode can be configured through the DIS_N and P_POL pins. It can be optionally
configured through TWI by enabling the feature through TWI. These interfaces can be controlled from an
external Power Delivery (PD) controller or microcontroller to set the plug orientation and the pin mapping of
the USB Type-C.
 * 
 * Note: TWI refers to the I2C control
 * 		#include "I2C_ReadWrite.h"
 * 
 */

#include "main.h"
#include "MCDP6200_I2C_ReadWrite.h"

#define MCDP6200_I2C_ReadWrite_PrintLine



/*
 * Description: Combines register address and data into an array then sends data to MCDP_6200
 *
 *
 */
int MCDP6200_I2C_Write_Register(uint8_t address, uint16_t _register, uint8_t *data)
{
	i2c_error_t errorStatus;
    uint8_t regData[6];
    regData[0] = (uint8_t) _register; // lower address
    regData[1] = (uint8_t) (_register >> 8); // upper address
    regData[2] = (uint8_t) (*data++);
    regData[3] = (uint8_t) (*data++);
    regData[4] = (uint8_t) (*data++);
    regData[5] = (uint8_t) (*data);

    errorStatus = I2C_Get_Data(I2C_PORT_0, address, regData, 6, XIIC_STOP);

    return errorStatus;
}

/*
 *
 *
 *
 */
int MCDP6200_I2C_Read_Register_Data(uint8_t address, uint16_t _register, uint8_t *data){
	int sentCount = 0; // the number of bytes sent
	int recCount = 0;
	uint8_t reg[2];

	reg[0] = (uint8_t)_register;
	reg[1] = (uint8_t)(_register >> 8);

	// send register value to read from
	sentCount = I2C_Send_Data(I2C_PORT_0, address, reg, 2, XIIC_STOP);
	if(sentCount != 1){
		return I2C_FAIL;
	}

	// read data
	recCount = I2C_Get_Data(I2C_PORT_0, address, data, 4, XIIC_STOP);
	if(recCount != 4){
		return I2C_FAIL;
	}

	return I2C_NOERR;
}






