// by Karl Yamashita September 2017

#include <CAN_BufferV2.h>
#include "main.h"
#include "ringBuff.h"


CanTxMsgTypeDef TxMessageBuffer1 = {0};
CanRxMsgTypeDef RxMessageBuffer1 = {0};


#ifdef USE_CAN_BUFFER_2
extern CAN_HandleTypeDef hcan2;
CanTxMsgTypeDef TxMessageBuffer2 = {0};
CanRxMsgTypeDef RxMessageBuffer2 = {0};

#endif


HAL_StatusTypeDef CAN_Status1; // make it global for debugger window
int SendCanTxMessage1(CAN_HandleTypeDef *hcan, CanTxMsgTypeDef *CAN_Msg) {
	uint32_t CAN_Tx_Mailboxes; // indicates which tx buffer was used

	if(CAN_Msg->ptr.iCnt_Handle) { // send available message
		CAN_Status1 = HAL_CAN_AddTxMessage(hcan, &CAN_Msg->Msg[CAN_Msg->ptr.iIndexOUT].CAN_TxHeaderTypeDef, CAN_Msg->Msg[CAN_Msg->ptr.iIndexOUT].data, &CAN_Tx_Mailboxes);
		if (CAN_Status1 == HAL_OK)
		{
			DRV_RingBuffPtr__Output(&CAN_Msg->ptr, CAN_MAX_TX_BUFF); // increment output buffer ptr
		}
	}
	return CAN_Msg->ptr.iCnt_Handle; // if no more message to handle then 0 will be returned
}

// add to Tx buffer
void AddCanTxBuffer1(CanTxMsgTypeDef *CAN_Msg, CanTxMsgTypeDef *canMsg) {
	unsigned char i;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.ExtId = canMsg->CAN_TxHeaderTypeDef.ExtId;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.StdId = canMsg->CAN_TxHeaderTypeDef.StdId;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.RTR = canMsg->CAN_TxHeaderTypeDef.RTR;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.IDE = canMsg->CAN_TxHeaderTypeDef.IDE;
	TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.DLC = canMsg->CAN_TxHeaderTypeDef.DLC;
	for(i = 0; i < TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.DLC; i++) {
		TxMessageBuffer1[TxMessagePtr1.iIndexIN].Data[i] = canMsg->Data[i];
	}
	DRV_RingBuffPtr__Input(&TxMessagePtr1, CAN_MAX_TX_BUFF); // increment input buffer ptr
}

// add to Rx buffer
void AddCanRxBuffer1(CanRxMsgTypeDef *CAN_Msg, CanRxMsgTypeDef *canMsg) {
	unsigned char i;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.ExtId = canMsg->CAN_RxHeaderTypeDef.ExtId;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.StdId = canMsg->CAN_RxHeaderTypeDef.StdId;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.RTR = canMsg->CAN_RxHeaderTypeDef.RTR;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.IDE = canMsg->CAN_RxHeaderTypeDef.IDE;
	RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.DLC = canMsg->CAN_RxHeaderTypeDef.DLC;
	for(i = 0; i < RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.DLC; i++) {
		RxMessageBuffer1[RxMessagePtr1.iIndexIN].Data[i] = canMsg->Data[i];
	}
	DRV_RingBuffPtr__Input(&RxMessagePtr1, CAN_MAX_TX_BUFF); // increment input buffer ptr
}

/*
 * Copy buffer to canMsg array
 * Input canMsg: pointer to array
 * Output: 1 if data available, 0 if no new data
 */
uint8_t Can1DataAvailable(CanRxMsgTypeDef *canMsg)
{
	uint8_t i, canMsgAvailable = 0;
	if(RxMessagePtr1.iCnt_Handle) {
		canMsg->CAN_RxHeaderTypeDef.ExtId = RxMessageBuffer1[RxMessagePtr1.iIndexOUT].CAN_RxHeaderTypeDef.ExtId;
		canMsg->CAN_RxHeaderTypeDef.StdId = RxMessageBuffer1[RxMessagePtr1.iIndexOUT].CAN_RxHeaderTypeDef.StdId;
		canMsg->CAN_RxHeaderTypeDef.RTR = RxMessageBuffer1[RxMessagePtr1.iIndexOUT].CAN_RxHeaderTypeDef.RTR;
		canMsg->CAN_RxHeaderTypeDef.IDE = RxMessageBuffer1[RxMessagePtr1.iIndexOUT].CAN_RxHeaderTypeDef.IDE;
		canMsg->CAN_RxHeaderTypeDef.DLC = RxMessageBuffer1[RxMessagePtr1.iIndexOUT].CAN_RxHeaderTypeDef.DLC;
		for(i = 0; i < RxMessageBuffer1[RxMessagePtr1.iIndexOUT].CAN_RxHeaderTypeDef.DLC; i++) {
			canMsg->Data[i] = RxMessageBuffer1[RxMessagePtr1.iIndexOUT].Data[i];
		}
		DRV_RingBuffPtr__Output(&RxMessagePtr1, CAN_MAX_RX_BUFF); // increment output buffer ptr
		canMsgAvailable = 1;
	}
	return canMsgAvailable;
}

#ifdef USE_CAN_BUFFER_2
/*
 * CAN2 routines
 *
 */
HAL_StatusTypeDef CAN_Status2; // make it global for debugger window
int SendCanTxMessage2(CAN_HandleTypeDef *hcan) {
	uint32_t CAN_Tx_Mailboxes; // indicates which tx buffer was used
	if(TxMessagePtr2.iCnt_Handle) { // send available message
		CAN_Status2 = HAL_CAN_AddTxMessage(hcan, &TxMessageBuffer2[TxMessagePtr2.iIndexOUT].CAN_TxHeaderTypeDef, TxMessageBuffer2[TxMessagePtr2.iIndexOUT].Data, &CAN_Tx_Mailboxes);
		if (CAN_Status2 == HAL_OK)
		{
			DRV_RingBuffPtr__Output(&TxMessagePtr2, CAN_MAX_TX_BUFF); // increment output buffer ptr
		}
	}
	return TxMessagePtr2.iCnt_Handle; // if no more message to handle then 0 will be returned
}

// add to Tx buffer
void AddCanTxBuffer2(CanTxMsgTypeDef *canMsg) {
	unsigned char i;
	TxMessageBuffer2[TxMessagePtr2.iIndexIN].CAN_TxHeaderTypeDef.ExtId = canMsg->CAN_TxHeaderTypeDef.ExtId;
	TxMessageBuffer2[TxMessagePtr2.iIndexIN].CAN_TxHeaderTypeDef.StdId = canMsg->CAN_TxHeaderTypeDef.StdId;
	TxMessageBuffer2[TxMessagePtr2.iIndexIN].CAN_TxHeaderTypeDef.RTR = canMsg->CAN_TxHeaderTypeDef.RTR;
	TxMessageBuffer2[TxMessagePtr2.iIndexIN].CAN_TxHeaderTypeDef.IDE = canMsg->CAN_TxHeaderTypeDef.IDE;
	TxMessageBuffer2[TxMessagePtr2.iIndexIN].CAN_TxHeaderTypeDef.DLC = canMsg->CAN_TxHeaderTypeDef.DLC;
	for(i = 0; i < TxMessageBuffer2[TxMessagePtr2.iIndexIN].CAN_TxHeaderTypeDef.DLC; i++) {
		TxMessageBuffer2[TxMessagePtr2.iIndexIN].Data[i] = canMsg->Data[i];
	}
	DRV_RingBuffPtr__Input(&TxMessagePtr2, CAN_MAX_TX_BUFF); // increment input buffer ptr
}

// add to Rx buffer
void AddCanRxBuffer2(CanRxMsgTypeDef *canMsg) {
	unsigned char i;
	RxMessageBuffer2[RxMessagePtr2.iIndexIN].CAN_RxHeaderTypeDef.ExtId = canMsg->CAN_RxHeaderTypeDef.ExtId;
	RxMessageBuffer2[RxMessagePtr2.iIndexIN].CAN_RxHeaderTypeDef.StdId = canMsg->CAN_RxHeaderTypeDef.StdId;
	RxMessageBuffer2[RxMessagePtr2.iIndexIN].CAN_RxHeaderTypeDef.RTR = canMsg->CAN_RxHeaderTypeDef.RTR;
	RxMessageBuffer2[RxMessagePtr2.iIndexIN].CAN_RxHeaderTypeDef.IDE = canMsg->CAN_RxHeaderTypeDef.IDE;
	RxMessageBuffer2[RxMessagePtr2.iIndexIN].CAN_RxHeaderTypeDef.DLC = canMsg->CAN_RxHeaderTypeDef.DLC;
	for(i = 0; i < RxMessageBuffer2[RxMessagePtr2.iIndexIN].CAN_RxHeaderTypeDef.DLC; i++) {
		RxMessageBuffer2[RxMessagePtr2.iIndexIN].Data[i] = canMsg->Data[i];
	}
	DRV_RingBuffPtr__Input(&RxMessagePtr2, CAN_MAX_TX_BUFF); // increment input buffer ptr
}

/*
 * Copy buffer to canMsg array
 * Input canMsg: pointer to array
 * Output: 1 if data available, 0 if no new data
 */
uint8_t Can2DataAvailable(CanRxMsgTypeDef *canMsg)
{
	uint8_t i, canMsgAvailable = 0;
	if(RxMessagePtr2.iCnt_Handle) {
		canMsg->CAN_RxHeaderTypeDef.ExtId = RxMessageBuffer2[RxMessagePtr2.iIndexOUT].CAN_RxHeaderTypeDef.ExtId;
		canMsg->CAN_RxHeaderTypeDef.StdId = RxMessageBuffer2[RxMessagePtr2.iIndexOUT].CAN_RxHeaderTypeDef.StdId;
		canMsg->CAN_RxHeaderTypeDef.RTR = RxMessageBuffer2[RxMessagePtr2.iIndexOUT].CAN_RxHeaderTypeDef.RTR;
		canMsg->CAN_RxHeaderTypeDef.IDE = RxMessageBuffer2[RxMessagePtr2.iIndexOUT].CAN_RxHeaderTypeDef.IDE;
		canMsg->CAN_RxHeaderTypeDef.DLC = RxMessageBuffer2[RxMessagePtr2.iIndexOUT].CAN_RxHeaderTypeDef.DLC;
		for(i = 0; i < RxMessageBuffer2[RxMessagePtr2.iIndexOUT].CAN_RxHeaderTypeDef.DLC; i++) {
			canMsg->Data[i] = RxMessageBuffer2[RxMessagePtr2.iIndexOUT].Data[i];
		}
		DRV_RingBuffPtr__Output(&RxMessagePtr2, CAN_MAX_RX_BUFF); // increment output buffer ptr
		canMsgAvailable = 1;
	}
	return canMsgAvailable;
}
#endif

// todo - add CAN2 buffer
void MsgCopy(CanTxMsgTypeDef *TxMsg, CanRxMsgTypeDef *RxMsg){
	/*
	int i = 0;

	TxMsg->CAN_TxHeaderTypeDef.ExtId = RxMsg->CAN_RxHeaderTypeDef.ExtId;
	TxMsg->CAN_TxHeaderTypeDef.StdId = RxMsg->CAN_RxHeaderTypeDef.StdId;
	TxMsg->CAN_TxHeaderTypeDef.IDE = RxMsg->CAN_RxHeaderTypeDef.IDE;
	TxMsg->CAN_TxHeaderTypeDef.RTR = RxMsg->CAN_RxHeaderTypeDef.RTR;
	TxMsg->CAN_TxHeaderTypeDef.DLC = RxMsg->CAN_RxHeaderTypeDef.DLC;
	for(i = 0; i < TxMsg->CAN_TxHeaderTypeDef.DLC; i++) {
		TxMsg->Data[i] = RxMsg->Data[i];
	}
	*/
}

