/*
 * SystemClock_Config.c
 *
 *  Created on: Jul 12, 2024
 *      Author: karl.yamashita
 */


#include "main.h"

#define LAUNCH_PAD

/*
 * Description: Configure the clock
 *
 *
 */
void SystemClock_Config(void)
{
    // setup clock
    //
    // Configure the device to run at 25 MHz
    //
    //
#ifdef LAUNCH_PAD
#warning "Configured for LAUNCHPAD at 16MHz!"
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); // Launchpad
#else
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN); // SIB
#endif
}

