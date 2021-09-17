/*
 * SPI_Init.h
 *
 *  Created on: Jul 27, 2021
 *      Author: karl.yamashita
 */

#ifndef SRC_SPI_INIT_H_
#define SRC_SPI_INIT_H_

int SPI_Init(void);
void SpiHandler(void *CallBackRef, u32 StatusEvent, unsigned int ByteCount);
void SPI_SetTransferInProgress(bool status);
bool SPI_GetTransferInProgress(void);
void SPI_SetErrorCount(uint32_t count);
int SPI_GetErrorCount(void);

#endif /* SRC_SPI_INIT_H_ */
