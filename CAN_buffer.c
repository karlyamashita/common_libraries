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

	CAN_SendMessage(msg);

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

