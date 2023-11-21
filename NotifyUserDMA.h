/*
 * NotifyUserDMA.h
 *
 *  Created on: Jan 3, 2023
 *      Author: karl.yamashita
 */

#ifndef INC_NOTIFYUSERDMA_H_
#define INC_NOTIFYUSERDMA_H_

void NotifyUserDMA(UART_DMA_QueueStruct *msg, char *str, bool lineFeed);

#endif /* INC_NOTIFYUSERDMA_H_ */
