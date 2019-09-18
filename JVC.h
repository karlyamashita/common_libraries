
#ifndef JVC_H
#define JVC_H


#define JVC_CUSTOM_CODE 0x8f

#define JVC_VOLUME_UP	0x84
#define JVC_VOLUME_DOWN	0x85
#define JVC_MUTE 0x8e
#define JVC_SOURCE 0x88
#define JVC_TRACK_UP 0x92
#define JVC_TRACK_DOWN 0x93
#define JVC_BAND_DISC_UP	0x94
#define JVC_PRESET_DISC_DOWN	0x95
#define JVC_SELECT 0x96
#define JVC_ATT 0x86
#define JVC_PHONE_RECEIVE 0x98
#define JVC_PHONE_REJECT 0x99
#define JVC_VOICE_DIAL 0x9a
#define JVC_POWER 0x80


void JVC_Command(unsigned char command);


#endif // JVC_H

