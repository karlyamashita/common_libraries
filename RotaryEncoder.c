#include "LSP.h"
#ifdef USE_ROTARY_ENCODER
#include "main.h"
#include "RotaryEncoder.h"
#include "tim.h"

uint16_t timEncoder = 0;

void RotaryEncoderCountUpdate(void) {
#ifdef TIM3
	timEncoder = TIM3->CNT; // this timer can be any timer. For this project we are using TIM3 with encoder selected
#endif // TIM3
}

uint16_t GetRotaryEncoderCount(void) {
	return timEncoder;
}

#endif // USE_ROTARY_ENCODER
