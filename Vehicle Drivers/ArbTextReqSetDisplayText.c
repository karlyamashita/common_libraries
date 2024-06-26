#include "LSP.h"
#ifdef USE_ARB_TEXT_REQ_SET_DISPLAY_TEXT
#include "main.h"
#include "ArbTextReqSetDisplayText.h"
#include "GMLAN_Defines.h"
#include "GMLAN_CAN_ID.h"
#include "ringBuff.h"
#include "Can.h"
#include "CAN_Buffer.h"
#include "mTimers.h"
#include "CAN_MessageDelayed.h"

#define MAX_RING_BUFFER_DISPLAY_TEXT_XM 16
RING_BUFF_INFO channelNamePrt;
RING_BUFF_INFO songNamePtr;
RING_BUFF_INFO artistNamePtr;
RING_BUFF_INFO catagoryPtr;

#define MAX_CHARCTERS_ARRAY 24 // displays have 16 or 18 characters for now
uint8_t channelNameArray[MAX_CHARCTERS_ARRAY];
uint8_t songNameArray[MAX_CHARCTERS_ARRAY];
uint8_t artistNameArray[MAX_CHARCTERS_ARRAY];
uint8_t catagoryArray[MAX_CHARCTERS_ARRAY];


/*
function: For radio replacement. Routine to save SDAR text to buffer then send text to IPC, RSE display. 
input: the data
output: flag when done receiving all text. Poll to send text to display
*/
ArbTextReqSetDisplayText arbTextReqSetDisplayTextXM;
#define arbTextCharacterType arbTextReqSetDisplayTextXM.CharacterType.characterType
#define arbTextFirstFrameActive arbTextReqSetDisplayTextXM.Status.firstFrameActive
#define arbTextFrameSequenceNumber arbTextReqSetDisplayTextXM.Status.frameSequenceNumber
#define arbTextTargetDisplayIdentifier arbTextReqSetDisplayTextXM.Status.targetDisplayIdentifier
#define arbTextRequestingDeviceIdentifier arbTextReqSetDisplayTextXM.Status.requestingDeviceIdentifier
bool ServiceArbitrationTextRequestSetDisplayTextXM(uint8_t *data) {
	uint8_t i;
	static	uint8_t frameSequenceNumber;
	static	uint8_t characterType;
	static	uint8_t characterAmount = 5; //if firstframeactive then characters = 5, then = 6
	static	uint8_t characterOffset = 0;
	
	for(i = 0; i < 8; i++){
		arbTextReqSetDisplayTextXM.ByteArray.byte[i] = data[i];
	}

	if(arbTextReqSetDisplayTextXM.Status.firstFrameActive)
	{ // if first frame then clear the array
		frameSequenceNumber = arbTextReqSetDisplayTextXM.Status.frameSequenceNumber; // current frame sequence
		characterType = arbTextCharacterType;
		characterAmount = 5; // set to 5, then 6 after
		characterOffset = 8 - characterAmount;
		switch (characterType) {
			case TEXT_CHANNEL_NAME:
				memset(channelNameArray, 0x00, sizeof(channelNameArray));
				break;
			case TEXT_SONG_NAME:
				memset(songNameArray, 0x00, sizeof(songNameArray));
				break;
			case TEXT_ARTIST_NAME:
				memset(artistNameArray, 0x00, sizeof(artistNameArray));
				break;
			case TEXT_CATAGORY_NAME:
				memset(catagoryArray, 0x00, sizeof(catagoryArray));
				break;
			default:
				break;
		}
	} else {
		characterAmount = 6;
		characterOffset = 8 - characterAmount;
	}

	// save text to array
	for (i = 0; i < characterAmount; i++)
	{
		switch (characterType) {
			case TEXT_CHANNEL_NAME:
				channelNameArray[channelNamePrt.iIndexIN] = arbTextReqSetDisplayTextXM.ByteArray.byte[i + characterOffset];
				DRV_RingBuffPtr__Input(&channelNamePrt, 4);
				break;
			case TEXT_SONG_NAME:
				songNameArray[songNamePtr.iIndexIN] = arbTextReqSetDisplayTextXM.ByteArray.byte[i + characterOffset];
				DRV_RingBuffPtr__Input(&songNamePtr, 4);
				break;
			case TEXT_ARTIST_NAME:
				artistNameArray[artistNamePtr.iIndexIN] = arbTextReqSetDisplayTextXM.ByteArray.byte[i + characterOffset];
				DRV_RingBuffPtr__Input(&artistNamePtr, 4);
				break;
			case TEXT_CATAGORY_NAME:
				catagoryArray[catagoryPtr.iIndexIN] = arbTextReqSetDisplayTextXM.ByteArray.byte[i + characterOffset];
				DRV_RingBuffPtr__Input(&catagoryPtr, 4);
				break;
			default:
				break;
		}
	}

	// check if done
	if ((arbTextReqSetDisplayTextXM.Status.firstFrameActive && arbTextReqSetDisplayTextXM.Status.frameSequenceNumber == 1) \
			|| (!arbTextReqSetDisplayTextXM.Status.firstFrameActive && arbTextReqSetDisplayTextXM.Status.frameSequenceNumber == frameSequenceNumber)) {
		switch (characterType) {
			case TEXT_CHANNEL_NAME:
//todo flag which display to send to
				break;
			case TEXT_SONG_NAME:

				break;
			case TEXT_ARTIST_NAME:
				
				break;
			case TEXT_CATAGORY_NAME:

				break;
			default:
				break;
		}
	}
	return false;
}

/*
function: For gateway. Send text to radio if we're in BT mode. Some displays will need Return (0x0A) at end of string or 0x20.
input: the text and which display to send to
output: none
*/
void SendArbTextReqSetDisplayText(ArbTextReqSetDisplayText *arbTextReqSetDisplayText){
	int i = 0;
	CanTxMsgTypeDef canMessage;
	canMessage.ExtId = ARB_TEXT_REQ_SET_DISPLAY_TEXT_SDAR_ID;
	canMessage.IDE = CAN_ID_EXT;
	canMessage.RTR = CAN_RTR_DATA;
	canMessage.DLC = 8;		

	for(i = 0; i < 8; i++){
		canMessage.Data[i] = arbTextReqSetDisplayText->ByteArray.byte[i];
	}
	AddDelayedCAN1_MessageToBuffer(&canMessage, 10);	// 10ms
}

/*
function: For gateway. This function will automatically send text with 0x0A at end which is needed for radio to accept the data. RSE might need 0x00, will need to verify.
input: The character type (Song, Artist, Catagory, Channel name) and pointer to string
output: none
*/
void SendArbTextReqSetDisplayTextCharacterType(uint8_t characterType, const char* mString) { // todo add parameter to send to specific display, for now just send to radio.
	uint8_t i, charactersToSend;
	static ArbTextReqSetStatus arbTextReqSetStatus = {0};
	static ArbTextReqSetDisplayText arbTextReqSetDisplayText;
		
	memset(&arbTextReqSetDisplayText, 0, sizeof(arbTextReqSetDisplayText)); // clear all
	memset(&arbTextReqSetStatus, 0, sizeof(arbTextReqSetStatus)); // clear all
	
	arbTextReqSetStatus.stringLength = strlen(mString);
	
	arbTextReqSetDisplayText.CharacterType.characterType = characterType;// song, channel name, etc..
	
	arbTextReqSetDisplayText.CharacterType.firstFrameActive = true;
	arbTextReqSetDisplayText.Status.frameSequenceNumber = 1; // this will change depending on how many frames
	arbTextReqSetDisplayText.Status.targetDisplayIdentifier = INFOTAINMENT_DISPLAY;
	arbTextReqSetDisplayText.Status.requestingDeviceIdentifier = REMOTE_RECEIVER;
	
	if(arbTextReqSetStatus.stringLength >= 11) { // find total frames to be sent
		arbTextReqSetStatus.totalFrames = 3; // total frames to be sent. I've never seen more than 3 frames being sent.
	} else if(arbTextReqSetStatus.stringLength >= 5) {
		arbTextReqSetStatus.totalFrames = 2;
	} else {
		arbTextReqSetStatus.totalFrames = 1;
	}
	
	charactersToSend = 5;// first frame will always be 5 characters
	arbTextReqSetStatus.totalFramesSent = 1; // need to start at 1

	//first frame
	while(arbTextReqSetStatus.totalFramesSent < (arbTextReqSetStatus.totalFrames + 1)){		
		if(arbTextReqSetDisplayText.CharacterType.firstFrameActive == true) {
			arbTextReqSetDisplayText.CharacterType.frameSequenceNumber = arbTextReqSetStatus.totalFrames;// #of frames to be sent depending on size of text
		} else {
			arbTextReqSetDisplayText.Status.frameSequenceNumber = arbTextReqSetStatus.totalFramesSent;
		}
		for(i = 0; i < charactersToSend; i++) {
			if(arbTextReqSetDisplayText.CharacterType.firstFrameActive == true) {
				arbTextReqSetDisplayText.CharacterType.characterString[i] = mString[arbTextReqSetStatus.charPtr];
			} else {
				arbTextReqSetDisplayText.Status.characterString[i] = mString[arbTextReqSetStatus.charPtr];
			}
			if(mString[arbTextReqSetStatus.charPtr] == 0x00 ||
					(arbTextReqSetStatus.totalFrames == 2 && arbTextReqSetStatus.charPtr == 10) ||
						(arbTextReqSetStatus.totalFrames == 3 && arbTextReqSetStatus.charPtr == 16)) {
				if(arbTextReqSetDisplayText.CharacterType.firstFrameActive == true) {			
					arbTextReqSetDisplayText.CharacterType.characterString[i] = 0x0A;// replace 0x00 with newline
				} else {
					arbTextReqSetDisplayText.Status.characterString[i] = 0x0A;// replace 0x00 with newline
				}
				break; // end of string
			}
			arbTextReqSetStatus.charPtr = arbTextReqSetStatus.charPtr + 1;
		}
		SendArbTextReqSetDisplayText(&arbTextReqSetDisplayText);// send the text to Tx buffer
		
		arbTextReqSetDisplayText.CharacterType.firstFrameActive = false;
		charactersToSend = 6;// now we can send 6 characters
		arbTextReqSetStatus.totalFramesSent += 1; //
		memset(&arbTextReqSetDisplayText.Status.characterString, 0 ,
				sizeof(arbTextReqSetDisplayText.Status.characterString)); // clear the character array
	}
}
#endif // USE_ARB_TEXT_REQ_SET_DISPLAY_TEXT

