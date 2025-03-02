/*
 * ADC_Buffer_Handler.h
 *
 *  Created on: Mar 1, 2025
 *      Author: codek
 */

#ifndef INC_ADC_BUFFER_HANDLER_STM32_H_
#define INC_ADC_BUFFER_HANDLER_STM32_H_



enum
{
	ADC_NO_ERROR,
	ADC_ERROR_INSTANCE_FULL
};

#define ADC_INSTANCE_SIZE 2


typedef struct
{
	uint16_t *buffer;
	uint32_t bufferSize;
	uint16_t average;
	RING_BUFF_STRUCT ptr;
}ADC_Buffer_Instance_t;

typedef struct
{
	ADC_HandleTypeDef *hadc;
	uint16_t *DMA_Buffer;
	ADC_Buffer_Instance_t *instance[ADC_INSTANCE_SIZE];
	uint32_t adcInstanceSize;
	uint32_t errorStatus;
	bool rdyFlag;
}ADC_Buffer_t;



int ADC_Buffer_Register(ADC_Buffer_t *adcBuffer, ADC_Buffer_Instance_t *adc_i);
int ADC_StartDMA(ADC_Buffer_t *adc);
int ADC_ConversionCheck(ADC_Buffer_t *adcBuffer);
uint16_t ADC_GetAverage(ADC_Buffer_Instance_t *adc_i);



#endif /* INC_ADC_BUFFER_HANDLER_STM32_H_ */
