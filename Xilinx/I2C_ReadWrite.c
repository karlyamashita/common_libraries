/*
 * I2C_ReadWrite.c
 *
 *  Created on: Jul 1, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "I2C_ReadWrite.h"

//#define I2C_ReadWritePrintLine

extern XIic Iic;
extern 	HandlerInfo handlerInfo;

extern volatile uint8_t TransmitComplete;	/* Flag to check completion of Transmission */
extern volatile uint8_t ReceiveComplete;	/* Flag to check completion of Reception */

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define IIC_BASE_ADDRESS	XPAR_IIC_0_BASEADDR

int I2C_Send_Data(uint8_t i2c_port, uint8_t devAddr, uint8_t *TxMsgPtr, int ByteCount, uint8_t option){
	int sentCount = 0;
	int errorCode = 0;
	uint32_t delayCount;
	uint32_t CntlReg;
#ifdef	I2C_ReadWritePrintLine
	PrintLine("I2C_Send_Data()");
#endif
	errorCode = XIic_SetAddress(&Iic, XII_ADDR_TO_SEND_TYPE, devAddr);
	if(errorCode != 0){
		return errorCode;
	}
#ifdef	I2C_ReadWritePrintLine
	PrintLine("XIic_SetAddress Successful!");
#endif
	delayCount = HAL_GetTick();

	do{

		sentCount = XIic_Send(IIC_BASE_ADDRESS, devAddr, TxMsgPtr, ByteCount, option);
		if (sentCount != ByteCount) {
			/* Send is aborted so reset Tx FIFO */
			CntlReg = XIic_ReadReg(IIC_BASE_ADDRESS, XIIC_CR_REG_OFFSET);
			XIic_WriteReg(IIC_BASE_ADDRESS, XIIC_CR_REG_OFFSET,	CntlReg | XIIC_CR_TX_FIFO_RESET_MASK);
			XIic_WriteReg(IIC_BASE_ADDRESS, XIIC_CR_REG_OFFSET,	XIIC_CR_ENABLE_DEVICE_MASK);
		}
		if((HAL_GetTick() - delayCount) >= I2C_TIMEOUT){
			return sentCount;
		}
	}while(sentCount != ByteCount);

	if(sentCount == ByteCount){
#ifdef	I2C_ReadWritePrintLine
		PrintLine("I2C_Send_Data Successful!");
#endif
	}

	return sentCount;
}


/*
 * Description: Interrupts don't work!! For now don't use.
 *
 * Input:
 * Output:
 */
/*
int I2C_Write_Data(uint8_t i2c_port, uint8_t devAddr, uint8_t *TxMsgPtr, int ByteCount)
{
	int status = 0;
	XIic IicInstance;
#ifdef	I2C_ReadWritePrintLine
	char str[92];

	PrintLine("I2C_Write_Data");
#endif
	if(i2c_port == I2C_PORT_0){
		IicInstance = Iic;
	}
#ifdef	I2C_ReadWritePrintLine
	// debug
//	sprintf(str, "i2c_port: %d, devAddr: %d, TxMsgPtr[0]: %d, TxMsgPtr[1]: %d, len: %d",  i2c_port, devAddr, TxMsgPtr[0], TxMsgPtr[1], ByteCount);
//	PrintLine(str);
	// end debug
#endif

	 * Set the defaults.

	handlerInfo.TransmitComplete = 1;
	IicInstance.Stats.TxErrors = 0;


	 * Start the IIC device.

	status = XIic_Start(&IicInstance);
	if (status != XST_SUCCESS) {
		return status;
	}
#ifdef	I2C_ReadWritePrintLine
	PrintLine("XIic_Start");
#endif
	status = XIic_SetAddress(&IicInstance, XII_ADDR_TO_SEND_TYPE, devAddr);
	if(status != 0){
		PrintLine("Error in setting I2C Slave address");
		return status;
	}
#ifdef	I2C_ReadWritePrintLine
	PrintLine("XIic_SetAddress");
#endif

	 * Send the Data.

	status = XIic_MasterSend(&IicInstance, TxMsgPtr, ByteCount);
	if(status != XST_SUCCESS){
		return status;
	}
#ifdef	I2C_ReadWritePrintLine
	PrintLine("XIic_MasterSend");

	PrintLine("Waiting for TransmitComplete");
#endif

	 * Wait till the transmission is completed.

	while ((handlerInfo.TransmitComplete) || (XIic_IsIicBusy(&IicInstance) == TRUE)) {

		 * This condition is required to be checked in the case where we
		 * are writing two consecutive buffers of data to the EEPROM.
		 * The EEPROM takes about 2 milliseconds time to update the data
		 * internally after a STOP has been sent on the bus.
		 * A NACK will be generated in the case of a second write before
		 * the EEPROM updates the data internally resulting in a
		 * Transmission Error.

		if (IicInstance.Stats.TxErrors != 0) {



			 * Enable the IIC device.

			status = XIic_Start(&IicInstance);
			if (status != XST_SUCCESS) {
				return status;
			}
#ifdef	I2C_ReadWritePrintLine
			PrintLine("XIic_Start");
#endif
			if (!XIic_IsIicBusy(&IicInstance)) {

				 * Send the Data.

				status = XIic_MasterSend(&IicInstance, TxMsgPtr, ByteCount);
				if (status == XST_SUCCESS) {
					IicInstance.Stats.TxErrors = 0;
#ifdef	I2C_ReadWritePrintLine
					PrintLine("XIic_MasterSend");
#endif
				}
				else {
				}
			}
		}
	}
#ifdef	I2C_ReadWritePrintLine
	PrintLine("TransmitComplete");
#endif

	 * Stop the IIC device.

	status = XIic_Stop(&IicInstance);
	if (status != XST_SUCCESS) {
		return status;
	}
#ifdef	I2C_ReadWritePrintLine
	PrintLine("XIic_Stop");
#endif
	return XST_SUCCESS;
}*/

/*
 * Description: Reads data directly. No register address.
 *
 * Input:
 * Output:
 *
 */
int I2C_Get_Data(uint8_t i2c_port, uint8_t devAddr, uint8_t *RxMsgPtr, int ByteCount, uint8_t option)
{
	int status = 0;
	int recvCount = 0;
	uint32_t tickCount = HAL_GetTick();

	status = XIic_SetAddress(&Iic, XII_ADDR_TO_SEND_TYPE, devAddr);
	if(status != 0){
		PrintLine("Error in setting I2C Slave address");
		return status;
	}

	while(XIic_WaitBusFree(IIC_BASE_ADDRESS) != XST_SUCCESS){
		if((HAL_GetTick() - tickCount) >= I2C_TIMEOUT) {
			PrintLine("I2C bus timeout!");
			return 0; // return 0 for no bytes received
		}
	}

	recvCount = XIic_Recv(IIC_BASE_ADDRESS, devAddr, RxMsgPtr, ByteCount, option);

	return recvCount;
}

int I2C_Get_Register_Data(uint8_t i2c_port, uint8_t devAddr, uint8_t regAddr, uint8_t *RxMsgPtr, int ByteCount)
{
	int status = 0;
	int byteCount = 0;
	uint32_t tickCount = HAL_GetTick();

	status = XIic_SetAddress(&Iic, XII_ADDR_TO_SEND_TYPE, devAddr);
	if(status != 0){
		PrintLine("Error in setting I2C Slave address");
		return status;
	}

	while(XIic_WaitBusFree(IIC_BASE_ADDRESS) != XST_SUCCESS){
		if((HAL_GetTick() - tickCount) >= I2C_TIMEOUT){
			PrintLine("I2C bus timeout!");
			return 0;
		}
	}

	//while(byteCount != 1){
		XIic_Send(IIC_BASE_ADDRESS, devAddr, &regAddr, 1, XIIC_REPEATED_START);
	//}
//	byteCount = 0;
//	tickCount = HAL_GetTick();
/*
		while(XIic_WaitBusFree(IIC_BASE_ADDRESS) != XST_SUCCESS){
		if((HAL_GetTick() - tickCount) >= 100) {
			PrintLine("I2C bus timeout!");
			return 0;
		}
	}
*/
	while(byteCount != ByteCount){
		byteCount = XIic_Recv(IIC_BASE_ADDRESS, devAddr, RxMsgPtr, ByteCount, XIIC_STOP);
	}
	return byteCount;
}

/*
 * Description: Interrupts don't work!! For now don't use.
 *
 * Input:
 * Output:
 */
/*
int I2C_Read_Data(uint8_t i2c_port, uint8_t devAddr, uint8_t regAddr, uint8_t *RxMsgPtr, int ByteCount)
{
	XIic IicInstance;
	int status;
	uint8_t TxMsgPtr[1];


	 * Set the Defaults.

	handlerInfo.ReceiveComplete = 1;

	if(i2c_port == I2C_PORT_0){
		IicInstance = Iic;
	}

	TxMsgPtr[0] =  regAddr;
	status = I2C_Write_Data(i2c_port, devAddr, TxMsgPtr, 1);
	if(status != HAL_OK){
		return status;
	}
#ifdef	I2C_ReadWritePrintLine
	PrintLine("I2C_Write_Data");
#endif

	 * Start the IIC device.

	status = XIic_Start(&IicInstance);
	if (status != XST_SUCCESS) {
		return status;
	}
#ifdef	I2C_ReadWritePrintLine
	PrintLine("XIic_Start");
#endif

	 * Receive the Data.

	status = XIic_MasterRecv(&IicInstance, RxMsgPtr, ByteCount);
	if(status != 0){
		return status;
	}
#ifdef	I2C_ReadWritePrintLine
	PrintLine("XIic_MasterRecv");
#endif

	 * Wait till all the data is received.

#ifdef	I2C_ReadWritePrintLine
	PrintLine("Wait for Receive Complete");
#endif
	while ((handlerInfo.ReceiveComplete) || (XIic_IsIicBusy(&IicInstance) == TRUE)) {

	}
#ifdef	I2C_ReadWritePrintLine
	PrintLine("ReceiveComplete");
#endif

	 * Stop the IIC device.

	status = XIic_Stop(&IicInstance);
	if (status != XST_SUCCESS) {
		return status;
	}
#ifdef	I2C_ReadWritePrintLine
	PrintLine("XIic_Stop");
#endif
	return XST_SUCCESS;
}*/

