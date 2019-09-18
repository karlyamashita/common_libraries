#include "LSP.h"
#ifdef USE_AUDIO_AMPLIFIER_STATUS
#include "main.h"
#include "AudioAmplifierStatus.h"
#include "can.h"
#include "gpio.h"

#if defined GMLAN_V1

#include "GMLAN_Defines.h"

uint8_t channelTypeData = STEREO;
uint8_t contentTypeData = DVD_AUDIO;
uint8_t amplifierPresentFlag, sentAmplifierSettingsFlag = 0;


AudioAmplifierStatus audioAmplifierStatus;
#define audioAmplifierStatusByte audioAmplifierStatus.Byte.audioAmplifierStatusByte
#define dsp_PresentBit audioAmplifierStatus.AmplifierControlBits.dspPresentBit
#define surroundSoundSystemPresentBit audioAmplifierStatus.AmplifierControlBits.surroundSoundSystemPresentBit
#define noiseCompressionPresentBit audioAmplifierStatus.AmplifierControlBits.noiseCompressionPresentBit
#define virtualDevicePresentBit audioAmplifierStatus.AmplifierControlBits.virtualDevicePresentBit 

enum{
	AMP_CONTROL_CONNECT,
	AMP_MUTE,
	AMP_VOLUME,
	AMP_FADE,
	AMP_BALANCE,
	AMP_BASS,
	AMP_MIDBASS,
	AMP_TREBLE,
	AMP_DIGITALPROC,
	AMP_NOISE,
	AMP_CONTROL_CONNECT2,
	AMP_UNMUTE,
};



union AudioMasterAmplifierControl
{
	struct{
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct{
		unsigned contentType:5;
		unsigned channelType:3;
		unsigned connectCommand:1;
		unsigned targetSink:3;
		unsigned :4;
	}Status;
}AudioMasterAmplifierControl;
#define audioMasterAmplifierControlByte0 AudioMasterAmplifierControl.Bytes.byte0
#define audioMasterAmplifierControlByte1 AudioMasterAmplifierControl.Bytes.byte1
#define audioMasterAmplifierControlTargetSink AudioMasterAmplifierControl.Status.targetSink
#define audioMasterAmplifierControlConnectCommand AudioMasterAmplifierControl.Status.connectCommand
#define audioMasterAmplifierControlChannelType AudioMasterAmplifierControl.Status.channelType
#define audioMasterAmplifierControlContentType AudioMasterAmplifierControl.Status.contentType

union AudioMasterAmplifierSetting
{
	struct{
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct{
		uint8_t settingTime;
		uint8_t settingData;
		unsigned settingsType:4;
		unsigned targetSink:3;
		unsigned :9;
	}Status;
}AudioMasterAmplifierSetting;
#define audioMasterAmplifierSettingByte0 AudioMasterAmplifierSetting.Bytes.byte0
#define audioMasterAmplifierSettingByte1 AudioMasterAmplifierSetting.Bytes.byte1
#define audioMasterAmplifierSettingByte2 AudioMasterAmplifierSetting.Bytes.byte2
#define audioMasterAmplifierSettingByte3 AudioMasterAmplifierSetting.Bytes.byte3
#define audioMasterAmplifierSettingTargetSink AudioMasterAmplifierSetting.Status.targetSink
#define audioMasterAmplifierSettingSettingsType AudioMasterAmplifierSetting.Status.settingsType
#define audioMasterAmplifierSettingSettingData AudioMasterAmplifierSetting.Status.settingData
#define audioMasterAmplifierSettingSettingTime AudioMasterAmplifierSetting.Status.settingTime

union AudioMasterAmplifierMute
{
	struct{
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct{
		uint8_t settingsTime;
		unsigned muteCommand:1;
		unsigned targetSink:3;
		unsigned :4;
	}Status;
}AudioMasterAmplifierMute;
#define audioMasterAmplifierMuteByte0 AudioMasterAmplifierMute.Bytes.byte0
#define audioMasterAmplifierMuteByte1 AudioMasterAmplifierMute.Bytes.byte1
#define audioMasterAmplifierMuteTarketSink AudioMasterAmplifierMute.Status.targetSink
#define audioMasterAmplifierMuteMuteCommand AudioMasterAmplifierMute.Status.muteCommand
#define audioMasterAmplifierMuteSettingsTime AudioMasterAmplifierMute.Status.settingsTime

// Connection request to amplifier
void SendAmplifierControl(uint8_t targetSink, uint8_t connectCommand, uint8_t channelType, uint8_t contentType ){
	audioMasterAmplifierControlTargetSink = targetSink;
	audioMasterAmplifierControlConnectCommand = connectCommand;
	audioMasterAmplifierControlChannelType = channelType;
	audioMasterAmplifierControlContentType = contentType;
}

// Send settings to amplifier, ex; Volume, balance, fader, bass, treble, etc.
void SendAmplifierSetting(uint8_t targetSink, uint8_t settingType,\
							 uint8_t settingData, uint8_t settingTime){
	audioMasterAmplifierSettingTargetSink = targetSink;
	audioMasterAmplifierSettingSettingsType = settingType;
	audioMasterAmplifierSettingSettingData = settingData;
	audioMasterAmplifierSettingSettingTime = settingTime;
}

// Mute and unmute amplifier
void SendAmplifierMute(uint8_t targetSink, uint8_t amplifierMute, uint8_t settingTime){
	audioMasterAmplifierMuteTarketSink = targetSink;
	audioMasterAmplifierMuteMuteCommand = amplifierMute;
	audioMasterAmplifierMuteSettingsTime = settingTime;
}

void SendAmplifierStatus(void) {
	static uint8_t mode = AMP_MUTE;

		if (1) { // get radio's remote output status
			if (amplifierPresentFlag) {
				if (!sentAmplifierSettingsFlag)
				{
					if (surroundSoundSystemPresentBit) {
						channelTypeData = SURROUND;
						contentTypeData = DVD_AUDIO;
					} else {
						channelTypeData = STEREO;
						contentTypeData = DVD_AUDIO;
					}
					switch (mode) {
						case AMP_MUTE:
							SendAmplifierMute(ANNOUNCEMENT1, AMP_MUTE, SETTING_TIME_0);
							mode = AMP_VOLUME;
							break;
						case AMP_VOLUME:
							SendAmplifierSetting(ENTERTAINMENT, AMP_VOLUME, VOLUME_SETTING_98, SETTING_TIME_0);
							mode = AMP_FADE;
							break;
						case AMP_FADE:
							SendAmplifierSetting(ENTERTAINMENT, AMP_FADE, MIDDLE_SETTING, SETTING_TIME_0);
							mode = AMP_BALANCE;
							break;
						case AMP_BALANCE:
							SendAmplifierSetting(ENTERTAINMENT, AMP_BALANCE, MIDDLE_SETTING, SETTING_TIME_0);
							mode = AMP_BASS;
							break;
						case AMP_BASS:
							SendAmplifierSetting(ENTERTAINMENT, AMP_BASS, MIDDLE_SETTING, SETTING_TIME_0);
							mode = AMP_MIDBASS;
							break;
						case AMP_MIDBASS:
							SendAmplifierSetting(ENTERTAINMENT, AMP_MIDBASS, MIDDLE_SETTING, SETTING_TIME_0);
							mode = AMP_TREBLE;
							break;
						case AMP_TREBLE:
							SendAmplifierSetting(ENTERTAINMENT, AMP_TREBLE, MIDDLE_SETTING, SETTING_TIME_0);
							mode = AMP_DIGITALPROC;
							break;
						case AMP_DIGITALPROC:
							SendAmplifierSetting(ENTERTAINMENT, AMP_DIGITALPROC, 0, SETTING_TIME_0);
							mode = AMP_NOISE;
							break;
						case AMP_NOISE:
							SendAmplifierSetting(ENTERTAINMENT, AMP_NOISE, 1, SETTING_TIME_0);
							mode = AMP_CONTROL_CONNECT2;
							break;
						case AMP_CONTROL_CONNECT2:
							SendAmplifierControl(ENTERTAINMENT, AUDIO_MASTER_AMPLIFIER_CONTROL_CONNECT, channelTypeData, contentTypeData);
							mode = AMP_UNMUTE;
							break;
						case AMP_UNMUTE:
							if (1) { // GetRadioRemoteStatus()
								SendAmplifierMute(ENTERTAINMENT, AMP_UNMUTE, SETTING_TIME_25);
							}
							sentAmplifierSettingsFlag = 1; 
							mode = AMP_MUTE;
							break;
					}
				}
			}
		} else { // remote is off
			if (amplifierPresentFlag) {
				if (sentAmplifierSettingsFlag) {
					SendAmplifierMute(ENTERTAINMENT, AMP_MUTE, SETTING_TIME_0);
					SendAmplifierControl(ENTERTAINMENT, AUDIO_MASTER_AMPLIFIER_CONTROL_DISCONNECT, channelTypeData, contentTypeData);
					sentAmplifierSettingsFlag = 0;
				}
			}
		}
}

void ServiceAmplifierData(void) {
		if (virtualDevicePresentBit) {
			amplifierPresentFlag = 1;
		}
		if (surroundSoundSystemPresentBit) {
			channelTypeData = SURROUND;
			contentTypeData = DVD_AUDIO;
		} else {
			channelTypeData = STEREO;
			contentTypeData = DVD_AUDIO;
		}
}

void ServiceAudioAmplifierStatus(uint8_t *data){
	audioAmplifierStatusByte = data[0];
}

uint8_t GetSurroundSoundSystemPresentBit(void) {
	return surroundSoundSystemPresentBit;
}

uint8_t GetAmplifierPresentFlag(void) {
	return amplifierPresentFlag;
}

#endif // GMLAN_V1


#ifdef CHRYSLER_V2
uint8_t amplifierPresentFlag = 0;

void ServiceAudioAmplifierStatus(uint8_t *data){
	amplifierPresentFlag = 1; // don't care about data, we just need to know amplifier is present
}

uint8_t GetAmplifierPresentFlag(void) {
	return amplifierPresentFlag;
}

void EmulateChryslerAmplifier(void) {
	
}

#endif // CHRYSLER_V2

#endif // USE_AUDIO_AMPLIFIER_STATUS

