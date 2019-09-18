#include "LSP.h"
#ifdef USE_KICK_THE_DOG
// for use with STM32, for now.

#include "main.h"
#include "kickTheDog.h"
#include "wwdg.h"

/*
Create a timer callback to call this funcion. 

TimeoutCalculation((hwwdg.Init.Counter - hwwdg.Init.Window) + 1) + 1 

  hwwdg.Init.Prescaler = WWDG_PRESCALER_4;
  hwwdg.Init.Window = 127;
  hwwdg.Init.Counter = 127;
  hwwdg.Init.EWIMode = WWDG_EWI_DISABLE;

*/
void KickTheDog(void) {
	HAL_WWDG_Refresh(&hwwdg);
}

/**
  * @brief  Timeout calculation function.
  *         This function calculates any timeout related to 
  *         WWDG with given prescaler and system clock.
  * @param  timevalue: period in term of WWDG counter cycle.
  * @retval None
  */
uint32_t TimeoutCalculation(uint32_t timevalue)
{
  uint32_t timeoutvalue = 0;
  uint32_t pclk1 = 0;
  uint32_t wdgtb = 0;

  /* considering APB divider is still 1, use HCLK value */
  pclk1 = HAL_RCC_GetPCLK1Freq();

  /* get prescaler */
  wdgtb = (1 << ((hwwdg.Init.Prescaler) >> 7)); /* 2^WDGTB[1:0] */

  /* calculate timeout */
  timeoutvalue = ((4096 * wdgtb * timevalue) / (pclk1 / 1000));

  return timeoutvalue;
}


#endif // USE_KICK_THE_DOG
