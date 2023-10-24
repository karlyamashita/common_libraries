#ifndef CAN_BUFFER_V3_H
#define CAN_BUFFER_V3_H


// typically this will be 8
#define CAN_DATA_BUFF_SIZE 8

// increase if buffer over run.
#define CAN_RX_QUEUE_SIZE 4
#define CAN_TX_QUEUE_SIZE 4

// rxMsg buffer
typedef struct
{
	CAN_RxHeaderTypeDef header;
	uint8_t data[CAN_DATA_BUFF_SIZE];
}CanRx;

// txMsg buffer
typedef struct
{
	CAN_TxHeaderTypeDef header;
	uint8_t data[CAN_DATA_BUFF_SIZE];
}CanTx;

typedef struct
{
	CAN_HandleTypeDef *hcan;
    struct
    {
        CanRx queue[CAN_RX_QUEUE_SIZE];
        CanRx *msgToParse;
        uint32_t msgQueueSize;
        RING_BUFF_STRUCT ptr;
    }rx;
    
    struct 
    {
        CanTx queue[CAN_TX_QUEUE_SIZE];
        uint32_t msgQueueSize;
        RING_BUFF_STRUCT ptr;
    }tx;

    bool canBusActive;
}CanMsg_Struct;

void CAN_IncRxBuffer(CanMsg_Struct *msg);
int CAN_MsgAvailable(CanMsg_Struct *msg);
void CAN_AddTxBuffer(CanMsg_Struct *msg, CanTx *canMsg);
void CAN_SendTxMessage(CanMsg_Struct *canMsg);
	
#endif // CAN_BUFFER_V3_H
