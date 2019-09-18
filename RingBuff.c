#include "LSP.h"
#ifdef USE_RING_BUFF
#include "main.h"
#include "ringBuff.h"

/*
typedef struct		
{
    unsigned int 	iIndexIN;			
    unsigned int 	iIndexOUT;	
    unsigned int	iCnt_Handle;
    unsigned int	iCnt_OverFlow;	
	
} RING_BUFF_INFO;
 */

//==========================================================
// Layer : DRIVER
//==========================================================

void DRV_RingBuffPtr__Clean(RING_BUFF_INFO *ptr) {
	ptr->iIndexIN = 0;
	ptr->iIndexOUT = 0;

	ptr->iCnt_Handle = 0;
	ptr->iCnt_OverFlow = 0;
}

void DRV_RingBuffPtr__Input(RING_BUFF_INFO *ptr, unsigned int iBufferSize) {
	ptr->iIndexIN++;
	if (ptr->iIndexIN >= iBufferSize)
		ptr->iIndexIN = 0;

	ptr->iCnt_Handle++;
	if (ptr->iIndexIN == ptr->iIndexOUT) {
		ptr->iCnt_OverFlow++;
		if (ptr->iCnt_OverFlow > 50000)
			ptr->iCnt_OverFlow = 0;
		if (ptr->iIndexIN == 0) {
			ptr->iIndexOUT = iBufferSize - 1;
		} else {
			ptr->iIndexOUT = ptr->iIndexIN - 1;
		}
		ptr->iCnt_Handle = 1;
	}
}

void DRV_RingBuffPtr__Output(RING_BUFF_INFO *ptr, unsigned int iBufferSize) {
	if (ptr->iCnt_Handle) {
		ptr->iIndexOUT++;
		if (ptr->iIndexOUT >= iBufferSize)
			ptr->iIndexOUT = 0;
		ptr->iCnt_Handle--;
	}
}

void DRV_RingBuffPtr__Output_JumpToNew(RING_BUFF_INFO *ptr, unsigned int iBufferSize) {
	if (ptr->iCnt_Handle > 1) {
		while (ptr->iCnt_Handle > 1) {
			ptr->iIndexOUT++;
			if (ptr->iIndexOUT >= iBufferSize)
				ptr->iIndexOUT = 0;
			ptr->iCnt_Handle--;
		}
	} else {
		ptr->iIndexOUT++;
		if (ptr->iIndexOUT >= iBufferSize)
			ptr->iIndexOUT = 0;
		ptr->iCnt_Handle--;
	}
}
#endif // USE_RING_BUFF

