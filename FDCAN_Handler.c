/*
 * FDCAN_Handler.c
 *
 *  Created on: Oct 7, 2023
 *      Author: karl.yamashita
 */

#include "main.h"


FDCAN_Buffer fdcan1;


/*
 * Description: Initialize FDCAN buffer.
 * 				Assign FDCAN instance to buffer instance. Set queueSize.
 */
void FDCAN_InitInstance(FDCAN_Buffer *fdcanBuffer, FDCAN_HandleTypeDef *fdcan)
{
	FDCAN_FilterTypeDef sFilterConfig;

	fdcanBuffer->rx.fdcan = fdcan;
	fdcanBuffer->rx.queueSize = FDCAN_RX_BUF_SIZE;

	fdcanBuffer->tx.fdcan = fdcan;
	fdcanBuffer->tx.queueSize = FDCAN_TX_BUF_SIZE;

	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig.FilterConfig = FDCAN_FILTER_DISABLE;
	sFilterConfig.FilterID1 = 0x101;
	sFilterConfig.FilterID2 = 0x7FF;
	if (HAL_FDCAN_ConfigFilter(fdcan, &sFilterConfig) != HAL_OK)
	{

	}


	/* Configure global filter on both FDCAN instances:
	     Filter all remote frames with STD and EXT ID
	     Reject non matching frames with STD ID and EXT ID */
	if (HAL_FDCAN_ConfigGlobalFilter(fdcan, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
	{

	}

	/* Activate Rx FIFO 0 new message notification on both FDCAN instances */
	if (HAL_FDCAN_ActivateNotification(fdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
	{

	}
}

/*
 * Description: Add data to buffer and increment buffer pointer
 *
 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != 0)
	{
		if(HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, fdcan1.rx.queue[fdcan1.rx.ptr.index_IN].pRxHeader, fdcan1.rx.queue[fdcan1.rx.ptr.index_IN].data) == HAL_OK)
		{
			RingBuff_Ptr_Input(&fdcan1.rx.ptr, fdcan1.rx.queueSize);

		}
	}
}

/*
 * Description: Check for new message and point msgToParse to buffer index to be parsed
 * return: True if message ready, else return false
 *
 */
int FDCAN_MsgReady(FDCAN_Buffer *fdcanBuffer)
{
	if(fdcanBuffer->rx.ptr.cnt_Handle)
	{
		fdcanBuffer->rx.msgToParse = &fdcanBuffer->rx.queue[fdcanBuffer->rx.ptr.index_OUT];
		RingBuff_Ptr_Output(&fdcanBuffer->rx.ptr, fdcanBuffer->rx.queueSize);
		return true;
	}

	return false;
}

/*
 * Description: copy data to tx buffer and increment buffer pointer.
 *
 */
void FDCAN_AddDataToTxBuffer(FDCAN_Buffer *fdcanBuffer, FDCAN_Tx *data)
{
	memcpy(&fdcanBuffer->tx.queue[fdcanBuffer->tx.ptr.index_IN].pTxHeader, &data->pTxHeader, sizeof(data->pTxHeader));
	memcpy(&fdcanBuffer->tx.queue[fdcanBuffer->tx.ptr.index_IN].data, &data->data, sizeof(data->data));

	RingBuff_Ptr_Input(&fdcanBuffer->tx.ptr, fdcanBuffer->tx.queueSize);
}

/*
 * Description: Call from polling routine
 * Input: The FDCAN_buffer
 *
 */
void FDCAN_Transmit(FDCAN_Buffer *fdcanBuffer)
{
	if(fdcanBuffer->tx.ptr.cnt_Handle)
	{
		if(HAL_FDCAN_AddMessageToTxFifoQ(fdcanBuffer->tx.fdcan, &fdcanBuffer->tx.queue[fdcanBuffer->tx.ptr.index_OUT].pTxHeader, fdcanBuffer->tx.queue[fdcanBuffer->tx.ptr.index_OUT].data) == HAL_OK)
		{
			RingBuff_Ptr_Output(&fdcanBuffer->tx.ptr, fdcanBuffer->tx.queueSize);
		}
	}
}

