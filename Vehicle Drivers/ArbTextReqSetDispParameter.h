
#ifndef ARB_TEXT_REQ_SET_DISP_PARAMETER_H
#define ARB_TEXT_REQ_SET_DISP_PARAMETER_H

/*
typedef struct ArbTextReqSetDispParameters{
	unsigned horizontalJustify:2;
	unsigned font:2;
	unsigned characterSet:2;
	unsigned char requesterCaretDisplayLineIdentifier;
	unsigned char sessionFunctionCode;
	unsigned requesterSessionIdentifier:3;
	unsigned targetDisplayDeviceSessioMode:4;
	unsigned targetDisplayDeviceIdentifier:4; 
	unsigned requestingDeviceIdentifier:3;
	unsigned :4; // 4 bits not used
	unsigned :8; // byte 0 is not used
}ArbTextReqSetDispParameters;
*/

typedef union
{
    struct
    {
        uint8_t data[5];
    }Bytes;
    struct
    {
        unsigned targetDisplayDeviceIdentifier:4;
        unsigned requestingDeviceIdentifier:3;
    	unsigned :1;

        unsigned requesterSessionIdentifier:3;
        unsigned targetDisplayDeviceSessioMode:4;
    	unsigned :1;
    	
    	unsigned sessionFunctionCode:8;
    	unsigned requesterCaretDisplayLineIdentifier:8;
    	
    	unsigned :2;
    	unsigned horizontalJustify:2;
    	unsigned font:2;
    	unsigned characterSet:2;
    	
    }Status;
}ArbTextReqSetDispParameters;

void ServiceArbTextReqSetDispParameters(uint8_t *data);

void SendArbTextReqSetDispParameters(ArbTextReqSetDispParameters *arbTextReqSetDispParameters);

void Emulate_ArbTextReqSetDispParameters(void);

#endif // ARB_TEXT_REQ_SET_DISP_PARAMETER_H
