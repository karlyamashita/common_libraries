#ifndef ARB_TEXT_REQ_SET_DISPLAY_TEXT_H
#define ARB_TEXT_REQ_SET_DISPLAY_TEXT_H

#define TEXT_SONG_NAME 1
#define TEXT_ARTIST_NAME 2
#define TEXT_RESERVED 3
#define TEXT_CHANNEL_NAME 4
#define TEXT_CATAGORY_NAME 5
#define END_OF_TRANSMISSION 0x04
#define NEW_LINE 0x0a

#define DISPLAY_MAX_CHARACTERS_16 16
#define DISPLAY_MAX_CHARACTERS_18 18

typedef union ArbTextReqSetDisplayText{
	struct {
		unsigned char byte[8];
	}ByteArray;
	struct{
		unsigned firstFrameActive:1;// this should be false
		unsigned targetDisplayIdentifier:4;
		unsigned requestingDeviceIdentifier:3; // bits start from bottom of struct going up from 7:0
		unsigned char frameSequenceNumber; // this will indicate what frame sequence we are at
		char characterString[6];
	}Status;
	struct{
		unsigned firstFrameActive:1; // this should be true
		unsigned targetDisplayIdentifier:4;
		unsigned requestingDeviceIdentifier:3; // bits start from bottom of struct going up from 7:0
		unsigned char frameSequenceNumber;// on first frame this will indicate how many frames total
		unsigned char characterType; // on first frame active byte2 is type of text (song, artist, catagory, etc)
		char characterString[5]; // only 5 characters if first frame active
	}CharacterType;
}ArbTextReqSetDisplayText;

typedef struct ArbTextReqSetStatus {
	uint8_t charPtr;
	uint8_t stringLength;
	uint8_t totalFrames;
	uint8_t totalFramesSent;
}ArbTextReqSetStatus;

bool ServiceArbitrationTextRequestSetDisplayTextXM(uint8_t *data);
void SendArbTextReqSetDisplayText(ArbTextReqSetDisplayText *arbTextReqSetDisplayText);
void SendArbTextReqSetDisplayTextCharacterType(uint8_t characterType, const char* mString);

#endif // ARB_TEXT_REQ_SET_DISPLAY_TEXT_H
