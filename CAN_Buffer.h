/*
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Karl Yamashita
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */

#ifndef CAN_BUFFER_H
#define CAN_BUFFER_H



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
	CanRxMsgTypeDef *rxQueue;
	uint32_t rxQueueSize;
	CanRxMsgTypeDef *msgToParse; // pointer to latest queue
	RING_BUFF_STRUCT rxPtr; // queue pointer
	bool canBusActive; // used for CAN bus activity to keep STM32 from going to sleep.
	uint32_t rxCounter; // for debugging
	// Tx
	CanTxMsgTypeDef *txQueue;
	uint32_t txQueueSize;
	RING_BUFF_STRUCT txPtr; // queue pointer
	uint32_t txCounter; // for debugging
}CAN_MsgStruct;

int CAN_SendMessage(CAN_MsgStruct *msg);
int CAN_AddTxBuffer(CAN_MsgStruct *msg, CanTxMsgTypeDef *txData);
bool CAN_DataAvailable(CAN_MsgStruct *msg);
bool CAN_GetBusActivity(CAN_MsgStruct *msg);
void CAN_ClearBusActivity(CAN_MsgStruct *msg);
void MsgCopy(CanTxMsgTypeDef *TxMsg, CanRxMsgTypeDef *RxMsg);
	
#endif // CAN_BUFFER_H

