/*
 * GPIO_ReadWrite.h
 *
 *  Created on: Jul 27, 2021
 *      Author: karl.yamashita
 */

#ifndef SRC_GPIO_READWRITE_H_
#define SRC_GPIO_READWRITE_H_



void GPIO_Read(XGpio *gpio, uint8_t channel, uint32_t *pins_OUT);
void GPIO_Write(XGpio *gpio, uint8_t channel, uint32_t *pins);
void GPIO_Set(XGpio *gpio, uint8_t channel, uint32_t *pins);
void GPIO_Clear(XGpio *gpio, uint8_t channel, uint32_t *pins);




#endif /* SRC_GPIO_READWRITE_H_ */
