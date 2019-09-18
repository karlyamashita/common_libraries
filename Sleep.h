#include "LSP.h"
#ifdef USE_SLEEP
#include "main.h"
#include "callback.h"

#ifndef SLEEP_H
#define SLEEP_H

void SleepCheck(uint32_t theSleepTime, Callback callback);
void SleepReset(void);

#endif // SLEEP_H
#endif // USE_SLEEP
