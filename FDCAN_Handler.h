/*
 * FDCAN_Handler.h
 *
 *  Created on: Oct 7, 2023
 *      Author: karl.yamashita
 */

#ifndef INC_FDCAN_HANDLER_H_
#define INC_FDCAN_HANDLER_H_


#define FDCAN_RX_BUF_SIZE 8
#define FDCAN_TX_BUF_SIZE 4


typedef struct
{
	FDCAN_RxHeaderTypeDef *pRxHeader;
	uint8_t data[64];
}FDCAN_Rx;

typedef struct
{
	FDCAN_TxHeaderTypeDef pTxHeader;
	uint8_t data[64];
}FDCAN_Tx;


typedef struct
{
	struct
	{
		FDCAN_HandleTypeDef *fdcan;
		FDCAN_Rx queue[FDCAN_RX_BUF_SIZE];
		FDCAN_Rx *msgToParse;
		RING_BUFF_STRUCT ptr;
		uint32_t queueSize;
	}rx;
	struct
	{
		FDCAN_HandleTypeDef *fdcan;
		FDCAN_Tx queue[FDCAN_TX_BUF_SIZE];
		RING_BUFF_STRUCT ptr;
		uint32_t queueSize;
	}tx;

}FDCAN_Buffer;


void FDCAN_InitInstance(FDCAN_Buffer *fdcanBuffer, FDCAN_HandleTypeDef *fdcan);

void FDCAN_AddToRxBuffer(FDCAN_Buffer *fdcanBuffer, FDCAN_Rx *rxData);

void FDCAN_AddDataToTxBuffer(FDCAN_Buffer *fdcanBuffer, FDCAN_Tx *data);
void FDCAN_Transmit(FDCAN_Buffer *fdcanBuffer);


#endif /* INC_FDCAN_HANDLER_H_ */
