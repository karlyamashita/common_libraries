#include "LSP.h"
#ifdef USE_CHIME_GENERATOR
#include "main.h"
#include "ChimeGenerator.h"

#include "dac.h"
#include "math.h"

#include "gpio.h"
#include "gpio_ports.h"
#include "tim.h"

uint8_t bufferOut[128];
uint8_t dacAmplitude = 30;
uint8_t decayFlag = 0;


/*
Call ChimeInit()
Call DoChimeDecay() in polling routine.
Call UpdateChimeSpeed(CHIME_SPEED_SLOW) to change speed.
Call ChimeStart() to start the chimes
Call ChimeStop() to stop the chimes

*/


void ChimeInit(void) {
	ChimeAmplitude(dacAmplitude); // starting amplitude
	UpdateChimeTone(FREQ_700); // the frequency to start
	
	UpdateChimeSpeed(CHIME_SPEED_FAST);
	
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)bufferOut, 64, DAC_ALIGN_8B_R);
}

void ChimeStart(void) {
	HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);
}

void ChimeStop(void) {
	HAL_TIM_Base_Stop_IT(&htim6);
	HAL_TIM_Base_Stop_IT(&htim7);
	HAL_DAC_Stop(&hdac, DAC_CHANNEL_2);
	dacAmplitude = 30;
}

void DoChimeDecay(void) {
	if(decayFlag){
		DecayAmplitude();
		decayFlag = 0;
	}
}

// based off TIM7
void DecayAmplitude(void) {
	dacAmplitude -= 1;
	if(dacAmplitude < 5) {
		dacAmplitude = 30;
	}
	ChimeAmplitude(dacAmplitude);
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)bufferOut, 64, DAC_ALIGN_8B_R);
}

/*
function: Updates the amplitude of sine wave
input: The amplitude
output: none
*/
void ChimeAmplitude(uint8_t dacAmplitude) {
	uint16_t i;
	for(i = 0; i < 128; i++) {
		bufferOut[i] = (int) (dacAmplitude*sin(6.28*i/128)) + 128;
	}
}

/*
function: The timer interrupt callback. This will generate the frequency depending on interrupt timing
input: timer handle
output: none
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM7){ // amplitude
		//DecayAmplitude();
		decayFlag = 1;
	}
}

/*
function: The frequency to produce
input: the frequency. FREQ_700, FREQ_1200
output: none
*/
void UpdateChimeTone(uint32_t tone) {
	htim6.Instance = TIM6;
  htim6.Init.Prescaler = 4;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = tone;
	HAL_TIM_Base_Init(&htim6);
}

/*
function: Adjust the chime speed.
input: CHIME_SPEED_SLOW, CHIME_SPEED_MEDIUM, CHIME_SPEED_FAST
output: none
*/
void UpdateChimeSpeed(uint32_t speed) {
	htim7.Instance = TIM7;
  htim7.Init.Prescaler = speed;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 65535;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}

#endif // USE_CHIME_GENERATOR
