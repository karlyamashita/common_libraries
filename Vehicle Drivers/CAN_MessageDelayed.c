#include "LSP.h"
#ifdef USE_CAN_MESSAGE_DELAYED
#include "main.h"
#include "CAN_MessageDelayed.h"
#include "CAN.h"
#include "CAN_Buffer.h"
#include "RingBuff.h"
#include "mTimers.h"

uint8_t can1MessageDelayTimer = 0; // timer instance
uint8_t can2MessageDelayTimer = 0; // timer instance


#define CAN_MSG_DELAY_MAX_BUFFER 16

CAN_MessageDelayedStructure can1MessageDelayedStructure[CAN_MSG_DELAY_MAX_BUFFER];
CAN_MessageDelayedStructure can2MessageDelayedStructure[CAN_MSG_DELAY_MAX_BUFFER];

void AddDelayedCAN1_MessageToBuffer(CanTxMsgTypeDef *canMessage, uint32_t delay) {
	int i;
	if(can1MessageDelayTimer == 0 ) {
		can1MessageDelayTimer = CreateTimer();
	}
	
	can1MessageDelayedStructure[CAN1_DelayMessagePtr.iIndexIN].delay = delay;
	
	can1MessageDelayedStructure[CAN1_DelayMessagePtr.iIndexIN].canMessage.ExtId = canMessage->ExtId;
	can1MessageDelayedStructure[CAN1_DelayMessagePtr.iIndexIN].canMessage.StdId = canMessage->StdId;
	can1MessageDelayedStructure[CAN1_DelayMessagePtr.iIndexIN].canMessage.IDE = canMessage->IDE;
	can1MessageDelayedStructure[CAN1_DelayMessagePtr.iIndexIN].canMessage.RTR = canMessage->RTR;
	can1MessageDelayedStructure[CAN1_DelayMessagePtr.iIndexIN].canMessage.DLC = canMessage->DLC;
	for(i = 0; i < canMessage->DLC; i++) {
		can1MessageDelayedStructure[CAN1_DelayMessagePtr.iIndexIN].canMessage.Data[i] = canMessage->Data[i];
	}
	
	if(CAN1_DelayMessagePtr.iCnt_Handle == 0) {
		SetTimer(can1MessageDelayTimer, 0);// no message in buffer so clear timer on this first message
	}
	
	DRV_RingBuffPtr__Input(&CAN1_DelayMessagePtr, CAN_MSG_DELAY_MAX_BUFFER);
}

void AddDelayedCAN2_MessageToBuffer(CanTxMsgTypeDef *canMessage, uint32_t delay) {
	int i;
	if(can2MessageDelayTimer == 0 ) {
		can2MessageDelayTimer = CreateTimer();
	}
	
	can1MessageDelayedStructure[CAN2_DelayMessagePtr.iIndexIN].delay = delay;
	
	can2MessageDelayedStructure[CAN2_DelayMessagePtr.iIndexIN].canMessage.ExtId = canMessage->ExtId;
	can2MessageDelayedStructure[CAN2_DelayMessagePtr.iIndexIN].canMessage.StdId = canMessage->StdId;
	can2MessageDelayedStructure[CAN2_DelayMessagePtr.iIndexIN].canMessage.IDE = canMessage->IDE;
	can2MessageDelayedStructure[CAN2_DelayMessagePtr.iIndexIN].canMessage.RTR = canMessage->RTR;
	can2MessageDelayedStructure[CAN2_DelayMessagePtr.iIndexIN].canMessage.DLC = canMessage->DLC;
	for(i = 0; i < canMessage->DLC; i++) {
		can2MessageDelayedStructure[CAN2_DelayMessagePtr.iIndexIN].canMessage.Data[i] = canMessage->Data[i];
	}
	
	if(CAN2_DelayMessagePtr.iCnt_Handle == 0) {
		SetTimer(can2MessageDelayTimer, 0);// no message in buffer so clear timer on this first message
	}
	
	DRV_RingBuffPtr__Input(&CAN2_DelayMessagePtr, CAN_MSG_DELAY_MAX_BUFFER);
}

void CheckCAN1_DelayMessageBuffer(void) {
	if(CAN1_DelayMessagePtr.iCnt_Handle) {
		if(GetTimer(can1MessageDelayTimer) >= can1MessageDelayedStructure[CAN1_DelayMessagePtr.iIndexOUT].delay) {
			SetTimer(can1MessageDelayTimer, 0); // reset timer for next message
			AddTxBuffer1(&can1MessageDelayedStructure[CAN1_DelayMessagePtr.iIndexOUT].canMessage);
			DRV_RingBuffPtr__Output(&CAN1_DelayMessagePtr, CAN_MSG_DELAY_MAX_BUFFER);
		}
	}
}

void CheckCAN2_DelayMessageBuffer(void) {
	if(CAN2_DelayMessagePtr.iCnt_Handle) {
		if(GetTimer(can2MessageDelayTimer) >= can2MessageDelayedStructure[CAN2_DelayMessagePtr.iIndexOUT].delay) {
			SetTimer(can2MessageDelayTimer, 0); // reset timer for next message
			AddTxBuffer2(&can2MessageDelayedStructure[CAN2_DelayMessagePtr.iIndexOUT].canMessage);
			DRV_RingBuffPtr__Output(&CAN2_DelayMessagePtr, CAN_MSG_DELAY_MAX_BUFFER);
		}
	}
}

#endif // USE_CAN_MESSAGE_DELAYED
