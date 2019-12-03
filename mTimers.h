#include "LSP.h"
#ifdef USE_MTIMERS

#ifndef M_TIMERS_H
#define M_TIMERS_H

uint8_t CreateTimer(void);

uint32_t GetTimer(uint8_t timer);
void SetTimer(uint8_t timer, uint32_t value);


#endif // M_TIMERS_H
#endif // USE_MTIMERS
