#include "LSP.h"
#ifdef USE_ARB_TEXT_REQ_SET_DISP_PARAMETER
#include "main.h"
#include "ArbTextReqSetDispParameter.h"
#include "ArbTextDisplayStatus.h"
#include "GMLAN_Defines.h"
#include "GMLAN_CAN_ID.h"
#include "CAN.h"
#include "CAN_Buffer.h"



// Service XM and OnStar command here
void ServiceArbTextReqSetDispParameters(uint8_t *data) {

	ArbTextReqSetDispParameters arbTextReqSetDispParameters;
	ArbTextDisplayStatus arbTextDisplayStatus;
	
	arbTextReqSetDispParameters.requestingDeviceIdentifier = (data[1] >> 1) & 0x07;
	arbTextReqSetDispParameters.targetDisplayDeviceIdentifier = (data[1] & 0x01) << 3;
	arbTextReqSetDispParameters.targetDisplayDeviceIdentifier += (data[2] >> 5) & 0x07;
	arbTextReqSetDispParameters.targetDisplayDeviceSessioMode = (data[2] >> 1) & 0x0f;
	arbTextReqSetDispParameters.requesterSessionIdentifier = (data[2] & 0x01) << 2;
	arbTextReqSetDispParameters.requesterSessionIdentifier += data[3] >> 6;
	arbTextReqSetDispParameters.sessionFunctionCode = data[3] << 2;
	arbTextReqSetDispParameters.sessionFunctionCode += data[4] >> 6;
	arbTextReqSetDispParameters.requesterCaretDisplayLineIdentifier = data[4] << 2;
	arbTextReqSetDispParameters.requesterCaretDisplayLineIdentifier += data[5] >> 6;
	arbTextReqSetDispParameters.characterSet = (data[5] >> 4) & 0x03;
	arbTextReqSetDispParameters.font = (data[5] >> 2) & 0x03;
	arbTextReqSetDispParameters.horizontalJustify = data[5] & 0x03;
	
	if(arbTextReqSetDispParameters.targetDisplayDeviceIdentifier == INFOTAINMENT_DISPLAY) {
		arbTextDisplayStatus.Status.displayDeviceIdentifier = INFOTAINMENT_DISPLAY;
		arbTextDisplayStatus.Status.requestingDeviceIdentifier = arbTextReqSetDispParameters.requestingDeviceIdentifier;
		arbTextDisplayStatus.Status.sessionIdentifier = arbTextReqSetDispParameters.requesterSessionIdentifier;
		arbTextDisplayStatus.Status.iconIdentifier = 0;
		arbTextDisplayStatus.Status.messageOperationStatus = DATA_TRANSFER_GRANTED;
		SendArbTextDisplayStatus(&arbTextDisplayStatus);
	}
}

void SendArbTextReqSetDispParameters(ArbTextReqSetDispParameters *arbTextReqSetDispParameters) {	
	CanTxMsgTypeDef canMessage;
	canMessage.ExtId = ARB_TEXT_REQ_SET_DISP_PARAMETERS_SDAR_ID;
	canMessage.IDE = CAN_ID_EXT;
	canMessage.RTR = CAN_RTR_DATA;
	canMessage.DLC = 8;
	
	memset(&canMessage.Data, 0, sizeof(canMessage.Data));
	
	canMessage.Data[1] = (arbTextReqSetDispParameters->requestingDeviceIdentifier & 0x07) << 1;
	canMessage.Data[1] += ((arbTextReqSetDispParameters->targetDisplayDeviceIdentifier) >> 3) & 0x01;
	canMessage.Data[2] = (arbTextReqSetDispParameters->targetDisplayDeviceIdentifier & 0x07) << 5;
	canMessage.Data[2] += (arbTextReqSetDispParameters->targetDisplayDeviceSessioMode & 0x0F) << 1;
	canMessage.Data[2] += (arbTextReqSetDispParameters->requesterSessionIdentifier & 0x01) >> 2;
	canMessage.Data[3] = arbTextReqSetDispParameters->requesterSessionIdentifier << 6;
	canMessage.Data[3] += arbTextReqSetDispParameters->sessionFunctionCode >> 2;
	canMessage.Data[4] = arbTextReqSetDispParameters->sessionFunctionCode << 6;
	canMessage.Data[4] += arbTextReqSetDispParameters->requesterCaretDisplayLineIdentifier >> 2;
	canMessage.Data[5] =  arbTextReqSetDispParameters->requesterCaretDisplayLineIdentifier << 6;
	canMessage.Data[5] += (arbTextReqSetDispParameters->characterSet & 0x03) << 4;
	canMessage.Data[5] += (arbTextReqSetDispParameters->font & 0x03) << 2;
	canMessage.Data[5] += arbTextReqSetDispParameters->horizontalJustify & 0x03 ;

	AddTxBuffer1(&canMessage); // CAN1
}

void Emulate_ArbTextReqSetDispParameters(void) {
	ArbTextReqSetDispParameters arbTextReqSetDispParameters;	

	arbTextReqSetDispParameters.characterSet = BASE_CHARACTER_SET;
	arbTextReqSetDispParameters.font = NONE;
	arbTextReqSetDispParameters.horizontalJustify = LEFT;
	arbTextReqSetDispParameters.requesterCaretDisplayLineIdentifier = 0;
	arbTextReqSetDispParameters.requesterSessionIdentifier = 1;
	arbTextReqSetDispParameters.requestingDeviceIdentifier = REMOTE_RECEIVER;
	arbTextReqSetDispParameters.sessionFunctionCode = REMOTE_RECEIVER_INFO_DATA_TRANSFER;
	arbTextReqSetDispParameters.targetDisplayDeviceIdentifier = INFOTAINMENT_DISPLAY;
	arbTextReqSetDispParameters.targetDisplayDeviceSessioMode = DOWNLOAD_ARBITRARY_TEXT_DATA;
	SendArbTextReqSetDispParameters(&arbTextReqSetDispParameters);

}
#endif // USE_ARB_TEXT_REQ_SET_DISP_PARAMETER

