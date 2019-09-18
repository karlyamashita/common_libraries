
#ifndef ADVANCED_REMOTE_RECEIVER_CONTROL_H
#define ADVANCED_REMOTE_RECEIVER_CONTROL_H

typedef union AdvancedRemoteReceiverControl{
	struct {
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct {
		unsigned remoteRecieverTuneValue:16;
		
		unsigned informationRequest:1;
		unsigned :2;
		unsigned remoteReceiverTuneCommand:5;
		
		unsigned :1;
		unsigned groupCommand:3;
		unsigned headUnitRemoteReceiverControllerTargetDevice:4;
	}AdvancedRemoteReceiverControlBits;
}AdvancedRemoteReceiverControl;


void ServiceAdvancedRemoteReceiverControl(uint8_t *data);
bool ServiceAdvancedRemoteReceiverControlAsGateway(uint8_t *data);

void SendAdvancedRemoteReceiverControlCAN2(AdvancedRemoteReceiverControl *advancedRemoteReceiverControl);


#endif // ADVANCED_REMOTE_RECEIVER_CONTROL_H
