#include "LSP.h"
#ifdef USE_CAN_FILTER
#include "main.h"
#include "CAN_Filter.h"


#ifdef __STM32F1xx_HAL_CAN_H
#include "can.h"

/*
Be sure to set CAN Rx I/O for GPIO_MODE_EVT_FALLING else won't wake up from sleep. This may have changed with new code changes from ST.
STM32F4xx doesn't seem to need this change.

STM32F1xx needs HAL_CAN_Receive_IT() to enable Rx interrupts.
*/

#endif

// this file needs to include CAN_Buffer.h to save Rx messages to ring buffer
#include "CAN_Buffer.h"

static CanRxMsgTypeDef	RxMessage1;

#if defined STM32F042x6 // STM32F042x6 only has one CAN controller defined as hcan not hcan1
extern CAN_HandleTypeDef hcan;
#else
extern CAN_HandleTypeDef hcan1;
#endif


#ifdef USE_CAN_BUFFER_2
static CanRxMsgTypeDef	RxMessage2;
extern CAN_HandleTypeDef hcan2;
#endif // USE_CAN_BUFFER_2

/*
function: Set CAN filter to pass all ID's
input: none
output: none
*/
#ifdef __STM32F1xx_HAL_CAN_H

void SetCanFilter(void) {
	static CAN_FilterConfTypeDef sFilterConfig;
	hcan1.pRxMsg = &RxMessage1;
	#ifdef USE_CAN_BUFFER_2
	hcan2.pRxMsg = &RxMessage2;
	#endif // USE_CAN_BUFFER_2
	
	sFilterConfig.FilterNumber           = 0;
	sFilterConfig.FilterMode             = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale            = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh           = 0x0000;
	sFilterConfig.FilterIdLow            = 0x0000;
	sFilterConfig.FilterMaskIdHigh       = 0x0000;
	sFilterConfig.FilterMaskIdLow        = 0x0000;
	sFilterConfig.FilterFIFOAssignment   = 0;
	sFilterConfig.FilterActivation       = ENABLE;

	HAL_CAN_ConfigFilter(&hcan1,&sFilterConfig);
	if (HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	// init same filter for hcan2
#ifdef USE_CAN_BUFFER_2
	HAL_CAN_ConfigFilter(&hcan2,&sFilterConfig);
	if (HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
#endif // CAN2
}

/*
function: HAL CAN Rx callback
input: CanHandle
output: none
note: Use CallbackTimer to clear sleep timer for "GotoSleepCallbackFunc()"
*/
void HAL CAN Rx callback(CAN_HandleTypeDef *CanHandle)
{
//	ClearTimerCallbackTimer(GotoSleepCallbackFunc); // make a function in polling routine
	HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0); // enable CAN interrupt. Every time we receive an interrupt we need to set it again.
	if(CanHandle->Instance == CAN1) {
		AddCanRxBuffer1(CanHandle);
	}

#ifdef USE_CAN_BUFFER_2
	else
	{
		HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0);
		if(CanHandle->Instance == CAN2) {
			AddRxBuffer2(CanHandle);
		}
	}
#endif
}

#endif


#if defined STM32F4xx_HAL_CAN_H || defined STM32F042x6

/*
 * Note: STM32F4xx needs HAL_CAN_ActivateNotification() to activate interrupts.
 */

/*
function: Set CAN filter to pass all ID's
input: none
output: none
*/
void SetCanFilter(void) {
	static CAN_FilterTypeDef sFilterConfig;

	//sFilterConfig.FilterNumber           = 0;
	sFilterConfig.FilterMode             = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale            = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh           = 0x0000;
	sFilterConfig.FilterIdLow            = 0x0000;
	sFilterConfig.FilterMaskIdHigh       = 0x0000;
	sFilterConfig.FilterMaskIdLow        = 0x0000;
	sFilterConfig.FilterFIFOAssignment   = 0;
	sFilterConfig.FilterActivation       = ENABLE;
#if defined STM32F042x6
	HAL_CAN_ConfigFilter(&hcan,&sFilterConfig);
	HAL_CAN_Start(&hcan);
	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) // enables CAN notification. This is not the same as enabling CAN reception.
	{
		/* Notification Error */
		Error_Handler();
	}
#else
	HAL_CAN_ConfigFilter(&hcan1,&sFilterConfig);
	HAL_CAN_Start(&hcan1);
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) // enables CAN notification. This is not the same as enabling CAN reception.
	{
		/* Notification Error */
		Error_Handler();
	}
#endif
	// init same filter for hcan2
#ifdef USE_CAN_BUFFER_2
	HAL_CAN_ConfigFilter(&hcan2,&sFilterConfig);
	HAL_CAN_Start(&hcan2);
	if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		/* Notification Error */
		Error_Handler();
	}
#endif // CAN2
}


/*
function: Callback when message is available. Devices with 1 CAN controller is defined as CAN instead of CAN1.
input: CanHandle
output: none
note: Use CallbackTimer to clear sleep timer for "GotoSleepCallbackFunc()"
*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
#if defined STM32F042x6
	//ClearTimerCallbackTimer(GotoSleepCallbackFunc); // make a function in polling routine
	if(hcan->Instance == CAN) {
		HAL_CAN_GetRxMessage(hcan, (uint32_t) CAN_RX_FIFO0, &RxMessage1.CAN_RxHeaderTypeDef, RxMessage1.Data); // enable CAN reception again
#else
	if(hcan->Instance == CAN1) {
		HAL_CAN_GetRxMessage(&hcan1, (uint32_t) CAN_RX_FIFO0, &RxMessage1.CAN_RxHeaderTypeDef, RxMessage1.Data); // enable CAN reception again
#endif
		AddCanRxBuffer1(&RxMessage1);
	}
	
#ifdef USE_CAN_BUFFER_2
	else if(hcan->Instance == CAN2) {
		HAL_CAN_GetRxMessage(&hcan2, (uint32_t) CAN_RX_FIFO0, &RxMessage2.CAN_RxHeaderTypeDef, RxMessage2.Data); // enable CAN reception again
		AddRxBuffer2(&RxMessage2);
	}
#endif
}

// We should never need this callback as the CAN buffer should take care of messages received.
// void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)

#endif // STM32F4xx_HAL_CAN_H

#endif // USE_CAN_FILTER
