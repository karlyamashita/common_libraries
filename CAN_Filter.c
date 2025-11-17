/*
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Karl Yamashita
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */

#include "main.h"


/*
function: Set CAN filter to pass all ID's
input: hcan instance
output: none
*/
void CAN_SetFilter(CAN_MsgStruct *msg)
{
	static CAN_FilterTypeDef sFilterConfig;

	if(msg->hcan == &hcan)
	{
		sFilterConfig.FilterBank = 0;
	}

	sFilterConfig.FilterMode             = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale            = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh           = 0x0000;
	sFilterConfig.FilterIdLow            = 0x0000;
	sFilterConfig.FilterMaskIdHigh       = 0x0000;
	sFilterConfig.FilterMaskIdLow        = 0x0000;
	sFilterConfig.FilterFIFOAssignment   = CAN_FILTER_FIFO0;
	sFilterConfig.FilterActivation       = CAN_FILTER_ENABLE;

	if(HAL_CAN_ConfigFilter(msg->hcan, &sFilterConfig) != HAL_OK)
	{
		Error_Handler(); // Error_Handler should never be called, so we won't worry about handling these errors.
	}

	if(HAL_CAN_Start(msg->hcan) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_CAN_ActivateNotification(msg->hcan, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
	{
		Error_Handler();
	}
}





