#include "main.h"
#include "MCDP6200.h"

/*
 * Description: Kenetic MCDP6200 USB Type-C Switching Retimer
 * 
 * The MCDP6200 operating mode can be configured through the DIS_N and P_POL pins. It can be optionally
configured through TWI by enabling the feature through TWI. These interfaces can be controlled from an
external Power Delivery (PD) controller or microcontroller to set the plug orientation and the pin mapping of
the USB Type-C.
 * 
 * Note: TWI refers to the I2C control
 * 
 */

IC_RT_CONFIG_30C_RW ic_RT_CONFIG_30C_RW; // debug



void MCDP6200_Init(void)
{
    
}


/*
 * Description: Combines register address and data into an array then sends data to MCDP_6200
 */
i2c_error_t MCDP6200_Write_Register_Data(uint8_t address, uint16_t _register, uint32_t *data)
{
    uint8_t regData[6];
    regData[0] = (uint8_t) _register; // lower address
    regData[1] = (uint8_t) (_register >> 8); // upper address
    regData[2] = (uint8_t) (*data);
    regData[3] = (uint8_t) (*data >> 8);
    regData[4] = (uint8_t) (*data >> 16);
    regData[5] = (uint8_t) (*data >> 24);
    return I2C_0_writeNBytes(address, &regData, sizeof(regData));
}

/*
* Description: Sets the TX_CONFIG_WINDOW_x for Read or Write, and for what Gen
* Input:
*  Output: none
*/
void MCDP6200_SetTxConfigWindow(uint8_t retimerAddress, uint16_t _txRegister, uint8_t rdWr, uint8_t gen){
	
}


