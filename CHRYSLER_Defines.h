#include "LSP.h"
#ifdef USE_CHRYSLER_DEFINES
#ifndef CHRYSLER_DEFINES_H
#define CHRYSLER_DEFINES_H

// Steering Wheel Controls
#define SWC_BUTTON_RELEASE 0x00
#define SWC_LEFT_UP 0x10
#define SWC_LEFT_DOWN 0x20
#define SWC_LEFT_MIDDLE 0x40
#define SWC_RIGHT_UP 0x04
#define SWC_RIGHT_DOWN 0x08
#define SWC_RIGHT_MIDDLE 0x01

//Power Mode
#if defined CHRYSLER_V5 // need to check if this is for CRYSLER_V4 or V3
#define POWER_MODE_OFF 0x00
#define POWER_MODE_ACCESSORY 0x03
#define POWER_MODE_IGNITION 0x04
#define POWER_MODE_CRANK 0x15
#define POWER_MODE_MASK 0x1F
#elif defined CHRYSLER_V3
#define POWER_MODE_OFF 0x00
#define POWER_MODE_ACCESSORY 0x03
#define POWER_MODE_IGNITION 0x04
#define POWER_MODE_CRANK 0x05
#define POWER_MODE_MASK 0x03


//Power Mode LSFTCAN
#elif defined CHRYSLER_V2
#define POWER_MODE_OFF 0x00
#define POWER_MODE_ACCESSORY 0x03
#define POWER_MODE_IGNITION 0x04
#define POWER_MODE_CRANK 0x05
#define POWER_MODE_MASK 0x03
#else
#warning "need to define which powerMode mask to use"
#endif // USE_LSFTCAN_POWERMODE

// Gear Information
#define GEAR_PARK 0x0D
#define GEAR_REVERSE 0x0B
#define GEAR_NUETRAL 0x00
#define GEAR_DRIVE 0x01

// 74HCT4052 or FSAV331 switches
#define NO_VIDEO 0xFF
#define REAR_VIDEO 0
#define FRONT_VIDEO 1
#define LEFT_VIDEO 2
#define RIGHT_VIDEO 3

#endif // CHRYSLER_DEFINES_H
#endif // USE_CHRYSLER_DEFINES
