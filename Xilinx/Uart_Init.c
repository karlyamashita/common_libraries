/*
 * Uart_Init.c
 *
 *  Created on: Jun 4, 2021
 *      Author: karl.yamashita
 */


#include "main.h"
#include "Uart_Init.h"

//#define Uart_PrintLine

// Interrupt Controller variable
extern XIntc InterruptController;

// uart variables
/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define INTC_DEVICE_ID	XPAR_INTC_0_DEVICE_ID
#define UARTLITE_DEVICE_ID      XPAR_UARTLITE_0_DEVICE_ID
#define UARTLITE_INT_IRQ_ID     XPAR_INTC_0_UARTLITE_0_VEC_ID

XUartLite UartLite;            /* The instance of the UartLite Device */
#define TEST_BUFFER_SIZE        100
uint8_t SendBuffer[TEST_BUFFER_SIZE];
uint8_t ReceiveBuffer[TEST_BUFFER_SIZE];
static volatile int TotalReceivedCount;
static volatile int TotalSentCount;


// prototypes
static int SetupUartInterruptSystem(XUartLite *UartLitePtr);



/*
 *
 *
 *
 */
int Init_Uart0(void){
	int Status;

	/*
	 * Initialize the UartLite driver so that it's ready to use.
	 */
	Status = XUartLite_Initialize(&UartLite, UARTLITE_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef Uart_PrintLine
	PrintLine("XUartLite_Initialize");
#endif
	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XUartLite_SelfTest(&UartLite);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef Uart_PrintLine
	PrintLine("XUartLite_SelfTest");
#endif
	/*
	 * Connect the UartLite to the interrupt subsystem such that interrupts can
	 * occur. This function is application specific.
	 */
	Status = SetupUartInterruptSystem(&UartLite);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef Uart_PrintLine
	PrintLine("SetupUartInterruptSystem");
#endif
	/*
	 * Setup the handlers for the UartLite that will be called from the
	 * interrupt context when data has been sent and received, specify a
	 * pointer to the UartLite driver instance as the callback reference so
	 * that the handlers are able to access the instance data.
	 */
	//XUartLite_SetSendHandler(&UartLite, SendHandler, &UartLite);
	//XUartLite_SetRecvHandler(&UartLite, RecvHandler, &UartLite);
	XUartLite_SetRecvHandler(&UartLite, Uart0ReceiveInterruptHandler, &UartLite);

	/*
	 * Enable the interrupt of the UartLite so that interrupts will occur.
	 */
	XUartLite_EnableInterrupt(&UartLite);

	return XST_SUCCESS;
}

/*
 *
 *
 *
 */
static int SetupUartInterruptSystem(XUartLite *UartLitePtr)
{
	int Status;

	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	//Status = XIntc_Initialize(&intr_ctrl, INTC_DEVICE_ID);
	//if (Status != XST_SUCCESS) {
	//	return XST_FAILURE;
	//}


	/*
	 * Connect a device driver handler that will be called when an interrupt
	 * for the device occurs, the device driver handler performs the
	 * specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(&InterruptController, UARTLITE_INT_IRQ_ID, (XInterruptHandler)XUartLite_InterruptHandler, (void *)UartLitePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Start the interrupt controller such that interrupts are enabled for
	 * all devices that cause interrupts, specific real mode so that
	 * the UartLite can cause interrupts through the interrupt controller.
	 */
/*
	Status = XIntc_Start(&intr_ctrl, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
*/

	/*
	 * Enable the interrupt for the UartLite device.
	 */
	XIntc_Enable(&InterruptController, UARTLITE_INT_IRQ_ID);

	/*
	 * Initialize the exception table.
	 */
	Xil_ExceptionInit();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XIntc_InterruptHandler, &InterruptController);

	/*
	 * Enable exceptions.
	 */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

