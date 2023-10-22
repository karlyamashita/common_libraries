#ifndef CAN_BUFFER_V2_H
#define CAN_BUFFER_V2_H

#include "RingBuff.h"

// typically this will be 8
#define CAN_RX_BUFF_SIZE 8
#define CAN_TX_BUFF_SIZE 8

// increase if buffer over run.
#define CAN_RX_QUEUE_SIZE 4
#define CAN_TX_QUEUE_SIZE 4



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

typedef struct
{
    struct
    {
        struct
        {
            CanRxMsgTemp_Struct msg;
        }QUEUE[CAN_RX_QUEUE_SIZE];
        RING_BUFF_INFO ptr;
    }rx;
    
    struct 
    {
        struct
        {
            CanTxMsgTemp_Struct msg;
        }QUEUE[CAN_TX_QUEUE_SIZE];
        RING_BUFF_INFO ptr;
    }tx;
}CanMsg_Struct;


void CAN_SendTxMessage(CAN_HandleTypeDef *hcan, CanMsg_Struct *canMsg);
void CAN_AddTxBuffer(CanMsg_Struct *canMsg_Out, CanTxMsgTemp_Struct *canMsg);
void AddCanRxBuffer(CanMsg_Struct *canMsg_Out, CanRxMsg_Struct *canMsg);
int CAN_MsgAvailable(CanMsg_Struct *msg);


	
#endif // CAN_BUFFER_V2_H
