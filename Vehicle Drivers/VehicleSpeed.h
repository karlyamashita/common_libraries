#include "LSP.h"
#ifdef USE_VEHICLE_SPEED

#ifndef VEHICLE_SPEED_H
#define VEHICLE_SPEED_H

#include "main.h"

#if defined GMLAN_V1

void ServiceVehicleSpeed(uint8_t *data);
uint8_t GetVehicleSpeed(void);

#endif // GMLAN_V1

#ifdef CHRYSLER_V5

void ServiceVehicleSpeed(uint8_t *data);
uint8_t GetVehicleSpeed(void);

#endif // CHRYSLER_V5

#ifdef CHRYSLER_V3

void ServiceVehicleSpeed(uint8_t *data);
uint8_t GetVehicleSpeed(void);

#endif // CHRYSLER_V3

#endif // VEHICLE_SPEED_H
#endif // USE_VEHICLE_SPEED
