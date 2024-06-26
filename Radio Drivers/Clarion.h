
#ifndef CLARION_H
#define CLARION_H

#include "main.h"

#define CLARION_CUSTOM_CODE 0x8660

#define CLARION_VOLUME_UP 0x1b
#define CLARION_VOLUME_DOWN 0x1d
#define CLARION_MUTE 0x05
#define CLARION_SOURCE 0x03
#define CLARION_TRACK_UP 0x17
#define CLARION_TRACK_DOWN 0x16
#define CLARION_BAND 0x0a
#define CLARION_SEND_END 0x25

// custom code 2
#define CLARION_CUSTOM_CODE_TRUE 1
#define CLARION_CUSTOM_CODE_2 0x833b
#define CLARION_SEND 0x5f
#define CLARION_END 0x60

void ClarionCommand(uint8_t	command);

	
#endif // CLARION_H
