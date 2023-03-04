/*
 * CAN_Handler.c
 *
 *  Created on: Dec 23, 2022
 *      Author: codek
 *
 *
 *      This is for CAN2 only. If user is using CAN1 or/and CAN3 then he/she needs to add more code.
 *
 */


#include "main.h"
#include "CAN_Handler.h"


extern CAN_HandleTypeDef hcan2;


CanTxMsg_Struct can2_txMsg = {0};
CanRxMsg_Struct can2_rxMsg = {0};



void CAN_FilterInit(CAN_HandleTypeDef *hcan)
{
	CAN_FilterTypeDef  sFilterConfig;

	sFilterConfig.FilterBank = 14;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK)
	{

	}
}

/*
 * Description: Start CAN module and Activate notification
 *
 */
int CAN_Start(CAN_HandleTypeDef *hcan)
{
	int status = 0;

	if (HAL_CAN_Start(hcan) != HAL_OK)
	{
		status = 1;
	}

	if(HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		status = 1;
	}

	return status;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	// If there are more than 1 CAN module enabled
	// then we want to know which one caused the interrupt
	if(hcan->Instance == hcan2.Instance)
	{
		CAN_GetMessage(hcan, &can2_rxMsg);
	}
}


void CAN_SendMsg(CAN_HandleTypeDef *hcan, CanTxMsg_Struct *canMsg)
{
	uint32_t TxMailbox;

	if(canMsg->ptr.iCnt_Handle)
	{
		if (HAL_CAN_AddTxMessage(hcan, &canMsg->QUEUE[can2_txMsg.ptr.iIndexIN].header, canMsg->QUEUE[canMsg->ptr.iIndexIN].data, &TxMailbox) == HAL_OK)
		{
			DRV_RingBuffPtr__Output(&canMsg->ptr, CAN_TX_QUEUE_SIZE);
		}
	}
}





