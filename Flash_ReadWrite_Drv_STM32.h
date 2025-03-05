/*
 * Flash_ReadWrite_Drv_STM32.h
 *
 *  Created on: Oct 10, 2024
 *      Author: codek
 */

#ifndef INC_FLASH_READWRITE_DRV_STM32_H_
#define INC_FLASH_READWRITE_DRV_STM32_H_


#define BOOTLOADER_FLAG_ADDRESS 0x8007800
#define FLASH_EEPROM_START_ADDRESS 0x8007000

// SerialPartNumber_t
#define FLASH_DOUBLE_WORD_SIZE 8 // the mount of FLASH_Data_t created in FLASH_DataStruct_t
#define FLASH_STRING_DATA_SIZE 48 // max amount of characters for each FLASH_Data_t
#define FLASH_SIZEOF_DATA_STRUCT (FLASH_DOUBLE_WORD_SIZE * FLASH_STRING_DATA_SIZE)
#define FLASH_WRITE_DOUBLE_WORD_SIZE (FLASH_SIZEOF_DATA_STRUCT / FLASH_DOUBLE_WORD_SIZE)


typedef struct
{
	FLASH_EraseInitTypeDef flashEraseInit;
	uint32_t address;
	uint8_t *data; // increment of 8 bytes
	uint32_t doubleWordAmount;
	uint32_t PageError;
}__attribute__ ((aligned (32))) FlashWriteInit_t;

typedef struct
{
	uint32_t address;
	uint8_t *data; // pointer to array buffer
	uint32_t count; // number of bytes to read
}__attribute__ ((aligned (32))) FlashReadInit_t;


int FLASH_WriteDoubleWord(FlashWriteInit_t *flashInit);

uint8_t FLASH_DATA_Read(uint32_t address);
int FLASH_DATA_ReadMultiBytes(FlashReadInit_t *flash);

#endif /* INC_FLASH_READWRITE_DRV_STM32_H_ */
