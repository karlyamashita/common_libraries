#include "LSP.h"
#ifdef USE_BD37531FV
/*
ROHM BD37531FV and BD37534FV Sound Processor with built-in 3-band equalizer.
Use with Chrysler amplifier replacement
*/

#include "main.h"
#include "BD37531FV.h"

#include "I2C.h"

// include BSP file
#include "BSP.h"

#include "RadioAudioStatus.h"
#include "KickTheDog.h"
#include "TimerCallback.h"
#include "mTimers.h"
#include "FlashProgram.h"

/*
Notes:
Chrysler volume value range: 0-38
Chrysler Bass,mid, treble, fade, balance range: 1-19, 10 is middle.
Need to define USING_I2C1 or USING_I2C2
*/

/*
uint8_t volumeTable[2][VOL_MAX_RANGE] = {
	{0, 1 , 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38},
	{INFINITE, ATT_79, ATT_71, ATT_69, ATT_67, ATT_65, ATT_63, ATT_61, ATT_59, ATT_57, ATT_56, // 0-10
	ATT_55, ATT_53, ATT_51, ATT_49, ATT_47, ATT_45, ATT_43, ATT_41, ATT_39, ATT_37, // 11-20
	ATT_35, ATT_33, ATT_31, ATT_29, ATT_27, ATT_25, ATT_23, ATT_21, ATT_19, ATT_17, // 21-30
	ATT_15, ATT_13, ATT_11, ATT_09, ATT_07, ATT_05, ATT_03, VOL_GAIN_00} // 31-38
};
*/
uint8_t volumeTable[2][VOL_MAX_RANGE] = {
	{0, 1 , 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38},
	{INFINITE, ATT_79, ATT_71, ATT_69, ATT_67, ATT_65, ATT_63, ATT_61, ATT_59, ATT_57, ATT_54, // 0-10
	ATT_51, ATT_47, ATT_44, ATT_41, ATT_37, ATT_34, ATT_31, ATT_27, ATT_24, ATT_21, // 11-20
	ATT_19, ATT_17, ATT_15, ATT_13, ATT_11, ATT_09, ATT_07, ATT_05, ATT_03, ATT_01, // 21-30
	VOL_GAIN_01, VOL_GAIN_03, VOL_GAIN_05, VOL_GAIN_07, VOL_GAIN_09, VOL_GAIN_11, VOL_GAIN_13, VOL_GAIN_15} // 31-38
};


uint8_t UconnectVolumeTable[2][VOL_MAX_RANGE] = {
	{0, 1 , 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38},
	{INFINITE, ATT_79, ATT_73, ATT_67, ATT_61, ATT_55, ATT_53, ATT_51, ATT_49, ATT_47, ATT_46, // 0-10
	ATT_45, ATT_43, ATT_41, ATT_39, ATT_37, ATT_35, ATT_33, ATT_31, ATT_29, ATT_27, // 11-20
	ATT_25, ATT_23, ATT_21, ATT_19, ATT_17, ATT_15, ATT_13, ATT_11, ATT_09, ATT_07, // 21-30
	ATT_05, ATT_03, ATT_01, VOL_GAIN_00, VOL_GAIN_01, VOL_GAIN_03, VOL_GAIN_05, VOL_GAIN_07} // 31-38
};

/*
Front = 1, Rear = 19, Center = 10
Left = 1, Right = 19
*/
uint8_t faderBalTable[2][FADE_MAX_RANGE] = {
	{1 , 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{ATT_79, ATT_24, ATT_21, ATT_18, ATT_15, ATT_12, ATT_09, ATT_06, ATT_03, // 1-9
		VOL_GAIN_00, // 10 middle
	}
};

uint8_t bassMiddleTrebleTable[2][BASS_MAX_RANGE] = {
	{1 , 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19},
	{GAIN_1, GAIN_2, GAIN_3, GAIN_4, GAIN_5, GAIN_6, GAIN_7, GAIN_8, GAIN_9, // 1-9
		GAIN_10, // 10
	 GAIN_11, GAIN_12, GAIN_13, GAIN_14, GAIN_15, GAIN_16, GAIN_17, GAIN_18, GAIN_19} // 11-19
};

InitSetup1Struct initSetup1Struct;
InitSetup2Struct initSetup2Struct;
InitSetup3Struct initSetup3Struct;
FrequencyCenter frequencyCenter;
FLASH_EraseInitTypeDef flashEraseStruct;

/*
Create a timer callback to periodically call this function until finish going through all case.
*/
void InitBD37531FV(void) {
	static uint8_t mode = 0;
	MuteAudio(true); // mute BD37531FV
	switch(mode) {
		case 0:
			GetFreqValues();
			mode++;
		break;
		case 1:
			initSetup1Struct.Status.advancedOnOff = ADVANCED_SWITCH_ON;
			initSetup1Struct.Status.advancedSwitchTimeInput = ADVANCED_SWITCH_TIME_INPUT_11_2;
			initSetup1Struct.Status.advancedSwitchTimeMute = ADVANCED_SWITCH_TIME_MUTE_0_6;
			initSetup1Struct.Status.bit2 = 1; // init to 1, do not change.
			if(SendSetup1Data(&initSetup1Struct) == HAL_I2C_STATE_READY) ++mode;
		break;
		case 2: // update BD37534 with values
			if(UpdateBD37534Frequency() == HAL_I2C_STATE_READY) ++mode; // updates Setup2 and Setup3
		break;
		case 3:
			if(SendInputSelectorData(INPUT_SELECTOR_E_FULL_DIFF, FULL_DIFF_BIAS_TYPE_SELECT_NEGATIVE) == HAL_I2C_STATE_READY) ++mode;
		break;
		case 4:
			if(SendInputGainData(MUTE_OFF, GAIN_0) == HAL_I2C_STATE_READY) ++mode; // Gain can't be greater than zero!
		break;
		case 5:
			if(SendVolFaderSubData(FADER_FRONT_LEFT_ADDR, 10) == HAL_I2C_STATE_READY) ++mode;
		break;
		case 6:
			if(SendVolFaderSubData(FADER_FRONT_RIGHT_ADDR, 10) == HAL_I2C_STATE_READY) ++mode;
		break;
		case 7:
			if(SendVolFaderSubData(FADER_REAR_LEFT_ADDR, 10) == HAL_I2C_STATE_READY) ++mode;
		break;
		case 8:
			if(SendVolFaderSubData(FADER_REAR_RIGHT_ADDR, 10) == HAL_I2C_STATE_READY) ++mode;
		break;
		case 9:
			if(SendVolFaderSubData(FADER_SUBWOOFER_ADDR, GetSubwooferValue_LAST()) == HAL_I2C_STATE_READY) ++mode;
		break;
		case 10:
			if(SendLoudnessData(LOUDNESS_HI_CUT_1, GetLoudnessValue_LAST()) == HAL_I2C_STATE_READY) {
				if(GetLoudnessValue_LAST() == GAIN_0 ) { 
					LED_Off(LED2_Red_Pin);
				} else { // GAIN_24
					LED_On(LED2_Red_Pin);
				}
				++mode;
			}
		break;
		case 11:
			MuteAudio(false); 
			EnableTimerCallback(InitBD37531FV, false); // done init
			mode = 0;	
		break;
		default:
			mode = 0;
		break;
	}
}

/*

*/
HAL_I2C_StateTypeDef SendSetup1Data(InitSetup1Struct *initSetup1Struct) {
	uint8_t dataBytes[2];
		
	dataBytes[0] = INIT_SETUP_1_ADDR;
	dataBytes[1] = initSetup1Struct->Byte.byte0;
	
#ifdef USING_I2C1
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY;
	}
#endif
#ifdef USING_I2C2
	if(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY; 
	}
#endif
	return HAL_I2C_STATE_READY;
}

/*

*/
HAL_I2C_StateTypeDef SendSetup2Data(InitSetup2Struct *initSetup2Struct) {
	uint8_t dataBytes[2];
	
	dataBytes[0] = INIT_SETUP_2_ADDR;
	dataBytes[1] = initSetup2Struct->Byte.byte0;
	
#ifdef USING_I2C1
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY; 
	}
#endif
#ifdef USING_I2C2
	if(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY;
	}
#endif
	return HAL_I2C_STATE_READY;
}

/*

*/
HAL_I2C_StateTypeDef SendSetup3Data(uint8_t loudness) {
	uint8_t dataBytes[2];
	InitSetup3Struct initSetup3Struct;
	memset(&initSetup3Struct, 0, sizeof(initSetup3Struct));// clear struct
	
	initSetup3Struct.Status.loudness = loudness;
	initSetup3Struct.Status.bit0 = 1; // init to 1, do not change.
	
	dataBytes[0] = INIT_SETUP_3_ADDR;
	dataBytes[1] = initSetup3Struct.Byte.byte0;
	
#ifdef USING_I2C1
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY; 
	}
#endif
#ifdef USING_I2C2
	if(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY;
	}
#endif
	return HAL_I2C_STATE_READY;
}

/*

*/
HAL_I2C_StateTypeDef SendInputSelectorData(uint8_t inputSelector, uint8_t fullDiffBiasTypeSelect) {
	uint8_t dataBytes[2];
	InputSelectorStruct inputSelectorStruct;
	memset(&inputSelectorStruct, 0, sizeof(inputSelectorStruct));
	
	inputSelectorStruct.Status.fullDiffBiasTypeSelect = fullDiffBiasTypeSelect;
	inputSelectorStruct.Status.inputSelector = inputSelector;
	
	dataBytes[0] = INPUT_SELECTOR_ADDR;
	dataBytes[1] = inputSelectorStruct.Byte.byte0;
	
#ifdef USING_I2C1
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY;
	}
#endif
#ifdef USING_I2C2
	if(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY; 
	}
#endif
	return HAL_I2C_STATE_READY;
}

/*

*/
HAL_I2C_StateTypeDef SendInputGainData(uint8_t mute, uint8_t gain) {
	uint8_t dataBytes[2];
	InputGainStruct inputGainStruct;
	memset(&inputGainStruct, 0, sizeof(inputGainStruct));
	
	inputGainStruct.Status.muteOnOff = mute;
	inputGainStruct.Status.inputGain = gain; // todo - need to validate in vehicle and do final gain adjustment
	
	dataBytes[0] = INPUT_GAIN_ADDR;
	dataBytes[1] = inputGainStruct.Byte.byte0;
	
#ifdef USING_I2C1
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY;
	}
#endif
#ifdef USING_I2C2
	if(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY;
	}
#endif
	return HAL_I2C_STATE_READY;
}

/*
function: Send Q and FO data to address. For Bass, Middle and Treble
input: address, q, fo
output: HAL return status
addresses: 41,44,47.
*/
HAL_I2C_StateTypeDef SendQData(uint8_t address, uint8_t q, uint8_t fo) {
	uint8_t dataBytes[2];
	QFactorStruct qFactorStruct;
	memset(&qFactorStruct, 0, sizeof(qFactorStruct));
	
	qFactorStruct.Status.qFactor = q;
	qFactorStruct.Status.freq = fo;
	
	dataBytes[0] = address;
	dataBytes[1] = qFactorStruct.Byte.byte0;
	
#ifdef USING_I2C1
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY;
	}
#endif
#ifdef USING_I2C2
	if(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY; 
	}
#endif
	return HAL_I2C_STATE_READY;
}

/*
function: Send gain value and boost/cut to address. For Bass, Middle, Treble
input: address, gain value, Boost or Cut
output: HAL return status
addresses: 51, 54, 57. 
*/
HAL_I2C_StateTypeDef SendGainData(uint8_t address, uint8_t gain, uint8_t boostCut) {
	uint8_t dataBytes[2];
	BassMiddleTrebleGainStruct bassMiddleTrebleGainStruct;
	memset(&bassMiddleTrebleGainStruct, 0, sizeof(bassMiddleTrebleGainStruct));
	
	bassMiddleTrebleGainStruct.Status.boostCut = boostCut;
	bassMiddleTrebleGainStruct.Status.gain = gain;
	
	dataBytes[0] = address;
	dataBytes[1] = bassMiddleTrebleGainStruct.Byte.byte0;
	
#ifdef USING_I2C1
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY; 
	}
#endif
#ifdef USING_I2C2
	if(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY; 
	}
#endif
	return HAL_I2C_STATE_READY;
}

/*
function: Send Loudness gain and hicut
input: hicut and gain
output: HAL return status
addresses: 75
*/
HAL_I2C_StateTypeDef SendLoudnessData(uint8_t hicut, uint8_t gain) {
	uint8_t dataBytes[2];
	LoudnessGainStruct loudnessGainStruct;
	memset(&loudnessGainStruct, 0, sizeof(loudnessGainStruct));
	
	loudnessGainStruct.Status.loudnessHiCut = hicut;
	loudnessGainStruct.Status.loudnessGain = gain;
	
	dataBytes[0] = LOUDNESS_GAIN_ADDR;
	dataBytes[1] = loudnessGainStruct.Byte.byte0;
	
#ifdef USING_I2C1
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY; 
	}
#endif
#ifdef USING_I2C2
	if(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY;
	}
#endif
	return HAL_I2C_STATE_READY;
}

/*
function: Send Volume/Fader Gain/Att. 
input: gain/att 
output: HAL return status
addresses: 20, 28, 29, 2A, 2B, 2C, 30
*/
uint8_t VolDataBytes[2];
HAL_I2C_StateTypeDef SendVolFaderSubData(uint8_t address, uint8_t gainAtt){
	
	uint8_t gainAttValue = 0;
	uint8_t i = 0; // TODO, make sure index doesn't go out of bound

#ifdef 	USING_I2C1
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY){
		return HAL_I2C_STATE_BUSY;
	}	
#endif
#ifdef USING_I2C2
	if(HAL_I2C_GetState(&hi2c2) == HAL_I2C_STATE_BUSY){
		return HAL_I2C_STATE_BUSY;
	}
#endif
	
	VolDataBytes[0] = address; // address to send gainAtt data to
	
	switch(address) {
		case VOLUME_GAIN_ADDR:
		case FADER_SUBWOOFER_ADDR:
			do {
				gainAttValue = volumeTable[1][i];
			}while(volumeTable[0][i++] != gainAtt) ; // go through array until match
		break;
		case MIXING_ADDR:
			do {
				gainAttValue = UconnectVolumeTable[1][i];
			}while(UconnectVolumeTable[0][i++] != gainAtt) ; // go through array until match	
		break;
		case FADER_FRONT_LEFT_ADDR:// no breaks for fader control. Uses same table array throughout
		case FADER_FRONT_RIGHT_ADDR:
		case FADER_REAR_LEFT_ADDR:
		case FADER_REAR_RIGHT_ADDR:
			do {
				gainAttValue = faderBalTable[1][i];
			}while(faderBalTable[0][i++] != gainAtt) ; // go through array until match
		break;
		default:
			break;	
	}
	
	VolDataBytes[1] = gainAttValue; // move value to array
		
#ifdef USING_I2C1
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)VolDataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY; 
	}
#endif
#ifdef USING_I2C2
	if(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)BD37531FV_ADDRESS, (uint8_t*)VolDataBytes, 2, 100) != HAL_OK) {
		return HAL_I2C_STATE_BUSY;
	}
#endif
	return HAL_I2C_STATE_READY;
}


uint8_t programmingMode = 0;
/*
function: check if in programming mode
input: none 
output: programmingMode
*/
uint8_t GetProgrammingModeStatus(void) {
	return programmingMode;
}

void SetProgrammingModeStatus(uint8_t mode) {
	programmingMode = mode;
}

/*
function: Program Q settings
input: none 
output: none
*/
int8_t toneValue = 0;
void CheckForProgrammingMode(void) {
	static uint8_t toneMode = 0;
	static int8_t frequencyValue;
	static int8_t loudnessLastState;
	LoudnessButtonStatus loudnessButtonStatus = GetLoudnessButtonStatus();

	if(loudnessButtonStatus.shortLongPressStatus == 1) { // long pressed state
		if(programmingMode == 0 ) {
			programmingMode = 1;
		} else if(programmingMode == 3 ) {
			programmingMode = 4;
		}
		ClearLoudnessLongPress();
	}
	switch(programmingMode) {
		case 0:
			// just sit here until called upon
		break;
		case 1:
			// Set LEDs to indicate in programming mode
			LED_On(LED1_Blue_Pin);
			LED_On(LED2_Blue_Pin);
			LED_Off(LED1_Red_Pin);
			LED_Off(LED2_Red_Pin);
			loudnessLastState = GetLoudnessValue_LAST();
			programmingMode = 2;
		break;
		case 2:
			// turn off LEDs 
			if(loudnessButtonStatus.buttonState == 0) {
				LED_Off(LED1_Blue_Pin);
				LED_Off(LED2_Blue_Pin);
				programmingMode = 3;
			}	
		break;
		case 3:// select tone. We will exit on a long press on loudness button.
			switch(toneMode) {
				case 0: // choose tone
					if(toneValue != -1) {
						TIM3->CNT = toneValue;
					}
					toneValue = SetLED1_Indicator();
					if(toneValue != -1) {				
						toneMode = 1;
					}
				break;
				case 1:
					frequencyValue = Program_Q_Settings();
					if(frequencyValue != -1) { // frequency was selected.	
						frequencyCenter.Bytes.data[toneValue] = frequencyValue;
						toneMode = 2; // save value
					}
				break;
				case 2:
					// save tone and frequency value to flash memory.
					SaveFreqValues();
					// updates BD37534 with new values
					if(UpdateBD37534Frequency() == HAL_I2C_STATE_READY) toneMode = 0;
				break;
				default:
					toneMode = 0;
				break;
			}
		break;
		case 4:
			// Set LEDs to indicate end programming
			RemoveTimerCallback(FlashRedLED1);
			RemoveTimerCallback(FlashRedLED2);
		
			LED_Off(LED1_Blue_Pin);
			LED_Off(LED2_Blue_Pin);
			LED_Off(LED1_Red_Pin);
			LED_Off(LED2_Red_Pin);
		
			RemoveTimerCallback(FlashBlueLED1); // remove and recreate with new timer value
			RemoveTimerCallback(FlashBlueLED2); // remove and recreate with new timer value
			CreateTimerCallback(FlashBlueLED1, 50, REPEAT_TIMER);
			CreateTimerCallback(FlashBlueLED2, 50, REPEAT_TIMER);
			programmingMode = 5;
		break;
		case 5:
			// turn off LEDs 
			if(loudnessButtonStatus.buttonState == 0) {
				RemoveTimerCallback(FlashBlueLED1);
				RemoveTimerCallback(FlashBlueLED2);
				LED_Off(LED1_Blue_Pin);
				LED_Off(LED2_Blue_Pin);
				programmingMode = 6;
			}
		break;
		case 6:
			if(loudnessLastState != GAIN_0) {
				LED_On(LED2_Red_Pin);
			}
			programmingMode = 0; // reset state machine
			toneMode = 0;
		break;	
	}
}

/*
function: LED 2 to indicate frequency
input: none 
output: the frequency selected
*/
int8_t Program_Q_Settings(void) {
	int8_t frequencySelected = -1;
	uint8_t frequencyMode;
	uint8_t encoderDirection;
	
	LoudnessButtonStatus loudnessButtonStatus = GetLoudnessButtonStatus();
	
	CreateTimerCallback(FlashBlueLED2, 500, REPEAT_TIMER);
	CreateTimerCallback(FlashRedLED1, 500, REPEAT_TIMER);

	encoderDirection = (TIM3->CR1 >> 4) & 0x01;// get what direction
	
	if(TIM3->CNT > 4) { // over flow, only 5 selections
		if(encoderDirection == 0) { // upcounter
			TIM3->CNT = 4;
		} else { // down counter
			TIM3->CNT = 0;
		}
	} 
	frequencyMode = 4 - TIM3->CNT;
	
	switch(frequencyMode) {
		case 0:
			EnableTimerCallback(FlashBlueLED2, 0);// disable
			EnableTimerCallback(FlashRedLED1, 0);// disable
			LED_On(LED2_Blue_Pin);
			LED_Off(LED1_Red_Pin);
		break;
		case 1:
			EnableTimerCallback(FlashBlueLED2, 0);// disable
			EnableTimerCallback(FlashRedLED1, 0);// disable
			LED_On(LED1_Red_Pin);
			LED_Off(LED2_Blue_Pin);
		break;
		case 2:
			EnableTimerCallback(FlashBlueLED2, 0);// disable
			EnableTimerCallback(FlashRedLED1, 0);// disable
			LED_On(LED2_Blue_Pin);
			LED_On(LED1_Red_Pin);	
		break;
		case 3:	
			if(toneValue == 3) {
				EnableTimerCallback(FlashRedLED1, 0);// disable
				EnableTimerCallback(FlashBlueLED2, 0);// enable
				LED_Off(LED1_Red_Pin);
				LED_Off(LED2_Blue_Pin);
				TIM3->CNT = 2;
			} else {
				EnableTimerCallback(FlashRedLED1, 0);// disable
				EnableTimerCallback(FlashBlueLED2, 1);// enable
				LED_Off(LED1_Red_Pin);
			}
		break;
		case 4:
			// flash Red led fast
			// blink only for Subwoofer
			if(toneValue == 4) {
				EnableTimerCallback(FlashBlueLED2, 0);// disable
				EnableTimerCallback(FlashRedLED1, 1);// enable
				LED_Off(LED2_Blue_Pin);
			}
		break;
		default:
		break;
	}
	if(loudnessButtonStatus.shortLongPressStatus == 0 && loudnessButtonStatus.buttonState == 0) {// short press and button is released
		frequencySelected = frequencyMode;
		RemoveTimerCallback(FlashBlueLED1);
		RemoveTimerCallback(FlashRedLED1);
		RemoveTimerCallback(FlashBlueLED2);
		RemoveTimerCallback(FlashRedLED2);
		LED_Off(LED1_Blue_Pin);
		LED_Off(LED1_Red_Pin);
		LED_Off(LED2_Blue_Pin);
		LED_Off(LED2_Red_Pin);
		ClearLoudnessLongPress(); // clear it to -1
	}
	return frequencySelected;
}

/*
function: LED 1 indicator for the tone selection
input: none 
output: the tone selected
*/
int8_t SetLED1_Indicator(void) {
	uint8_t qMode;
	uint8_t encoderDirection;
	int8_t toneSelected = -1;
	LoudnessButtonStatus loudnessButtonStatus = GetLoudnessButtonStatus();
	
	encoderDirection = (TIM3->CR1 >> 4) & 0x01;// get what direction
	
	if(TIM3->CNT > 4) { // over flow
		if(encoderDirection == 0) { // upcounter
			TIM3->CNT = 4;
		} else { // down counter
			TIM3->CNT = 0;
		}
	} 
	qMode = 4 - TIM3->CNT;
	
	CreateTimerCallback(FlashBlueLED1, 500, REPEAT_TIMER);
	CreateTimerCallback(FlashRedLED2, 500, REPEAT_TIMER);

	switch(qMode) {
		case 0:
			EnableTimerCallback(FlashBlueLED1, 0);// disable
			EnableTimerCallback(FlashRedLED2, 0);// disable
			LED_On(LED1_Blue_Pin);
			LED_Off(LED2_Red_Pin);
		break;
		case 1:
			EnableTimerCallback(FlashBlueLED1, 0);// disable
			EnableTimerCallback(FlashRedLED2, 0);// disable
			LED_On(LED2_Red_Pin);
			LED_Off(LED1_Blue_Pin);
		break;
		case 2:
			EnableTimerCallback(FlashBlueLED1, 0);// disable
			EnableTimerCallback(FlashRedLED2, 0);// disable
			LED_On(LED1_Blue_Pin);
			LED_On(LED2_Red_Pin);	
		break;
		case 3:
			// flash Blue led 1/sec	
			EnableTimerCallback(FlashRedLED2, 0);// disable
			EnableTimerCallback(FlashBlueLED1, 1);// enable
			LED_Off(LED2_Red_Pin);
		break;
		case 4:
			// flash Red led 1/sec
			EnableTimerCallback(FlashBlueLED1, 0);// disable
			EnableTimerCallback(FlashRedLED2, 1);// enable
			LED_Off(LED1_Blue_Pin);
		break;
		default:
		break;
	}
	if(loudnessButtonStatus.shortLongPressStatus == 0 && loudnessButtonStatus.buttonState == 0) {// short press and button is released
		toneSelected = qMode;
		ClearLoudnessLongPress(); // clear it to -1
	}
	return toneSelected;
}

/*
function: read flash memory and save values to frequencyCenter struct
input: none 
output: none
*/
void GetFreqValues(void) {
	int i;
	uint32_t flashAddress = FREQUENCY_PARAM_ADDRESS;
	uint32_t flashData;
	for(i = 0; i < 5; i++) { // read 5 bytes
		FlashRead(flashAddress, &flashData);
		if((uint8_t) flashData == 0xFF) { // flash address empty
			frequencyCenter.Bytes.data[i] = 0; // default values
		} else {
			frequencyCenter.Bytes.data[i] = (uint8_t) flashData;
		}
		flashAddress += 4;
	}
	return;
}

/*
function: erase page and then save new values
input: none 
output: none
*/
void SaveFreqValues(void) {
	uint8_t dataLen;
	flashEraseStruct.Banks = 1;
	flashEraseStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	flashEraseStruct.PageAddress = FREQUENCY_PARAM_ADDRESS;
	flashEraseStruct.NbPages = 1; 
	dataLen = 5;	
	if(FlashPageErase(&flashEraseStruct) == 1) { // erase 1 page
		WriteBufToMem(FREQUENCY_PARAM_ADDRESS, frequencyCenter.Bytes.data, dataLen); // write data to address
	} 
}

/*
function: update the frequency for given tone
input: none 
output: return status
*/
uint8_t UpdateBD37534Frequency(void) {
	static uint8_t mode = 0;
	uint8_t returnStatus = HAL_I2C_STATE_BUSY; // busy
	switch(mode) {
		case 0:
			if(SendQData(BASS_Q_SETUP_ADDR, 0, frequencyCenter.Param.bass) == HAL_I2C_STATE_READY) ++mode;
		break;
		case 1:
			if(SendQData(MIDDLE_Q_SETUP_ADDR, 0, frequencyCenter.Param.midrange) == HAL_I2C_STATE_READY) ++mode;
		break;
		case 2:
			if(SendQData(TREBLE_Q_SETUP_ADDR, 0, frequencyCenter.Param.treble) == HAL_I2C_STATE_READY) ++mode;
		break;
		case 3:// loudness
			if(SendSetup3Data(frequencyCenter.Param.loudness) == HAL_I2C_STATE_READY) ++mode;
		break;
		case 4: // subwoofer frequency
			initSetup2Struct.Status.lowPassPhase = LOWPASS_PHASE_0;
			initSetup2Struct.Status.levelMeterReset = LEVEL_METER_RESET_HOLD;
			initSetup2Struct.Status.subwooferOutputSelect = SUBWOOFER_OUTPUT_SELECT_SUB;
			initSetup2Struct.Status.subwooferLowPassFreq = frequencyCenter.Param.sub;
			if(SendSetup2Data(&initSetup2Struct) == HAL_I2C_STATE_READY) ++mode;
		break;
		case 5:
			returnStatus = HAL_I2C_STATE_READY;
			mode = 0;// reset
		break;
	}
	return returnStatus;
}

/*
 need to place this function in BSP and set appropriate mute pin
*/
__weak void MuteAudio(uint8_t mode) {
	if(mode) {
		//PortC_Off(MuteAudio_Pin); // mutes
	} else {
		//PortC_On(MuteAudio_Pin); // unmutes
	}
}

#endif // USE_BD37531FV
