#ifndef CAN_BUFFER_V2_H
#define CAN_BUFFER_V2_H

#include "RingBuff.h"

#define CAN_MAX_RX_BUFF 8
#define CAN_MAX_TX_BUFF 8

#define CAN_STD_ID 0x00
#define CAN_EXT_ID 0x04


typedef struct
{
	uint8_t Data[8];
}CAN_DataBytes;

// Receive
typedef struct CanRxMsgTypeDef{
    struct
    {
	    CAN_RxHeaderTypeDef CAN_RxHeaderTypeDef;
	    CAN_DataBytes data;
    }Msg;
    RING_BUFF_INFO ptr;
}CanRxMsgTypeDef;

// Transmit
typedef struct CanTxMsgTypeDef{
    struct
    {
	    CAN_TxHeaderTypeDef CAN_TxHeaderTypeDef;
	    uint8_t data[8];
    }Msg[CAN_MAX_TX_BUFF];
    RING_BUFF_INFO ptr;
}CanTxMsgTypeDef;


int SendCanTxMessage1(CAN_HandleTypeDef *hcan, CanTxMsgTypeDef *CAN_TxMsg);
void AddCanTxBuffer1(CanTxMsgTypeDef *CAN_Msg, CanTxMsgTypeDef *canMsg);
void AddCanRxBuffer1(CanRxMsgTypeDef *CAN_Msg, CanRxMsgTypeDef *canMsg);
uint8_t Can1DataAvailable(CanRxMsgTypeDef *canMsg);

#ifdef USE_CAN_BUFFER_2
int SendCanTxMessage2(CAN_HandleTypeDef *hcan);
void AddCanTxBuffer2(CanTxMsgTypeDef *canMsg);
void AddCanRxBuffer2(CanRxMsgTypeDef *canMsg);
uint8_t Can2DataAvailable(CanRxMsgTypeDef *canMsg);
#endif // USE_CAN_BUFFER_2

void MsgCopy(CanTxMsgTypeDef *TxMsg, CanRxMsgTypeDef *RxMsg);
	
#endif // CAN_BUFFER_V2_H
