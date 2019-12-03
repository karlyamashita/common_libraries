#include "LSP.h"

#ifdef USE_CAN_FILTER
#ifndef CAN_FILTER_H
#define CAN_FILTER_H

#ifdef __STM32F1xx_HAL_CAN_H
//#include "can.h"
#endif // __STM32F1xx_HAL_CAN_H

void SetCanFilter(void);

#endif // CAN_FILTER_H
#endif // USE_CAN_FILTER
