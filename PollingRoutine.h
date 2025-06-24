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

//#include "RingBuffer.h"
//#include "TimerCallback.h"

#include "PollingRoutine.h"

*/
#ifndef __weak
#define __weak __attribute__((weak))
#endif

void PollingInit(void);
void PollingRoutine(void);

/*
void UART_ParseCommands(UART_DMA_Struct_t *msg);
void STM32_Ready(UART_DMA_Struct_t *msg);
*/

/* copy these 2 to main.h
extern UART_HandleTypeDef huart2;
extern TimerCallbackStruct timerCallback;
*/

#endif /* INC_POLLINGROUTINE_H_ */
