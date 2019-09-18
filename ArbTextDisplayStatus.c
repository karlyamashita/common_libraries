#include "LSP.h"
#ifdef USE_ARB_TEXT_DISPLAY_STATUS
#include "main.h"
#include "ArbTextDisplayStatus.h"
#include "GMLAN_Defines.h"
#include "CAN.h"
#include "GMLAN_CAN_ID.h"
#include "CAN_Buffer.h"
#include "ArbTextReqSetDispParameter.h"
#include "NCA_From_SDAR.h"
#include "RemoteReceiverStatus.h"


ArbTextDisplayStatus arbTextDisplayStatus;
uint8_t displayStatus;
bool ServiceArbTextDisplayStatus(uint8_t *data){
	arbTextDisplayStatus.Bytes.byte0 = data[0];
	arbTextDisplayStatus.Bytes.byte1 = data[1];
	arbTextDisplayStatus.Bytes.byte2 = data[2];
	arbTextDisplayStatus.Bytes.byte3 = data[3];
	
	displayStatus = arbTextDisplayStatus.Status.messageOperationStatus;
	
	switch(arbTextDisplayStatus.Status.displayDeviceIdentifier) {
		case INFOTAINMENT_DISPLAY:
			switch (arbTextDisplayStatus.Status.requestingDeviceIdentifier) {
				case ALL_REQUESTING_DEVICES:
					
					switch(arbTextDisplayStatus.Status.messageOperationStatus) {
						case REQUESTED_DISPLAY_CANCELED:
							if(GetNCA_From_SDAR_PresentFlag() == false) {
								Emulate_ArbTextReqSetDispParameters();
								SendRemoteReceiverStatusFixedData();
							}
						break;
						case DATA_TRANSFER_NOT_GRANTED:

						break;
						case DATA_TRANSFER_GRANTED:

						break;
						case DISPLAY_AVAILABLE:
							if(GetNCA_From_SDAR_PresentFlag() == false) {
								Emulate_ArbTextReqSetDispParameters();
								SendRemoteReceiverStatusFixedData();
							}
						break;
						case INVALID_DATA_LENGTH:
							
						break;
						case INVALID_DATA:
							
						break;
					}
				break;
				case REMOTE_RECEIVER:
					switch(arbTextDisplayStatus.Status.messageOperationStatus) {
						case REQUESTED_DISPLAY_CANCELED:
							if(GetNCA_From_SDAR_PresentFlag() == false) {
								Emulate_ArbTextReqSetDispParameters();
								SendRemoteReceiverStatusFixedData();
							}
						break;
						case DATA_TRANSFER_NOT_GRANTED:

						break;
						case DATA_TRANSFER_GRANTED:

						break;
						case DISPLAY_AVAILABLE:
							if(GetNCA_From_SDAR_PresentFlag() == false) {
								Emulate_ArbTextReqSetDispParameters();
								SendRemoteReceiverStatusFixedData();
							}
						break;
						case INVALID_DATA_LENGTH:
							
						break;
						case INVALID_DATA:
							
						break;
					}
				break;
				default:
					break;
			}
		break;
		case CLUSTER_DISPLAY:
			switch (arbTextDisplayStatus.Status.requestingDeviceIdentifier) {
				case ALL_REQUESTING_DEVICES:
					//break;
				case TELEMATICS:
					
					break;
				default:
					break;
			}

			break;
		case REAR_SEAT_DISPLAY:
			switch (arbTextDisplayStatus.Status.requestingDeviceIdentifier) {
				case ALL_REQUESTING_DEVICES:
					//break;
				case HEAD:

					break;
				default:
					break;
			}
			break;
			case HEADUP_DISPLAY:
				switch (arbTextDisplayStatus.Status.requestingDeviceIdentifier) {
					case ALL_REQUESTING_DEVICES:
						//break;
					case REMOTE_RECEIVER:

						break;
					default:
						break;
				}
			break;
		default:
			break;
	}
	return true;
}	

ArbTextDisplayStatus GetArbTextDisplayStatus(void) {
	return arbTextDisplayStatus;
}

void SendArbTextDisplayStatus(ArbTextDisplayStatus *arbTextDisplayStatus) {
	
	CanTxMsgTypeDef canMessage;
	canMessage.ExtId = ARB_TEXT_DISPLAY_STATUS_IRC_ID;	
  canMessage.IDE = CAN_ID_EXT;
	canMessage.RTR = CAN_RTR_DATA;										
	canMessage.DLC = 8;

	canMessage.Data[0] = arbTextDisplayStatus->Bytes.byte0;
	canMessage.Data[1] = arbTextDisplayStatus->Bytes.byte1;
	canMessage.Data[2] = arbTextDisplayStatus->Bytes.byte2;
	canMessage.Data[3] = arbTextDisplayStatus->Bytes.byte3;

	AddTxBuffer2(&canMessage);
}
#endif // USE_ARB_TEXT_DISPLAY_STATUS
