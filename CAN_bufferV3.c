// by Karl Yamashita September 2017

#include "main.h"

/*
 * Description: Increment pointer for rx buffer
 *
 */
void CAN_IncRxBuffer(CanMsg_Struct *msg)
{
	RingBuff_Ptr_Input(&msg->rx.ptr, msg->rx.msgQueueSize);
}


/*
 * Description: Copies rxMsg to canMsg if message available. Needs to be called from polling routine
 * Input: The rxMsg queue.
 * Output: The canMsg that has the copy of the rxMsg queue
 * Return: 1 if message available, else 0 for no message
 */
int CAN_MsgAvailable(CanMsg_Struct *msg)
{
	if(msg->rx.ptr.cnt_Handle)
	{
		msg->rx.msgToParse = &msg->rx.queue[msg->rx.ptr.index_OUT];

		RingBuff_Ptr_Output(&msg->rx.ptr, msg->rx.msgQueueSize);
		return 1; // msgToParse points to msg
	}

	return 0; // no msg
}


/*
 * Description: Add message to Tx buffer
 *
 */
void CAN_AddTxBuffer(CanMsg_Struct *msg, CanTx *canMsg)
{
	memcpy(&msg->tx.queue[msg->tx.ptr.index_IN].header, &canMsg->header, sizeof(canMsg->header));
	memcpy(&msg->tx.queue[msg->tx.ptr.index_IN].data, &canMsg->data, sizeof(canMsg->data));
	RingBuff_Ptr_Input(&msg->tx.ptr, msg->tx.msgQueueSize); // increment input buffer ptr
}

/*
 * Description: Check for new Tx CAN message and add to HAL_CAN_AddTxMessage. Called from polling routine.
 * Input: The CAN data structure to send.
 *
 */
void CAN_SendTxMessage(CanMsg_Struct *msg)
{
	uint32_t CAN_Tx_Mailboxes; // not used but needed as an argument
	CanTx *ptr;

	if(msg->tx.ptr.cnt_Handle) // send available message
	{
		ptr = &msg->tx.queue[msg->tx.ptr.index_OUT];

		if(HAL_CAN_AddTxMessage(msg->hcan, &ptr->header, ptr->data, &CAN_Tx_Mailboxes) == HAL_OK)
		{
			RingBuff_Ptr_Output(&msg->tx.ptr, msg->tx.msgQueueSize); // increment output buffer ptr
		}
	}
}



