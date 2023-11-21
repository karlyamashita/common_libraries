/* 
 * File:   NotifyUser.h
 * Author: karl.yamashita
 *
 * Created on June 10, 2022, 12:01 PM
 */

#ifndef NOTIFYUSER_H
#define	NOTIFYUSER_H

#include "UartCharBuffer.h"


#ifdef	__cplusplus
extern "C" {
#endif


void NotifyUser(UartBufferStruct *msg, char *str, bool lineFeed);


#ifdef	__cplusplus
}
#endif

#endif	/* NOTIFYUSER_H */

