/*
 * USB_CDC_Buffer.h
 *
 *  Created on: Jan 22, 2023
 *      Author: codek
 */

#ifndef INC_USB_CDC_BUFFER_H_
#define INC_USB_CDC_BUFFER_H_


#define USB_CDC_QUEUE_SIZE 16
#define ID_AND_SIZE_LENGTH 16
#define USB_CDC_DATA_SIZE (256 - ID_AND_SIZE_LENGTH) //256 - id - size


typedef union
{
	struct
	{
		uint8_t data[USB_CDC_DATA_SIZE + ID_AND_SIZE_LENGTH];
	}Byte;
	struct
	{
		uint32_t id;
		uint32_t size;
		uint8_t data[USB_CDC_DATA_SIZE];
	}Status;
}USB_CDC_Data;

typedef struct
{
	struct
	{
		USB_CDC_Data queue[USB_CDC_QUEUE_SIZE];
		USB_CDC_Data *msgToParse;
		uint32_t msgQueueSize;
		RING_BUFF_STRUCT ptr;
	}rx;
	struct
	{
		USB_CDC_Data queue[USB_CDC_QUEUE_SIZE];
		uint32_t msgQueueSize;
		RING_BUFF_STRUCT ptr;
	}tx;
}USB_CDC_MsgStruct;

int USB_CDC_AddRxMsg(USB_CDC_MsgStruct *msg, uint8_t *data, uint32_t size);
int USB_CDC_MsgRdy(USB_CDC_MsgStruct *msg);

int USB_CDC_AddTxMsg(USB_CDC_MsgStruct *msg, USB_CDC_Data *msgIn);
int USB_CDC_SendMsg(USB_CDC_MsgStruct *msg);


#endif /* INC_USB_CDC_BUFFER_H_ */
