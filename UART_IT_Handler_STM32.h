/*
 * UART_IT_Handler_STM32.h
 *
 *  Created on: Sep 17, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_UART_IT_HANDLER_STM32_H_
#define INC_UART_IT_HANDLER_STM32_H_




#define UART_RX_IRQ_BYTE_SIZE 1 // Typically this is 1 byte for most uC.

#define UART_RX_BYTE_BUFFER_SIZE 128 // this holds all the IRQ data
#define UART_TX_BYTE_BUFFER_SIZE 128
#define UART_RX_MESSAGE_QUEUE_SIZE 8// buffer size of complete strings or packets.
#define UART_TX_MESSAGE_QUEUE_SIZE 8 // buffer size of complete strings or packets.




enum UART_TYPE
{
	UART_ASCII,
	UART_BINARY
};

typedef enum CheckSumType
{
	CHECKSUM_MOD256, // 8 bit
	CHECKSUM_16 // 16 bit
}CheckSumType;

typedef struct
{
	uint8_t data[UART_RX_BYTE_BUFFER_SIZE];
	uint32_t size;
}UartDataStruct;

typedef struct
{
	UART_HandleTypeDef *huart;
    struct
	{
    	uint8_t irqByte; // UART IRQ needs to save Rx byte to this variable
		UartDataStruct queue[UART_RX_MESSAGE_QUEUE_SIZE];
		uint32_t queueSize;
		uint32_t bytePtrSize; // also used in binaryQueue
		UartDataStruct *msgToParse;

		uint8_t binaryBuffer[UART_RX_BYTE_BUFFER_SIZE]; // holds all bytes. Packets will be extracted from this buffer later
		uint32_t packetSize; // for binary packets

		RING_BUFF_STRUCT bytePtr; // pointer for byteBuffer
		RING_BUFF_STRUCT ptr; // pointer for queue
		uint8_t uartType; // UART_ASCII or UART_BINARY, default UART_ASCII
		uint8_t checksumType; // For use with binary packets. 0 = MOD256, 1 = 16 bit (not implemented yet)

		HAL_StatusTypeDef HAL_Status;
	}rx;
	struct
	{
		UartDataStruct queue[UART_TX_MESSAGE_QUEUE_SIZE];
		uint32_t queueSize;
		UartDataStruct *msgToSend;
		uint32_t msgToSend_BytePtr;
		bool msgToSend_Pending;
		RING_BUFF_STRUCT ptr; // pointer for queue
	}tx;
}UartBufferStruct;


void UART_AddByteToBuffer(UartBufferStruct *msg);
void UART_SortPacketBuffer(UartBufferStruct *msg);

void UART_EnableRxInterrupt(UartBufferStruct *msg);
void UART_CheckRxIntError(UartBufferStruct *msg);
void UART_InitPacketSize(UartBufferStruct *msg, uint32_t size);
bool UART_RxMsgRdy(UartBufferStruct *msg);

void UART_NotifyUser(UartBufferStruct *msg, char *str, uint32_t strlen, bool lineFeed);

void UART_TX_AddDataToBuffer(UartBufferStruct *msg, uint8_t *msgIN, uint32_t dataSize);
int UART_TxMessage_IT(UartBufferStruct *msg);


#endif /* INC_UART_IT_HANDLER_STM32_H_ */
