/*
 * CAN_Handler.c
 *
 *  Created on: Jan 7, 2023
 *      Author: codek
 */


#include "main.h"
#include "CAN_Handler.h"


extern CAN_HandleTypeDef hcan;
extern CanMsg_Struct CAN_Msg;


/*
Description: Callback when message is pending. Get message by passing buffer address. Then increment buffer pointer.
input: CAN instance
output: none
*/

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *_hcan)
{
	CanRx *ptr;

	if(_hcan == &hcan)
	{
		ptr = &CAN_Msg.rx.queue[CAN_Msg.rx.ptr.index_IN];
		CAN_Msg.canBusActive = 1; // indicate msg received
		HAL_CAN_GetRxMessage(_hcan, (uint32_t) CAN_RX_FIFO0, &ptr->header, ptr->data);
		RingBuff_Ptr_Input(&CAN_Msg.rx.ptr, CAN_Msg.rx.msgQueueSize);
	}
}


// We should never need this callback as the CAN buffer should take care of messages received.
// void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)

