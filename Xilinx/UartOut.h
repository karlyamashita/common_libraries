/*
 * UartOut.h
 *
 *  Created on: Jun 2, 2021
 *      Author: karl.yamashita
 */

#ifndef SRC_UARTOUT_H_
#define SRC_UARTOUT_H_

#include "main.h"


HAL_StatusTypeDef UartTxMessage(UartCharBufferTxStruct *uartBufferPointer);
void PrintUart1(const char8 *ptr);
void PrintLine(const char8 *ptr);

#endif /* SRC_UARTOUT_H_ */
