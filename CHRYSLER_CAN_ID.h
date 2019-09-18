#include "LSP.h"
#ifdef USE_CHRYSLER_ID
#ifndef CHRYSLER_CAN_ID
#define CHRYSLER_CAN_ID


#ifdef CHRYSLER_V5
// MSCAN (IHS)
// Dodge Durango 2017 // // hands free on BOTH front speaker outputs
// Dodge Charger 2016 // hands free only on front RIGHT speaker output
// Jeep Grand Cherokee 2017 // hands free only on front RIGHT speaker output
// Dodge Ram 2017 // hands free only on front RIGHT speaker output

#define POWER_MODE_ID 0x122
#define RADIO_AUDIO_STATUS_ID 0x30F 
#define RADIO_PHONE_AUDIO_STATUS_ID 0x30D // Hands free uses seperate volume
#define SWC_TURN_SIGNAL_ID 0x318// SWC BT Command VR and Answer in Dodge Ram Diesle Truck
#define SWC_ID 0x22D // basic audio controls. Dodge Ram SWC ID is 0x318
#define LIGHTING_STATUS_ID 0x3B2
#define DIMMER_DOOR_PARKING_BRAKE_ID 0x2FA
#define BRAKE_GEAR_INFO_ID 0x332
#define RAP_ID 0x12B
#define DASH_BUTTONS_ID 0x2D3
#define AMP_STATUS_ID 0x354
#define VEHICLE_SPEED_ID 0x340 // or 0x322?
#define VIN_ID 0x3E0
#define DASH_BUTTONS_ID 0x2D3
#define VOLUME_TUNE_KNOB_ID 0x273
#define RPM_SPEED_ID 0x322
#define TURN_SIGNALS_SWITCH_ACTIVE_ID 0x2A8

// Chrysler 300 2015
//#define SWC_ID 0x22D 
/*
Chrysler 300 2015
No parking light id. Use dimmer ID 0x2fa. Doors are 0x2fa
SWC id is 0x22d
Low gear 0x4c state encoded
Rap is same
Windows open position from 0-0x38 or 200%
Mirrors same
Flash to pass or high beams not available
Turn signals same
*/


// below for Jeep Cherokee 2015-2017, 52 pin connector
#define POWER_MODE_JEEP_CHEROKEE_ID 0x20B
#define HYDRALIC_BRAKE_JEEP_CHEROKEE_ID 0x5DC
#define GEAR_INFORMATION_JEEP_CHEROKEE_ID 0x190
#define TURN_SIGNAL_JEEP_CHEROKEE_ID 0x4A1
#define DIMMER_JEEP_CHEROKEE_ID 0x5CA
#define LIGHTING_STATUS_JEEP_CHEROKEE_ID 0x337
#define DOOR_STATUS_JEEP_CHEROKEE_ID 0x339
#define RAP_JEEP_CHEROKEE_ID 0x192
#define VIN_JEEP_CHEROKEE_ID 0x3D3
#define VOICE_ANSWER_HANGUP_ID 0x4A3
#define HYDRAULIC_BRAKES_ID 0x5DC
#define DIMMER_ID 0x5CA // 34-200



#endif // CHRYSLER_V5


#ifdef CHRYSLER_V4 // what version are these?
// MSCAN
// White 22 pin connector?
// Chrysler 300 2011-2014
// Dodge Charger 2011-2014
// Dodge Journey 2011-2015



#endif // CHRYSLER_V4


#ifdef CHRYSLER_V3
// MSCAN
// Gray 22 pin connector

// Dodge Grand Caravan 2017
#define POWER_MODE_ID 0x20B
#define SWC_ID 0x3A3
#define LIGHTING_STATUS_ID 0x208 // RAP, turn signals status (not switch active).
#define DOOR_STATUS_ID 0x244
#define FOOT_BRAKE_GEAR_ID 0x20E
#define RPM_ID 0x211
#define DIMMER_STATUS_ID 0x308
#define RADIO_AUDIO_STATUS_ID 0x3D9
#define PARKING_BRAKE_ID 0x2E7
#define LIGHT_STATUS_SWITCH_ID 0x2E1 // turn signals
#define RADIO_SOH_ID 0x416
#define TURN_SIGNALS_ID 0x2A8 // switch active
#define XM_TEXT_TO_RADIO_ID 0x294
#define VIN_ID 0x219

#endif // CHRYSLER_V3

#ifdef CHRYSLER_V2
//LSFTCAN
// Gray 22 pin connector
#define POWER_MODE_ID 0x000
#define RADIO_AUDIO_STATUS_ID 0x3D0//
#define DOOR_STATUS_ID 0x013
#define AMPLIFIER_STATUS_ID 0x3F1 // DLC=2, B0=0x42,B1=0x00. This turns radio into fixed level audio signal
#define TURN_SIGNAL_ID 0x11D
#define LIGHTING_STATUS_ID 0x1C8
#define PARKING_BRAKE_ID 0x0D0
#define GEAR_INFORMATION_ID 0x003
#define VIN_ID 0x01B
#define DIMMER_ID 0x210
#define RADIO_SOH_ID 0x416
#define GEAR_FOOT_BRAKE_ID 0x003
#define BACK_UP_SENSOR_ID 0x1A4
#define SWC_ID 0x3A0
#define AMPLIFIER_SOH_ID 0x43E
#define UCONNECT_ID 0x08F

#define RAP_ID 0x006 // 2007-2016 Patriot
#define RAP2_ID 0x210 // 2016 Patriot, need to verify

#endif // CHRYSLER_V2



#endif // CHRYSLER_CAN_ID
#endif // USE_CHRYSLER_ID

