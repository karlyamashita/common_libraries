/*
 * InterruptController_Init.c
 *
 *  Created on: Jun 4, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "InterruptController_Init.h"

//#define Init_InterruptControllerPrintLine // uncomment to print debug messages

// interrupt controller variable
XIntc InterruptController; //

#define INTC_DEVICE_ID          XPAR_INTC_0_DEVICE_ID

/*
 * Description:
 *
 *
 */
int Init_InterruptController(void){
	 int Status;
#ifdef Init_InterruptControllerPrintLine
	 PrintLine("Init_InterruptController()");
#endif
	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	Status = XIntc_Initialize(&InterruptController, INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef Init_InterruptControllerPrintLine
	PrintLine("XIntc_Initialize success");
#endif
	/*
	 * Perform a self-test to ensure that the hardware was built
	 * correctly.
	 */
	Status = XIntc_SelfTest(&InterruptController);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef Init_InterruptControllerPrintLine
	PrintLine("XIntc_SelfTest success");
#endif
	/*
	 * Start the interrupt controller such that interrupts are recognized
	 * and handled by the processor.
	 */
	Status = XIntc_Start(&InterruptController, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef Init_InterruptControllerPrintLine
	PrintLine("XIntc_Start success");
#endif
	return XST_SUCCESS;
}


