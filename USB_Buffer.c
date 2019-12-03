// Karl Yamashita 11/24/2019
#include "LSP.h"
#ifdef USE_USB_BUFFER
#include "main.h"
#include "ringBuff.h"
#include "USB_Buffer.h"
#include "usbd_customhid.h" // path to USBD_CUSTOMHID_OUTREPORT_BUF_SIZE and USBD_CUSTOM_HID_REPORT_DESC_SIZE defines

uint8_t usbTxBuffer[USB_MAX_RING_BUFF][USBD_CUSTOMHID_OUTREPORT_BUF_SIZE]; // array buffer to hold Tx data
RING_BUFF_INFO usbTxRingBuffPtr; // pointer to USB Tx array buffer

uint8_t usbRxBuffer[USB_MAX_RING_BUFF][USBD_CUSTOMHID_OUTREPORT_BUF_SIZE]; // array buffer to hold Rx data
RING_BUFF_INFO usbRxRingBuffPtr;

extern USBD_HandleTypeDef hUsbDeviceFS;

// poll this to check if usb data is available to send
int SendUsbMessage(void) {
	uint8_t USB_Status;
	if(usbTxRingBuffPtr.iCnt_Handle) { // send available message
		USB_Status = USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbTxBuffer[usbTxRingBuffPtr.iIndexOUT], USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
		if (USB_Status == USBD_OK) // make sure data was sent before incrementing pointer
		{
			DRV_RingBuffPtr__Output(&usbTxRingBuffPtr, USB_MAX_RING_BUFF); // increment output buffer ptr
		}
	}
	return usbTxRingBuffPtr.iCnt_Handle; // if no more message to handle then 0 will be returned
}

// adds data to USB Tx buffer
void AddUsbTxBuffer(uint8_t *data) {
	memset(&usbTxBuffer[usbTxRingBuffPtr.iIndexIN], 0, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
	for(int i = 0; i < USBD_CUSTOMHID_OUTREPORT_BUF_SIZE; i++) {
		usbTxBuffer[usbTxRingBuffPtr.iIndexIN][i] = data[i];
	}
	DRV_RingBuffPtr__Input(&usbTxRingBuffPtr, USB_MAX_RING_BUFF);
}

// add data to USB Rx buffer
void AddUsbRxBuffer(uint8_t *data) {
	memset(&usbRxBuffer[usbRxRingBuffPtr.iIndexIN], 0, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
	for(int i = 0; i < USBD_CUSTOMHID_OUTREPORT_BUF_SIZE; i++) {
		usbRxBuffer[usbRxRingBuffPtr.iIndexIN][i] = data[i];
	}
	DRV_RingBuffPtr__Input(&usbRxRingBuffPtr, USB_MAX_RING_BUFF);
}

/*
 * Check for USB Rx message
 * Input data: pointer to array to save data from Rx buffer
 * Output return 1 if data available, 0 if no data
 */
uint8_t UsbDataAvailable(uint8_t *data) {
	uint8_t dataAvailableFlag = 0;
	if(usbRxRingBuffPtr.iCnt_Handle) {
		for(int i = 0; i < USBD_CUSTOMHID_OUTREPORT_BUF_SIZE; i++) {
			data[i] = usbRxBuffer[usbRxRingBuffPtr.iIndexOUT][i];
		}
		dataAvailableFlag = 1;
		DRV_RingBuffPtr__Output(&usbRxRingBuffPtr, USB_MAX_RING_BUFF);
	}
	return dataAvailableFlag;
}

#endif
