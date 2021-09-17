/*
 * PeripheralsInit.h
 *
 *  Created on: Mar 12, 2021
 *      Author: karl.yamashita
 */

#ifndef PERIPHERALSINIT_H_
#define PERIPHERALSINIT_H_

#define ADC_SAMPLE_BUF_SIZE     64
#define SEQ_0 0
#define SEQ_1 1
#define SEQ_2 2
#define SEQ_3 3

#define I2C1_SLAVE_ADDRESS 0x22

void SystemTickInit(void);
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART0_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_TIM1_Init(void);
void MX_I2C0_Init(void);
void MX_I2C1_Init(void);
void MX_ADC0_Init(void);
void MX_EEPROM_Init(void);
void MX_WDOG_Init(void);


#endif /* PERIPHERALSINIT_H_ */
