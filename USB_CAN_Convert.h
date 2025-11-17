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

#ifndef USB_CAN_CONVERT_H
#define USB_CAN_CONVERT_H


// Node defines
#define CAN1_NODE 0
#define CAN2_NODE 1
#define SWCAN1_NODE 2
#define LSFTCAN1_NODE 3
#define LIN1_NODE 4
#define ETH1_NODE 5
#define SWCAN2_NODE 6
#define LSFTCAN2_NODE 7

#define CAN_STD_ID 0x00
#define CAN_EXT_ID 0x04

#define CAN_DATA_SIZE 17

// the structure sent by the PC.
typedef	union UsbCanStruct {
	struct {
		uint8_t data[CAN_DATA_SIZE];
	}array;
	struct {
		uint32_t IDE:8; // ide, STD or EXT
		uint32_t RTR:8;// rtr
		uint32_t na:8; // node
		uint32_t :8; // reserved

		uint32_t ArbID; // ArbID
		uint32_t DLC:8; // dlc
		uint8_t array[8]; // 8 data bytes
	}dataBytes;
	struct  {
		uint32_t IDE:8; // byte1
		uint32_t RTR:1; // RTR, ERR, byte2
		uint32_t Err:1; // not implemented
		uint32_t :6;
		uint32_t Node:4; // byte3, see Node defines
		uint32_t :4;
		uint32_t :8; // reserved

		// ArbID byte4:7
		uint32_t ArbId; // holds 11 and 29 bit ID. This has to align within 32bit address
		//
		uint32_t DLC:8; // byte8
		uint8_t Byte1; // byte9
		uint8_t Byte2;
		uint8_t Byte3;
		uint8_t Byte4;
		uint8_t Byte5;
		uint8_t Byte6;
		uint8_t Byte7;
		uint8_t Byte8; // byte16
	}msgBytes;
}UsbCanStruct;

void USB_to_CAN_Send(CAN_MsgStruct *msg, uint8_t *data);
void CAN_to_USB_Send(CAN_MsgStruct *msg, uint8_t node);

#endif // USB_CAN_CONVERT_H

