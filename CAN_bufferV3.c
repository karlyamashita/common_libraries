// by Karl Yamashita September 2017

#include "main.h"


/*
 * Description: Check for new CAN message. Called from polling routine.
 * Input: The CAN module to send on. The pointer to the CAN message buffer.
 *
 */
void CAN_SendTxMessage(CAN_HandleTypeDef *hcan, CanTxMsg_Struct *canMsg)
{
	uint32_t CAN_Tx_Mailboxes; // not used but needed as an argument

	if(canMsg->ptr.iCnt_Handle) { // send available message
		if(HAL_CAN_AddTxMessage(hcan, &canMsg->QUEUE[canMsg->ptr.iIndexOUT].header, canMsg->QUEUE[canMsg->ptr.iIndexOUT].data, &CAN_Tx_Mailboxes) == HAL_OK)
		{
			DRV_RingBuffPtr__Output(&canMsg->ptr, CAN_TX_QUEUE_SIZE); // increment output buffer ptr
		}
	}
}

/*
 * Description: Add message to Tx buffer
 *
 */
void CAN_AddTxBuffer(CanTxMsg_Struct *canMsg_Out, CanTxMsgTemp_Struct *canMsg)
{
	unsigned char i;
	canMsg_Out->QUEUE[canMsg_Out->ptr.iIndexIN].header.ExtId = canMsg->header.ExtId;
	canMsg_Out->QUEUE[canMsg_Out->ptr.iIndexIN].header.StdId = canMsg->header.StdId;
	canMsg_Out->QUEUE[canMsg_Out->ptr.iIndexIN].header.RTR = canMsg->header.RTR;
	canMsg_Out->QUEUE[canMsg_Out->ptr.iIndexIN].header.IDE = canMsg->header.IDE;
	canMsg_Out->QUEUE[canMsg_Out->ptr.iIndexIN].header.DLC = canMsg->header.DLC;
	for(i = 0; i < canMsg_Out->QUEUE[canMsg_Out->ptr.iIndexIN].header.DLC; i++) {
		canMsg_Out->QUEUE[canMsg_Out->ptr.iIndexIN].data[i] = canMsg->data[i];
	}
	DRV_RingBuffPtr__Input(&canMsg_Out->ptr, CAN_TX_QUEUE_SIZE); // increment input buffer ptr
}

/*
 * Description: Check the CAN module for a message and copy to rxMsg.
 * 				This is typically called from HAL_CAN_RxFifo0MsgPendingCallback()
 *
 */
void CAN_GetMessage(CAN_HandleTypeDef *hcan, CanRxMsg_Struct *rxMsg)
{
	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxMsg->QUEUE[rxMsg->ptr.iIndexIN].header, rxMsg->QUEUE[rxMsg->ptr.iIndexIN].data) == HAL_OK)
	{
		DRV_RingBuffPtr__Input(&rxMsg->ptr, CAN_RX_QUEUE_SIZE);
	}
}

/*
 * Description: Copies rxMsg to canMsg if message available. Needs to be called from polling routine
 * Input: The rxMsg queue.
 * Output: The canMsg that has the copy of the rxMsg queue
 * Return: 1 if message available, else 0 for no message
 */
int CAN_MsgAvailable(CanRxMsg_Struct *rxMsg, CanTxMsgTemp_Struct *canMsg)
{
	int i;

	if(rxMsg->ptr.iCnt_Handle)
	{
		canMsg->header.IDE = rxMsg->QUEUE[rxMsg->ptr.iIndexOUT].header.IDE;
		canMsg->header.StdId = rxMsg->QUEUE[rxMsg->ptr.iIndexOUT].header.StdId;
		canMsg->header.ExtId = rxMsg->QUEUE[rxMsg->ptr.iIndexOUT].header.ExtId;
		canMsg->header.DLC = rxMsg->QUEUE[rxMsg->ptr.iIndexOUT].header.DLC;
		canMsg->header.RTR = rxMsg->QUEUE[rxMsg->ptr.iIndexOUT].header.RTR;
		canMsg->header.TransmitGlobalTime = rxMsg->QUEUE[rxMsg->ptr.iIndexOUT].header.Timestamp;

		for(i = 0; i < CAN_RX_BUFF_SIZE; i ++)
		{
			canMsg->data[i] = rxMsg->QUEUE[rxMsg->ptr.iIndexOUT].data[i];
		}

		DRV_RingBuffPtr__Output(&rxMsg->ptr, CAN_RX_QUEUE_SIZE);
		return 1; // rxMsg copied to canMsg
	}

	return 0; // no msg
}

