

#ifndef RINGBUFF_H
#define RINGBUFF_H

#include "main.h"
#include <stdint.h>

typedef struct {
	uint16_t iIndexIN; // change all these if need more than 16bits.
	uint16_t iIndexOUT;
	uint16_t iCnt_Handle;
	uint16_t iCnt_OverFlow;
}RING_BUFF_INFO;


//================================================
// FUNCTION
//================================================

void DRV_RingBuffPtr__Clean(RING_BUFF_INFO *ptr);

void DRV_RingBuffPtr__Input(RING_BUFF_INFO *ptr, unsigned int iBufferSize);

void DRV_RingBuffPtr__Output(RING_BUFF_INFO *ptr, unsigned int iBufferSize);
void DRV_RingBuffPtr__Output_JumpToNew(RING_BUFF_INFO *ptr, unsigned int iBufferSize);


#endif // RINGBUFF_H
