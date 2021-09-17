/*
 * SPI_ReadWrite.c
 *
 *  Created on: Jul 27, 2021
 *      Author: karl.yamashita
 */


#include "main.h"
#include "SPI_ReadWrite.h"

//#define SPI_ReadWrite_PrintLine

extern XSpi Spi;


/*
 * Description: Write data.
 *
 *
 */
int SPI_WriteData(uint8_t *SendBufPtr, uint8_t ByteCount){
	int status = 0;
#ifdef SPI_ReadWrite_PrintLine
//	PrintLine("SPI_WriteData()");
#endif
	SPI_SetTransferInProgress(true);
	status = XSpi_Transfer(&Spi, SendBufPtr, NULL, ByteCount);
	if(status != 0){
		return status;
	}

	while (SPI_GetTransferInProgress());
	if(SPI_GetErrorCount() != 0) {
		SPI_SetErrorCount(0);
		return XST_FAILURE;
	}

#ifdef SPI_ReadWrite_PrintLine
	PrintLine("SPI_WriteData success");
#endif
	return status;
}

/*
 * Description: Send and receive data
 *
 *
 */
int SPI_TransferData(uint8_t *SendBufPtr, uint8_t *RecvBufPtr, uint8_t ByteCount){
	int status = 0;

#ifdef SPI_ReadWrite_PrintLine
//	PrintLine("SPI_TransferData()");
#endif
	SPI_SetTransferInProgress(true);
	status = XSpi_Transfer(&Spi, SendBufPtr, RecvBufPtr, ByteCount);
	if(status != 0){
		return status;
	}

	while (SPI_GetTransferInProgress());
	if(SPI_GetErrorCount() != 0) {
		SPI_SetErrorCount(0);
		return XST_FAILURE;
	}
#ifdef SPI_ReadWrite_PrintLine
	PrintLine("SPI_TransferData success");
#endif
	return status;
}

/*
 *
 *
 *
 */
int SPI_WaitForNotBusy(void){
	int status = 0;


	return status;
}


