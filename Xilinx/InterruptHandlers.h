/*
 * InterruptHandlers.h
 *
 *  Created on: Jun 2, 2021
 *      Author: karl.yamashita
 */

#ifndef SRC_INTERRUPTHANDLERS_H_
#define SRC_INTERRUPTHANDLERS_H_


#define HAL_MAX_DELAY      0xFFFFFFFFU

typedef enum
{
  HAL_TICK_FREQ_10HZ         = 100U,
  HAL_TICK_FREQ_100HZ        = 10U,
  HAL_TICK_FREQ_1KHZ         = 1U,
  HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ
} HAL_TickFreqTypeDef;

/*
 * The following structure contains fields that are used with the callbacks
 * (handlers) of the IIC driver. The driver asynchronously calls handlers
 * when abnormal events occur or when data has been sent or received. This
 * structure must be volatile to work when the code is optimized.
 */
 volatile typedef struct {
	int  EventStatus;
	int  RemainingRecvBytes;
	int EventStatusUpdated;
	int RecvBytesUpdated;
	int ReceiveComplete;
	int TransmitComplete;
} HandlerInfo;

extern uint32_t uwTick;
extern HAL_TickFreqTypeDef uwTickFreq;

void SysTick_Handler(void* ref);
void HAL_IncTick(void);
uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t Delay);

void GPIO_Interrupt0(void *ref);
void Uart0ReceiveInterruptHandler(void *CallBackRef, unsigned int EventData);
void I2CSendHandler(XIic *InstancePtr);
void I2CRecvHandler(void *CallbackRef, int ByteCount);
void I2CStatusHandler(void *CallbackRef, int Status);


#endif /* SRC_INTERRUPTHANDLERS_H_ */
