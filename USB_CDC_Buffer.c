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
#include "usbd_cdc_if.h"



// poll this to check if usb data is available to send
int USB_SendMessage(USB_MsgStruct *msg)
{
	uint8_t USB_Status = USBD_OK;

	if(msg->txPtr.cnt_Handle) // send available message
	{
		USB_Status = CDC_Transmit_FS(msg->txQueue[msg->txPtr.index_OUT].Byte.data, ID_AND_SIZE_LENGTH + msg->txQueue[msg->txPtr.index_OUT].Status.size);
		if (USB_Status == USBD_OK) // make sure data was sent before incrementing pointer
		{
			RingBuff_Ptr_Output(&msg->txPtr, msg->txQueueSize); // increment output buffer ptr
		}
	}
	return msg->txPtr.cnt_Handle; // if no more message to handle then 0 will be returned
}

// adds data to USB Tx buffer
void USB_AddTxBuffer(USB_MsgStruct *msg, USB_Data_t *data)
{
	int i = 0;

	memset(&msg->txQueue[msg->txPtr.index_IN], 0, sizeof(msg->txQueue[msg->txPtr.index_IN]));
	for(i = 0; i < (ID_AND_SIZE_LENGTH + data->Status.size); i++)
	{
		msg->txQueue[msg->txPtr.index_IN].Byte.data[i] = data->Byte.data[i];
	}
	msg->txQueue[msg->txPtr.index_IN].Status.size = data->Status.size;
	RingBuff_Ptr_Input(&msg->txPtr, msg->txQueueSize);
}

// add data to USB Rx buffer
void USB_AddRxBuffer(USB_MsgStruct *msg, uint8_t *data, uint32_t size)
{
	int i = 0;

	memset(&msg->rxQueue[msg->rxPtr.index_IN], 0, sizeof(msg->rxQueue[msg->rxPtr.index_IN]));
	for(i = 0; i < size; i++)
	{
		msg->rxQueue[msg->rxPtr.index_IN].Byte.data[i] = data[i];
	}

	msg->rxQueue[msg->rxPtr.index_IN].Status.size = size;

	RingBuff_Ptr_Input(&msg->rxPtr, msg->rxQueueSize);
}

/*
 * Check for USB Rx message
 * Input data: pointer to array to save data from Rx buffer
 * Output return 1 if data available, 0 if no data
 */
uint8_t USB_DataAvailable(USB_MsgStruct *msg)
{
	if(msg->rxPtr.cnt_Handle)
	{
		msg->msgToParse = &msg->rxQueue[msg->rxPtr.index_OUT];
		RingBuff_Ptr_Output(&msg->rxPtr, msg->rxQueueSize);
		return 1;
	}

	return 0;
}


/*
 * Example initializing usb instance


#define USB_QUEUE_SIZE 16
USB_Data_t usbRxQueue[USB_QUEUE_SIZE] = {0};
USB_Data_t usbTxQueue[USB_QUEUE_SIZE] = {0};

USB_MsgStruct usb_msg =
{
	.rx.queue = usbRxQueue,
	.rx.queueSize = USB_QUEUE_SIZE,
	.tx.queue = usbTxQueue,
	.tx.queueSize = USB_QUEUE_SIZE
};


 */
