#include "LSP.h"

#ifdef USE_CAN_BUFFER
#ifndef CAN_BUFFER_H
#define CAN_BUFFER_H
#include "can.h"
#include "RingBuff.h"

#define CAN_MAX_RX_BUFF 32
#define CAN_MAX_TX_BUFF 32

#ifdef STM32F4xx_HAL_CAN_H
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
#endif

#ifdef CAN1
void InitTxMessagesCAN1(void);
int SendTxMessage1(void);

#ifdef STM32F4xx_HAL_CAN_H
void AddTxBuffer1(CanTxMsgTypeDef *canMessage);
void AddRxBuffer1(CanRxMsgTypeDef *hcan1);
#else
void AddTxBuffer1(CanTxMsgTypeDef *canMessage);
void AddRxBuffer1(CAN_HandleTypeDef *hcan1);
#endif // STM32F4xx_HAL_CAN_H
#endif // CAN1

#ifdef CAN2
void InitTxMessagesCAN2(void);
int SendTxMessage2(void);
void AddTxBuffer2(CanTxMsgTypeDef *canMessage);
void AddRxBuffer2(CAN_HandleTypeDef *hcan2);
#endif // CAN2

void MsgCopy(CanTxMsgTypeDef *TxMsg, CanRxMsgTypeDef *RxMsg);
	
#endif // CAN_BUFFER_H
#endif // USE_CAN_BUFFER
