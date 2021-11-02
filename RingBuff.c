/*
 * RingBuff.c
 *
 *  Created on: Sep 18, 2019
 *      Author: Karl
 *
 *
*/

#include "main.h"
#include "ringBuff.h"

/*
typedef struct {
    uint16_t iIndexIN; // change all these if need more than 16bits.
    uint16_t iIndexOUT;
    uint16_t iCnt_Handle;
    uint16_t iCnt_OverFlow;
}RING_BUFF_INFO;
*/

/*
 * To setup an Rx buffer:
 *         Create variable pointer, example; RING_BUFF_INFO uartRxPointer;
 *         Create an array buffer, in this case a read buffer: uint8_t uartRxBuffer[BUFFER_SIZE];
 *
 * To see if Rx data available:
 *                           while(uart0Pointer.iCnt_Handle)
 *                           {
 *                              // then do stuff
 *                              some_routine = uartRxBuffer[uartRxPointer.iIndexOUT];
 *
 *                              // increment output buffer
 *                              DRV_RingBuffPtr__Output(&uartRxPointer, BUFFER_SIZE);
 *                           }
 *
 * When receiving data:
 *                      // routine that receives uart data
 *                      {
 *                          uartRxBuffer[uartRxPointer.iIndexIN] = UART_DATA_BYTE;
 *                          // increment input buffer
 *                          DRV_RingBuffPtr__Input(&uartRxPointer, BUFFER_SIZE);
 *                      }
 *
 * The same setup above can be done for a Tx buffer
 *
 */




/*
****  Example of using an array buffer of a CAN message structure *****
*/
/*
  typedef struct
  {
      uint32_t StdId;    //!< Specifies the standard identifier.
                         //     This parameter must be a number between Min_Data = 0 and Max_Data = 0x7FF.

      uint32_t ExtId;    //!< Specifies the extended identifier.
                         //     This parameter must be a number between Min_Data = 0 and Max_Data = 0x1FFFFFFF.

      uint32_t IDE;      //!< Specifies the type of identifier for the message that will be transmitted.
                         //     This parameter can be a value of @ref CAN_identifier_type

      uint32_t RTR;      //!< Specifies the type of frame for the message that will be transmitted.
                         //     This parameter can be a value of @ref CAN_remote_transmission_request

      uint32_t DLC;      //!< Specifies the length of the frame that will be transmitted.
                         //     This parameter must be a number between Min_Data = 0 and Max_Data = 8.

      uint32_t Timestamp; //!< Specifies the timestamp counter value captured on start of frame reception.
                          //    @note: Time Triggered Communication Mode must be enabled.
                          //    This parameter must be a number between Min_Data = 0 and Max_Data = 0xFFFF.

      uint32_t FilterMatchIndex; //!< Specifies the index of matching acceptance filter element.
                              // This parameter must be a number between Min_Data = 0 and Max_Data = 0xFF.

} CAN_RxHeaderTypeDef;


  // Receive
  typedef struct CanRxMsgTypeDef{
      CAN_RxHeaderTypeDef CAN_RxHeaderTypeDef;
      uint8_t Data[8];
  }CanRxMsgTypeDef;

  // define variables
  CanTxMsgTypeDef TxMessageBuffer1[CAN_MAX_TX_BUFF];
  RING_BUFF_INFO TxMessagePtr1;
  CanRxMsgTypeDef RxMessageBuffer1[CAN_MAX_RX_BUFF];
  RING_BUFF_INFO RxMessagePtr1;


  // function to add to Rx buffer
    void AddCanRxBuffer1(CanRxMsgTypeDef *canMsg) {
        unsigned char i;
        RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.ExtId = canMsg->CAN_RxHeaderTypeDef.ExtId;
        RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.StdId = canMsg->CAN_RxHeaderTypeDef.StdId;
        RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.RTR = canMsg->CAN_RxHeaderTypeDef.RTR;
        RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.IDE = canMsg->CAN_RxHeaderTypeDef.IDE;
        RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.DLC = canMsg->CAN_RxHeaderTypeDef.DLC;
        for(i = 0; i < RxMessageBuffer1[RxMessagePtr1.iIndexIN].CAN_RxHeaderTypeDef.DLC; i++) {
            RxMessageBuffer1[RxMessagePtr1.iIndexIN].Data[i] = canMsg->Data[i];
        }
        DRV_RingBuffPtr__Input(&RxMessagePtr1, CAN_MAX_TX_BUFF); // increment input buffer ptr
    }

    // add to Tx buffer
    void AddCanTxBuffer1(CanTxMsgTypeDef *canMsg) {
        unsigned char i;
        TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.ExtId = canMsg->CAN_TxHeaderTypeDef.ExtId;
        TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.StdId = canMsg->CAN_TxHeaderTypeDef.StdId;
        TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.RTR = canMsg->CAN_TxHeaderTypeDef.RTR;
        TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.IDE = canMsg->CAN_TxHeaderTypeDef.IDE;
        TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.DLC = canMsg->CAN_TxHeaderTypeDef.DLC;
        for(i = 0; i < TxMessageBuffer1[TxMessagePtr1.iIndexIN].CAN_TxHeaderTypeDef.DLC; i++) {
            TxMessageBuffer1[TxMessagePtr1.iIndexIN].Data[i] = canMsg->Data[i];
        }
        DRV_RingBuffPtr__Input(&TxMessagePtr1, CAN_MAX_TX_BUFF); // increment input pointer
    }

    int main(void){

        while(RxMessagePtr1.iCnt_Handle){ // doesn't have to be while, can be some other state machine.
            // do something with data
            something = RxMessageBuffer1[RxMessagePtr1.iIndexOUT].CAN_RxHeaderTypeDef.ExtId
                                         // other data


            // increment output pointer
            DRV_RingBuffPtr__Output(&RxMessagePtr1, CAN_MAX_RX_BUFF);
        }

        CanTxMsgTypeDef canMsg;
        while(TxMessagePtr1.iCnt_Handle){
            // getting Transmit data from buffer
            canMsg.CAN_TxHeaderTypeDef.ExtId = TxMessageBuffer1[TxMessagePtr1.iIndexOUT].CAN_TxHeaderTypeDef.ExtId;
                                         // other data

            // send data
            CAN_SendRoutine(&canMsg);

            // increment output pointer
            DRV_RingBuffPtr__Output(&TxMessagePtr1, CAN_MAX_TX_BUFF);
        }
    }
 */



//==========================================================
// Layer : DRIVER
//==========================================================

void DRV_RingBuffPtr__Clean(RING_BUFF_INFO *ptr) {
	ptr->iIndexIN = 0;
	ptr->iIndexOUT = 0;

	ptr->iCnt_Handle = 0;
	ptr->iCnt_OverFlow = 0;
}

void DRV_RingBuffPtr__Input(RING_BUFF_INFO *ptr, unsigned int iBufferSize) {
	ptr->iIndexIN++;
	if (ptr->iIndexIN >= iBufferSize)
		ptr->iIndexIN = 0;

	ptr->iCnt_Handle++;
	if (ptr->iIndexIN == ptr->iIndexOUT) {
		ptr->iCnt_OverFlow++;
		if (ptr->iCnt_OverFlow > 50000)
			ptr->iCnt_OverFlow = 0;
		if (ptr->iIndexIN == 0) {
			ptr->iIndexOUT = iBufferSize - 1;
		} else {
			ptr->iIndexOUT = ptr->iIndexIN - 1;
		}
		ptr->iCnt_Handle = 1;
	}
}

void DRV_RingBuffPtr__Output(RING_BUFF_INFO *ptr, unsigned int iBufferSize) {
	if (ptr->iCnt_Handle) {
		ptr->iIndexOUT++;
		if (ptr->iIndexOUT >= iBufferSize)
			ptr->iIndexOUT = 0;
		ptr->iCnt_Handle--;
	}
}

void DRV_RingBuffPtr__Output_JumpToNew(RING_BUFF_INFO *ptr, unsigned int iBufferSize) {
	if (ptr->iCnt_Handle > 1) {
		while (ptr->iCnt_Handle > 1) {
			ptr->iIndexOUT++;
			if (ptr->iIndexOUT >= iBufferSize)
				ptr->iIndexOUT = 0;
			ptr->iCnt_Handle--;
		}
	} else {
		ptr->iIndexOUT++;
		if (ptr->iIndexOUT >= iBufferSize)
			ptr->iIndexOUT = 0;
		ptr->iCnt_Handle--;
	}
}

