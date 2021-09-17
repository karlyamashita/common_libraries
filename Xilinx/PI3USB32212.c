/*
 * PI3USB32212.c
 *
 *  Created on: Jun 28, 2021
 *      Author: karl.yamashita
 */


#include "main.h"
#include "PI3USB32212.h"

extern XGpio Gpio_1;


/*
 * Description: Sets the IO's for PD, OE, SEL1 and SEL2
 *
 * Input: The function to execute
 * Output: none
 *
 */
void PI3USB32212_SetMux(uint32_t function){
	uint32_t pinSet = 0;
	uint32_t pinClear = 0;
	const uint8_t CHANNEL = 1;

	switch(function){
	case 1:
		pinClear |= 1 << SSMUX1_PD;
		pinClear |= 1 << SSMUX1_SEL1;
		break;
	case 2:
		pinClear |= 1 << SSMUX1_PD;
		pinSet |= 1 << SSMUX1_SEL1;
		break;
	case 3:
		pinSet |= 1 << SSMUX1_PD;
		break;
	case 4:
		pinClear |= 1 << SSMUX1_OE;
		break;
	case 5:
		pinSet |= 1 << SSMUX1_OE;
		pinSet |= 1 << SSMUX1_PD;
		break;
	case 6:
		pinClear |= 1 << SSMUX1_OE;
		pinClear |= 1 << SSMUX1_SEL2;
		break;
	case 7:
		pinClear |= 1 << SSMUX1_OE;
		pinSet |= 1 << SSMUX1_SEL2;
		break;
	default:
		break;
	}

	XGpio_DiscreteSet(&Gpio_1, CHANNEL, pinSet);
	XGpio_DiscreteClear(&Gpio_1, CHANNEL, pinClear);
}

/*
 *
 *
 *
 */
void PI3USB32212_SetDemux(uint32_t function){
	uint8_t pinSet = 0;
	uint8_t pinClear = 0;
	const uint8_t CHANNEL = 1;

	switch(function){
	case 1:
		pinClear |= 1 << SSMUX2_PD;
		pinClear |= 1 << SSMUX2_SEL1;
		break;
	case 2:
		pinClear |= 1 << SSMUX2_PD;
		pinSet |= 1 << SSMUX2_SEL1;
		break;
	case 3:
		pinSet |= 1 << SSMUX2_PD;
		break;
	case 4:
		pinClear |= 1 << SSMUX2_OE;
		break;
	case 5:
		pinSet |= 1 << SSMUX2_OE;
		pinSet |= 1 << SSMUX2_PD;
		break;
	case 6:
		pinClear |= 1 << SSMUX2_OE;
		pinClear |= 1 << SSMUX2_SEL2;
		break;
	case 7:
		pinClear |= 1 << SSMUX2_OE;
		pinSet |= 1 << SSMUX2_SEL2;
		break;
	default:
		break;
	}

	XGpio_DiscreteSet(&Gpio_1, CHANNEL, pinSet);
	XGpio_DiscreteClear(&Gpio_1, CHANNEL, pinClear);
}
