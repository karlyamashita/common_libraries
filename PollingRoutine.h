
/*
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Karl Yamashita
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */

/*
 * PollingRoutine.h
 *
 *  Created on: Oct 24, 2023
 *      Author: karl.yamashita
 *
 *
 *      Template
 */

#ifndef INC_POLLINGROUTINE_H_
#define INC_POLLINGROUTINE_H_


/*

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#define Nop() asm(" NOP ")

// most commonly used. Uncomment if needed.
//#include "ErrorStatus.h"
//#include "RingBuffer.h"
//#include "UART_DMA_Idle_Circular_Drv_STM32.h"
//#include "StringManip.h"
//#include "TimerCallback.h"

#include "PollingRoutine.h"

*/
#ifndef __weak
#define __weak __attribute__((weak))
#endif

void PollingInit(void);
void PollingRoutine(void);

// uncomment if needed

//void UART_ParseCommands(UART_DMA_Struct_t *msg);
//void STM32_Ready(UART_DMA_Struct_t *msg);
//int Version(char *msg, char *retStr)


/* copy these 3 to main.h
extern UART_HandleTypeDef huart2;

extern UART_DMA_Struct_t uart2_msg; // uses USART2, but can be changed
extern TimerCallbackStruct timerCallback;
*/


#endif /* INC_POLLINGROUTINE_H_ */
