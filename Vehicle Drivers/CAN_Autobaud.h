#ifndef CAN_AUTOBAUD_H
#define CAN_AUTOBAUD_H
#include "main.h"
#include "stm32f1xx_hal.h"


#define COUNT_BITRATE 6

typedef struct Bitrate_stm {
	uint8_t prescaler;
	uint32_t BS1;
	uint32_t BS2;
}Bitrate_stm;


int8_t CAN_Autobaud(void);

#endif // CAN_AUTOBAUD_H

