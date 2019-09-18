#ifndef CHIME_GENERATOR_H
#define CHIME_GENERATOR_H
#include "main.h"


/*
These frequencies are based off TIM6 and DMA with buffer array of 256
*/
#define FREQ_700 400 // Prescaler=4, period=400
#define FREQ_1200 225 // prescaler=4, period=225


/*
Decay
Slow = 50
medium = 25
fast = 20
*/
#define CHIME_SPEED_SLOW 80
#define CHIME_SPEED_MEDIUM 50
#define CHIME_SPEED_FAST 20

void ChimeInit(void);
void ChimeStart(void);
void ChimeStop(void);

void DoChimeDecay(void);
void DecayAmplitude(void);

void ChimeAmplitude(uint8_t dacAmplitude);
void UpdateChimeTone(uint32_t tone);
void UpdateChimeSpeed(uint32_t speed);

#endif // CHIME_GENERATOR_H
