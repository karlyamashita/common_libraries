#include "LSP.h"
#if defined USE_CAN_AUTOBAUD
#include "main.h"
#include "CAN_Autobaud.h"
#include "can.h"
#include "can_buffer.h"
#include "ringBuff.h"


extern RING_BUFF_INFO RxMessagePtr1;
extern CanRxMsgTypeDef RxMessageBuffer1[CAN_MAX_RX_BUFF];

const Bitrate_stm bitrate_stm[COUNT_BITRATE]= {  
//    PRESCal     BS1  BS2        
     { 6    , CAN_BS1_8TQ, CAN_BS2_3TQ}  // 500kbit   S6                           
,    { 12   , CAN_BS1_8TQ, CAN_BS2_3TQ}  // 250kbit   S5                            
,    { 24   , CAN_BS1_8TQ, CAN_BS2_3TQ}  // 125kbit   S4                            
,    { 30   , CAN_BS1_8TQ, CAN_BS2_3TQ}  // 100kbit   S3                                                        
,    { 36   , CAN_BS1_8TQ, CAN_BS2_3TQ}  // 83kbit    S9                                                          
,    { 90   , CAN_BS1_8TQ, CAN_BS2_3TQ}  // 33kbit                                
};

/*
Auto detect the CAN bus baudrate
*/
int8_t CAN_Autobaud(void) {
	int8_t baudRate = -1;
	static uint8_t mode = 0, baudRateCount = 0;
	Bitrate_stm bitrate;
	
	while(1) {
		switch(mode) {
			case 0: // set new baudrate
				bitrate = bitrate_stm[baudRateCount];
			//	MX_CAN1_SetBaudRate(&bitrate);
			break;
			case 1:
				if(RxMessagePtr1.iCnt_Handle) {// check for messages
					if(RxMessageBuffer1[RxMessagePtr1.iIndexOUT].IDE == CAN_ID_STD) { // std id
						switch(RxMessageBuffer1[RxMessagePtr1.iIndexOUT].StdId) {
							case 0x000: // power mode for 83.333kbs
							case 0x20B: // newer 83.333kbs
								
							break;
							case 0x122: // power mode for 125kbs
								
							break;
							default:
							break;
						}
					} 
					DRV_RingBuffPtr__Output(&RxMessagePtr1, CAN_MAX_RX_BUFF); // increment output buffer ptr	
				}
			break;
			case 2:
				
			break;
		}
		if(1) {
			break;
		}
	}
	
	return baudRate;
}

#endif // USE_CAN_AUTOBAUD
