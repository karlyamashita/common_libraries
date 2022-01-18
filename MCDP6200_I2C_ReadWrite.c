/*
 * Description: Kenetic MCDP6200 USB Type-C Switching Retimer
 * 
 * The MCDP6200 operating mode can be configured through the DIS_N and P_POL pins. It can be optionally
 * configured through TWI by enabling the feature through TWI. These interfaces can be controlled from an
 * external Power Delivery (PD) controller or microcontroller to set the plug orientation and the pin mapping of
 * the USB Type-C.
 * 
 * Note: TWI refers to the I2C control
 * 		#include "I2C_ReadWrite.h"
 * 
 */

#include "main.h"
#include "MCDP6200_I2C_ReadWrite.h"

#define MCDP6200_I2C_ReadWrite_PrintLine

static int Get_MCDP6200_Register_Data(uint8_t devAddr, uint8_t *regAddr, uint32_t regSize, uint8_t *RxMsgPtr, uint32_t byteCount);
static int Set_MCDP6200_Register_Data(uint8_t devAddr, uint8_t *TxMsgPtr, uint32_t byteCount);


/*
 * Description: Reads register data
 * Input: I2C slave address, Register address
 * Output: the 32 bit data from the register address
 *	Return: I2C error status
 *
 */
int MCDP6200_Read_Register_Data(uint8_t devAddr, uint32_t regAddr, uint8_t *data)
{
	int status = NO_ERROR;
	uint8_t regAddrArray[2];

	regAddrArray[0] = (uint8_t) regAddr; // lower address
	regAddrArray[1] = (uint8_t) (regAddr >> 8); // upper address

	status = Get_MCDP6200_Register_Data(devAddr, regAddrArray, REG_SIZE, data, 4);

	return status;
}

/*
 * Description: Combines register address and data into an array then sends data to MCDP_6200
 * Input: I2C address, Register address, 32bit data
 * Return: I2C error status
 *
 */
int MCDP6200_Write_Register(uint8_t devAddr, uint32_t regAddr, uint8_t *data)
{
	int status = NO_ERROR;
    uint8_t regData[6];

    regData[0] = (uint8_t) regAddr; // lower address
    regData[1] = (uint8_t) (regAddr >> 8); // upper address
    regData[2] = (uint8_t) (*data++);
    regData[3] = (uint8_t) (*data++);
    regData[4] = (uint8_t) (*data++);
    regData[5] = (uint8_t) (*data);

    status = Set_MCDP6200_Register_Data(devAddr, regData, 6);

    return status;
}

#ifdef ccs // Code Composer Studio

/*
 * Description: Gets register data
 * Input: I2C peripheral #, device address, register address to read from, pointer to receive buffer, how many bytes to receive
 * Output: RxMsgPtr and byte received
 * Return: error status
 */
static int Get_MCDP6200_Register_Data(uint8_t devAddr, uint8_t *regAddr, uint32_t regSize, uint8_t *RxMsgPtr, uint32_t byteCount)
{
	int status = NO_ERROR;
	uint32_t i2c_base = I2C0_BASE;

	status = I2C_Master_ReadRegister(i2c_base, devAddr, regAddr, regSize, RxMsgPtr, byteCount);

	return status;
}

/*
 * Description:
 *
 *
 */
static int Set_MCDP6200_Register_Data(uint8_t devAddr, uint8_t *TxMsgPtr, uint32_t byteCount)
{
	int status = NO_ERROR;
	uint32_t i2c_base = I2C0_BASE;

	status = I2C_Master_Transmit(i2c_base, devAddr, TxMsgPtr, byteCount);

	return status;
}

#else // Xilinx FPGA
#define IIC_RETIMER_BASE_ADDRESS XPAR_IIC_1_BASEADDR

/*
 * Description: Gets register data
 * Input: I2C peripheral #, device address, register address to read from, pointer to receive buffer, how many bytes to receive
 * Output: RxMsgPtr and byte received
 * Return: error status
 */
static int Get_MCDP6200_Register_Data(uint8_t devAddr, uint8_t *regAddr, uint8_t *RxMsgPtr, uint32_t byteCount)
{
    int status = NO_ERROR;
    uint32_t bytesRecvd = 0;
    uint32_t bytesSent = 0;
//  XIic i2c_instance;
    uint32_t tickCount;

//  status = XIic_SetAddress(&i2c_instance, XII_ADDR_TO_SEND_TYPE, devAddr);
    if(status != 0){
        return I2C_ERROR;
    }

    tickCount = HAL_GetTick();
//  while(XIic_WaitBusFree(IIC_RETIMER_BASE_ADDRESS) != XST_SUCCESS){
        if((HAL_GetTick() - tickCount) >= I2C_TIMEOUT){
            return TIMEOUT;
        }
//  }

//  bytesSent = XIic_Send(IIC_RETIMER_BASE_ADDRESS, devAddr, regAddr, 2, XIIC_REPEATED_START);
    if(bytesSent != 2){
        return I2C_ERROR;
    }

    tickCount = HAL_GetTick();
    while(bytesRecvd != byteCount){
        if((HAL_GetTick() - tickCount) >= I2C_TIMEOUT){
            return TIMEOUT;
        }
//      bytesRecvd = XIic_Recv(IIC_RETIMER_BASE_ADDRESS, devAddr, RxMsgPtr, byteCount, XIIC_STOP);
    }
    return NO_ERROR;
}

/*
 * Description:
 *
 *
 */
static int Set_MCDP6200_Register_Data(uint8_t devAddr, uint8_t *TxMsgPtr, uint32_t byteCount)
{
    int status = NO_ERROR;
    uint32_t byteSent = 0;

//  XIic i2c_instance;
    uint32_t tickCount = HAL_GetTick();

//  status = XIic_SetAddress(&i2c_instance, XII_ADDR_TO_SEND_TYPE, devAddr);
    if(status != 0){
        return I2C_ERROR;
    }

    tickCount = HAL_GetTick();
//  while(XIic_WaitBusFree(IIC_RETIMER_BASE_ADDRESS) != XST_SUCCESS){
        if((HAL_GetTick() - tickCount) >= I2C_TIMEOUT){
            return TIMEOUT;
        }
//  }

//  byteSent = XIic_Send(IIC_RETIMER_BASE_ADDRESS, devAddr, TxMsgPtr, byteCount, XIIC_STOP);
    if(byteSent != byteCount){
        status = I2C_ERROR;
    }

    return status;
}

#endif




