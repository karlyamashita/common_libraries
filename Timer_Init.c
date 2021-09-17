/*
 * Timer_Init.c
 *
 *  Created on: Jun 4, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "Timer_Init.h"

//#define Timer_PrintLine

/*
 *
83.333 mHz clock = .000000012 ns

.001 seconds / .000000012 seconds = 83333.333

0xFFFFFFFF - 83,333 = FFFE BA7A // 1ms interrupt
 *
 */

// Interrupt Controller variable
extern XIntc InterruptController;


// timer variables
/*
 * The following constant is used to set the reset value of the timer counter,
 * making this number larger reduces the amount of time this example consumes
 * because it is the value the timer counter is loaded with when it is started
 */
#define RESET_VALUE 0xFFFEBA7A // 1 ms
//#define RESET_VALUE 0xFB086F2A // 1 second
#define INTC_DEVICE_ID	XPAR_INTC_0_DEVICE_ID
#define INTC		XIntc
#define INTC_HANDLER	XIntc_InterruptHandler

XTmrCtr TmrCtrInstancePtr;

// prototypes
static int TmrCtrSetupIntrSystem(INTC *IntcInstancePtr, XTmrCtr *TmrCtrInstancePtr, u16 DeviceId, u16 IntrId, u8 TmrCtrNumber);

/*
 *
 *
 *
 */
int Init_Timer(void){
	int Status;

	/*
	 * Initialize the timer counter so that it's ready to use,
	 * specify the device ID that is generated in xparameters.h
	 */
	Status = XTmrCtr_Initialize(&TmrCtrInstancePtr, XPAR_AXI_TIMER_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef Timer_PrintLine
	PrintLine("XTmrCtr_Initialize");
#endif
	/*
	 * Perform a self-test to ensure that the hardware was built
	 * correctly, use the 1st timer in the device (0)
	 */
	Status = XTmrCtr_SelfTest(&TmrCtrInstancePtr, 0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef Timer_PrintLine
	PrintLine("XTmrCtr_SelfTest");
#endif
	/*
	 * Connect the timer counter to the interrupt subsystem such that
	 * interrupts can occur.  This function is application specific.
	 */
	Status = TmrCtrSetupIntrSystem(&InterruptController, &TmrCtrInstancePtr, XPAR_AXI_TIMER_0_DEVICE_ID, XPAR_INTC_0_TMRCTR_0_VEC_ID, 0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef Timer_PrintLine
	PrintLine("TmrCtrSetupIntrSystem");
#endif
	/*
	 * Setup the handler for the timer counter that will be called from the
	 * interrupt context when the timer expires, specify a pointer to the
	 * timer counter driver instance as the callback reference so the
	 * handler is able to access the instance data
	 */
	XTmrCtr_SetHandler(&TmrCtrInstancePtr, (XTmrCtr_Handler)SysTick_Handler, &TmrCtrInstancePtr);

	/*
	 * Enable the interrupt of the timer counter so interrupts will occur
	 * and use auto reload mode such that the timer counter will reload
	 * itself automatically and continue repeatedly, without this option
	 * it would expire once only
	 */
	XTmrCtr_SetOptions(&TmrCtrInstancePtr, 0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);

	/*
	 * Set a reset value for the timer counter such that it will expire
	 * eariler than letting it roll over from 0, the reset value is loaded
	 * into the timer counter when it is started
	 */
	XTmrCtr_SetResetValue(&TmrCtrInstancePtr, 0, RESET_VALUE);

	/*
	 * Start the timer counter such that it's incrementing by default,
	 * then wait for it to timeout a number of times
	 */
	XTmrCtr_Start(&TmrCtrInstancePtr, 0);

	return XST_SUCCESS;
}

/*
 *
 *
 *
 */
static int TmrCtrSetupIntrSystem(INTC *IntcInstancePtr, XTmrCtr *TmrCtrInstancePtr, u16 DeviceId, u16 IntrId, u8 TmrCtrNumber)
{
	 int Status;

	/*
	 * Initialize the interrupt controller driver so that
	 * it's ready to use, specify the device ID that is generated in
	 * xparameters.h
	 */
	//Status = XIntc_Initialize(IntcInstancePtr, INTC_DEVICE_ID);
	//if (Status != XST_SUCCESS)
	//	return XST_FAILURE;

	/*
	 * Connect a device driver handler that will be called when an interrupt
	 * for the device occurs, the device driver handler performs the
	 * specific interrupt processing for the device
	 */
	Status = XIntc_Connect(IntcInstancePtr, IntrId, (XInterruptHandler)XTmrCtr_InterruptHandler, (void *)TmrCtrInstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Start the interrupt controller such that interrupts are enabled for
	 * all devices that cause interrupts, specific real mode so that
	 * the timer counter can cause interrupts thru the interrupt controller.
	 */
	Status = XIntc_Start(IntcInstancePtr, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable the interrupt for the timer counter
	 */
	XIntc_Enable(IntcInstancePtr, IntrId);

	/*
	 * Initialize the exception table.
	 */
	Xil_ExceptionInit();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler) INTC_HANDLER,	IntcInstancePtr);

	/*
	 * Enable non-critical exceptions.
	 */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

