/*
 * InterruptHandlers.c
 *
 *  Created on: Jun 2, 2021
 *      Author: karl.yamashita
 */


#include "main.h"
#include "InterruptHandlers.h"

extern XUartLite UartLite;
extern HandlerInfo handlerInfo; // I2C

uint32_t uwTick;
HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;  /* 1KHz */

/*
 * Description: System Tick
 *
 *
 */
void SysTick_Handler(void* ref){
	HAL_IncTick();
	TimerCallbackIncrement();
}

void HAL_IncTick(void)
{
  uwTick += uwTickFreq;
}

uint32_t HAL_GetTick(void)
{
  return uwTick;
}

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
void GPIO_Interrupt0(void *ref){
	PrintLine("GPIO Interrupt");
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
void Uart0ReceiveInterruptHandler(void *CallBackRef, unsigned int EventData){
	HAL_StatusTypeDef HAL_Status;
	uint8_t buffer[32] = {0};

	if(!XUartLite_IsReceiveEmpty(XPAR_UARTLITE_0_BASEADDR)){
		XUartLite_Recv(&UartLite , buffer, 1);

		HAL_Status = UartAddCharToBuffer(UART_PORT_0, (char*)buffer);
		if(HAL_Status != HAL_OK){
			// TODO - check HAL Status and act accordingly. If buffer overflows then try to increase character buffer size.
		}
	}
}

/*****************************************************************************/
/**
* This Send handler is called asynchronously from an interrupt
* context and indicates that data in the specified buffer has been sent.
*
* @param	InstancePtr is not used, but contains a pointer to the IIC
*		device driver instance which the handler is being called for.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void I2CSendHandler(XIic *InstancePtr)
{
	handlerInfo.TransmitComplete = 0;
}

/*****************************************************************************/
/**
* This receive handler is called asynchronously from an interrupt context and
* and indicates that data in the specified buffer was received. The byte count
* should equal the byte count of the buffer if all the buffer was filled.
*
* @param	CallBackRef is a pointer to the IIC device driver instance which
*		the handler is being called for.
* @param	ByteCount indicates the number of bytes remaining to be received of
*		the requested byte count. A value of zero indicates all requested
*		bytes were received.
*
* @return	None.
*
* @notes	None.
*
****************************************************************************/
void I2CRecvHandler(void *CallbackRef, int ByteCount)
{
	handlerInfo.RemainingRecvBytes = ByteCount;
	handlerInfo.RecvBytesUpdated = TRUE;
	handlerInfo.ReceiveComplete = 0;
}

/*****************************************************************************/
/**
* This status handler is called asynchronously from an interrupt context and
* indicates that the conditions of the IIC bus changed. This  handler should
* not be called for the application unless an error occurs.
*
* @param	CallBackRef is a pointer to the IIC device driver instance which the
*		handler is being called for.
* @param	Status contains the status of the IIC bus which changed.
*
* @return	None.
*
* @notes	None.
*
****************************************************************************/
void I2CStatusHandler(void *CallbackRef, int Status)
{
	handlerInfo.EventStatus |= Status;
	handlerInfo.EventStatusUpdated = TRUE;
}



