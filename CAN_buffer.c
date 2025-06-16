// by Karl Yamashita September 2017

#include "main.h"


HAL_StatusTypeDef CAN_Status; // make it global for debugger window
/*
 * Description: Send available Tx queue
 */
int CAN_SendMessage(CAN_MsgStruct *msg)
{
	uint32_t CAN_Tx_Mailboxes; // indicates which tx mailbox was used.
	CanTxMsgTypeDef *ptr;

	if(msg->txPtr.cnt_Handle) // send available message
	{
		ptr = &msg->txQueue[msg->txPtr.index_OUT];
		CAN_Status = HAL_CAN_AddTxMessage(msg->hcan, &ptr->header, ptr->data, &CAN_Tx_Mailboxes);
		if (CAN_Status == HAL_OK)
		{
			RingBuff_Ptr_Output(&msg->txPtr, msg->txQueueSize); // increment queue ptr
		}
	}

	return msg->txPtr.cnt_Handle; // if no more message to handle then 0 will be returned
}

/*
 * Description: Add message to tx queue;
 * Input:
 * return: 1 if queue is full, 0 if successful
 */
int CAN_AddTxBuffer(CAN_MsgStruct *msg, CanTxMsgTypeDef *txData)
{
	CanTxMsgTypeDef *ptr;
	int i;

	if(msg->txPtr.cnt_OverFlow) return 1; // queue is full

	ptr = &msg->txQueue[msg->txPtr.index_IN];

	ptr->header.DLC = txData->header.DLC;
	ptr->header.ExtId = txData->header.ExtId;
	ptr->header.IDE = txData->header.IDE;
	ptr->header.RTR = txData->header.RTR;
	ptr->header.StdId = txData->header.StdId;
	ptr->header.TransmitGlobalTime = txData->header.TransmitGlobalTime;

	for(i = 0; i < ptr->header.DLC; i++)
	{
		ptr->data[i] = txData->data[i];
	}

	RingBuff_Ptr_Input(&msg->txPtr, msg->txQueueSize); // increment input buffer ptr

	CAN_SendMessage(msg); // try sending if not busy

	return 0;
}


/*
 * Description: msgToParse points to available message
 * Input:
 * Output: true if data available, false if no new data
 */
bool CAN_DataAvailable(CAN_MsgStruct *msg)
{
	if(msg->rxPtr.cnt_Handle)
	{
		msg->msgToParse = &msg->rxQueue[msg->rxPtr.index_OUT];
		RingBuff_Ptr_Output(&msg->rxPtr, msg->rxQueueSize); // increment output buffer ptr
		return true;
	}

	return false;
}

bool CAN_GetBusActivity(CAN_MsgStruct *msg)
{
	return msg->canBusActive;
}

void CAN_ClearBusActivity(CAN_MsgStruct *msg)
{
	msg->canBusActive = false;
}

/*
 * Description: Copy the Rx data to the Tx data.
 * 		note: The header is a different struct between Tx and Rx.
 * 				We have to copy each header over instead of memcpy.
 */
void MsgCopy(CanTxMsgTypeDef *TxMsg, CanRxMsgTypeDef *RxMsg)
{
	int i = 0;

	TxMsg->header.ExtId = RxMsg->header.ExtId;
	TxMsg->header.StdId = RxMsg->header.StdId;
	TxMsg->header.IDE = RxMsg->header.IDE;
	TxMsg->header.RTR = RxMsg->header.RTR;
	TxMsg->header.DLC = RxMsg->header.DLC;
	for(i = 0; i < TxMsg->header.DLC; i++)
	{
		TxMsg->data[i] = RxMsg->data[i];
	}
}

/*



#define CAN1_RX_QUEUE_SIZE 8
#define CAN1_TX_QUEUE_SIZE 8
CanRxMsgTypeDef can1RxQueue[CAN1_RX_QUEUE_SIZE] = {0};
CanTxMsgTypeDef can1TxQueue[CAN1_TX_QUEUE_SIZE] = {0};

CAN_MsgStruct can1_msg =
{
	.hcan = &hcan1,
	.rxQueue = can1RxQueue,
	.txQueue = can1TxQueue,
	.rxQueueSize = CAN1_RX_QUEUE_SIZE,
	.txQueueSize = CAN1_TX_QUEUE_SIZE,
	.rxPtr = {0},
	.txPtr = {0}
};


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	HAL_StatusTypeDef hal_status;
	CanRxMsgTypeDef ptr;

	if(hcan == &hcan1)
	{
		ptr = can1_msg.rxQueue[can1_msg.rxPtr.index_IN];
		hal_status = HAL_CAN_GetRxMessage(can1_msg.hcan, CAN_RX_FIFO0, &ptr.header, ptr.data);
		if(hal_status == HAL_OK)
		{
			RingBuff_Ptr_Input(&can1_msg.rxPtr, can1_msg.rxQueueSize);
		}
	}
	else if(hcan == &hcan2)
	{
		ptr = can2_msg.rxQueue[can2_msg.rxPtr.index_IN];
		hal_status = HAL_CAN_GetRxMessage(can2_msg.hcan, CAN_RX_FIFO0, &ptr.header, ptr.data);
		if(hal_status == HAL_OK)
		{
			RingBuff_Ptr_Input(&can2_msg.rxPtr, can2_msg.rxQueueSize);
		}
	}
}

// make duplicate for mailbox 1 and 2
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan == &hcan1)
	{
		CAN_SendMessage(&can1_msg);
	}
	else if(hcan == &hcan2)
	{
		CAN_SendMessage(&can2_msg);
	}
}


// add to polling init
CAN_SetFilter(&can1_msg);



 */

