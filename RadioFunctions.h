
#ifndef RADIO_FUNCTIONS_H
#define RADIO_FUNCTIONS_H

#include "main.h"

#ifndef SWC_Pin
#define SWC_Pin 0
#warning "SWC_Pin was not defined!"
#endif // SWC_Pin

void RadioCommands(uint8_t command);

#endif // RADIO_FUNCTIONS_H
