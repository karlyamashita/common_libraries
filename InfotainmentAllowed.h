

#ifndef INFOTAINMENT_ALLOWED_H
#define INFOTAINMENT_ALLOWED_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"


// start gateway
void ServiceInfotainmentAllowed(uint8_t *data);
bool GetInfotainmentAllowedStatus(void);
// end gateway


// start radio replacement
void SendInfotainmentAllowedCallbackCAN1(int status);

void SendInfotainmentAllowed(bool flag);

#endif //INFOTAINMENT_ALLOWED_H
