// by Karl Yamashita September 2017

#include "main.h"


HAL_StatusTypeDef can_status; // make it global for debugger window
/*
 * Description: Send available Tx queue
 */
int CANFD_SendMessage(CANFD_MsgStruct *msg)
{
	CANFD_TxMsgTypeDef *ptr;

	if(msg->txPtr.cnt_Handle) // send available message
	{
		ptr = &msg->txQueue[msg->txPtr.index_OUT];
		//can_status = HAL_FDCAN_AddTxMessage(msg->hfdcan, &ptr->header, ptr->data, &CAN_Tx_Mailboxes);
		can_status = HAL_FDCAN_AddMessageToTxFifoQ(msg->hfdcan, &ptr->header, ptr->data);
		if (can_status == HAL_OK)
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
int CANFD_AddTxBuffer(CANFD_MsgStruct *msg, CANFD_TxMsgTypeDef *txData)
{
	CANFD_TxMsgTypeDef *ptr;
	int i;

	if(msg->txPtr.cnt_OverFlow) return 1; // queue is full

	ptr = &msg->txQueue[msg->txPtr.index_IN];

	ptr->header.DataLength = txData->header.DataLength;
	ptr->header.FDFormat = txData->header.FDFormat;
	ptr->header.Identifier = txData->header.Identifier;
	ptr->header.IdType = txData->header.IdType;
	ptr->header.TxFrameType = txData->header.TxFrameType;
	ptr->header.TxEventFifoControl = txData->header.TxEventFifoControl;
	ptr->header.BitRateSwitch = txData->header.BitRateSwitch;
	ptr->header.MessageMarker = txData->header.MessageMarker;
	ptr->header.ErrorStateIndicator = txData->header.ErrorStateIndicator;

	for(i = 0; i < ptr->header.DataLength; i++)
	{
		ptr->data[i] = txData->data[i];
	}

	RingBuff_Ptr_Input(&msg->txPtr, msg->txQueueSize); // increment input buffer ptr

	CANFD_SendMessage(msg); // try sending if not busy

	return 0;
}


/*
 * Description: msgToParse points to available message
 * Input:
 * Output: true if data available, false if no new data
 */
bool CANFD_DataAvailable(CANFD_MsgStruct *msg)
{
	if(msg->rxPtr.cnt_Handle)
	{
		msg->msgToParse = &msg->rxQueue[msg->rxPtr.index_OUT];
		RingBuff_Ptr_Output(&msg->rxPtr, msg->rxQueueSize); // increment output buffer ptr
		return true;
	}

	return false;
}

bool CANFD_GetBusActivity(CANFD_MsgStruct *msg)
{
	return msg->canBusActive;
}

void CANFD_ClearBusActivity(CANFD_MsgStruct *msg)
{
	msg->canBusActive = false;
}

/*
 * Description: Copy the Rx data to the Tx data.
 * 		note: The header is a different struct between Tx and Rx.
 * 				We have to copy each header over instead of memcpy.
 */
void CANFD_MsgCopy(CANFD_TxMsgTypeDef *TxMsg, CANFD_RxMsgTypeDef *RxMsg)
{
	int i = 0;

	TxMsg->header.Identifier = RxMsg->header.Identifier;
	TxMsg->header.IdType = RxMsg->header.IdType;
	TxMsg->header.FDFormat = RxMsg->header.FDFormat;
	TxMsg->header.TxFrameType = RxMsg->header.RxFrameType;
	TxMsg->header.DataLength = RxMsg->header.DataLength;
	TxMsg->header.BitRateSwitch = RxMsg->header.BitRateSwitch;
	for(i = 0; i < TxMsg->header.DataLength; i++)
	{
		TxMsg->data[i] = RxMsg->data[i];
	}
}

/*

#define CANFD1_RX_QUEUE_SIZE 8
#define CANFD1_TX_QUEUE_SIZE 8
CANFD_RxMsgTypeDef canfd1_RxQueue[CANFD1_RX_QUEUE_SIZE] = {0};
CANFD_TxMsgTypeDef canfd1_TxQueue[CANFD1_TX_QUEUE_SIZE] = {0};

CANFD_MsgStruct canfd1_msg =
{
	.hcan = &hcan1,
	.rxQueue = canfd1_RxQueue,
	.txQueue = canfd1_TxQueue,
	.rxQueueSize = CANFD1_RX_QUEUE_SIZE,
	.txQueueSize = CANFD1_TX_QUEUE_SIZE,
	.rxPtr = {0},
	.txPtr = {0}
};


// create a IRQ_Handler.c file and place these 2 callbacks in it
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	HAL_StatusTypeDef hal_status;
	CANFD_RxMsgTypeDef *ptr;

	if(hfdcan == canfd_msg.hfdcan)
	{
		ptr = &canfd_msg.rxQueue[canfd_msg.rxPtr.index_IN];
		hal_status = HAL_FDCAN_GetRxMessage(canfd_msg.hfdcan, FDCAN_RX_FIFO0, &ptr->header, ptr->data);
		if(hal_status == HAL_OK)
		{
			RingBuff_Ptr_Input(&canfd_msg.rxPtr, canfd_msg.rxQueueSize);
			TimerCallbackRepetitionStart(&timerCallback, LED_Blue_Toggle, 20, 2); // blip LED
		}
	}
}

void HAL_FDCAN_TxFifoEmptyCallback(FDCAN_HandleTypeDef *hfdcan)
{
	if(hfdcan == canfd_msg.hfdcan)
	{
		TimerCallbackRepetitionStart(&timerCallback, LED_Green_Toggle, 20, 2); // blip LED
		CANFD_SendMessage(&canfd_msg);
	}
}



 */

