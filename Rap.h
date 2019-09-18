#include "LSP.h"


#ifndef RAP_H
#define RAP_H

#include "main.h"
#include "Sleep.h"

#ifdef USE_GMLAN_DEFINES
void SetRapTimerFlag(bool flag);
void CheckRapStatus(uint32_t rapTimer, Callback callback);
#endif // USE_GMLAN_DEFINES

#if defined USE_CHRYSLER_DEFINES

void ServiceRapMode(uint8_t *data);
uint8_t GetRapModeFlag(void);

#endif // USE_CHRYSLER_DEFINES

#endif //RAP_H

