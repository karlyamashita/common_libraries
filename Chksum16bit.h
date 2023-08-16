/* 
 * File:   Checksum16bit.h
 * Author: karl.yamashita
 *
 * Created on May 18, 2023, 11:38 AM
 */

#ifndef CHECKSUM16BIT_H
#define	CHECKSUM16BIT_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "main.h"

bool ChkSum16_Validate(uint8_t *data, uint8_t len, bool invert);
void ChkSum16_Calculate(uint8_t *data, uint8_t len, bool invert);


#ifdef	__cplusplus
}
#endif

#endif	/* CHECKSUM16BIT_H */

