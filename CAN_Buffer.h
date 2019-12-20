#include "LSP.h"

#ifdef USE_CAN_BUFFER
#ifndef CAN_BUFFER_H
#define CAN_BUFFER_H

#include "RingBuff.h"

#define CAN_MAX_RX_BUFF 8
#define CAN_MAX_TX_BUFF 8

#define CAN_STD_ID 0x00
#define CAN_EXT_ID 0x04


// Receive
typedef struct CanRxMsgTypeDef{
	CAN_RxHeaderTypeDef CAN_RxHeaderTypeDef;
	uint8_t Data[8];
}CanRxMsgTypeDef;

// Transmit
typedef struct CanTxMsgTypeDef{
	CAN_TxHeaderTypeDef CAN_TxHeaderTypeDef;
	uint8_t Data[8];
}CanTxMsgTypeDef;


int SendCanTxMessage1(CAN_HandleTypeDef *hcan);
void AddCanTxBuffer1(CanTxMsgTypeDef *canMsg);
void AddCanRxBuffer1(CanRxMsgTypeDef *canMsg);
uint8_t Can1DataAvailable(CanRxMsgTypeDef *canMsg);

#ifdef USE_CAN_BUFFER_2
int SendCanTxMessage2(CAN_HandleTypeDef *hcan);
void AddCanTxBuffer2(CanTxMsgTypeDef *canMsg);
void AddCanRxBuffer2(CanRxMsgTypeDef *canMsg);
uint8_t Can2DataAvailable(CanRxMsgTypeDef *canMsg);
#endif // USE_CAN_BUFFER_2

void MsgCopy(CanTxMsgTypeDef *TxMsg, CanRxMsgTypeDef *RxMsg);
	
#endif // CAN_BUFFER_H
#endif // USE_CAN_BUFFER
