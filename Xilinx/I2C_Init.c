/*
 * I2C_Init.c
 *
 *  Created on: Jun 4, 2021
 *      Author: karl.yamashita
 */


#include "main.h"
#include "I2C_Init.h"

//#define I2C_PrintLine // uncomment to print debug messages

// Interrupt Controller variable
extern XIntc InterruptController;
HandlerInfo handlerInfo;

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define IIC_DEVICE_ID		XPAR_IIC_0_DEVICE_ID

#define INTC_DEVICE_ID	XPAR_INTC_0_DEVICE_ID
#define IIC_INTR_ID	XPAR_INTC_0_IIC_0_VEC_ID
//#define INTC			XIntc
#define INTC_HANDLER	XIntc_InterruptHandler

XIic Iic;		  /* The instance of the IIC device */


// prototypes
static int SetupI2CInterruptSystme(XIic *IicInstPtr);

/*
 * Description:
 *
 * Input:
 * Output:
 */
int I2C_Init(void){


	int Status;
	XIic_Config *ConfigPtr;	/* Pointer to configuration data */

	/*
	 * Initialize the IIC driver so that it is ready to use.
	 */
	ConfigPtr = XIic_LookupConfig(IIC_DEVICE_ID);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}

	Status = XIic_CfgInitialize(&Iic, ConfigPtr, ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef I2C_PrintLine
	PrintLine("XIic_CfgInitialize");
#endif
	Status = SetupI2CInterruptSystme(&Iic);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef I2C_PrintLine
	PrintLine("SetupI2CInterruptSystme");
#endif
	/*
	 * Set the Handlers for transmit and reception.
	 */
	XIic_SetSendHandler(&Iic, (void *)&handlerInfo,	(XIic_Handler) I2CSendHandler);
	XIic_SetRecvHandler(&Iic, (void *)&handlerInfo,	(XIic_Handler) I2CRecvHandler);
	XIic_SetStatusHandler(&Iic, (void *)&handlerInfo, (XIic_StatusHandler) I2CStatusHandler);
#ifdef I2C_PrintLine
	PrintLine("XIic_SetSendHandler");
#endif
	/*
	 * Start the IIC driver such that it is ready to send and
	 * receive messages on the IIC interface, set the address
	 * to send to which is the temperature sensor address
	 */
	Status = XIic_Start(&Iic);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef I2C_PrintLine
	PrintLine("XIic_Start");
#endif
	Status = XIic_SetAddress(&Iic, XII_ADDR_TO_SEND_TYPE, AD5627R_SLAVE_ADDRESS);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef I2C_PrintLine
	PrintLine("XIic_SetAddress");
#endif
	return XST_SUCCESS;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
static int SetupI2CInterruptSystme(XIic *IicInstPtr){
	int Status;

	/*
	 * Initialize the interrupt controller driver so that it's ready to use.
	 */
	Status = XIntc_Initialize(&InterruptController, INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
	//	return XST_FAILURE;
	}

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(&InterruptController, IIC_INTR_ID, (XInterruptHandler) XIic_InterruptHandler, &IicInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Start the interrupt controller so interrupts are enabled for all
	 * devices that cause interrupts.
	 */
	Status = XIntc_Start(&InterruptController, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable the interrupts for the IIC device.
	 */
	XIntc_Enable(&InterruptController, IIC_INTR_ID);

	/*
	 * Initialize the exception table and register the interrupt
	 * controller handler with the exception table
	 */
	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)INTC_HANDLER, &InterruptController);

	/* Enable non-critical exceptions */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}


