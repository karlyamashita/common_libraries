/*
 * This library is used in the USB to CAN analyzer devices. It formats the data for the CAN-X software
 *
 *
 *
 */


#include "main.h"
#include "UsbAndCanConvert.h"

extern CanMsg_Struct CAN_Msg;
extern USB_CDC_MsgStruct usbMsg;


/* Description: parse usb data to send over CAN
 * Input canChannel: which CAN node to send message to
 * Input data: The USB data
 * Output none
 */
void USB_to_CAN_Send(CanMsg_Struct *msg, uint8_t *data)
{
	UsbCanStruct usbCanStruct = {0};
	CanTx *ptr;

	ptr = &msg->tx.queue[msg->tx.ptr.index_IN];
	memcpy(usbCanStruct.array.msgArray, data, USB_MESSAGE_SIZE);

	if(ptr->header.IDE == CAN_ID_STD) {
		ptr->header.StdId = usbCanStruct.msgBytes.ArbId;
	} else {
		ptr->header.ExtId = usbCanStruct.msgBytes.ArbId;
	}
	ptr->header.RTR = usbCanStruct.msgBytes.RTR;// RTR
	ptr->header.DLC = usbCanStruct.msgBytes.DLC;//

	memcpy(&ptr->data, &usbCanStruct.dataBytes.array, 8);

	RingBuff_Ptr_Input(&msg->tx.ptr, msg->tx.msgQueueSize);
}

// just the opposite, copy CAN to USB data
void CAN_to_USB_Send(CanMsg_Struct *msg, uint8_t node) {
	UsbCanStruct usbCanStruct = {0};
	USB_CDC_Data usbMsgTemp = {0};
	CanRx *ptr = &msg->rx.queue[msg->rx.ptr.index_OUT];

	usbCanStruct.msgBytes.Command = COMMAND_MESSAGE;
	usbCanStruct.msgBytes.IDE = ptr->header.IDE & 0x0F;

	if(usbCanStruct.msgBytes.IDE == CAN_EXT_ID) {
		usbCanStruct.msgBytes.ArbId = ptr->header.ExtId;
	} else {
		usbCanStruct.msgBytes.ArbId = ptr->header.StdId;
	}

	usbCanStruct.msgBytes.RTR = ptr->header.RTR & 0x0F;
	usbCanStruct.msgBytes.Node = node;
	usbCanStruct.msgBytes.DLC = ptr->header.DLC & 0x0F;

	memcpy(&usbCanStruct.dataBytes.array, ptr->data, 8);

	memcpy(&usbMsgTemp.Byte.data, &usbCanStruct.array.msgArray, USB_MESSAGE_SIZE);
	usbMsgTemp.Status.size = USB_MESSAGE_SIZE;
	USB_CDC_AddTxMsg(&usbMsg, &usbMsgTemp);
}

