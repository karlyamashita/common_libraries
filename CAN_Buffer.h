#ifndef CAN_BUFFER_H
#define CAN_BUFFER_H


#define CAN_RX_QUEUE_SIZE 8
#define CAN_TX_QUEUE_SIZE 8


// Receive
typedef struct CanRxMsgTypeDef
{
	CAN_RxHeaderTypeDef header;
	uint8_t data[8];
}CanRxMsgTypeDef;

// Transmit
typedef struct CanTxMsgTypeDef
{
	CAN_TxHeaderTypeDef header;
	uint8_t data[8];
}CanTxMsgTypeDef;

typedef struct
{
	CAN_HandleTypeDef *hcan; // CAN instance

	// Rx
	CanRxMsgTypeDef rxQueue[CAN_RX_QUEUE_SIZE];
	uint32_t rxQueueSize;
	CanRxMsgTypeDef *msgToParse; // pointer to latest queue
	RING_BUFF_STRUCT rxPtr; // queue pointer
	bool canBusActive; // used for CAN bus activity to keep STM32 from going to sleep.
	// Tx
	CanTxMsgTypeDef txQueue[CAN_TX_QUEUE_SIZE];
	uint32_t txQueueSize;
	RING_BUFF_STRUCT txPtr; // queue pointer
}CAN_MsgStruct;

int CAN_SendMessage(CAN_MsgStruct *msg);
int CAN_AddTxBuffer(CAN_MsgStruct *msg, CanTxMsgTypeDef *txData);
bool CAN_DataAvailable(CAN_MsgStruct *msg);
bool CAN_GetBusActivity(CAN_MsgStruct *msg);
void CAN_ClearBusActivity(CAN_MsgStruct *msg);
void MsgCopy(CanTxMsgTypeDef *TxMsg, CanRxMsgTypeDef *RxMsg);
	
#endif // CAN_BUFFER_H

