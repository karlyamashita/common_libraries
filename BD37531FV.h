#ifndef BD37531FV_H
#define BD37531FV_H

#include "main.h"
#include "i2c.h"

#define VOL_MAX_RANGE 39 // volume is 0-38
#define FADE_MAX_RANGE 10 // 1-19, 10 is middle. 
#define BASS_MAX_RANGE 19

#define FREQUENCY_PARAM_ADDRESS 0x0801F000


#define BD37531FV_ADDRESS 0x80 // slave address

// Select address
#define INIT_SETUP_1_ADDR 0x1
#define INIT_SETUP_2_ADDR 0x2
#define INIT_SETUP_3_ADDR 0x3
#define INPUT_SELECTOR_ADDR 0x5
#define INPUT_GAIN_ADDR 0x6
#define VOLUME_GAIN_ADDR 0x20
#define FADER_FRONT_LEFT_ADDR 0x28
#define FADER_FRONT_RIGHT_ADDR 0x29
#define FADER_REAR_LEFT_ADDR 0x2A
#define FADER_REAR_RIGHT_ADDR 0x2B
#define FADER_SUBWOOFER_ADDR 0x2C
#define MIXING_ADDR 0x30
#define BASS_Q_SETUP_ADDR 0x41
#define MIDDLE_Q_SETUP_ADDR 0x44
#define TREBLE_Q_SETUP_ADDR 0x47
#define BASS_GAIN_ADDR 0x51
#define MIDDLE_GAIN_ADDR 0x54
#define TREBLE_GAIN_ADDR 0x57
#define LOUDNESS_GAIN_ADDR 0x75
#define SYSTEM_RESET_ADDR 0xFE

// address 1
#define ADVANCED_SWITCH_TIME_MUTE_0_6 0x00 // 0.6 // default
#define ADVANCED_SWITCH_TIME_MUTE_1_0 0x01 // 1.0
#define ADVANCED_SWITCH_TIME_MUTE_1_4 0x02 // 1.4
#define ADVANCED_SWITCH_TIME_MUTE_3_2 0x03 // 3.2

#define ADVANCED_SWITCH_TIME_INPUT_4_7 0x00 // 4.7
#define ADVANCED_SWITCH_TIME_INPUT_7_1 0x01 // 7.1
#define ADVANCED_SWITCH_TIME_INPUT_11_2 0x02 // 11.2 // default
#define ADVANCED_SWITCH_TIME_INPUT_14_4 0x03 // 14.4

#define ADVANCED_SWITCH_OFF 0x00 
#define ADVANCED_SWITCH_ON 0x01 // default

// address 2
#define LOWPASS_PHASE_0 0 // default
#define LOWPASS_PHASE_180 1

#define LEVEL_METER_RESET_HOLD 0 // default
#define LEVEL_METER_RESET_RESET 1

#define SUBWOOFER_OUTPUT_SELECT_SUB 0x00 // default
#define SUBWOOFER_OUTPUT_SELECT_FRONT 0x01
#define SUBWOOFER_OUTPUT_SELECT_REAR 0x02

#define SUBWOOFER_LOWPASS_FREQ_OFF 0 // default
#define SUBWOOFER_LOWPASS_FREQ_55HZ 1
#define SUBWOOFER_LOWPASS_FREQ_85HZ 2
#define SUBWOOFER_LOWPASS_FREQ_120HZ 3
#define SUBWOOFER_LOWPASS_FREQ_160HZ 4

// address 3
#define LOUDNESS_FO_250HZ 0x00 // default
#define LOUDNESS_FO_400HZ 0x01
#define LOUDNESS_FO_800HZ 0x02

// address 5
#define INPUT_SELECTOR_A 0x00 // default
#define INPUT_SELECTOR_B 0x01
#define INPUT_SELECTOR_C 0x02
#define INPUT_SELECTOR_D_SINGLE 0x03
#define INPUT_SELECTOR_E1_SINGLE 0x0A
#define INPUT_SELECTOR_E2_SINGLE 0x0B
#define INPUT_SELECTOR_A_DIFF 0x0F
#define INPUT_SELECTOR_C_DIFF 0x10
#define INPUT_SELECTOR_D_DIFF 0x06
#define INPUT_SELECTOR_E_FULL_DIFF 0x08
#define INPUT_SELECTOR_INPUT_SHORT 0x09

#define FULL_DIFF_BIAS_TYPE_SELECT_NEGATIVE 0 // default
#define FULL_DIFF_BIAS_TYPE_SELECT_BIAS 1

// address 6 and 51,54,57,75
#define GAIN_0 0x00
#define GAIN_1 0x01
#define GAIN_2 0x02
#define GAIN_3 0x03
#define GAIN_4 0x04
#define GAIN_5 0x05
#define GAIN_6 0x06
#define GAIN_7 0x07
#define GAIN_8 0x08
#define GAIN_9 0x09
#define GAIN_10 0x0A
#define GAIN_11 0x0B
#define GAIN_12 0x0C
#define GAIN_13 0x0D
#define GAIN_14 0x0E
#define GAIN_15 0x0F
#define GAIN_16 0x10
#define GAIN_17 0x11
#define GAIN_18 0x12
#define GAIN_19 0x13
#define GAIN_20 0x14

// only address 6
#define MUTE_OFF 0x00 // default
#define MUTE_ON 0x01

// only addres 51,54,57
#define BOOST 0x00
#define CUT 0x01 // default

// only address 75
#define LOUDNESS_HI_CUT_1 0x00 // default
#define LOUDNESS_HI_CUT_2 0x01
#define LOUDNESS_HI_CUT_3 0x02
#define LOUDNESS_HI_CUT_4 0x03

// address 20, 28, 29, 2A, 2B, 2C, 30 - Vol and fader 
#define VOL_GAIN_15 0x71
#define VOL_GAIN_14 0x72
#define VOL_GAIN_13 0x73
#define VOL_GAIN_12 0x74
#define VOL_GAIN_11 0x75
#define VOL_GAIN_10 0x76
#define VOL_GAIN_09 0x77
#define VOL_GAIN_08 0x78
// mixing gain goes up to 7 only
#define VOL_GAIN_07 0x79
#define VOL_GAIN_06 0x7A
#define VOL_GAIN_05 0x7B
#define VOL_GAIN_04 0x7C
#define VOL_GAIN_03 0x7D
#define VOL_GAIN_02 0x7E
#define VOL_GAIN_01 0x7F
#define VOL_GAIN_00 0x80

#define ATT_01 0x81
#define ATT_02 0x82
#define ATT_03 0x83
#define ATT_04 0x84
#define ATT_05 0x85
#define ATT_06 0x86
#define ATT_07 0x87
#define ATT_08 0x88
#define ATT_09 0x89
#define ATT_10 0x8A
#define ATT_11 0x8B
#define ATT_12 0x8C
#define ATT_13 0x8D
#define ATT_14 0x8E
#define ATT_15 0x8F

#define ATT_16 0x90
#define ATT_17 0x91
#define ATT_18 0x92
#define ATT_19 0x93
#define ATT_20 0x94
#define ATT_21 0x95
#define ATT_22 0x96
#define ATT_23 0x97
#define ATT_24 0x98
#define ATT_25 0x99
#define ATT_26 0x9A
#define ATT_27 0x9B
#define ATT_28 0x9C
#define ATT_29 0x9D
#define ATT_30 0x9E
#define ATT_31 0x9F

#define ATT_32 0xA0
#define ATT_33 0xA1
#define ATT_34 0xA2
#define ATT_35 0xA3
#define ATT_36 0xA4
#define ATT_37 0xA5
#define ATT_38 0xA6
#define ATT_39 0xA7
#define ATT_40 0xA8
#define ATT_41 0xA9
#define ATT_42 0xAA
#define ATT_43 0xAB
#define ATT_44 0xAC
#define ATT_45 0xAD
#define ATT_46 0xAE
#define ATT_47 0xAF

#define ATT_48 0xB0
#define ATT_49 0xB1
#define ATT_50 0xB2
#define ATT_51 0xB3
#define ATT_52 0xB4
#define ATT_53 0xB5
#define ATT_54 0xB6
#define ATT_55 0xB7
#define ATT_56 0xB8
#define ATT_57 0xB9
#define ATT_58 0xBA
#define ATT_59 0xBB
#define ATT_60 0xBC
#define ATT_61 0xBD
#define ATT_62 0xBE
#define ATT_63 0xBF

#define ATT_64 0xC0
#define ATT_65 0xC1
#define ATT_66 0xC2
#define ATT_67 0xC3
#define ATT_68 0xC4
#define ATT_69 0xC5
#define ATT_70 0xc6
#define ATT_71 0xC7
#define ATT_72 0xC8
#define ATT_73 0xC9
#define ATT_74 0xCA
#define ATT_75 0xCB
#define ATT_76 0xCC
#define ATT_77 0xCD
#define ATT_78 0xCE
#define ATT_79 0xCF
#define INFINITE 0xFF

// address 30
// Use above range but only -79 to 7dB, not 15dB
#define MIXING_OFF 0xFF


// address 41
#define BASS_Q_FACTOR_0_5 0x00 // default
#define BASS_Q_FACTOR_1_0 0x01
#define BASS_Q_FACTOR_1_5 0x02
#define BASS_Q_FACTOR_2_0 0x03

#define BASS_FO_60HZ 0x00 // default
#define BASS_FO_80HZ 0x01
#define BASS_FO_100HZ 0x02
#define BASS_FO_120HZ 0x03

// address 44
#define MIDDLE_Q_FACTOR_0_75 0x00 // default
#define MIDDLE_Q_FACTOR_1_0 0x01
#define MIDDLE_Q_FACTOR_1_25 0x02
#define MIDDLE_Q_FACTOR_1_5 0x03

#define MIDDLE_FO_500HZ 0x00 // default
#define MIDDLE_FO_1KHZ 0x01
#define MIDDLE_FO_1_5KHZ 0x02
#define MIDDLE_FO_2_5KHZ 0x03

// address 47
#define TREBLE_Q_FACTOR_0_75 0x00 // default
#define TREBLE_Q_FACTOR_1_25 0x01

#define TREBLE_FO_7_5KHZ 0x00 // default
#define TREBLE_FO_10KHZ 0x01
#define TREBLE_FO_12_5KHZ 0x02
#define TREBLE_FO_15KHZ 0x03

#define SYSTEM_RESET_DATA 0x81

typedef union InitSetup1Struct{
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned advancedSwitchTimeMute:2;
		unsigned bit2:1; // init to 1
		unsigned bit3:1;
		unsigned advancedSwitchTimeInput:2;
		unsigned bit6:1;
		unsigned advancedOnOff:1;
	}Status;
}InitSetup1Struct;

typedef union InitSetup2Struct{
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned subwooferLowPassFreq:3;
		unsigned :1;
		unsigned subwooferOutputSelect:2;
		unsigned levelMeterReset:1;
		unsigned lowPassPhase:1;
	}Status;
}InitSetup2Struct;

typedef union InitSetup3Struct{
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned bit0:1; // init to 1
		unsigned :2;
		unsigned loudness:2;
		unsigned :3;
	}Status;
}InitSetup3Struct;

typedef union InputSelectorStruct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned inputSelector:5;
		unsigned :2;
		unsigned fullDiffBiasTypeSelect:1;
	}Status;
}InputSelectorStruct;

typedef union InputGainStruct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned inputGain:5;
		unsigned :2;
		unsigned muteOnOff:1;
	}Status;
}InputGainStruct;

typedef union QFactorStruct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned qFactor:2;
		unsigned :2;
		unsigned freq:2;
		unsigned :2;
	}Status;
}QFactorStruct;

typedef union BassMiddleTrebleGainStruct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned gain:5;
		unsigned :2;
		unsigned boostCut:1;
	}Status;
}BassMiddleTrebleGainStruct;

typedef union LoudnessGainStruct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned loudnessGain:5;
		unsigned loudnessHiCut:2;
		unsigned :1;
	}Status;
}LoudnessGainStruct;

typedef union FrequencyCenter {
	struct{
		uint8_t data[5];
	}Bytes;
	struct{
		uint8_t bass;
		uint8_t midrange;
		uint8_t treble;
		uint8_t loudness;
		uint8_t sub;	
	}Param;
}FrequencyCenter;


void InitBD37531FV(void);
HAL_I2C_StateTypeDef SendSetup1Data(InitSetup1Struct *initSetup1Struct);
HAL_I2C_StateTypeDef SendSetup2Data(InitSetup2Struct *initSetup2Struct);
HAL_I2C_StateTypeDef SendSetup3Data(uint8_t loudness);
HAL_I2C_StateTypeDef SendInputSelectorData(uint8_t inputSelector, uint8_t fullDiffBiasTypeSelect);
HAL_I2C_StateTypeDef SendInputGainData(uint8_t mute, uint8_t gain);
HAL_I2C_StateTypeDef SendQData(uint8_t address, uint8_t q, uint8_t fo);
HAL_I2C_StateTypeDef SendGainData(uint8_t address, uint8_t gain, uint8_t boostCut);// bass, middle, treble
HAL_I2C_StateTypeDef SendLoudnessData(uint8_t hicut, uint8_t gain);

HAL_I2C_StateTypeDef SendVolFaderSubData(uint8_t address, uint8_t gainAtt);

uint8_t BD37531FV_Init_Status(void);
void DeInitBD37531FV(void);

void CheckForProgrammingMode(void);
uint8_t GetProgrammingModeStatus(void);
void SetProgrammingModeStatus(uint8_t mode);
int8_t Program_Q_Settings(void);
int8_t SetLED1_Indicator(void);
void GetFreqValues(void);
void SaveFreqValues(void);
uint8_t UpdateBD37534Frequency(void);


#endif // BD37531FV_H
