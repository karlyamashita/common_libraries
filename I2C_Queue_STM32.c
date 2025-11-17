/*
 * I2C_Queue_STM32.c
 *
 *  Created on: Oct 21, 2025
 *      Author: karl.yamashita
 */


#include "main.h"


int I2C_QUEUE_Add(I2C_QueueStruct_t *queue, I2C_GenericQueueDef *i2c)
{
	// copy i2c to queue
	memcpy(&queue->queueData[queue->ptr.index_IN], i2c, sizeof(I2C_GenericQueueDef));

	// increment pointer
	RingBuff_Ptr_Input(&queue->ptr, queue->queueSize);

	// try sending/receiving
	I2C_QUEUE_Transmit(queue);

	return 0;
}

/*
 * Description: Transmit I2C message which can be a Read or Write
 */
int I2C_QUEUE_Transmit(I2C_QueueStruct_t *queue)
{
	int status = NO_ERROR;

	if(queue->ptr.cnt_Handle)
	{
		if(!queue->queueData->pending)
		{
			queue->msgToParse = &queue->queueData[queue->ptr.index_OUT];

			switch(queue->msgToParse->readWriteType)
			{
			case WRITE:
				status = I2C_QUEUE_Master_Transmit_Generic_Method(queue->msgToParse);
				break;
			case READ:
				status = I2C_QUEUE_Master_Receive_Generic_Method(queue->msgToParse);
				break;
			case MEM_WRITE:
				status = I2C_QUEUE_Mem_Write_Generic_Method(queue->msgToParse);
				break;
			case MEM_READ:
				status = I2C_QUEUE_Mem_Read_Generic_Method(queue->msgToParse);
				break;
			default:
				status = READ_WRITE_TYPE;
				break;
			}

			if(status == HAL_OK)
			{
				RingBuff_Ptr_Output(&queue->ptr, queue->queueSize);
			}
		}
	}

	return status;
}


// Prototypes
// Polling
static int I2C_QUEUE_Master_Receive_Generic(I2C_GenericQueueDef *i2c);
static int I2C_QUEUE_Master_Transmit_Generic(I2C_GenericQueueDef *i2c);
static int I2C_QUEUE_Mem_Read_Generic(I2C_GenericQueueDef *i2c);
static int I2C_QUEUE_Mem_Write_Generic(I2C_GenericQueueDef *i2c);

// interrupt
static int I2C_QUEUE_Master_Receive_Generic_IT(I2C_GenericQueueDef *i2c);
static int I2C_QUEUE_Master_Transmit_Generic_IT(I2C_GenericQueueDef *i2c);
static int I2C_QUEUE_Mem_Read_Generic_IT(I2C_GenericQueueDef *i2c);
static int I2C_QUEUE_Mem_Write_Generic_IT(I2C_GenericQueueDef *i2c);

// DMA
static int I2C_QUEUE_Master_Receive_Generic_DMA(I2C_GenericQueueDef *i2c);
static int I2C_QUEUE_Master_Transmit_Generic_DMA(I2C_GenericQueueDef *i2c);
static int I2C_QUEUE_Mem_Read_Generic_DMA(I2C_GenericQueueDef *i2c);
static int I2C_QUEUE_Mem_Write_Generic_DMA(I2C_GenericQueueDef *i2c);


/*
 * Description: Calling this function will use specific I2C driver, Polling, Interrupt or DMA.
 */
int I2C_QUEUE_Master_Receive_Generic_Method(I2C_GenericQueueDef *i2c)
{
	int status = NO_ERROR;

	switch(i2c->transferType)
	{
		case I2C_POLLING:
			status = I2C_QUEUE_Master_Receive_Generic(i2c);
			if(i2c->RxISR)
			{
				i2c->RxISR(i2c); // There is no interrupt so we manually call the callback
			}
			break;
		case I2C_INTERRUPT:
			status = I2C_QUEUE_Master_Receive_Generic_IT(i2c);
			break;
		case I2C_DMA:
			status = I2C_QUEUE_Master_Receive_Generic_DMA(i2c);
			break;
		default:
			status = I2C_QUEUE_Master_Receive_Generic(i2c);
			break;
	}

	return status;
}

/*
 * Description: Calling this function will use specific I2C driver, Polling, Interrupt or DMA.
 */
int I2C_QUEUE_Master_Transmit_Generic_Method(I2C_GenericQueueDef *i2c)
{
	int status = NO_ERROR;

	switch(i2c->transferType)
	{
		case I2C_POLLING:
			status = I2C_QUEUE_Master_Transmit_Generic(i2c);
			if(i2c->TxISR)
			{
				i2c->TxISR(i2c); // There is no interrupt so we manually call the callback
			}
			break;
		case I2C_INTERRUPT:
			status = I2C_QUEUE_Master_Transmit_Generic_IT(i2c);
			break;
		case I2C_DMA:
			status = I2C_QUEUE_Master_Transmit_Generic_DMA(i2c);
			break;
		default:
			status = I2C_QUEUE_Master_Transmit_Generic(i2c);
			break;
	}

	return status;
}

/*
 * Description: Calling this function will use specific I2C driver, Polling, Interrupt or DMA.
 */
int I2C_QUEUE_Mem_Read_Generic_Method(I2C_GenericQueueDef *i2c)
{
	int status = NO_ERROR;

	switch(i2c->transferType)
	{
		case I2C_POLLING:
			status = I2C_QUEUE_Mem_Read_Generic(i2c);
			if(status == NO_ERROR)
			{
				if(i2c->RxISR)
				{
					i2c->RxISR(i2c); // There is no interrupt so we manually call the callback
				}
			}
			break;
		case I2C_INTERRUPT:
			status = I2C_QUEUE_Mem_Read_Generic_IT(i2c);
			break;
		case I2C_DMA:
			status = I2C_QUEUE_Mem_Read_Generic_DMA(i2c);
			break;
		default:
			status = I2C_QUEUE_Mem_Read_Generic(i2c);
			break;
	}

	if(status == HAL_ERROR)
	{
		Nop();
		++xdpCount.hal_error_count;
	}
	else if(status == HAL_BUSY)
	{
		++xdpCount.hal_busy_count;;
	}
	else if(status == HAL_OK)
	{
		++xdpCount.hal_ok_count;
	}

	return status;
}

/*
 * Description: Calling this function will use specific I2C driver, Polling, Interrupt or DMA.
 */
int I2C_QUEUE_Mem_Write_Generic_Method(I2C_GenericQueueDef *i2c)
{
	int status = NO_ERROR;

	switch(i2c->transferType)
	{
		case I2C_POLLING:
			status = I2C_QUEUE_Mem_Write_Generic(i2c);
			if(status == NO_ERROR)
			{
				if(i2c->TxISR)
				{
					i2c->TxISR(i2c); // There is no interrupt so we manually call the callback
				}
			}
			break;
		case I2C_INTERRUPT:
			status = I2C_QUEUE_Mem_Write_Generic_IT(i2c);
			break;
		case I2C_DMA:
			status = I2C_QUEUE_Mem_Write_Generic_DMA(i2c);
			break;
		default:
			status = I2C_QUEUE_Mem_Write_Generic(i2c);
			break;
	}

	return status;
}


// ****************** Polling ****************

/*
 * Description: Receive some data with polling
 *
 */
static int I2C_QUEUE_Master_Receive_Generic(I2C_GenericQueueDef *i2c)
{
	if(HAL_I2C_Master_Receive(i2c->i2c_instance, i2c->deviceAddr, i2c->data, i2c->dataSize, i2c->timeout) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	return 0;
}

/*
 * Description: Transmit some data with polling
 *
 */
static int I2C_QUEUE_Master_Transmit_Generic(I2C_GenericQueueDef *i2c)
{
	if(HAL_I2C_Master_Transmit(i2c->i2c_instance, i2c->deviceAddr, i2c->data, i2c->dataSize, i2c->timeout) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	return 0;
}



/*
 * Description: Read data from memory/register using polling
 *
 */
static int I2C_QUEUE_Mem_Read_Generic(I2C_GenericQueueDef *i2c)
{
	if(HAL_I2C_Mem_Read(i2c->i2c_instance, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->data, i2c->dataSize, i2c->timeout) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}

	return 0;
}

/*
 * Description: Write data to memory/register using polling
 *
 */
static int I2C_QUEUE_Mem_Write_Generic(I2C_GenericQueueDef *i2c)
{
	if(HAL_I2C_Mem_Write(i2c->i2c_instance, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->data, i2c->dataSize, i2c->timeout) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}

	return 0;
}

// ***************** Interrupt ******************

/*
 * Description: Receive some data with interrupt
 *
 */
static int I2C_QUEUE_Master_Receive_Generic_IT(I2C_GenericQueueDef *i2c)
{
	if(HAL_I2C_Master_Receive_IT(i2c->i2c_instance, i2c->deviceAddr, i2c->data, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}
	i2c->pending = true;

	return 0;
}

/*
 * Description: Transmit some data with interrupt
 *
 */
static int I2C_QUEUE_Master_Transmit_Generic_IT(I2C_GenericQueueDef *i2c)
{
	if(HAL_I2C_Master_Transmit_IT(i2c->i2c_instance, i2c->deviceAddr, i2c->data, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}
	i2c->pending = true;

	return 0;
}

/*
 * Description: Read memory/register in interrupt mode. Be sure to assign a callback function.
 */
static int I2C_QUEUE_Mem_Read_Generic_IT(I2C_GenericQueueDef *i2c)
{
	// Read memory
	if(HAL_I2C_Mem_Read_IT(i2c->i2c_instance, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->data, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}
	i2c->pending = true;

	return 0;
}

/*
 * Description: Write memory/register in interrupt mode
 */
static int I2C_QUEUE_Mem_Write_Generic_IT(I2C_GenericQueueDef *i2c)
{
	if(HAL_I2C_Mem_Write_IT(i2c->i2c_instance, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->data, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}
	i2c->pending = true;

	return 0;
}

// ****************** DMA *************************
/*
 * Description: Transmit some data with DMA
 *
 */
static int I2C_QUEUE_Master_Transmit_Generic_DMA(I2C_GenericQueueDef *i2c)
{
	if(HAL_I2C_Master_Transmit_DMA(i2c->i2c_instance, i2c->deviceAddr, i2c->data, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_WRITE;
	}
	i2c->pending = true;

	return 0;
}

/*
 * Description: Receive some data with DMA
 *
 */
static int I2C_QUEUE_Master_Receive_Generic_DMA(I2C_GenericQueueDef *i2c)
{
	if(HAL_I2C_Master_Receive_DMA(i2c->i2c_instance, i2c->deviceAddr, i2c->data, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}
	i2c->pending = true;

	return 0;
}

/*
 * Description: Read memory/register in DMA mode. Be sure to assign a callback function.
 */
static int I2C_QUEUE_Mem_Read_Generic_DMA(I2C_GenericQueueDef *i2c)
{
	// Read memory
	if(HAL_I2C_Mem_Read_DMA(i2c->i2c_instance, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->data, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}
	i2c->pending = true;

	return 0;
}

/*
 * Description: Write memory/register in DMA mode
 */
static int I2C_QUEUE_Mem_Write_Generic_DMA(I2C_GenericQueueDef *i2c)
{
	if(HAL_I2C_Mem_Write_DMA(i2c->i2c_instance, i2c->deviceAddr, i2c->registerAddr[0], i2c->regSize, i2c->data, i2c->dataSize) != HAL_OK)
	{
		return I2C_ERROR_READ;
	}
	i2c->pending = true;

	return 0;
}





/*
 * Example initializing code

 1. create new queue and assign created i2c_txQueue buffer to queueData pointer.

#define I2C_QUEUE_SIZE 8
I2C_GenericDef i2c_txQueue[I2C_QUEUE_SIZE] = {0};
I2C_QueueStruct_t i2c_queue =
{
	.queueData = i2c_txQueue,
	.queueSize = I2C_QUEUE_SIZE
};


2. Using I2C_GenericHandler_STM32.c/h driver, create I2C data structure for I2C device

uint8_t xdp_data[8] = {0};
I2C_GenericDef xdp_i2c =
{
	.i2c_instance = &hi2c2,
	.deviceAddr = 0x62 << 1,
	.regSize = 1,
	.dataPtr = xdp_data,
	.transferType = I2C_INTERRUPT,
	.RxISR = NULL,
	.timeout = 100,
};


3. Example of adding I2C data to queue.
   Modify xdp_i2c member data when needed, then add to queue

xdp_i2c.registerAddr[0] = READ_VOUT_paged; // the register we want to read data from
xdp_i2c.RxISR = XDPE192C3D_Voltage_1_Callback; // assign a callback function to call when finished reading data from device.
xdp_i2c.readWriteType = MEM_READ; // this is a memory(register) read.
I2C_Queue_Add(&i2c_queue, &xdp_i2c);


4. Whenever the I2C finishes reading data from device, this callback is called.
   The RxISR is call if was assigned a function. This custom function can use the data in the buffer.
   Call I2C_QueueTransmit to see if more I2C messages are in queue.

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == xdp_i2c.i2c_instance)
	{
		if(xdp_i2c.RxISR)
		{
			xdp_i2c.RxISR(&xdp_i2c); // passes it's own data structure to function. The function can then extract the data it read.
		}

		I2C_QueueTransmit(&i2c_queue); // transmit if more messages
	}
}

5. When writing to I2C device is complete, this callback is called.
   The RxISR is called if assigned a function. More than likely this won't be used,
   but available if needed to do something after writing to device.
   Call I2C_QueueTransmit to see if more I2C messages are in queue.

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == xdp_i2c.i2c_instance)
	{
		if(xdp_i2c.TxISR)
		{
			xdp_i2c.TxISR(&xdp_i2c); // passes it's own data structure to function.
		}

		I2C_QueueTransmit(&i2c_queue); // transmit if more messages
	}
}

 */
