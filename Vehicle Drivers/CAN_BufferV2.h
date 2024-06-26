#ifndef CAN_BUFFER_V2_H
#define CAN_BUFFER_V2_H

#include "RingBuff.h"

// typically this will be 8
#define CAN_RX_BUFF_SIZE 8
#define CAN_TX_BUFF_SIZE 8

// increase if buffer over run.
#define CAN_RX_QUEUE_SIZE 4
#define CAN_TX_QUEUE_SIZE 4


// Receive
typedef struct
{
    struct
    {
	    CAN_RxHeaderTypeDef header;
	    uint8_t data[CAN_RX_BUFF_SIZE];
    }QUEUE[CAN_RX_QUEUE_SIZE];

    RING_BUFF_INFO ptr;
}CanRxMsg_Struct;

// Transmit
typedef struct
{
    struct
	{
	    CAN_TxHeaderTypeDef header;
	    uint8_t data[CAN_TX_BUFF_SIZE];
    }QUEUE[CAN_TX_QUEUE_SIZE];

    RING_BUFF_INFO ptr;
}CanTxMsg_Struct;

// temporary rxMsg buffer without queue buffer
typedef struct
{
	CAN_RxHeaderTypeDef header;
	uint8_t data[CAN_RX_BUFF_SIZE];
}CanRxMsgTemp_Struct;

// temporary txMsg buffer without queue buffer
typedef struct
{
	CAN_TxHeaderTypeDef header;
	uint8_t data[CAN_TX_BUFF_SIZE];
}CanTxMsgTemp_Struct;


void CAN_SendTxMessage(CAN_HandleTypeDef *hcan, CanTxMsg_Struct *canMsg);
void CAN_AddTxBuffer(CanTxMsg_Struct *canMsg_Out, CanTxMsgTemp_Struct *canMsg);
void AddCanRxBuffer(CanRxMsg_Struct *canMsg_Out, CanRxMsg_Struct *canMsg);
void CAN_GetMessage(CAN_HandleTypeDef *hcan, CanRxMsg_Struct *rxMsg);
int CAN_MsgAvailable(CanRxMsg_Struct *rxMsg, CanTxMsgTemp_Struct *canMsg);


	
#endif // CAN_BUFFER_V2_H
