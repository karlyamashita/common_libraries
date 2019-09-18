// by Karl Yamashita September 2017
#include "LSP.h"

#ifdef USE_CAN_BUFFER
#include "main.h"
#include "CAN_Buffer.h"
#include "ringBuff.h"
#include "can.h"
// add your header files


CanTxMsgTypeDef TxMessageBuffer1[CAN_MAX_TX_BUFF];
CanTxMsgTypeDef TxMessageBuffer2[CAN_MAX_TX_BUFF];
CanRxMsgTypeDef RxMessageBuffer1[CAN_MAX_TX_BUFF];
CanRxMsgTypeDef RxMessageBuffer2[CAN_MAX_TX_BUFF];

RING_BUFF_INFO TxMessagePtr1;
RING_BUFF_INFO TxMessagePtr2;
RING_BUFF_INFO RxMessagePtr1;
RING_BUFF_INFO RxMessagePtr2;

#ifdef __STM32F1xx_HAL_CAN_H
#ifdef USE_CAN_BUFFER_1
static CanTxMsgTypeDef	TxMessage1;
void InitTxMessagesCAN1(void) {
	hcan1.pTxMsg = &TxMessage1;
}

#endif // CAN1
#ifdef USE_CAN_BUFFER_2
static CanTxMsgTypeDef	TxMessage2;
// use can buffer 2
void InitTxMessagesCAN2(void) {
	hcan2.pTxMsg = &TxMessage2;	
}
#endif // CAN2

#ifdef USE_CAN_BUFFER_1
// using can buffer 1
uint32_t transmitCount1 = 0;
HAL_StatusTypeDef CAN_Status1;
int SendTxMessage1(void) {
	unsigned char dataLength, i = 0;

	if(TxMessagePtr1.iCnt_Handle) { // send available message
		transmitCount1 += 1;
		hcan1.pTxMsg->ExtId = TxMessageBuffer1[TxMessagePtr1.iIndexOUT].ExtId;
		hcan1.pTxMsg->StdId = TxMessageBuffer1[TxMessagePtr1.iIndexOUT].StdId;
		hcan1.pTxMsg->RTR = TxMessageBuffer1[TxMessagePtr1.iIndexOUT].RTR;
		hcan1.pTxMsg->IDE = TxMessageBuffer1[TxMessagePtr1.iIndexOUT].IDE;
		hcan1.pTxMsg->RTR = TxMessageBuffer1[TxMessagePtr1.iIndexOUT].RTR;
		dataLength = TxMessageBuffer1[TxMessagePtr1.iIndexOUT].DLC;
		hcan1.pTxMsg->DLC = dataLength;
		
		if(dataLength != 0) {
			for(i = 0; i < dataLength; i++) {
				hcan1.pTxMsg->Data[i] = TxMessageBuffer1[TxMessagePtr1.iIndexOUT].Data[i];
			}
		}
		CAN_Status1 = HAL_CAN_Transmit(&hcan1, 100);
		/*
		if (CAN_Status1 != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}	
		*/

		//HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0);
			
		DRV_RingBuffPtr__Output(&TxMessagePtr1, CAN_MAX_TX_BUFF); // increment output buffer ptr
	}
	return TxMessagePtr1.iCnt_Handle; // if no more message to handle then 0 will be returned
}

// add to Tx buffer
void AddTxBuffer1(CanTxMsgTypeDef *canMessage) {
	unsigned char i;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].ExtId = canMessage->ExtId;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].StdId = canMessage->StdId;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].RTR = canMessage->RTR;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].IDE = canMessage->IDE;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].DLC = canMessage->DLC;
	for(i = 0; i < TxMessageBuffer1[TxMessagePtr1.iIndexIN].DLC; i++) {
		TxMessageBuffer1[TxMessagePtr1.iIndexIN].Data[i] = canMessage->Data[i];
	}
	DRV_RingBuffPtr__Input(&TxMessagePtr1, CAN_MAX_TX_BUFF); // increment input buffer ptr
}

// add to Rx buffer
void AddRxBuffer1(CAN_HandleTypeDef *hcan1) {
	unsigned char i;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].ExtId = hcan1->pRxMsg->ExtId;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].StdId = hcan1->pRxMsg->StdId;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].RTR = hcan1->pRxMsg->RTR;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].IDE = hcan1->pRxMsg->IDE;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].DLC = hcan1->pRxMsg->DLC;
	for(i = 0; i < RxMessageBuffer1[RxMessagePtr1.iIndexIN].DLC; i++) {
		RxMessageBuffer1[RxMessagePtr1.iIndexIN].Data[i] = hcan1->pRxMsg->Data[i];
	}
	DRV_RingBuffPtr__Input(&RxMessagePtr1, CAN_MAX_TX_BUFF); // increment input buffer ptr
}

#endif // CAN1

#ifdef USE_CAN_BUFFER_2
uint32_t transmitCount2 = 0;
HAL_StatusTypeDef CAN_Status2;
int SendTxMessage2(void) {
	unsigned char dataLength, i = 0;

	if(TxMessagePtr2.iCnt_Handle) { // send available message
		transmitCount2 += 1;
		hcan2.pTxMsg->ExtId = TxMessageBuffer2[TxMessagePtr2.iIndexOUT].ExtId;
		hcan2.pTxMsg->StdId = TxMessageBuffer2[TxMessagePtr2.iIndexOUT].StdId;
		hcan2.pTxMsg->RTR = TxMessageBuffer2[TxMessagePtr2.iIndexOUT].RTR;
		hcan2.pTxMsg->IDE = TxMessageBuffer2[TxMessagePtr2.iIndexOUT].IDE;
		hcan2.pTxMsg->RTR = TxMessageBuffer2[TxMessagePtr2.iIndexOUT].RTR;
		dataLength = TxMessageBuffer2[TxMessagePtr2.iIndexOUT].DLC;
		hcan2.pTxMsg->DLC = dataLength;

		if(dataLength != 0) {
			for(i = 0; i < dataLength; i++) {
				hcan2.pTxMsg->Data[i] = TxMessageBuffer2[TxMessagePtr2.iIndexOUT].Data[i];
			}
		}
		CAN_Status2 = HAL_CAN_Transmit(&hcan2, 100);
		/*
		if (CAN_Status2 != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
		*/

		//HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0);

		DRV_RingBuffPtr__Output(&TxMessagePtr2, CAN_MAX_TX_BUFF); // increment output buffer ptr
	}

	return TxMessagePtr2.iCnt_Handle; // if no more message to handle then 0 will be returned
}

void AddTxBuffer2(CanTxMsgTypeDef *canMessage) {
	unsigned char i;
	TxMessageBuffer2[TxMessagePtr2.iIndexIN].ExtId = canMessage->ExtId;
	TxMessageBuffer2[TxMessagePtr2.iIndexIN].StdId = canMessage->StdId;
	TxMessageBuffer2[TxMessagePtr2.iIndexIN].IDE = canMessage->IDE;
	TxMessageBuffer2[TxMessagePtr2.iIndexIN].DLC = canMessage->DLC;
	for(i = 0; i < TxMessageBuffer2[TxMessagePtr2.iIndexIN].DLC; i++) {
		TxMessageBuffer2[TxMessagePtr2.iIndexIN].Data[i] = canMessage->Data[i];
	}
	DRV_RingBuffPtr__Input(&TxMessagePtr2, CAN_MAX_TX_BUFF); // increment input buffer ptr
}

void AddRxBuffer2(CAN_HandleTypeDef *hcan2) {
	unsigned char i;
	RxMessageBuffer2[RxMessagePtr2.iIndexIN].ExtId = hcan2->pRxMsg->ExtId; 
	RxMessageBuffer2[RxMessagePtr2.iIndexIN].StdId = hcan2->pRxMsg->StdId; 
	RxMessageBuffer2[RxMessagePtr2.iIndexIN].RTR = hcan2->pRxMsg->RTR; 
	RxMessageBuffer2[RxMessagePtr2.iIndexIN].IDE = hcan2->pRxMsg->IDE; 
	RxMessageBuffer2[RxMessagePtr2.iIndexIN].DLC = hcan2->pRxMsg->DLC; 
	for(i = 0; i < RxMessageBuffer2[RxMessagePtr2.iIndexIN].DLC; i++) {
		RxMessageBuffer2[RxMessagePtr2.iIndexIN].Data[i] = hcan2->pRxMsg->Data[i];
	}
	DRV_RingBuffPtr__Input(&RxMessagePtr2, CAN_MAX_TX_BUFF); // increment input buffer ptr
}
#endif // CAN2

void MsgCopy(CanTxMsgTypeDef *TxMsg, CanRxMsgTypeDef *RxMsg){
	int i = 0;
	TxMsg->ExtId = RxMsg->ExtId;
	TxMsg->StdId = RxMsg->StdId;
	TxMsg->IDE = RxMsg->IDE;
	TxMsg->RTR = RxMsg->RTR;
	TxMsg->DLC = RxMsg->DLC;
	for(i = 0; i < TxMsg->DLC; i++) {
		TxMsg->Data[i] = RxMsg->Data[i];
	}
}
#endif // STM32F1xx_HAL_CAN_H

#ifdef STM32F4xx_HAL_CAN_H

HAL_StatusTypeDef CAN_Status1;
int SendTxMessage1(void) {
	uint32_t CAN_Tx_Mailboxes;

	if(TxMessagePtr1.iCnt_Handle) { // send available message

		CAN_Status1 = HAL_CAN_AddTxMessage(&hcan1, &TxMessageBuffer1->CAN_TxHeaderTypeDef, TxMessageBuffer1->Data, &CAN_Tx_Mailboxes);
		/*
		if (CAN_Status1 != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}	
		*/
			
		DRV_RingBuffPtr__Output(&TxMessagePtr1, CAN_MAX_TX_BUFF); // increment output buffer ptr
	}
	return TxMessagePtr1.iCnt_Handle; // if no more message to handle then 0 will be returned
}

// add to Tx buffer
void AddTxBuffer1(CanTxMsgTypeDef *canMessage) {
	unsigned char i;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.ExtId = canMessage->CAN_TxHeaderTypeDef.ExtId;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.StdId = canMessage->CAN_TxHeaderTypeDef.StdId;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.RTR = canMessage->CAN_TxHeaderTypeDef.RTR;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.IDE = canMessage->CAN_TxHeaderTypeDef.IDE;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.DLC = canMessage->CAN_TxHeaderTypeDef.DLC;
	for(i = 0; i < TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.DLC; i++) {
		TxMessageBuffer1[TxMessagePtr1.iIndexIN].Data[i] = canMessage->Data[i];
	}
	DRV_RingBuffPtr__Input(&TxMessagePtr1, CAN_MAX_TX_BUFF); // increment input buffer ptr
}

// add to Rx buffer
void AddRxBuffer1(CanRxMsgTypeDef *hcan1) {
	unsigned char i;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.ExtId = hcan1->CAN_RxHeaderTypeDef.ExtId;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.StdId = hcan1->CAN_RxHeaderTypeDef.StdId;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.RTR = hcan1->CAN_RxHeaderTypeDef.RTR;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.IDE = hcan1->CAN_RxHeaderTypeDef.IDE;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.DLC = hcan1->CAN_RxHeaderTypeDef.DLC;
	for(i = 0; i < RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.DLC; i++) {
		RxMessageBuffer1[RxMessagePtr1.iIndexIN].Data[i] = hcan1->Data[i];
	}
	DRV_RingBuffPtr__Input(&RxMessagePtr1, CAN_MAX_TX_BUFF); // increment input buffer ptr
}

void MsgCopy(CanTxMsgTypeDef *TxMsg, CanRxMsgTypeDef *RxMsg){
	int i = 0;
	TxMsg->CAN_TxHeaderTypeDef.ExtId = RxMsg->CAN_RxHeaderTypeDef.ExtId;
	TxMsg->CAN_TxHeaderTypeDef.StdId = RxMsg->CAN_RxHeaderTypeDef.StdId;
	TxMsg->CAN_TxHeaderTypeDef.IDE = RxMsg->CAN_RxHeaderTypeDef.IDE;
	TxMsg->CAN_TxHeaderTypeDef.RTR = RxMsg->CAN_RxHeaderTypeDef.RTR;
	TxMsg->CAN_TxHeaderTypeDef.DLC = RxMsg->CAN_RxHeaderTypeDef.DLC;
	for(i = 0; i < TxMsg->CAN_TxHeaderTypeDef.DLC; i++) {
		TxMsg->Data[i] = RxMsg->Data[i];
	}
}

#endif // STM32F4xx_HAL_CAN_H

#endif // USE_CAN_BUFFER

