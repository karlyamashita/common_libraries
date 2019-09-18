#include "LSP.h"
#ifdef USE_CAN_MESSAGE_DELAYED
#ifndef CAN_MESSAGE_DELAY_H
#define CAN_MESSAGE_DELAY_H

#include "CAN.h"


typedef struct CAN_MessageDelayedStructure {
	CanTxMsgTypeDef canMessage;
	uint32_t delay;
}CAN_MessageDelayedStructure;


/*
function: Places the message in CAN1 buffer with delay into a structure to be compared in 
				CheckCAN_DelayMessageBuffer1() function.
input: the CAN message and the delay before sending on CAN bus
output: none
note: This added message will start it's delay after all previous messages are sent.
*/
void AddDelayedCAN1_MessageToBuffer(CanTxMsgTypeDef *canMessage, uint32_t delay);
void AddDelayedCAN2_MessageToBuffer(CanTxMsgTypeDef *canMessage, uint32_t delay);

/*
function: Check if specific message delay is reached then place message in CAN1 Tx buffer
input: none
output: none
*/
void CheckCAN1_DelayMessageBuffer(void);
void CheckCAN2_DelayMessageBuffer(void);

#endif // CAN_MESSAGE_DELAY_H
#endif // USE_CAN_MESSAGE_DELAY
