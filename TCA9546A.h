/* 
 * File:   TCA9546A.h
 * Author: karl.yamashita
 *
 * Created on January 19, 2021, 11:37 AM
 */

#ifndef TCA9546A_H
#define	TCA9546A_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <atmel_start.h>

#define TCA9546A_ADDRESS (uint8_t)0b1110000    // 0x70
#define TCA9546A_CHANNEL_0 0x01
#define TCA9546A_CHANNEL_1 0x04 // schematic error, needs to be 0x04
#define TCA9546A_CHANNEL_2 0x02 // schematic error, needs to be 0x02
#define TCA9546A_CHANNELS_RESET 0
    
i2c_error_t TCA9546A_SetChannel(uint8_t channel);
i2c_error_t TCA9546A_GetSelectedChannel(uint8_t *data);


#ifdef	__cplusplus
}
#endif

#endif	/* TCA9546A_H */

