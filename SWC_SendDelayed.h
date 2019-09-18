#ifndef SWC_SEND_DELAYED_H
#define SWC_SEND_DELAYED_H


typedef struct SWC_Structure {
	uint8_t command;
	uint32_t delay;
	bool buttonState; // pressed or release state
}SWC_Structure;


/*
function: Places the message in CAN1 buffer with delay into a structure to be compared in 
				CheckDelayedSWC_CAN1() function.
input: the CAN message and the delay before sending on CAN bus
output: none
note: This added message will start it's delay after all previous messages are sent.
*/
void AddSWC_CommandToBufferCAN1(uint8_t command, uint32_t delay);
void CheckDelayedSWC_CAN1(void);

#endif// SWC_SEND_DELAYED_H
