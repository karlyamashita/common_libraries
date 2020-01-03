#include "LSP.h"
#ifdef USE_GPIO_PORTS
#include "main.h"
#include "GPIO_Ports.h"

#ifdef STM32F407xx
#include "stm32f4xx_hal_gpio.h"
#endif


#ifdef __STM32F1xx_HAL_CAN_H
#include "GPIO.h"
#endif

/*
 * Port On/Off/Toggle
 *
 *
 */
// port A
int8_t PortA_On (uint32_t pin) {
		HAL_GPIO_WritePin(GPIOA, pin, GPIO_PIN_SET);
  return 0;
}

int8_t PortA_Off (uint32_t pin) {
	HAL_GPIO_WritePin(GPIOA, pin, GPIO_PIN_RESET);
  return 0;
}

int8_t PortA_Toggle (uint32_t pin) {
	HAL_GPIO_TogglePin(GPIOA, pin);
  return 0;
}

// port B
int8_t PortB_On (uint32_t pin) {
		HAL_GPIO_WritePin(GPIOB, pin, GPIO_PIN_SET);
  return 0;
}

int8_t PortB_Off (uint32_t pin) {
	HAL_GPIO_WritePin(GPIOB, pin, GPIO_PIN_RESET);
  return 0;
}

int8_t PortB_Toggle (uint32_t pin) {
	HAL_GPIO_TogglePin(GPIOB, pin);
  return 0;
}

// port C
int8_t PortC_On (uint32_t pin) {
		HAL_GPIO_WritePin(GPIOC, pin, GPIO_PIN_SET);
  return 0;
}

int8_t PortC_Off (uint32_t pin) {
	HAL_GPIO_WritePin(GPIOC, pin, GPIO_PIN_RESET);
  return 0;
}

int8_t PortC_Toggle (uint32_t pin) {
	HAL_GPIO_TogglePin(GPIOC, pin);
  return 0;
}

#if defined(__STM32F1xx_HAL_CAN_H)
// port D
int8_t PortD_On (uint32_t pin) {
		HAL_GPIO_WritePin(GPIOD, pin, GPIO_PIN_SET);
  return 0;
}

int8_t PortD_Off (uint32_t pin) {
	HAL_GPIO_WritePin(GPIOD, pin, GPIO_PIN_RESET);
  return 0;
}

int8_t PortD_Toggle (uint32_t pin) {
	HAL_GPIO_TogglePin(GPIOD, pin);
  return 0;
}
#endif // __STM32F1xx_HAL_CAN_H

#ifdef STM32F407xx
// port D
int8_t PortD_On (uint32_t pin) {
		HAL_GPIO_WritePin(GPIOD, pin, GPIO_PIN_SET);
  return 0;
}

int8_t PortD_Off (uint32_t pin) {
	HAL_GPIO_WritePin(GPIOD, pin, GPIO_PIN_RESET);
  return 0;
}

int8_t PortD_Toggle (uint32_t pin) {
	HAL_GPIO_TogglePin(GPIOD, pin);
  return 0;
}

// port E
int8_t PortE_On (uint32_t pin) {
		HAL_GPIO_WritePin(GPIOE, pin, GPIO_PIN_SET);
  return 0;
}

int8_t PortE_Off (uint32_t pin) {
	HAL_GPIO_WritePin(GPIOE, pin, GPIO_PIN_RESET);
  return 0;
}

int8_t PortE_Toggle (uint32_t pin) {
	HAL_GPIO_TogglePin(GPIOE, pin);
  return 0;
}
#endif

/*
 * Read pins
 *
 *
 */
// Functions to Read pin
int8_t ReadPortA(uint16_t pin) {
	return HAL_GPIO_ReadPin(GPIOA, pin);
}

int8_t ReadPortB(uint16_t pin) {
	return HAL_GPIO_ReadPin(GPIOB, pin);
}

int8_t ReadPortC(uint16_t pin) {
	return HAL_GPIO_ReadPin(GPIOC, pin);
}

#ifdef STM32F407xx
int8_t ReadPortD(uint16_t pin) {
	return HAL_GPIO_ReadPin(GPIOD, pin);
}

int8_t ReadPortE(uint16_t pin) {
	return HAL_GPIO_ReadPin(GPIOE, pin);
}
#endif// STM32F407xx

#ifdef __STM32F1xx_HAL_CAN_H
int8_t ReadPortD(uint16_t pin) {
	return HAL_GPIO_ReadPin(GPIOD, pin);
}
#endif
#endif // USE_GPIO_PORTS
