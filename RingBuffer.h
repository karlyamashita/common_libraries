#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "main.h"

#define RING_BUFF_OVERFLOW_SIZE 100

typedef struct {
	uint32_t index_IN; // pointer to where the data will be save to
	uint32_t index_OUT; // pointer to next available data in buffer if cnt_handle is not zero
	uint32_t cnt_Handle; // if not zero then message available
	uint32_t cnt_OverFlow; // has overflow if not zero
	uint8_t SkipOverFlow;
}RING_BUFF_STRUCT;

void RingBuff_Ptr_Reset(RING_BUFF_STRUCT *ptr);
void RingBuff_Ptr_Input(RING_BUFF_STRUCT *ptr, uint32_t bufferSize);
void RingBuff_Ptr_Output(RING_BUFF_STRUCT *ptr, uint32_t bufferSize);



#endif // RING_BUFFER_H
