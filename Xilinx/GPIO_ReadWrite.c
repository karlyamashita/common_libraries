/*
 * GPIO_ReadWrite.c

 *
 *  Created on: Jul 27, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "GPIO_ReadWrite.h"


extern XGpio Gpio_0; /* The Instance of the GPIO Driver */
extern XGpio Gpio_1; /* The Instance of the GPIO Driver */
extern XGpio Gpio_2; /* The Instance of the GPIO Driver */



/*
 * Description:
 *
 *
 *
 */
void GPIO_Read(XGpio *gpio, uint8_t channel, uint32_t *pins_OUT){
	*pins_OUT = XGpio_DiscreteRead(gpio, channel);
}

/*
 * Description: writes value to the pins
 *
 *
 */
void GPIO_Write(XGpio *gpio, uint8_t channel, uint32_t *pins){
	XGpio_DiscreteWrite(gpio, channel, (uint32_t)pins);
}

/*
 * Description: Sets the pins
 *
 *
 */
void GPIO_Set(XGpio *gpio, uint8_t channel, uint32_t *pins){
	XGpio_DiscreteSet(gpio, channel, (uint32_t)pins);
}

/*
 * Description: Clears the pins
 * Input: The GPIO instance and the pin mask
 *
 */
void GPIO_Clear(XGpio *gpio, uint8_t channel, uint32_t *pins){
	XGpio_DiscreteClear(gpio, channel, (uint32_t)pins);
}



