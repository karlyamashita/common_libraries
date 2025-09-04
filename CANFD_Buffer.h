#ifndef CANFD_BUFFER_H
#define CANFD_BUFFER_H



// Receive
typedef struct
{
	FDCAN_RxHeaderTypeDef header;
	uint8_t data[64];
}CANFD_RxMsgTypeDef;

// Transmit
typedef struct
{
	FDCAN_TxHeaderTypeDef header;
	uint8_t data[64];
}CANFD_TxMsgTypeDef;

typedef struct
{
	FDCAN_HandleTypeDef *hfdcan; // CAN instance

	// Rx
	CANFD_RxMsgTypeDef *rxQueue;
	uint32_t rxQueueSize;
	CANFD_RxMsgTypeDef *msgToParse; // pointer to latest queue
	RING_BUFF_STRUCT rxPtr; // queue pointer
	bool canBusActive; // used for CAN bus activity to keep STM32 from going to sleep.
	// Tx
	CANFD_TxMsgTypeDef *txQueue;
	uint32_t txQueueSize;
	RING_BUFF_STRUCT txPtr; // queue pointer
}CANFD_MsgStruct;

int CANFD_SendMessage(CANFD_MsgStruct *msg);
int CANFD_AddTxBuffer(CANFD_MsgStruct *msg, CANFD_TxMsgTypeDef *txData);
bool CANFD_DataAvailable(CANFD_MsgStruct *msg);
bool CANFD_GetBusActivity(CANFD_MsgStruct *msg);
void CANFD_ClearBusActivity(CANFD_MsgStruct *msg);
void CANFD_MsgCopy(CANFD_TxMsgTypeDef *TxMsg, CANFD_RxMsgTypeDef *RxMsg);
	
#endif // CAN_BUFFER_H

