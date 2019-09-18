#ifndef MIRROR_MOVEMENT_REQUEST_H
#define MIRROR_MOVEMENT_REQUEST_H


typedef union MirrorMovementRequest {
	struct {
		uint8_t byte1;
		uint8_t byte0;
	}ByteArray;
	struct {
		unsigned MirrorFoldRequest:2;
		unsigned MirrorExtensionRequest:2;
		unsigned PassengerMirrorParkingTiltRequest:2;
		unsigned :2;
		unsigned MirrorSwitchActivationLeftRight:2;
		unsigned MirrorSwitchActivationUpDown:2;
		unsigned MirrorSelectSwitchStatus:2;
		unsigned :2;
	}Status;
}MirrorMovementRequest;

typedef struct {
	bool mirrorUpActive;
	bool mirrorDownActive;
	bool mirrorLeftActive;
	bool mirrorRightActive;
	unsigned :4;
}MirrorStatus;

void ServiceMirrorMovementRequest(uint8_t *data);
void CheckMirrorMovementRequest(uint32_t delay);
MirrorStatus GetMirrorStatusActiveFlag(void);

#endif // MIRROR_MOVEMENT_REQUEST_H
