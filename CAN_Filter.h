#ifndef CAN_FILTER_H
#define CAN_FILTER_H

#include "can.h"

void SetCanFilter(void);
void ClearCAN_Interrupts(CAN_HandleTypeDef *CanHandle);
#endif // CAN_FILTER_H
