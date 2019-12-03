#include "LSP.h"
#ifdef USE_USB_BUFFER
#ifndef USB_BUFFER_H
#define USB_BUFFER_H

#define USB_MAX_RING_BUFF 8

int SendUsbMessage(void);
void AddUsbTxBuffer(uint8_t *data);
void AddUsbRxBuffer(uint8_t *data);
uint8_t UsbDataAvailable(uint8_t *data);

#endif // USB_BUFFER_H
#endif // USE_USB_BUFFER
