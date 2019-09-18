#include "LSP.h"
#ifdef USE_TDA7721
/*

ST.com TDA7721 5 band audio processor


Interface Protocol:
• A Start Condition (S)
• A chip address byte (the LSB dtermines read/write transmission)
• A subaddress byte
• A sequence of data (N-bytes + acknowledge)
• A stop condition (P)
• The max. clock speed is 400 kbit/s
• 3.3V logic compatible

Receive Mode:
S 1 0 0 0 1 0 0 R/W ACK TS X AI A4 A3 A2 A1 A0 ACK Data ACK P

Transmission Mode:
S 1 0 0 0 1 0 0 R/W ACK X X X X X X BZ SM ACK P






*/

#include "main.h"
#include "TDA7721.h"
#include "I2C.h"
#include "GPIO_Ports.h"


uint8_t volumeTable[2][34] = {
	{0, 1 , 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33},
	{VOLUME_ATT_31, VOLUME_ATT_30, VOLUME_ATT_29, VOLUME_ATT_28, VOLUME_ATT_27, VOLUME_ATT_26, VOLUME_ATT_25
		,	VOLUME_ATT_24, VOLUME_ATT_23, VOLUME_ATT_22, VOLUME_ATT_21, VOLUME_ATT_20, VOLUME_ATT_19, VOLUME_ATT_18, VOLUME_ATT_17
		, VOLUME_ATT_16, VOLUME_ATT_15, VOLUME_ATT_14, VOLUME_ATT_13, VOLUME_ATT_12, VOLUME_ATT_11, VOLUME_ATT_10, VOLUME_ATT_9
		, VOLUME_ATT_8, VOLUME_ATT_7, VOLUME_ATT_6, VOLUME_ATT_5, VOLUME_ATT_4, VOLUME_ATT_3, VOLUME_ATT_2, VOLUME_ATT_1
		, VOLUME_ATT_0, VOLUME_GAIN_1, VOLUME_GAIN_2}	
};

uint8_t loudnessTable[2][16] = {
	{0, 1 , 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
	{LOUDNESS_ATTENUATION_0, LOUDNESS_ATTENUATION_1, LOUDNESS_ATTENUATION_2, LOUDNESS_ATTENUATION_3, LOUDNESS_ATTENUATION_4,
	LOUDNESS_ATTENUATION_5, LOUDNESS_ATTENUATION_6, LOUDNESS_ATTENUATION_7, LOUDNESS_ATTENUATION_8, LOUDNESS_ATTENUATION_9,
	LOUDNESS_ATTENUATION_10, LOUDNESS_ATTENUATION_11, LOUDNESS_ATTENUATION_12, LOUDNESS_ATTENUATION_14, LOUDNESS_ATTENUATION_15}	
};

uint8_t trebleMidBassTable[2][22] = { // same numbers for treble, middle and bass. 
	{0, 1 , 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21},
	{TMB_ATT_10, TMB_ATT_9,	TMB_ATT_8, TMB_ATT_7, TMB_ATT_6, TMB_ATT_5, TMB_ATT_4, TMB_ATT_3, TMB_ATT_2,
	TMB_ATT_1, TMB_ATT_0,	TMB_GAIN_1, TMB_GAIN_2, TMB_GAIN_3, TMB_GAIN_4, TMB_GAIN_5, TMB_GAIN_6, TMB_GAIN_7,
	TMB_GAIN_8, TMB_GAIN_9,	TMB_GAIN_10, TMB_GAIN_11}	
};


HAL_I2C_StateTypeDef InitTDA7721(void) {
	// define struct
	MainSelectorStruct mainSelectorStruct;
	SoftMuteStruct softMuteStruct;
	SoftStep_1_Struct softStep_1_Struct;
	SoftStep_2_Struct softStep_2_Struct;
	SubaddressStruct subaddressStruct;
	SpeakerGainAttStruct speakerGainAttStruct;
	TrebleFilterStruct trebleFilterStruct;
	MiddleFilterStruct middleFilterStruct;
	BassFilterStruct bassFilterStruct;
	
	SubwooferMiddleBassStruct subwooferMiddleBassStruct;
	
	// clear struct
	memset(&mainSelectorStruct, 0, sizeof(mainSelectorStruct));
	memset(&softMuteStruct, 0, sizeof(softMuteStruct));
	memset(&softStep_1_Struct, 0, sizeof(softStep_1_Struct));
	memset(&softStep_2_Struct, 0, sizeof(softStep_2_Struct));
	memset(&subaddressStruct, 0, sizeof(subaddressStruct));
	memset(&speakerGainAttStruct, 0, sizeof(speakerGainAttStruct));
	memset(&trebleFilterStruct, 0, sizeof(trebleFilterStruct));
	memset(&middleFilterStruct, 0, sizeof(middleFilterStruct));
	memset(&bassFilterStruct, 0, sizeof(bassFilterStruct));
	
	memset(&subwooferMiddleBassStruct, 0, sizeof(subwooferMiddleBassStruct));
	
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY){
		return HAL_I2C_STATE_BUSY;
	}		
	
	// main selector
	
	// QD1
	// QD2_FD1
	// SE4
	
	// FD_QD2_SELECTION_FD
	// FD_QD2_SELECTION_QD2
	
	mainSelectorStruct.Status.mainSourceSelector = QD2_FD1;
	mainSelectorStruct.Status.fd_QD2_Selection = FD_QD2_SELECTION_FD;
	mainSelectorStruct.Status.mainSourceInputGainSelect = MAIN_SOURCE_INPUT_GAIN_0dB;
	mainSelectorStruct.Status.subwooferFlat = SUBWOOFER_FLAT_OFF;
	MainSelector(&mainSelectorStruct);
	
	
	// soft mute
	softMuteStruct.Status.softMute = SOFT_MUTE_ON;
	softMuteStruct.Status.pinInfluenceForMute = PIN_INFLUENCE_MUTE_PIN_IIC;
	softMuteStruct.Status.softMuteTime = SOFT_MUTE_TIME_15_36;
	softMuteStruct.Status.subwooferInputSource = SUBWOOFER_INPUT_SOURCE_BASS_OUTPUT; // SUBWOOFER_INPUT_SOURCE_BASS_OUTPUT varies with volume
	softMuteStruct.Status.subwooferEnable = SUBWOOFER_ENABLE_ON;
	softMuteStruct.Status.fastCharge = FAST_CHARGE_OFF;
	softMuteStruct.Status.antiAliasFilter = ANTI_ALIAS_FILTER_OFF;
	SoftMute(&softMuteStruct);

	
	// speaker gain attenuation	
	//left front
	subaddressStruct.Status.selection = SPEAK_ATT_LEFT_FRONT_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	speakerGainAttStruct.Status.gain_attenuation = SPEAKER_ATT_dB_0;
	speakerGainAttStruct.Status.softStepAction = SOFT_STEP_ACTION_ACT;
	SpeakerGainAttAdjust(&subaddressStruct, &speakerGainAttStruct);
	
	//right front
	subaddressStruct.Status.selection = SPEAK_ATT_RIGHT_FRONT_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	speakerGainAttStruct.Status.gain_attenuation = SPEAKER_ATT_dB_0;
	speakerGainAttStruct.Status.softStepAction = SOFT_STEP_ACTION_ACT;
	SpeakerGainAttAdjust(&subaddressStruct, &speakerGainAttStruct);
	
	// left rear
	subaddressStruct.Status.selection = SPEAK_ATT_LEFT_REAR_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	speakerGainAttStruct.Status.gain_attenuation = SPEAKER_ATT_dB_0;
	speakerGainAttStruct.Status.softStepAction = SOFT_STEP_ACTION_ACT;
	SpeakerGainAttAdjust(&subaddressStruct, &speakerGainAttStruct) ;
	
	// right rear 
	subaddressStruct.Status.selection = SPEAK_ATT_RIGHT_REAR_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	speakerGainAttStruct.Status.gain_attenuation = SPEAKER_ATT_dB_0;
	speakerGainAttStruct.Status.softStepAction = SOFT_STEP_ACTION_ACT;
	SpeakerGainAttAdjust(&subaddressStruct, &speakerGainAttStruct);
	
	// left sub
	subaddressStruct.Status.selection = SUBWOOFER_ATT_LEFT_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	speakerGainAttStruct.Status.gain_attenuation = SPEAKER_GAIN_dB_0;
	speakerGainAttStruct.Status.softStepAction = SOFT_STEP_ACTION_ACT;
	SpeakerGainAttAdjust(&subaddressStruct, &speakerGainAttStruct);
	
	// right sub
	subaddressStruct.Status.selection = SUBWOOFER_ATT_RIGHT_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	speakerGainAttStruct.Status.gain_attenuation = SPEAKER_GAIN_dB_0;
	speakerGainAttStruct.Status.softStepAction = SOFT_STEP_ACTION_ACT;
	SpeakerGainAttAdjust(&subaddressStruct, &speakerGainAttStruct);
	
	
	subwooferMiddleBassStruct.Status.bassCenterFreq = BASS_CENTER_FREQ_HZ_200;
	subwooferMiddleBassStruct.Status.cutoffFreq = SUBWOOFER_CUTOFF_FREQ_HZ_120;
	SubwooferMiddleBassAdjust(&subwooferMiddleBassStruct);
	
	
	
	//soft step 1
	softStep_1_Struct.Byte.byte0 = 0;
	while(SoftStep_1_Adjust(&softStep_1_Struct) != HAL_I2C_STATE_READY);

		
	//soft step 2	
	softStep_2_Struct.Status.SoftStepTime = SOFT_STEP_TIME_10MS;
	softStep_2_Struct.Status.SpeakerRRsoftStep = SPEAKER_RR_SOFT_STEP_ON; // TODO change to "SOFT_STEP_ON or OFF"
	softStep_2_Struct.Status.subwooferLeftSoftStep = SUBWOOFER_LEFT_SOFT_STEP_ON;
	softStep_2_Struct.Status.subwooferRightSoftStep = SUBWOOFER_RIGHT_SOFT_STEP_ON;
	while(SoftStep_2_Adjust(&softStep_2_Struct) != HAL_I2C_STATE_READY);
	
	// treble adjust
	trebleFilterStruct.Status.softStepAction = SOFT_STEP_ACTION_ACT;
	trebleFilterStruct.Status.centerFreq = TREBLE_CENTER_FREQ_KHZ_10;
	trebleFilterStruct.Status.gain_attenuation = TMB_GAIN_3;
	while(TrebleAdjust(&trebleFilterStruct) != HAL_I2C_STATE_READY);
	
	// middle adjust
	middleFilterStruct.Status.softStepAction = SOFT_STEP_ACTION_ACT;
	middleFilterStruct.Status.Q_Factor = MIDDLE_Q_FACTOR_1_25;
	middleFilterStruct.Status.gain_attenuation = TMB_GAIN_3;
	while(MiddleAdjust(&middleFilterStruct) != HAL_I2C_STATE_READY);
	
	// bass adjust
	bassFilterStruct.Status.softStepAction = SOFT_STEP_ACTION_ACT;
	bassFilterStruct.Status.Q_Factor = BASS_Q_FACTOR_Q_2;
	bassFilterStruct.Status.gain_attenuation = TMB_GAIN_0;
	while(BassAdjust(&bassFilterStruct) != HAL_I2C_STATE_READY);
	

	return HAL_I2C_STATE_READY;
}

uint8_t mainSelectorBytes[2];
HAL_I2C_StateTypeDef MainSelector(MainSelectorStruct* mainSelectorStruct) {
	uint8_t dataBytes[2];
	SubaddressStruct subaddressStruct;
	
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY) return HAL_I2C_STATE_BUSY;
	
	subaddressStruct.Status.selection = MAIN_SELECTOR_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	dataBytes[0] = subaddressStruct.Byte.byte0;
	dataBytes[1] = mainSelectorStruct->Byte.byte0;
	mainSelectorBytes[0] = subaddressStruct.Byte.byte0;
	mainSelectorBytes[1] = mainSelectorStruct->Byte.byte0;
	
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)TDA7718N_ADDRESS_RECEIVE_MODE, dataBytes, 2, 100) != HAL_OK) {
		// todo error
	}
	return HAL_I2C_STATE_READY;
}

HAL_I2C_StateTypeDef SoftMute(SoftMuteStruct* softMuteStruct) {
	uint8_t dataBytes[2];
	SubaddressStruct subaddressStruct;
	
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY) return HAL_I2C_STATE_BUSY;
	
	subaddressStruct.Status.selection = SOFT_MUTE_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	dataBytes[0] = subaddressStruct.Byte.byte0;
	dataBytes[1] = softMuteStruct->Byte.byte0;
	mainSelectorBytes[0] = subaddressStruct.Byte.byte0;
	mainSelectorBytes[1] = softMuteStruct->Byte.byte0;
	PortD_Off(LED_Red_Pin);
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)TDA7718N_ADDRESS_RECEIVE_MODE, dataBytes, 2, 100) != HAL_OK) {
		// todo error
	}
	return HAL_I2C_STATE_READY;
}

HAL_I2C_StateTypeDef VolumeAdjust(uint8_t volumeStatus) {
	
	uint8_t dataBytes[2];
	uint8_t volumeMatch = 0;
	uint8_t i = 0;
	SubaddressStruct subaddressStruct;
	VolumeStruct volumeStruct;
	memset(&subaddressStruct, 0, sizeof(SubaddressStruct));
	memset(&volumeStruct, 0, sizeof(volumeStruct));

	
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY){
		return HAL_I2C_STATE_BUSY;
	}		
	
	subaddressStruct.Status.selection = VOLUME_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	dataBytes[0] = subaddressStruct.Byte.byte0;
	do {
		volumeMatch = volumeTable[1][i];
	}while(volumeTable[0][i++] != volumeStatus) ;
	
	volumeStruct.Status.volumeGainAtt = volumeMatch;
	volumeStruct.Status.outputGain = OUTPUT_GAIN_0dB;
	volumeStruct.Status.softStepAction = SOFT_STEP_ACTION_ACT;
	
	dataBytes[1] = volumeStruct.Byte.byte0;
	
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)TDA7718N_ADDRESS_RECEIVE_MODE, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		//	_Error_Handler(__FILE__, __LINE__); 
	}
	return HAL_I2C_STATE_READY;
}

HAL_I2C_StateTypeDef SoftStep_1_Adjust(SoftStep_1_Struct* softStep_1_Struct) {
	uint8_t dataBytes[2];
	SubaddressStruct subaddressStruct;
	
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY) return HAL_I2C_STATE_BUSY;
	
	subaddressStruct.Status.selection = SOFT_STEP1_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	dataBytes[0] = subaddressStruct.Byte.byte0;
	dataBytes[1] = softStep_1_Struct->Byte.byte0;
	
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)TDA7718N_ADDRESS_RECEIVE_MODE, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		// todo error
	}
	return HAL_I2C_STATE_READY;
}

HAL_I2C_StateTypeDef SoftStep_2_Adjust(SoftStep_2_Struct* softStep_2_Struct) {
	uint8_t dataBytes[2];
	SubaddressStruct subaddressStruct;
	
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY) return HAL_I2C_STATE_BUSY;
	
	subaddressStruct.Status.selection = SOFT_STEP2_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	dataBytes[0] = subaddressStruct.Byte.byte0;
	dataBytes[1] = softStep_2_Struct->Byte.byte0;
	
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)TDA7718N_ADDRESS_RECEIVE_MODE, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		// todo error
	}
	return HAL_I2C_STATE_READY;
}

HAL_I2C_StateTypeDef LoudnessAdjust(LoudnessStruct* loudnessStruct) {
	uint8_t dataBytes[2];
	SubaddressStruct subaddressStruct;
	
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY) return HAL_I2C_STATE_BUSY;
	
	subaddressStruct.Status.selection = LOUDNESS_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	dataBytes[0] = subaddressStruct.Byte.byte0;
	dataBytes[1] = loudnessStruct->Byte.byte0;
	
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)TDA7718N_ADDRESS_RECEIVE_MODE, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		// todo error
	}
	return HAL_I2C_STATE_READY;
}

HAL_I2C_StateTypeDef BassAdjust(BassFilterStruct* bassFilterStruct) {
	uint8_t dataBytes[2];
	SubaddressStruct subaddressStruct;
	
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY) return HAL_I2C_STATE_BUSY;
	
	subaddressStruct.Status.selection = BASS_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	dataBytes[0] = subaddressStruct.Byte.byte0;
	dataBytes[1] = bassFilterStruct->Byte.byte0;
	
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)TDA7718N_ADDRESS_RECEIVE_MODE, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		// todo error
	}
	return HAL_I2C_STATE_READY;
}

HAL_I2C_StateTypeDef TrebleAdjust(TrebleFilterStruct* trebleFilterStruct) {
	uint8_t dataBytes[2];
	SubaddressStruct subaddressStruct;
	
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY) return HAL_I2C_STATE_BUSY;
	
	subaddressStruct.Status.selection = TREBLE_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	dataBytes[0] = subaddressStruct.Byte.byte0;
	dataBytes[1] = trebleFilterStruct->Byte.byte0;
	
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)TDA7718N_ADDRESS_RECEIVE_MODE, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		// todo error
	}
	return HAL_I2C_STATE_READY;
}

HAL_I2C_StateTypeDef MiddleAdjust(MiddleFilterStruct* middleFilterStruct) {
	uint8_t dataBytes[2];
	SubaddressStruct subaddressStruct;
	
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY) return HAL_I2C_STATE_BUSY;
	
	subaddressStruct.Status.selection = MIDDLE_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	dataBytes[0] = subaddressStruct.Byte.byte0;
	dataBytes[1] = middleFilterStruct->Byte.byte0;
	
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)TDA7718N_ADDRESS_RECEIVE_MODE, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		// todo error
	}
	return HAL_I2C_STATE_READY;
}

uint8_t speakerGainAttBytes[2];
HAL_I2C_StateTypeDef SpeakerGainAttAdjust(SubaddressStruct* subaddressStruct, SpeakerGainAttStruct* speakerGainAttStruct) {
	uint8_t dataBytes[2];
	
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY) return HAL_I2C_STATE_BUSY;
	
	dataBytes[0] = subaddressStruct->Byte.byte0;
	dataBytes[1] = speakerGainAttStruct->Byte.byte0;
	speakerGainAttBytes[0] = subaddressStruct->Byte.byte0;
	speakerGainAttBytes[1] = speakerGainAttStruct->Byte.byte0;
	
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)TDA7718N_ADDRESS_RECEIVE_MODE, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		// todo error
	}
	return HAL_I2C_STATE_READY;
}

HAL_I2C_StateTypeDef SubwooferMiddleBassAdjust(SubwooferMiddleBassStruct* subwooferMiddleBassStruct) {
	uint8_t dataBytes[2];
	SubaddressStruct subaddressStruct;
	
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY) return HAL_I2C_STATE_BUSY;
	
	subaddressStruct.Status.selection = SUBWOOFER_MIDDLE_BASS_SUB_ADDR;
	subaddressStruct.Status.autoIncMode = AUTO_INCREMENT_OFF_SUB_ADDR;
	subaddressStruct.Status.testingMode = TESTING_MODE_OFF_SUB_ADDR;
	
	dataBytes[0] = subaddressStruct.Byte.byte0;
	dataBytes[1] = subwooferMiddleBassStruct->Byte.byte0;
	
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)TDA7718N_ADDRESS_RECEIVE_MODE, (uint8_t*)dataBytes, 2, 100) != HAL_OK) {
		// todo error
	}
	return HAL_I2C_STATE_READY;
}

#endif // USE_TDA7721

