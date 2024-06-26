#include "LSP.h"
#ifdef USE_SONY
#include "main.h"
#include "Sony.h"

#include "GPIO.h"
#include "GPIO_Ports.h"




void SonyCommand(uint8_t command) {
	switch(command) {
		case 0:
			DirectionPortC(Sony0_Pin, GPIO_DIRECTION_INPUT);
			DirectionPortC(Sony1_Pin, GPIO_DIRECTION_INPUT);
			DirectionPortC(Sony2_Pin, GPIO_DIRECTION_INPUT);
			DirectionPortC(Sony3_Pin, GPIO_DIRECTION_INPUT);
			DirectionPortC(Sony4_Pin, GPIO_DIRECTION_INPUT);
			DirectionPortC(Sony5_Pin, GPIO_DIRECTION_INPUT);
			DirectionPortC(Sony6_Pin, GPIO_DIRECTION_INPUT);
			DirectionPortD(Sony7_Pin, GPIO_DIRECTION_INPUT);
			DirectionPortB(Sony8_Pin, GPIO_DIRECTION_INPUT);
		/*
			DirectionPortC(Sony0_Pin, GPIO_DIRECTION_OUTPUT);
			DirectionPortC(Sony1_Pin, GPIO_DIRECTION_OUTPUT);
			DirectionPortC(Sony2_Pin, GPIO_DIRECTION_OUTPUT);
			DirectionPortC(Sony3_Pin, GPIO_DIRECTION_OUTPUT);
			DirectionPortC(Sony4_Pin, GPIO_DIRECTION_OUTPUT);
			DirectionPortC(Sony5_Pin, GPIO_DIRECTION_OUTPUT);
			DirectionPortC(Sony6_Pin, GPIO_DIRECTION_OUTPUT);
			DirectionPortD(Sony7_Pin, GPIO_DIRECTION_OUTPUT);
			DirectionPortB(Sony8_Pin, GPIO_DIRECTION_OUTPUT);
		
			HAL_GPIO_WritePin(GPIOC, Sony0_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Sony1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Sony2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Sony3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Sony4_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Sony5_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Sony6_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, Sony7_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, Sony8_Pin, GPIO_PIN_SET);
		*/
		break;
		case 1:// volume up
			DirectionPortC(Sony6_Pin, GPIO_DIRECTION_OUTPUT);
		break;
		case 2:// volume down
			DirectionPortC(Sony5_Pin, GPIO_DIRECTION_OUTPUT);
		break;
		case 3:// mute
			DirectionPortC(Sony1_Pin, GPIO_DIRECTION_OUTPUT);
		break;
		case 4:// track up
			DirectionPortC(Sony4_Pin, GPIO_DIRECTION_OUTPUT);
		break;
		case 5:// track down
			DirectionPortC(Sony3_Pin, GPIO_DIRECTION_OUTPUT);
		break;
		case 6:// source
			DirectionPortC(Sony0_Pin, GPIO_DIRECTION_OUTPUT);
		break;
		case 7:// preset up
			//shift 
			//DirectionPortC(Sony4_Pin, GPIO_DIRECTION_OUTPUT);	
		break;
		case 8:// preset down
			//shift
			//DirectionPortC(Sony3_Pin, GPIO_DIRECTION_OUTPUT);	
		 break;
		case 9:// display
			DirectionPortC(Sony2_Pin, GPIO_DIRECTION_OUTPUT);
		break;
		case 10:// select
			DirectionPortD(Sony7_Pin, GPIO_DIRECTION_OUTPUT);
		break;
		case 11:// band
			DirectionPortB(Sony8_Pin, GPIO_DIRECTION_OUTPUT);
		break;
		case 12:// off
			//DirectionPortC(Sony0_Pin, GPIO_DIRECTION_OUTPUT);
		break;
		default:
			break;		
	}
}

int8_t DirectionPortB(uint16_t pin, uint8_t direction) {
	GPIO_InitTypeDef GPIO_InitStruct;
	if(direction == GPIO_DIRECTION_INPUT) { // input mode
		GPIO_InitStruct.Pin = pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	} else { // output mode
		GPIO_InitStruct.Pin = pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOB, pin, GPIO_PIN_RESET);
	}
	return 0;
}

int8_t DirectionPortC(uint16_t pin, uint8_t direction) {
	GPIO_InitTypeDef GPIO_InitStruct;
	if(direction == GPIO_DIRECTION_INPUT) { // input mode
		GPIO_InitStruct.Pin = pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	} else { // output mode
		GPIO_InitStruct.Pin = pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOC, pin, GPIO_PIN_RESET);
	}
	return 0;
}

int8_t DirectionPortD(uint16_t pin, uint8_t direction) {
	GPIO_InitTypeDef GPIO_InitStruct;
	if(direction == GPIO_DIRECTION_INPUT) { // input mode
		GPIO_InitStruct.Pin = pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	} else { // output mode
		GPIO_InitStruct.Pin = pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOD, pin, GPIO_PIN_RESET);
	}
	return 0;
}

#endif // USE_SONY

