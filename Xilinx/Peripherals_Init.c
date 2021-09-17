/*
 * Peripherals_Init.c
 *
 *  Created on: May 26, 2021
 *      Author: karl.yamashita
 */


#include "main.h"
#include "Peripherals_Init.h"

#define PeripheralsInit_PrintLine

/*
 * Description: Initiates peripherals
 * Input: none
 * Output: the number of errors counted
 */
int Peripherals_Init(void){
	int status;
	int errorCount = 0;
	status = Init_InterruptController();
	if (status != XST_SUCCESS) {
		errorCount += 1;
#ifdef PeripheralsInit_PrintLine
	PrintLine("CMD01:1");
#endif
	}

	status = Init_Timer();
	if (status != XST_SUCCESS) {
#ifdef PeripheralsInit_PrintLine
	PrintLine("CMD01:2");
#endif
	}

	status = Init_Uart0();
	if (status != XST_SUCCESS) {
		errorCount += 1;
#ifdef PeripheralsInit_PrintLine
	PrintLine("CMD01:3");
#endif
	}

	status = I2C_Init();
	if (status != XST_SUCCESS) {
		errorCount += 1;
#ifdef PeripheralsInit_PrintLine
	PrintLine("CMD01:4");
#endif
	}

	status = SPI_Init();
	if (status != XST_SUCCESS) {
		errorCount += 1;
#ifdef PeripheralsInit_PrintLine
	PrintLine("CMD01:5");
#endif
	}

	status = Init_GPIO();
	if (status != XST_SUCCESS) {
		errorCount += 1;
#ifdef PeripheralsInit_PrintLine
	PrintLine("CMD01:6");
#endif
	}

	return errorCount;
}






