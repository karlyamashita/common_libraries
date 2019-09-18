#include "LSP.h"
#ifdef USE_CAN_FILTER
#include "main.h"
#include "CAN_Filter.h"
#include "BSP.h"

#include "can.h"

#include "CAN_Buffer.h"
#include "TimerCallback.h"
#include "PollingRoutines.h"

static CanRxMsgTypeDef	RxMessage1;
#ifdef USE_CAN_BUFFER_2
static CanRxMsgTypeDef	RxMessage2;
#endif // USE_CAN_BUFFER_2

/*
Be sure to set CAN Rx I/O for GPIO_MODE_EVT_FALLING else won't wake up from sleep.
*/

/*
function: Set CAN filter to pass all ID's
input: none
output: none
*/
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
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *CanHandle)
{
	ClearTimerCallbackTimer(GotoSleepCallbackFunc); // make a function in polling routine

	if(CanHandle->Instance == CAN1) {
		AddRxBuffer1(CanHandle);
		ClearCAN_Interrupts(&hcan1);
	}
	
#ifdef USE_CAN_BUFFER_2
	else if(CanHandle->Instance == CAN2) {
		AddRxBuffer2(CanHandle);
		ClearCAN_Interrupts(&hcan2);
	}
#endif
}

void ClearCAN_Interrupts(CAN_HandleTypeDef *CanHandle) {
 	// "HAL_CAN_Receive_IT(&hcan1/2)" has issues when in gateway mode. Use this function after saving data to buffer.
 
 	if(CanHandle->State == HAL_CAN_STATE_BUSY_TX)
 		CanHandle->State = HAL_CAN_STATE_BUSY_TX_RX0;
 	else {
 		CanHandle->State = HAL_CAN_STATE_BUSY_RX0;
 	 
 		/* Set CAN error code to none */
 		CanHandle->ErrorCode = HAL_CAN_ERROR_NONE;
 	 
 		/* Enable Error warning Interrupt */
 		__HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_EWG);
 	 
 		/* Enable Error passive Interrupt */
 		__HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_EPV);
 	 
 		/* Enable Bus-off Interrupt */
 		__HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_BOF);
 	 
 		/* Enable Last error code Interrupt */
 		__HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_LEC);
 	 
 		/* Enable Error Interrupt */
 		__HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_ERR);
 	}
 	// Enable FIFO 0 message pending Interrupt
 		__HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_FMP0);
 
}

#endif // USE_CAN_FILTER
