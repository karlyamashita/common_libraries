/*
 * Flash_ReadWrite_Drv_STM32.c
 *
 *  Created on: Oct 10, 2024
 *      Author: codek
 */



#include "main.h"


/*
 * Description:
 */
int FLASH_WriteDoubleWord(FlashWriteInit_t *flashMsg)
{
	int status = 0;
	HAL_StatusTypeDef hal_status;
	uint8_t data[8];
	int i, n = 0;
	uint8_t *dataPtr;

	dataPtr = flashMsg->data;

	HAL_FLASH_Unlock();

	hal_status = HAL_FLASHEx_Erase(&flashMsg->flashEraseInit, &flashMsg->PageError);
	if(hal_status != HAL_OK)
	{
		return FLASH_ERASED_FAILED;
	}

	for(i = 0, n = 0; i < flashMsg->doubleWordAmount; i++, n += FLASH_DOUBLE_WORD_SIZE) // each write is a double word
	{
		memcpy(&data, &dataPtr[n], FLASH_DOUBLE_WORD_SIZE);
		hal_status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashMsg->address, (uint64_t)(*((uint64_t *)((uint32_t)data))));
		if(hal_status != HAL_OK)
		{
			return FLASH_WRITE_FAILED;
		}
		flashMsg->address += FLASH_DOUBLE_WORD_SIZE; // double word
	}

	HAL_FLASH_Lock();

	return status;
}

/*
 * Description: Read 1 byte from Address.
 */
uint8_t FLASH_DATA_Read(uint32_t address)
{
  return (*(uint8_t *)(address));
}

/*
 * Description: Read multiple bytes.
 */
int FLASH_DATA_ReadMultiBytes(FlashReadInit_t *flash)
{
	int i = 0;

	for(i = 0; i < flash->count; i++)
	{
		flash->data[i] = (*(uint8_t *)(flash->address));
		flash->address += 1;
	}

	return NO_ERROR;
}

