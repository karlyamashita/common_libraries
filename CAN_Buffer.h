#include "LSP.h"

#ifdef USE_CAN_BUFFER
#ifndef CAN_BUFFER_H
#define CAN_BUFFER_H
#ifdef __STM32F1xx_HAL_CAN_H
#include "can.h"
#endif // __STM32F1xx_HAL_CAN_H

#include "RingBuff.h"

#define CAN_MAX_RX_BUFF 8
#define CAN_MAX_TX_BUFF 8

#define CAN_STD_ID 0x00
#define CAN_EXT_ID 0x04

#if defined STM32F4xx_HAL_CAN_H || defined STM32F042x6
// Receive
typedef struct CanRxMsgTypeDef{
	CAN_RxHeaderTypeDef CAN_RxHeaderTypeDef;
	uint8_t Data[8];
}CanRxMsgTypeDef;

// Transmit
typedef struct CanTxMsgTypeDef{
	CAN_TxHeaderTypeDef CAN_TxHeaderTypeDef;
	uint8_t Data[8];
}CanTxMsgTypeDef;
#endif // STM32F4xx_HAL_CAN_H

#ifdef __STM32F1xx_HAL_CAN_H
void InitTxMessagesCAN1(void);
#endif

int SendCanTxMessage1(void);
#if defined STM32F4xx_HAL_CAN_H || defined STM32F042x6
void AddCanTxBuffer1(CanTxMsgTypeDef *hcan1);
void AddCanRxBuffer1(CanRxMsgTypeDef *hcan1);
uint8_t Can1DataAvailable(CanRxMsgTypeDef *canMsg);
#else
void AddCanTxBuffer1(CanTxMsgTypeDef *hcan1);
void AddCanRxBuffer1(CAN_HandleTypeDef *hcan1);
#endif // STM32F4xx_HAL_CAN_H


#ifdef USE_CAN_BUFFER_2
void InitTxMessagesCAN2(void);
int SendTxMessage2(void);
void AddTxBuffer2(CanTxMsgTypeDef *hcan2);
void AddRxBuffer2(CAN_HandleTypeDef *hcan2);


#endif // CAN2

void MsgCopy(CanTxMsgTypeDef *TxMsg, CanRxMsgTypeDef *RxMsg);
	


#endif // CAN_BUFFER_H
#endif // USE_CAN_BUFFER
