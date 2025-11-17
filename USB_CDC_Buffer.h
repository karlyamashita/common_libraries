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

#ifndef USB_CDC_BUFFER_H
#define USB_CDC_BUFFER_H




#define ID_AND_SIZE_LENGTH 4 // (uint16_t id) (uint16_t size)
#define USB_DATA_SIZE (256 - ID_AND_SIZE_LENGTH) //256 - id - size

typedef union
{
	struct
	{
		uint8_t data[USB_DATA_SIZE + ID_AND_SIZE_LENGTH];
	}Byte;
	struct
	{
		uint32_t id:8;
		uint32_t :16; // reserved
		uint32_t size:8;
		uint8_t data[USB_DATA_SIZE];
	}Status;
}USB_Data_t;

typedef struct
{
	// rx
	USB_Data_t *rxQueue;
	USB_Data_t *msgToParse;
	uint32_t rxQueueSize;
	RING_BUFF_STRUCT rxPtr;
	// tx
	USB_Data_t *txQueue;
	uint32_t txQueueSize;
	RING_BUFF_STRUCT txPtr;
}USB_MsgStruct;



int USB_SendMessage(USB_MsgStruct *msg);
void USB_AddTxBuffer(USB_MsgStruct *msg, USB_Data_t *data);
void USB_AddRxBuffer(USB_MsgStruct *msg, uint8_t *data, uint32_t size);
uint8_t USB_DataAvailable(USB_MsgStruct *msg);

#endif // USB_CDC_BUFFER_H
