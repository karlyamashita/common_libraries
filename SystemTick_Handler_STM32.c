/*
 * SystemTick_Handler_STM32.c
 *
 *  Created on: Oct 6, 2025
 *      Author: karl.yamashita
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
 * Description: Register a callback that will get called from HAL_IncTick
 */
void SystmeTickCallbackRegister(SystemTick_t *sysTick, void *callback)
{
	sysTick->callback = callback;
}

