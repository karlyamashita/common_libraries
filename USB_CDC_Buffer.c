/*
 * USB_CDC_Buffer.c
 *
 *  Created on: Jan 22, 2023
 *      Author: codek
 */

#include "main.h"
#include "USB_CDC_Buffer.h"
#include "usbd_cdc_if.h"

// init usb message buffer
USB_CDC_MsgStruct usbMsg =
{
	.rx.msgQueueSize = USB_CDC_QUEUE_SIZE,
	.tx.msgQueueSize = USB_CDC_QUEUE_SIZE
};


int USB_CDC_AddRxMsg(USB_CDC_MsgStruct *msg, uint8_t *data, uint32_t size)
{
	int status = 0;
	USB_CDC_Data *ptr;

	if(msg->rx.ptr.cnt_OverFlow > 0)
	{
		return 1; // overflow
	}

	ptr = &msg->rx.queue[msg->rx.ptr.index_IN];
	ptr->Status.size = size;
	memcpy(&ptr->Byte.data, data, size);

	RingBuff_Ptr_Input(&msg->rx.ptr, msg->rx.msgQueueSize);

	return status;
}

int USB_CDC_MsgRdy(USB_CDC_MsgStruct *msg)
{
	int status = 1;

	if(msg->rx.ptr.cnt_Handle)
	{
		msg->rx.msgToParse = &msg->rx.queue[msg->rx.ptr.index_OUT];
		RingBuff_Ptr_Output(&msg->rx.ptr, msg->rx.msgQueueSize);
	}
	else
	{
		status = 0; // no new message
	}

	return status;
}

int USB_CDC_AddTxMsg(USB_CDC_MsgStruct *msg, USB_CDC_Data *msgIn)
{
	USB_CDC_Data *ptr;

	if(msg->tx.ptr.cnt_OverFlow > 0)
	{
		return 1;
	}

	ptr = &msg->tx.queue[msg->tx.ptr.index_IN];
	ptr->Status.size = msgIn->Status.size;
	memcpy(&ptr->Byte.data, msgIn->Byte.data, msgIn->Status.size);

	RingBuff_Ptr_Input(&msg->tx.ptr, msg->tx.msgQueueSize);

	return 0;
}

int USB_CDC_SendMsg(USB_CDC_MsgStruct *msg)
{
	USB_CDC_Data *ptr;

	if(msg->tx.ptr.cnt_Handle)
	{
		ptr = &msg->tx.queue[msg->tx.ptr.index_OUT];
#if (DEVICE_HS == 1)
		if(CDC_Transmit_HS(ptr->Byte.data, ptr->Status.size) == 0)
#else
		if(CDC_Transmit_HS(ptr->Byte.data, ptr->Status.size) == 0)

#endif
		{
			RingBuff_Ptr_Output(&msg->tx.ptr, msg->tx.msgQueueSize);
		}
		else
		{
			return 1;
		}
	}

	return 0;
}

