#ifndef REMOTE_RECEIVER_STATUS_H
#define REMOTE_RECEIVER_STATUS_H

typedef union RemoteReceiverStatus {
	struct {
		uint8_t byte7;
		uint8_t byte6;
		uint8_t byte5;
		uint8_t byte4;
		uint8_t byte3;
		uint8_t byte2;
		uint8_t byte1;
		uint8_t byte0;
	}Bytes;
	struct{		
		unsigned currentAudioProgramServiceIdentifier:8;
		unsigned serviceProvider :3;
		unsigned receiverType :4;
		unsigned :1;
		unsigned receiverTrafficInterruptStatus:3;
		unsigned receiverSeekingStatus:4;
		unsigned :1;
		unsigned receiverTuningStatus:1;
		unsigned receiverEmergencyInterruptStatus:3;
		unsigned receiverScanningStatus:4;
		unsigned currentTunedStation:16;
		unsigned receiverInformationAvailableActive:1;
		unsigned receiverInitialinzingActive:1;
		unsigned receiverCommandStatus:3;
		unsigned receiverDataType:3;
		unsigned currentStationQuality:1;
		unsigned currentStationService:3;
		unsigned currentStationPresetNumber:4;
	}Status;
}RemoteReceiverStatus;

void SendRemoteReceiverStatus(RemoteReceiverStatus *remoteReceiverStatus);
void SendRemoteReceiverStatusFixedData(void);


#endif // REMOTE_RECIEVER_STATUS_H
