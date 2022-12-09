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

/*
 * Description: User should configure the UART port to use. Default is UART_PORT_1 if not defined.
 */
#ifndef NOTIFY_USER_UART_PORT
#define NOTIFY_USER_UART_PORT UART_PORT_1
#endif

void NotifyUser(UartTxBufferStruct *buffer, char *str, bool lineFeed);


#ifdef	__cplusplus
}
#endif

#endif	/* NOTIFYUSER_H */

