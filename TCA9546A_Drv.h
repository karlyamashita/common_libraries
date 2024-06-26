/*
 * TCA9546A_Drv.h
 *
 *  Created on: Jun 21, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_TCA9546A_DRV_H_
#define INC_TCA9546A_DRV_H_


#define TCA9546A_ADDRESS_70 (uint8_t)0b1110000    // 0x70, south board
#define TCA9546A_ADDRESS_74 (uint8_t)0b1110100    // 0x74, north board


#define TCA9546A_CHANNEL_1 0x01
#define TCA9546A_CHANNEL_2 0x04 // schematic error, needs to be 0x04
#define TCA9546A_CHANNEL_3 0x02 // schematic error, needs to be 0x02
#define TCA9546A_CHANNEL_4 0x08
#define TCA9546A_CHANNELS_RESET 0


typedef struct __TCA9546A_HandlerTypeDef
{
	char *commandCopy;

	I2C_GenericDef *i2c_Instance;
	void (*RxISR)(struct __TCA9546A_HandlerTypeDef *tca); // callback
} TCA9546A_HandlerTypeDef;

void TCA9546A_Init(TCA9546A_HandlerTypeDef *tca, I2C_GenericDef *i2c, uint32_t transferType);

int TCA9546A_Write(TCA9546A_HandlerTypeDef *tca, void (*callback));
int TCA9546A_Read(TCA9546A_HandlerTypeDef *tca, void (*callback));


#endif /* INC_TCA9546A_DRV_H_ */
