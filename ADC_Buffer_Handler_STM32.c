/*
 * ADC_Buffer_Handler.c
 *
 *  Created on: Mar 1, 2025
 *      Author: codek
 */

#ifndef INC_ADC_BUFFER_HANDLER_C_
#define INC_ADC_BUFFER_HANDLER_C_

#include "main.h"

/*
 * Description: Register each ADC instance (channel)
 * input:
 * return: error status
 */
int ADC_Buffer_Register(ADC_Buffer_t *adcBuffer, ADC_Buffer_Instance_t *adc_i)
{
	int i = 0;

	for(i = 0; i < adcBuffer->adcInstanceSize; i++)
	{
		if(adcBuffer->instance[i] == NULL)
		{
			adcBuffer->instance[i] = adc_i;
			return 0;
		}
	}

	return ADC_ERROR_INSTANCE_FULL;
}

/*
 * Description: Start the ADC DMA. Only need to call once.
 * 				Note: ADC DMA should be in circular mode
 * return: HAL Status
 */
int ADC_StartDMA(ADC_Buffer_t *adc)
{
	HAL_StatusTypeDef hal_status;

	hal_status = HAL_ADC_Start_DMA(adc->hadc, (uint32_t*)adc->DMA_Buffer, adc->adcInstanceSize);

	return hal_status;
}

/*
 * Description: Checks if conversion done. Call from polling routine.
 * 				Copy DMA data to buffer. Calculate the average for each instance.
 */
int ADC_ConversionCheck(ADC_Buffer_t *adcBuffer)
{
	int status = NO_ERROR;
	int i, n = 0;

	if(adcBuffer->rdyFlag)
	{
		adcBuffer->rdyFlag = false; // reset
		// copy DMA data to buffer
		for(i = 0; i < adcBuffer->adcInstanceSize; i++)
		{
			adcBuffer->instance[i]->buffer[adcBuffer->instance[i]->ptr.index_IN] = adcBuffer->DMA_Buffer[i];
			RingBuff_Ptr_Input(&adcBuffer->instance[i]->ptr, adcBuffer->adcInstanceSize);
		}
		// Calculate average for each instance
		for(i = 0; i < adcBuffer->adcInstanceSize; i++)
		{
			// add total for buffer size
			for(n = 0; n < adcBuffer->instance[i]->bufferSize; n++)
			{
				adcBuffer->instance[i]->average += adcBuffer->instance[i]->buffer[n];
			}
			// divide to get average
			adcBuffer->instance[i]->average /= adcBuffer->instance[i]->bufferSize;
		}
	}

	return status;
}

/*
 * Description: Get ADC register average. User will still need to convert return value to voltage
 * input: adc instance
 * output: none
 * return: adc 16bit value
 */
uint16_t ADC_GetAverage(ADC_Buffer_Instance_t *adc_i)
{
	int status = NO_ERROR;

	return adc_i->average;

	return status;
}

int ADC_SetRdyFlag(ADC_Buffer_t *adcBuffer)
{
	int status = NO_ERROR;

	adcBuffer->rdyFlag = true;

	return status;
}

/*

1. Create DMA buffer and add to adc1_buffer
uint16_t dma_buffer[ADC_INSTANCE_SIZE];
ADC_Buffer_t adc1_buffer =
{
	.hadc = &hadc1,
	.DMA_Buffer = dma_buffer,
	.adcInstanceSize = ADC_INSTANCE_SIZE,
	.errorStatus = ADC_NO_ERROR
};

2. Create ADC instance for each ADC channel
#define ADC_INPUT1_SIZE 8
uint16_t adc_input1Buffer[ADC_INPUT1_SIZE];
ADC_Buffer_Instance_t adc_input1 =
{
	.buffer = adc_input1Buffer,
	.bufferSize = ADC_INPUT1_SIZE,
	.ptr.SkipOverFlow = true
};

3. Register each data structure before while loop
ADC_Buffer_Register(&adc1_buffer, &adc_input1);

4. Start ADC DMA
ADC_StartDMA(&adc1_buffer);

 */



#endif /* INC_ADC_BUFFER_C_ */
