/*
 * SystemTick_Handler_STM32.c
 *
 *  Created on: Oct 6, 2025
 *      Author: karl.yamashita
 *
 *      Objective:
 *      Instead of writing code in SysTick_Handler() in stm32xxxx_it.c,
 *      you can add these SystemTick_Handler.c/h files to your project.
 *      This will override the weak function, HAL_IncTick();
 *
 *      You can also register a callback to call after the uwTick variable is incremented.
 *
 *
 *		Initialization step: Be sure to extern below variables, typically in main.h
 * 				extern __IO uint32_t uwTick;
 * 				extern SystemTick_t sysTick;
 */

#include "main.h"

SystemTick_t sysTick = {0};


/*
 * Description: This overrides the weak function
 */
void HAL_IncTick(void)
{
	uwTick += (uint32_t)uwTickFreq;

	if(sysTick.callback != NULL)
	{
	  sysTick.callback();
	}
}

/*
 * Description: Register a callback, typically before main while loop, that will get called from HAL_IncTick
 *
 * 			Example;
 * 			SystemTickCallbackRegister(&sysTick, TimerCallbackHandlerTick);
 */
void SystemTickCallbackRegister(SystemTick_t *sysTick, void *callback)
{
	sysTick->callback = callback;
}

