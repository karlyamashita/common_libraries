/*
 * GPIO_Init.c
 *
 *  Created on: Jun 4, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "GPIO_Init.h"

//#define GPIO_PrintLine


/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */
XGpio Gpio_0; /* The Instance of the GPIO Driver */
XGpio Gpio_1; /* The Instance of the GPIO Driver */
XGpio Gpio_2; /* The Instance of the GPIO Driver */



/*
 *
 *
 *
 */
int Init_GPIO(void){
	int Status;

	// gpio 0
	Status = XGpio_Initialize(&Gpio_0, XPAR_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef GPIO_PrintLine
	PrintLine("XGpio_Initialize 0");
#endif
	Status = XGpio_SelfTest(&Gpio_0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef GPIO_PrintLine
	PrintLine("XGpio_SelfTest");
#endif
	// gpio 1
	Status = XGpio_Initialize(&Gpio_1, XPAR_GPIO_1_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef GPIO_PrintLine
	PrintLine("XGpio_Initialize 1");
#endif
	Status = XGpio_SelfTest(&Gpio_1);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef GPIO_PrintLine
	PrintLine("XGpio_SelfTest");
#endif
	// gpio 2
	Status = XGpio_Initialize(&Gpio_2, XPAR_GPIO_2_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef GPIO_PrintLine
	PrintLine("XGpio_Initialize 2");
#endif
	Status = XGpio_SelfTest(&Gpio_2);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#ifdef GPIO_PrintLine
	PrintLine("XGpio_SelfTest");
#endif

	//XGpio_InterruptEnable(&Gpio_0, 0x0f);
	//XGpio_InterruptGlobalEnable(&Gpio_0);

	GPIO_Init_Direction();

	return XST_SUCCESS;
}

/*
 *
 *
 *
 */
void GPIO_Init_Direction(void){
	uint32_t DirectionMask = 0;

	// QSPI and JTAG
	XGpio_SetDataDirection(&Gpio_0, CHANNEL_1, DirectionMask);
	XGpio_SetDataDirection(&Gpio_0, CHANNEL_2, DirectionMask);

	// SSMUX
	XGpio_SetDataDirection(&Gpio_1, CHANNEL_1, DirectionMask);
	// redriver config
	XGpio_SetDataDirection(&Gpio_1, CHANNEL_2, DirectionMask);

	// Test I/O's
	XGpio_SetDataDirection(&Gpio_2, CHANNEL_1, DirectionMask);
}




