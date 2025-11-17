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

/*
 * This library is used in the USB to CAN analyzer devices. It formats the data for the CAN-X software
 *
 *
 *
 */

#include "main.h"


/* Description: parse usb data to send over CAN
 * Input can msg: which CAN message to add data to
 * Input data: The USB data
 * Output none
 */
void USB_to_CAN_Send(CAN_MsgStruct *msg, uint8_t *data)
{
	UsbCanStruct usbCanStruct = {0};
	CanTxMsgTypeDef *ptr;

	ptr = &msg->txQueue[msg->txPtr.index_IN];

	// copy usb data to CAN structure
	memcpy(&usbCanStruct.array.data, data, CAN_DATA_SIZE);

	ptr->header.IDE = usbCanStruct.msgBytes.IDE;

	if(usbCanStruct.msgBytes.IDE == CAN_ID_STD)
	{
		ptr->header.StdId = usbCanStruct.msgBytes.ArbId;
	}
	else
	{
		ptr->header.ExtId = usbCanStruct.msgBytes.ArbId;
	}
	ptr->header.RTR = usbCanStruct.msgBytes.RTR;// RTR
	ptr->header.DLC = usbCanStruct.msgBytes.DLC;//

	memcpy(&ptr->data, &usbCanStruct.dataBytes.array, 8);
	RingBuff_Ptr_Input(&msg->txPtr, msg->txQueueSize);

	CAN_SendMessage(&can_msg); // try sending
}

// just the opposite, copy CAN to USB data
void CAN_to_USB_Send(CAN_MsgStruct *msg, uint8_t node)
{
	UsbCanStruct usbCanStruct = {0};
	USB_Data_t usbMsgTemp = {0};
	CanRxMsgTypeDef *ptr = msg->msgToParse;

	// convert PC message to CAN structure
	usbCanStruct.msgBytes.IDE = ptr->header.IDE & 0x0F;

	if(usbCanStruct.msgBytes.IDE == CAN_STD_ID)
	{
		usbCanStruct.msgBytes.ArbId = ptr->header.StdId;
	}
	else
	{
		usbCanStruct.msgBytes.ArbId = ptr->header.ExtId;
	}

	usbCanStruct.msgBytes.RTR = ptr->header.RTR & 0x0F;
	usbCanStruct.msgBytes.Node = node;
	usbCanStruct.msgBytes.DLC = ptr->header.DLC & 0x0F;

	memcpy(&usbCanStruct.dataBytes.array, ptr->data, 8);

	// copy CAN message structure to USB message structure
	usbMsgTemp.Status.id = CMD_MESSAGE;
	usbMsgTemp.Status.size = CAN_DATA_SIZE;
	memcpy(&usbMsgTemp.Status.data, &usbCanStruct.array.data, CAN_DATA_SIZE);

	USB_AddTxBuffer(&usb_msg, &usbMsgTemp);
}

