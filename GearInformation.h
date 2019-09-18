
#ifndef GEAR_INFORMATION_H
#define GEAR_INFORMATION_H

#if defined GMLAN_V1

#define BETWEEN_RANGES 0
#define PARK_RANGE 1
#define REVERSE_RANGE 2
#define NUETRAL_RANGE 3
#define FORWARD_RANGE_A 4

#endif // GMLAN_V1

#if defined USING_GMLAN_V2

// TODO -  need to verify these values
#define BETWEEN_RANGES 0
#define PARK_RANGE 0x0F
#define REVERSE_RANGE 0x0E
#define NUETRAL_RANGE 0x0D
#define DRIVE_RANGE 0x01

#endif // USING_GMLAN_V2

#if defined CHRYSLER_V3

#define PARK_RANGE 0x0D
#define REVERSE_RANGE 0x0B
#define NUETRAL_RANGE 0x00
#define DRIVE_RANGE 0x01

#endif // CHYSLER_V3

void ServiceGearInformation(uint8_t *data);
uint8_t GetGearInformation(void);


#endif // GEAR_INFORMATION_H
