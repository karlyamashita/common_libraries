#include "LSP.h"
#ifdef USE_RING_BUFF

#ifndef RINGBUFF_H
#define RINGBUFF_H

typedef struct {
	unsigned int iIndexIN;
	unsigned int iIndexOUT;
	unsigned int iCnt_Handle;
	unsigned int iCnt_OverFlow;
}RING_BUFF_INFO;


//================================================
// FUNCTION
//================================================

void DRV_RingBuffPtr__Clean(RING_BUFF_INFO *ptr);

void DRV_RingBuffPtr__Input(RING_BUFF_INFO *ptr, unsigned int iBufferSize);

void DRV_RingBuffPtr__Output(RING_BUFF_INFO *ptr, unsigned int iBufferSize);
void DRV_RingBuffPtr__Output_JumpToNew(RING_BUFF_INFO *ptr, unsigned int iBufferSize);

#endif // RINGBUFF_H
#endif // USE_RING_BUFF
