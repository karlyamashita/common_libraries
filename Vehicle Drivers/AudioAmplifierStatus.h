#ifndef AUDIO_AMPLFIER_H
#define AUDIO_AMPLFIER_H

#include "main.h"

#if defined GMLAN_V1

typedef union AudioAmplifierStatus{
	struct {
		uint8_t audioAmplifierStatusByte;
	}Byte;
	struct {
		unsigned dspPresentBit:1;
		unsigned surroundSoundSystemPresentBit:1;
		unsigned noiseCompressionPresentBit:1;
		unsigned virtualDevicePresentBit:1;
		unsigned :1;
		unsigned :1;
		unsigned :1;
		unsigned :1;
	} AmplifierControlBits;
}AudioAmplifierStatus;



void ServiceAudioAmplifier(uint8_t *data);
void SendAmplifierControl(uint8_t targetSink, uint8_t connectCommand, \
	uint8_t channelType, uint8_t contentType );
void SendAmplifierSetting(uint8_t targetSink, uint8_t settingType,\
							 uint8_t settingData, uint8_t settingTime);
void SendAmplifierMute(uint8_t targetSink, uint8_t amplifierMute, uint8_t settingTime);
void SendAmplifierStatus(void);							 
void ServiceAmplifierData(void);
uint8_t GetSurroundSoundSystemPresentBit(void);
uint8_t GetAmplifierPresentFlag(void);

#endif // GMLAN_V1

#if defined CHRYSLER_V2
void ServiceAudioAmplifierStatus(uint8_t *data);
uint8_t GetAmplifierPresentFlag(void);
void EmulateChryslerAmplifier(void);
#endif // CHRYSLER_V2

							 
#endif // AUDIO_AMPLFIER_H
