#include "LSP.h"
#ifdef USE_TTP229_LSF

#ifndef TTP229_LSF_H
#define TTP229_LSF_H

#include "main.h"

#define TTP229_LSF_ADDRESS 0xAE

extern uint8_t i2cData[2];

HAL_I2C_StateTypeDef GetButtonsPressed(void);

#endif // TTP229_LSF_H
#endif // USE_TTP229_LSF
