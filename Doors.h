
#ifndef DOORS_H
#define DOORS_H


void ServiceDriversDoorStatus(unsigned char *data);
void ServicePassengerDoorStatus(unsigned char *data);
bool GetDriversDoorStatus(void);

#endif // DOORS_H
