#include "LSP.h"
#ifdef USE_VIN

#ifndef VIN_H
#define VIN_H


#if defined GMLAN_V1

void Service_VIN_2_9(unsigned char *data);
void Service_VIN_10_17(unsigned char *data);


int GetYear(void);
bool GetHatchbackFlag(void);

void GetVIN_2_9(unsigned char * data);
void GetVIN_10_17(unsigned char * data);
#endif // GMLAN_V1
#endif // VIN_H
#endif // USE_VIN
