/*
 * SPI_Init.c
 *
 *  Created on: Jul 27, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "SPI_Init.h"

//#define SPI_PrintLine
#define SPI_USE_INTERRUPT_SYSTEM

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define SPI_DEVICE_ID		XPAR_SPI_0_DEVICE_ID
#define SPI_INTR_ID		XPAR_INTC_0_SPI_0_VEC_ID

/*
 * The following constant defines the slave select signal that is used to
 * to select the EEPROM device on the SPI bus, this signal is typically
 * connected to the chip select of the device
 */
#define SEEPROM_SPI_SELECT 0x01

/*
 * The following data type is used to send and receive data to the serial
 * EEPROM device connected to the SPI interface.  It is an array of bytes
 * rather than a structure for portability avoiding packing issues.  The
 * application must setup the data to be written in this buffer and retrieve
 * the data read from it.
 */
typedef u8 EepromBuffer[4];

extern XIntc InterruptController;

XSpi Spi;

/*
 * The following variables are shared between non-interrupt processing and
 * interrupt processing such that they must be global.
 */
volatile int TransferInProgress;

/*
 * The following variable tracks any errors that occur during interrupt
 * processing
 */
int ErrorCount;

#ifdef SPI_USE_INTERRUPT_SYSTEM
static int SetupInterruptSystem(XSpi *SpiPtr);
#endif

int SPI_Init(void){
	int Status = 0;

	XSpi_Config *ConfigPtr;	/* Pointer to Configuration data */
#ifdef SPI_PrintLine
	PrintLine("SPI_Init");
#endif
	/*
	 * Initialize the SPI driver so that it is  ready to use.
	 */
	ConfigPtr = XSpi_LookupConfig(SPI_DEVICE_ID);
	if (ConfigPtr == NULL) {
		return XST_DEVICE_NOT_FOUND;
	}

	Status = XSpi_CfgInitialize(&Spi, ConfigPtr,
				  ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef SPI_PrintLine
	PrintLine("XSpi_CfgInitialize");
#endif

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XSpi_SelfTest(&Spi);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef SPI_PrintLine
	PrintLine("XSpi_SelfTest");
#endif

#ifdef SPI_USE_INTERRUPT_SYSTEM
	/*
	 * Connect the SPI driver to the interrupt subsystem such that
	 * interrupts can occur.  This function is application specific.
	 */
	Status = SetupInterruptSystem(&Spi);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Setup the handler for the SPI that will be called from the interrupt
	 * context when an SPI status occurs, specify a pointer to the SPI
	 * driver instance as the callback reference so the handler is able to
	 * access the instance data
	 */
	XSpi_SetStatusHandler(&Spi, &Spi, (XSpi_StatusHandler)SpiHandler);
#endif

	/*
	 * Set the SPI device as a master and in manual slave select mode such
	 * that the slave select signal does not toggle for every byte of a
	 * transfer, this must be done before the slave select is set
	 */
	Status = XSpi_SetOptions(&Spi, XSP_MASTER_OPTION | XSP_MANUAL_SSELECT_OPTION);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef SPI_PrintLine
	PrintLine("XSpi_SetOptions");
#endif
	/*
	 * Select the slave on the SPI bus, the EEPROM device so that it can be
	 * read and written using the SPI bus
	 */
	Status = XSpi_SetSlaveSelect(&Spi, SEEPROM_SPI_SELECT);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef SPI_PrintLine
//	PrintLine("XSpi_SetSlaveSelect");
#endif
	/*
	 * Start the SPI driver so that interrupts and the device are enabled
	 */
	XSpi_Start(&Spi);

	/*
	 * Disable Global interrupt to use polled mode operation
	 */
#ifndef SPI_USE_INTERRUPT_SYSTEM
	XSpi_IntrGlobalDisable(&Spi);
#endif
	return Status;
}

#ifdef SPI_USE_INTERRUPT_SYSTEM
/****************************************************************************
*
*
* This function setups the interrupt system such that interrupts can occur
* for the SPI driver.  This function is application specific since the actual
* system may or may not have an interrupt controller. The SPI device could
* be directly connected to a processor without an interrupt controller.  The
* user should modify this function to fit the application.
*
* @param 	SpiPtr contains a pointer to the instance of the XSpi component
* 		which is going to be connected to the interrupt controller.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
****************************************************************************/
static int SetupInterruptSystem(XSpi *SpiPtr)
{

	int Status;


	/*
	 * Connect a device driver handler that will be called when an interrupt
	 * for the device occurs, the device driver handler performs the
	 * specific interrupt processing for the device
	 */
	Status = XIntc_Connect(&InterruptController, SPI_INTR_ID, (XInterruptHandler)XSpi_InterruptHandler, (void *)SpiPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef SPI_PrintLine
	PrintLine("XIntc_Connect");
#endif
	/*
	 * Start the interrupt controller such that interrupts are enabled for
	 * all devices that cause interrupts, specific real mode so that
	 * the SPI can cause interrupts through the interrupt controller.
	 */
	Status = XIntc_Start(&InterruptController, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef SPI_PrintLine
	PrintLine("XIntc_Start");
#endif
	/*
	 * Enable the interrupt for the Spi device
	 */
	XIntc_Enable(&InterruptController, SPI_INTR_ID);


	/*
	 * Initialize the exception table
	 */
	Xil_ExceptionInit();

	/*
	 * Register the interrupt controller handler with the exception table
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XIntc_InterruptHandler, &InterruptController);

	/*
	 * Enable non-critical exceptions
	 */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

/******************************************************************************/
/**
*
* This function is the handler which performs processing for the SPI driver.
* It is called from an interrupt context such that the amount of processing
* performed should be minimized.  It is called when a transfer of SPI data
* completes or an error occurs.
*
* This handler provides an example of how to handle SPI interrupts
* but is application specific.
*
*
* @param 	CallBackRef is a reference passed to the handler.
* @param	StatusEvent is the status of the SPI .
* @param	ByteCount is the number of bytes transferred.
*
* @return	None
*
* @note		None.
*
******************************************************************************/
void SpiHandler(void *CallBackRef, u32 StatusEvent, unsigned int ByteCount)
{
	/*
	 * Indicate the transfer on the SPI bus is no longer in progress
	 * regardless of the status event
	 */
	TransferInProgress = FALSE;

	/*
	 * If the event was not transfer done, then track it as an error
	 */
	if (StatusEvent != XST_SPI_TRANSFER_DONE) {
		ErrorCount++;
	}
}

void SPI_SetTransferInProgress(bool status){
	TransferInProgress = status;
}

bool SPI_GetTransferInProgress(void){
	return TransferInProgress;
}

void SPI_SetErrorCount(uint32_t count){
	ErrorCount = count;
}

int SPI_GetErrorCount(void){
	return ErrorCount;
}
#endif


