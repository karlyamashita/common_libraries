

#ifndef BRAKE_H
#define BRAKE_H

#if defined GMLAN_V1

void ServiceParkingFootBrake(unsigned char *data);
unsigned char GetParkingBrakeStatus(void);
unsigned char GetFootBrakeStatus(void);

#endif // GMLAN_V1

#if defined FORD
void ServiceParkingFootBrake(unsigned char *data);

#endif // FORD

#endif // BRAKE_H
