/*
 * irqHandler.c
 *
 *  Created on: Mar 5, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "IRQ_Handler.h"

uint32_t uwTick;
HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;  /* 1KHz */

volatile uint32_t lastAlertAStatus = 1;
volatile uint32_t lastAlertBStatus = 1;

bool tmp101_Alert1Flag;
bool tmp101_Alert2Flag;


volatile uint32_t ulADC0Value[ADC0_FIFO_SIZE];
volatile uint32_t ulADC1Value[ADC1_FIFO_SIZE];

/*
 *
 *
 *
 */
uint32_t GetADC0_Value(uint8_t ptr){
    return ulADC0Value[ptr];
}

/*
 *
 *
 *
 */
uint32_t GetADC1_Value(uint8_t ptr){
    return ulADC1Value[ptr];
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
void SysTick_Handler(void){
    HAL_IncTick();
//    HAL_SYSTICK_Callback(); // mTimers.c
    TimerCallbackIncrement(); // TimerCallback.c
}

/*
 * Description: Increments a system tick
 *
 *
 */
void HAL_IncTick(void)
{
  uwTick += uwTickFreq;
}

/*
 * Description: gets the current system tick value
 * Return: tick value
 *
 */
uint32_t HAL_GetTick(void)
{
  return uwTick;
}

/*
 * Description: clears the tick value
 *
 *
 */
void HAL_ClearTick(void)
{
    uwTick = 0;
}

/*
 * Description: Delay in milliseconds. This is a blocking delay.
 *
 *
 */
void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a freq to guarantee minimum wait */
  if (wait < HAL_MAX_DELAY)
  {
    wait += (uint32_t)(uwTickFreq);
  }

  while ((HAL_GetTick() - tickstart) < wait)
  {
  }
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
void ADC0_IRQHandler(void){
    uint32_t tempData[ADC0_FIFO_SIZE] = {0};

    ADCIntClear(ADC0_BASE, SEQ_0);

    ADCSequenceDataGet(ADC0_BASE, SEQ_0, tempData);
    memcpy(&ulADC0Value, &tempData, sizeof(ulADC0Value));
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
void ADC1_IRQHandler(void){
    uint32_t tempData[ADC1_FIFO_SIZE] = {0};

    ADCIntClear(ADC1_BASE, SEQ_1);

    ADCSequenceDataGet(ADC1_BASE, SEQ_1, tempData);
    memcpy(&ulADC1Value, &tempData, sizeof(ulADC1Value));
}


/*
 * Description: TMP101
 *
 * Input:
 * Output:
 */
void I2C0_EV_IRQHandler(void){
//    HAL_I2C_EV_IRQHandler(I2C0_BASE);
}

/*
 * Description: Slave interrupt
 *
 * Input:
 * Output:
 */
void I2C1_EV_IRQHandler(void){
//    I2CSlaveIntClear(I2C1_BASE);
}


/*
 * Description: Handle PORTA pin interrupts
 *
 * Input:
 * Output:
 */
void PortAIntHandler(void){
    uint32_t currentPinStatus;

    // read pin status
    currentPinStatus = GPIOPinRead(GPIO_PORTA_BASE, LTM4675_Alert_Pin);

    GPIOIntClear(GPIO_PORTA_BASE, LTM4675_Alert_Pin);

    if((currentPinStatus & LTM4675_Alert_Pin) != lastAlertAStatus){
        lastAlertAStatus = currentPinStatus;
    }
}


/*
 * Description: Handle PORTB pin interrupts
 *
 * Input:
 * Output:
 */
void PortBIntHandler(void){
    uint32_t currentPinStatus;

    // read pin status
    currentPinStatus = GPIOPinRead(GPIO_PORTB_BASE, TMP101_Alert_Pin);

    GPIOIntClear(GPIO_PORTB_BASE, TMP101_Alert_Pin);

    if((currentPinStatus & TMP101_Alert_Pin) != lastAlertBStatus){
        lastAlertBStatus = currentPinStatus;
    }
}

/*
 * Description: Uart interrupt handler
 *
 */
void USART0_IRQHandler(void){
    Uart_Receive(UART0_BASE);
}

/*
 *
 *
 *
 */
void USART3_IRQHandler(void){
    Uart_Receive(UART3_BASE);
}


