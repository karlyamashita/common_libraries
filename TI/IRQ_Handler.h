/*
 * irqHandler.h
 *
 *  Created on: Mar 5, 2021
 *      Author: karl.yamashita
 */

#ifndef IRQ_HANDLER_H_
#define IRQ_HANDLER_H_


#define ADC0_FIFO_SIZE 8
#define ADC1_FIFO_SIZE 4

#define HAL_MAX_DELAY      0xFFFFFFFFU

typedef enum
{
  HAL_TICK_FREQ_10HZ         = 100U,
  HAL_TICK_FREQ_100HZ        = 10U,
  HAL_TICK_FREQ_1KHZ         = 1U,
  HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ
} HAL_TickFreqTypeDef;


void SysTick_Handler(void);
void HAL_IncTick(void);
uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t Delay);

void ADC0_IRQHandler(void);
void ADC1_IRQHandler(void);
void I2C0_EV_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
void PortAIntHandler(void);
void PortBIntHandler(void);
void USART0_IRQHandler(void);
void USART3_IRQHandler(void);

uint32_t GetADC0_Value(uint8_t ptr);
uint32_t GetADC1_Value(uint8_t ptr);

#endif /* IRQ_HANDLER_H_ */
