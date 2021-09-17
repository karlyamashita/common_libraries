/*
 * NotifyUser.h
 *
 *  Created on: Sep 15, 2021
 *      Author: karl.yamashita
 */

#ifndef NOTIFYUSER_H_
#define NOTIFYUSER_H_

void NotifyErrorStatus(uint8_t error);
void NotifyFPGA_Addr_0x22_Value(uint8_t value);
void NotifyAck(void);
void NotifyNack(void);
void NotifyResetCause(void);

#endif /* NOTIFYUSER_H_ */
