/*
 * SystemTick_Handler_STM32.h
 *
 *  Created on: Oct 6, 2025
 *      Author: karl.yamashita
 */

#ifndef INC_SYSTEMTICK_HANDLER_STM32_H_
#define INC_SYSTEMTICK_HANDLER_STM32_H_


typedef void (*SystemTickCallback)(void);// no data is passed

typedef struct
{
	SystemTickCallback callback;
}SystemTick_t;

void SystemTickCallbackRegister(SystemTick_t *sysTick, void *callback);

#endif /* INC_SYSTEMTICK_HANDLER_STM32_H_ */
