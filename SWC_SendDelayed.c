#include "LSP.h"
#ifdef USE_SWC_SEND_DELAYED
#include "main.h"
#include "SWC_SendDelayed.h"
#include "RingBuff.h"
#include "mTimers.h"
#include "SWC.h"

/*
	This section is used to send delayed SWC command 
*/
#define MAX_SWC_STRUCTURE 4
SWC_Structure swc_structure[MAX_SWC_STRUCTURE]; // buffer to hold command and delay value
RING_BUFF_INFO swc_StructurePtr;

uint8_t swcSendDelayTimer = 0; // timer instance

/*
function: Add SWC command with delay value to buffer
input: The command and delay in milliseconds
output: none
*/
void AddSWC_CommandToBufferCAN1(uint8_t command, uint32_t delay) {
	if(swcSendDelayTimer == 0) {
		swcSendDelayTimer = CreateTimer();
	}
	swc_structure[swc_StructurePtr.iIndexIN].command = command;
	swc_structure[swc_StructurePtr.iIndexIN].delay = delay; // the delay before sending the SWC message
	if(swc_StructurePtr.iCnt_Handle == 0) {
		// buffer empty so reset timer for first command
		SetTimer(swcSendDelayTimer, 0); // clear timer
	}
	DRV_RingBuffPtr__Input(&swc_StructurePtr, MAX_SWC_STRUCTURE); // increment input buffer ptr
}


/*
function: Check if time is reached then call SendSWC_Command_CAN1() function to place SWC command in Tx buffer
input: none
output: none
*/
// this needs to be called in polling routine
void CheckDelayedSWC_CAN1(void) {
	if(swc_StructurePtr.iCnt_Handle){ // continue if there is a pending swc command to be sent
		if(GetTimer(swcSendDelayTimer) >= swc_structure[swc_StructurePtr.iIndexOUT].delay) {
			SetTimer(swcSendDelayTimer, 0); // clear timer for next swc command
			SendSWC_Command_CAN1(swc_structure[swc_StructurePtr.iIndexOUT].command);
			DRV_RingBuffPtr__Output(&swc_StructurePtr, MAX_SWC_STRUCTURE); // increment output buffer ptr
		}
	}
}
#endif // USE_SWC_SEND_DELAYED

