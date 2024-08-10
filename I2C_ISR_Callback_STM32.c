/*
 * I2C_ISR_Callback_STM32.c
 *
 *  Created on: Aug 8, 2024
 *      Author: karl.yamashita
 *
 *
 *      User can test for which I2C handle caused interrupt and call the ISR routine
 *
 */



#include "main.h"


// Add extern for each I2C device instance
extern I2C_GenericDef tmp102_data_1;



/*
 * Description: Callback after memory/register has been Read.
 */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == tmp102_data_1.i2c_instance)
	{
		if(tmp102_data_1.RxISR)
		{
			tmp102_data_1.RxISR(&tmp102_data_1);
		}
	}
}

/*
 * Description: Callback after memory/register has been written.
 */
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == tmp102_data_1.i2c_instance)
	{
		if(tmp102_data_1.TxISR)
		{
			tmp102_data_1.TxISR(&tmp102_data_1);
		}
	}
}

/*
 * Description: Callback after Read.
 */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == tmp102_data_1.i2c_instance)
	{
		if(tmp102_data_1.RxISR)
		{
			tmp102_data_1.RxISR(&tmp102_data_1);
		}
	}
}

/*
 * Description: Callback after Write.
 */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == tmp102_data_1.i2c_instance)
	{
		if(tmp102_data_1.TxISR)
		{
			tmp102_data_1.TxISR(&tmp102_data_1);
		}
	}
}



