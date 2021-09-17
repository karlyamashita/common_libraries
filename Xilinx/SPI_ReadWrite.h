/*
 * SPI_ReadWrite.h
 *
 *  Created on: Jul 27, 2021
 *      Author: karl.yamashita
 */

#ifndef SRC_SPI_READWRITE_H_
#define SRC_SPI_READWRITE_H_

int SPI_WriteData(uint8_t *SendBufPtr, uint8_t ByteCount);
int SPI_TransferData(uint8_t *SendBufPtr, uint8_t *RecvBufPtr, uint8_t ByteCount);
int SPI_WaitForNotBusy(void);


#endif /* SRC_SPI_READWRITE_H_ */
