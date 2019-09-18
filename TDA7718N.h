#ifdef USE_TDA7718N
#ifndef TDA7718N_H
#define TDA7718N_H

#include "main.h"
#include "i2c.h"

#define TDA7718N_ADDRESS_RECEIVE_MODE 0x88 // chip can be programmed by STM32
#define TDA7718N_ADDRESS_TRANSMIT_MODE 0x88 // data can be receive by STM32

#define SOFT_STEP_ACTION_ACT 0
#define SOFT_STEP_ACTION_WAIT 1

/*
subaddress (receive mode)
*/

typedef union SubaddressStruct {
	struct{
		uint8_t byte0;
	}Byte;
	struct{
		unsigned selection:5;
		unsigned autoIncMode:1;
		unsigned notused:1;
		unsigned testingMode:1;
	}Status;
}SubaddressStruct;


#define TESTING_MODE_OFF_SUB_ADDR 0
#define TESTING_MODE_ON_SUB_ADDR 1

#define AUTO_INCREMENT_OFF_SUB_ADDR 0
#define AUTO_INCREMENT_ON_SUB_ADDR 1

#define MAIN_SELECTOR_SUB_ADDR 0x00
#define SOFT_MUTE_SUB_ADDR		0x04
#define SOFT_STEP1_SUB_ADDR		0x05
#define SOFT_STEP2_SUB_ADDR		0x06
#define LOUDNESS_SUB_ADDR			0x07
#define VOLUME_SUB_ADDR				0x08
#define TREBLE_SUB_ADDR				0x09
#define MIDDLE_SUB_ADDR				0x0A
#define BASS_SUB_ADDR					0x0B
#define SUBWOOFER_MIDDLE_BASS_SUB_ADDR 0x0C

#define SPEAK_ATT_LEFT_FRONT_SUB_ADDR 0x0D
#define SPEAK_ATT_RIGHT_FRONT_SUB_ADDR 0x0E
#define SPEAK_ATT_LEFT_REAR_SUB_ADDR 0x0F
#define SPEAK_ATT_RIGHT_REAR_SUB_ADDR 0x10
#define SUBWOOFER_ATT_LEFT_SUB_ADDR 0x11
#define SUBWOOFER_ATT_RIGHT_SUB_ADDR 0x12

#define TESTING_AUDIO_PROCESSOR1_SUB_ADDR 0x13
#define TESTING_ADUIO_PROCESSOR2_SUB_ADDR 0x14
#define TESTING_AUDIO_PROCESSOR3_SUB_ADDR 0x15


typedef union MainSelectorStruct {
	struct{
		uint8_t byte0;
	}Byte;
	struct{
		unsigned mainSourceSelector:3;
		unsigned fd_QD2_Selection:1;
		unsigned mainSourceInputGainSelect:1;
		unsigned subwooferFlat:1;
		unsigned notused:2;
	}Status;
}MainSelectorStruct;
/*
main selector
*/
#define SE1 0
#define SE3 1
#define QD1 2
#define QD2_FD1 3
#define SE2 4
#define SE4 5
#define SE5 6
#define MUTE 7

#define FD_QD2_SELECTION_FD 0
#define FD_QD2_SELECTION_QD2 1 // bit 3, FD = 0, QD2 = 1

#define MAIN_SOURCE_INPUT_GAIN_0dB 0
#define MAIN_SOURCE_INPUT_GAIN_3dB 1 // bit 4, 0dB = 0, 3dB = 1

#define SUBWOOFER_FLAT_OFF 0
#define SUBWOOFER_FLAT_ON 1 // bit 5, off = 0, on = 1


typedef union SoftMuteStruct {
	struct{
		uint8_t byte0;
	}Byte;
	struct{
		unsigned softMute:1;
		unsigned pinInfluenceForMute:1;
		unsigned softMuteTime:2;
		unsigned subwooferInputSource:1;
		unsigned subwooferEnable:1;
		unsigned fastCharge:1;
		unsigned antiAliasFilter:1;
	}Status;
}SoftMuteStruct;
/* 
soft mute
*/
#define SOFT_MUTE_OFF 0
#define SOFT_MUTE_ON 1 // bit 0, off = 0, on = 1

#define PIN_INFLUENCE_MUTE_PIN_IIC 0  // bit 1, Pin and IIC = 0, IIC = 1
#define PIN_INFLUENCE_MUTE_IIC 1

#define SOFT_MUTE_TIME_0_48 0 // bit 2:3,
#define SOFT_MUTE_TIME_0_96 1
#define SOFT_MUTE_TIME_7_68 2
#define SOFT_MUTE_TIME_15_36 3

#define SUBWOOFER_INPUT_SOURCE_INPUT_MUX 0 // bit 4
#define SUBWOOFER_INPUT_SOURCE_BASS_OUTPUT 1

#define SUBWOOFER_ENABLE_ON 0 
#define SUBWOOFER_ENABLE_OFF 1 // bit 5, on = 0

#define FAST_CHARGE_ON 0
#define FAST_CHARGE_OFF 1 // bit 6

#define ANTI_ALIAS_FILTER_ON 0
#define ANTI_ALIAS_FILTER_OFF 1 // bit 7, on = 0


typedef union SoftStep_1_Struct {
	struct{
		uint8_t byte0;
	}Byte;
	struct{
		unsigned LoudnessSoftStep:1;
		unsigned VolumeSoftStep:1;
		unsigned TrebleSoftStep:1;
		unsigned MiddleSoftStep:1;
		unsigned BassSoftStep:1;
		unsigned speakerLFsoftStep:1;
		unsigned speakerRFsoftStep:1;
		unsigned speakerLRsoftStep:1;
	}Status;
}SoftStep_1_Struct;
/*
soft step 1
*/
#define LOUDNESS_SOFT_STEP_ON 0 
#define LOUDNESS_SOFT_STEP_OFF 1 // bit 0, on = 0
#define VOLUME_SOFT_STEP_ON 0
#define VOLUME_SOFT_STEP_OFF 1 // bit 1, on = 0
#define TREBLE_SOFT_STEP_ON 0
#define TREBLE_SOFT_STEP_OFF 1 // bit 2, on = 0
#define MIDDLE_SOFT_STEP_ON 0
#define MIDDLE_SOFT_STEP_OFF 1 // bit 3, on = 0
#define BASS_SOFT_STEP_ON 0
#define BASS_SOFT_STEP_OFF 1 // bit 4, on = 0
#define SPEAKER_LF_SOFT_STEP_ON 0
#define SPEAKER_LF_SOFT_STEP_OFF 1 // bit 5, on = 0
#define SPEAKER_RF_SOFT_STEP_ON 0 
#define SPEAKER_RF_SOFT_STEP_OFF 1 // bit 6, on = 0
#define SPEAKER_LR_SOFT_STEP_ON 0
#define SPEAKER_LR_SOFT_STEP_OFF 1 // bit 7, on = 0


typedef union SoftStep_2_Struct {
	struct{
		uint8_t byte0;
	}Byte;
	struct{
		unsigned SpeakerRRsoftStep:1;
		unsigned subwooferLeftSoftStep:1;
		unsigned subwooferRightSoftStep:1;
		unsigned SoftStepTime:1;
		unsigned zeroComparatorWindowSize:2;
		unsigned spikeRejectionTimeConstant:2;
	}Status;
}SoftStep_2_Struct;
/*
soft step 2
*/
#define SPEAKER_RR_SOFT_STEP_ON 0
#define SPEAKER_RR_SOFT_STEP_OFF 1 // bit 0, on = 0
#define SUBWOOFER_LEFT_SOFT_STEP_ON 0
#define SUBWOOFER_LEFT_SOFT_STEP_OFF 1 // bit 1, on = 0
#define SUBWOOFER_RIGHT_SOFT_STEP_ON 0
#define SUBWOOFER_RIGHT_SOFT_STEP_OFF 1 // bit 2, on = 0
#define SOFT_STEP_TIME_5MS 0
#define SOFT_STEP_TIME_10MS 1 // bit 3, 5ms = 0

#define ZERO_COMPARATOR_WINDOW_SIZE_100mv 0 // bit 5:4
#define ZERO_COMPARATOR_WINDOW_SIZE_75mv 1 // bit 5:4
#define ZERO_COMPARATOR_WINDOW_SIZE_50mv 2 // bit 5:4
#define ZERO_COMPARATOR_WINDOW_SIZE_25mv 3 // bit 5:4

#define SPIKE_REJECTION_TIME_11us 0 // bit 7:6
#define SPIKE_REJECTION_TIME_22us 1 // bit 7:6
#define SPIKE_REJECTION_TIME_33us 2 // bit 7:6
#define SPIKE_REJECTION_TIME_44us 3 // bit 7:6


/*
volume
*/
typedef union VolumeStruct {
	struct{
		uint8_t byte0;
	}Byte;
	struct{
		unsigned volumeGainAtt:6;
		unsigned outputGain:1;
		unsigned softStepAction:1;
	}Status;
}VolumeStruct;


// gain
#define VOLUME_GAIN_0  0x00
#define VOLUME_GAIN_1  0x01
#define VOLUME_GAIN_2  0x02
#define VOLUME_GAIN_3  0x03
#define VOLUME_GAIN_4  0x04
#define VOLUME_GAIN_5  0x05
#define VOLUME_GAIN_6 0x06 
#define VOLUME_GAIN_7 0x07 
#define VOLUME_GAIN_8 0x08 
#define VOLUME_GAIN_9 0x09 
#define VOLUME_GAIN_10 0x0A 
#define VOLUME_GAIN_11 0x0B 
#define VOLUME_GAIN_12 0x0C 
#define VOLUME_GAIN_13 0x0D 
#define VOLUME_GAIN_14 0x0E 
#define VOLUME_GAIN_15 0x0F 
#define VOLUME_GAIN_16 0x10
#define VOLUME_GAIN_17 0x11
#define VOLUME_GAIN_18 0x12
#define VOLUME_GAIN_19 0x13
#define VOLUME_GAIN_20 0x14
#define VOLUME_GAIN_21 0x15
#define VOLUME_GAIN_22 0x16
#define VOLUME_GAIN_23 0x17
//attenuation
#define VOLUME_ATT_0  0x20
#define VOLUME_ATT_1  0x21
#define VOLUME_ATT_2  0x22
#define VOLUME_ATT_3  0x23
#define VOLUME_ATT_4  0x24
#define VOLUME_ATT_5  0x25
#define VOLUME_ATT_6 0x26 
#define VOLUME_ATT_7 0x27 
#define VOLUME_ATT_8 0x28 
#define VOLUME_ATT_9 0x29 
#define VOLUME_ATT_10 0x2A 
#define VOLUME_ATT_11 0x2B 
#define VOLUME_ATT_12 0x2C 
#define VOLUME_ATT_13 0x2D 
#define VOLUME_ATT_14 0x2E 
#define VOLUME_ATT_15 0x2F 
#define VOLUME_ATT_16 0x30
#define VOLUME_ATT_17 0x31
#define VOLUME_ATT_18 0x32
#define VOLUME_ATT_19 0x33
#define VOLUME_ATT_20 0x34
#define VOLUME_ATT_21 0x35
#define VOLUME_ATT_22 0x36
#define VOLUME_ATT_23 0x37
#define VOLUME_ATT_24 0x38
#define VOLUME_ATT_25 0x39
#define VOLUME_ATT_26 0x3A
#define VOLUME_ATT_27 0x3B
#define VOLUME_ATT_28 0x3C
#define VOLUME_ATT_29 0x3D
#define VOLUME_ATT_30 0x3E
#define VOLUME_ATT_31 0x3F

#define OUTPUT_GAIN_1dB 0
#define OUTPUT_GAIN_0dB 1 

#define SOFT_STEP_ACTION_ACT 0
#define SOFT_STEP_ACTION_WAIT 1


/*
loudness (values are negative -x dB)
*/
typedef union LoudnessStruct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned attenuation:4;
		unsigned centerFreq:2;
		unsigned highBoost:1;
		unsigned softStepAction:1;
	}Status;
}LoudnessStruct;

#define LOUDNESS_ATTENUATION_0 0x00
#define LOUDNESS_ATTENUATION_1 0x01
#define LOUDNESS_ATTENUATION_2 0x02
#define LOUDNESS_ATTENUATION_3 0x03
#define LOUDNESS_ATTENUATION_4 0x04
#define LOUDNESS_ATTENUATION_5 0x05
#define LOUDNESS_ATTENUATION_6 0x06
#define LOUDNESS_ATTENUATION_7 0x07
#define LOUDNESS_ATTENUATION_8 0x08
#define LOUDNESS_ATTENUATION_9 0x09
#define LOUDNESS_ATTENUATION_10 0x0A
#define LOUDNESS_ATTENUATION_11 0x0B
#define LOUDNESS_ATTENUATION_12 0x0C
#define LOUDNESS_ATTENUATION_13 0x0D
#define LOUDNESS_ATTENUATION_14 0x0E
#define LOUDNESS_ATTENUATION_15 0x0F

#define LOUDNESS_CENTER_FREQ_HZ_FLAT 0
#define LOUDNESS_CENTER_FREQ_HZ_400 1
#define LOUDNESS_CENTER_FREQ_HZ_800 2
#define LOUDNESS_CENTER_FREQ_HZ_2400 3

#define LOUDNESS_HIGH_BOOST_ON 0
#define LOUDNESS_HIGH_BOOST_OFF 1



/*
treble filter
*/
typedef union TrebleFilterStruct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned gain_attenuation:5;
		unsigned centerFreq:2;
		unsigned softStepAction:1;
	}Status;
}TrebleFilterStruct;
//attenuation, same for midlle and bass
#define TMB_ATT_15 0x00
#define TMB_ATT_14 0x01
#define TMB_ATT_13 0x02
#define TMB_ATT_12 0x03
#define TMB_ATT_11 0x04
#define TMB_ATT_10 0x05
#define TMB_ATT_9 0x06
#define TMB_ATT_8 0x07
#define TMB_ATT_7 0x08
#define TMB_ATT_6 0x09
#define TMB_ATT_5 0x0A
#define TMB_ATT_4 0x0B
#define TMB_ATT_3 0x0C
#define TMB_ATT_2 0x0D
#define TMB_ATT_1 0x0E
#define TMB_ATT_0 0x0F
// gain
#define TMB_GAIN_0 0x1F
#define TMB_GAIN_1 0x1E
#define TMB_GAIN_2 0x1D
#define TMB_GAIN_3 0x1C
#define TMB_GAIN_4 0x1B
#define TMB_GAIN_5 0x1A
#define TMB_GAIN_6 0x19
#define TMB_GAIN_7 0x18
#define TMB_GAIN_8 0x17
#define TMB_GAIN_9 0x16
#define TMB_GAIN_10 0x15
#define TMB_GAIN_11 0x14
#define TMB_GAIN_12 0x13
#define TMB_GAIN_13 0x12
#define TMB_GAIN_14 0x11
#define TMB_GAIN_15 0x10

#define TREBLE_CENTER_FREQ_KHZ_10 0
#define TREBLE_CENTER_FREQ_KHZ_12_5 1
#define TREBLE_CENTER_FREQ_KHZ_15 2
#define TREBLE_CENTER_FREQ_KHZ_17_5 3

/*
middle filter
*/
typedef union MiddleFilterStruct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned gain_attenuation:5;
		unsigned Q_Factor:2;
		unsigned softStepAction:1;
	}Status;
}MiddleFilterStruct;

#define MIDDLE_Q_FACTOR_0_75 0
#define MIDDLE_Q_FACTOR_1 1
#define MIDDLE_Q_FACTOR_1_25 2

/*
bass filter
*/
typedef union BassFilterStruct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned gain_attenuation:5;
		unsigned Q_Factor:2;
		unsigned softStepAction:1;
	}Status;
}BassFilterStruct;

#define BASS_Q_FACTOR_Q_1 0
#define BASS_Q_FACTOR_Q_1_25 1
#define BASS_Q_FACTOR_Q_1_5 2
#define BASS_Q_FACTOR_Q_2 3

/*
subwoofer/middle/bass frequency
*/
typedef union SubwooferMiddleBassStruct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned cutoffFreq:2;
		unsigned outputPhase:1;
		unsigned middleCenterFreq:2;
		unsigned bassCenterFreq:2;
		unsigned bassDC_mode:1;
	}Status;
}SubwooferMiddleBassStruct;
#define SUBWOOFER_CUTOFF_FREQ_HZ_55 0
#define SUBWOOFER_CUTOFF_FREQ_HZ_85 0
#define SUBWOOFER_CUTOFF_FREQ_HZ_120 0
#define SUBWOOFER_CUTOFF_FREQ_HZ_160 0

#define SUBWOOFER_OUTPUT_PHASE_180 0
#define SUBWOOFER_OUTPUT_PHASE_0 1

#define MIDDLE_CENTER_FREQ_HZ_500 0
#define MIDDLE_CENTER_FREQ_HZ_1000 1
#define MIDDLE_CENTER_FREQ_HZ_1500 2
#define MIDDLE_CENTER_FREQ_HZ_2500 3

#define BASS_CENTER_FREQ_HZ_60 0
#define BASS_CENTER_FREQ_HZ_80 1
#define BASS_CENTER_FREQ_HZ_100 2
#define BASS_CENTER_FREQ_HZ_200 3

#define BASS_DC_MODE_ON 0
#define BASS_DC_MODE_OFF 1

/*
speaker attenuation
*/
typedef union SpeakerGainAttStruct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned gain_attenuation:7; // 0dB to -79dB and mute
		unsigned softStepAction:1; // act = 0, wait = 1
	}Status;
}SpeakerGainAttStruct;
// gain
#define SPEAKER_GAIN_dB_0 0x00
#define SPEAKER_GAIN_dB_1 0x01
#define SPEAKER_GAIN_dB_2 0x02
#define SPEAKER_GAIN_dB_3 0x03
#define SPEAKER_GAIN_dB_4 0x04
#define SPEAKER_GAIN_dB_5 0x05
#define SPEAKER_GAIN_dB_6 0x06
#define SPEAKER_GAIN_dB_7 0x07
#define SPEAKER_GAIN_dB_8 0x08
#define SPEAKER_GAIN_dB_9 0x09
#define SPEAKER_GAIN_dB_10 0x0A
#define SPEAKER_GAIN_dB_11 0x0B
#define SPEAKER_GAIN_dB_12 0x0C
#define SPEAKER_GAIN_dB_13 0x0D
#define SPEAKER_GAIN_dB_14 0x0E
#define SPEAKER_GAIN_dB_15 0x0F
// att
#define SPEAKER_ATT_dB_0 0x10
#define SPEAKER_ATT_dB_1 0x11
#define SPEAKER_ATT_dB_2 0x12
#define SPEAKER_ATT_dB_3 0x13
#define SPEAKER_ATT_dB_4 0x14
#define SPEAKER_ATT_dB_5 0x15
#define SPEAKER_ATT_dB_6 0x16
#define SPEAKER_ATT_dB_7 0x17
#define SPEAKER_ATT_dB_8 0x18
#define SPEAKER_ATT_dB_9 0x19
#define SPEAKER_ATT_dB_10 0x1A
#define SPEAKER_ATT_dB_11 0x1B
#define SPEAKER_ATT_dB_12 0x1C
#define SPEAKER_ATT_dB_13 0x1D
#define SPEAKER_ATT_dB_14 0x1E
#define SPEAKER_ATT_dB_15 0x1F
#define SPEAKER_ATT_dB_16 0x30
#define SPEAKER_ATT_dB_17 0x31
#define SPEAKER_ATT_dB_18 0x32

// todo finish the rest
#define SPEAKER_ATT_dB_78 0x5E
#define SPEAKER_ATT_dB_79 0x5F
#define SPEAKER_MUTE 0x60


/*
testing audio processor1
*/
typedef union TestingAudioProc1Struct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned audioProcessorTestingMode:1;
		unsigned testMultiplexerSE1L:4;
		unsigned clockFastMode:1;
		unsigned clockSource:1;
		unsigned attenuatorGainClockControl:1;
	}Status;
}TestingAudioProc1Struct;


/*
testing audio processor2
*/
typedef union TestingAudioProc2Struct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned estArchiture:1;
		unsigned oscillatorClock:1;
		unsigned softStepCurve:1;
		unsigned manualSetBusySignal:2;
		unsigned requestForClkGenerator:2;
		unsigned noDCOSpikeRejection:1; // on = 0, off = 1
		unsigned notuse:2;
	}Status;
}TestingAudioProc2Struct;
#define TEST_ARCHITECT_NORMAL 0
#define TEST_ARCHITECT_SPLIT 1
#define OSCILLATOR_CLOCK_400_KHZ 0
#define OSCILLATOR_CLOCK_800_KHZ 1
#define SOFTSTEP_CURVE_S_CURVE 0
#define SOFTSTEP_CURVE_LINEAR_CURVE 1
#define MANUAL_SET_BUSY_SIGNAL_AUTO 0
#define MANUAL_SET_BUSY_SIGNAL_0 2
#define MANUAL_SET_BUSY_SIGNAL_1 3
#define REQUEST_FOR_CLK_GENERATOR_ALLOW 0
#define REQUEST_FOR_CLK_GENERATOR_ALLOW__ 1
#define REQUEST_FOR_CLK_GENERATOR_STOPPED 2
#define REQUEST_FOR_CLK_GENERATOR_STOPPED__ 3


/*
testing audio processor3
*/
typedef union TestingAudioProc3Struct {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned enableClockForFLFRRLRRSWLSWR:1; // on = 0, off = 1
		unsigned enableClockForVolume:1; // on = 0, off = 1
		unsigned enableClockForTrebleBass:1; // on = 0, off = 1
		unsigned enableClockForLoudnessMiddle:1; // on = 0, off = 1
		unsigned notused:4;
	}Status;
}TestingAudioProc3Struct;

//prototypes
HAL_I2C_StateTypeDef InitTDA7718N(void);
HAL_I2C_StateTypeDef MainSelector(MainSelectorStruct* mainSelectorStruct);
HAL_I2C_StateTypeDef SoftMute(SoftMuteStruct* softMuteStruct);
HAL_I2C_StateTypeDef SoftStep_1_Adjust(SoftStep_1_Struct* softStep_1_Struct);
HAL_I2C_StateTypeDef SoftStep_2_Adjust(SoftStep_2_Struct* softStep_2_Struct);
HAL_I2C_StateTypeDef VolumeAdjust(uint8_t volumeStatus);
HAL_I2C_StateTypeDef LoudnessAdjust(LoudnessStruct* loudnessStruct);
HAL_I2C_StateTypeDef BassAdjust(BassFilterStruct* bassFilterStruct);
HAL_I2C_StateTypeDef TrebleAdjust(TrebleFilterStruct* trebleFilterStruct); 
HAL_I2C_StateTypeDef MiddleAdjust(MiddleFilterStruct* MiddleFilterStruct); 
HAL_I2C_StateTypeDef SpeakerGainAttAdjust(SubaddressStruct* subaddressStruct, SpeakerGainAttStruct* speakerGainAttStruct);
HAL_I2C_StateTypeDef SubwooferMiddleBassAdjust(SubwooferMiddleBassStruct* subwooferMiddleBassStruct);

#endif // TDA7718N
#endif // USE_TDA7718N

