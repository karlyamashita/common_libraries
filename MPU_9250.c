#include "LSP.h"
#ifdef USE_MPU_9250
#include "main.h"
#include "MPU_9250.h"

#include "stm32f7xx_hal_i2c.h"

/*
     (#) No-Blocking mode functions with Interrupt are :
        (++) HAL_I2C_Master_Transmit_IT()
        (++) HAL_I2C_Master_Receive_IT()
        (++) HAL_I2C_Mem_Write_IT()
        (++) HAL_I2C_Mem_Read_IT()
        (++) HAL_I2C_Master_Seq_Transmit_IT()
        (++) HAL_I2C_Master_Seq_Receive_IT()

// transfer options
#define I2C_FIRST_FRAME                 ((uint32_t)I2C_SOFTEND_MODE)
#define I2C_FIRST_AND_NEXT_FRAME        ((uint32_t)(I2C_RELOAD_MODE | I2C_SOFTEND_MODE))
#define I2C_NEXT_FRAME                  ((uint32_t)(I2C_RELOAD_MODE | I2C_SOFTEND_MODE))
#define I2C_FIRST_AND_LAST_FRAME        ((uint32_t)I2C_AUTOEND_MODE)
#define I2C_LAST_FRAME                  ((uint32_t)I2C_AUTOEND_MODE)
#define I2C_LAST_FRAME_NO_STOP          ((uint32_t)I2C_SOFTEND_MODE)

 */


int32_t GetOffSetGyro(GyroOffsUsr gyroValue, int8_t type)
{
    int32_t gyroSensitivity = 0;
    int32_t offSetLsb = 0;
    int32_t offSetDps = 0;
    int32_t thePowerOf = pow(2,FS_SEL);

    offSetLsb = ((gyroValue.Value.value * 4) / thePowerOf);

    gyroSensitivity = thePowerOf * offSetLsb / 500;

    offSetDps = (gyroValue.Value.value * 4) / thePowerOf / gyroSensitivity;

    if (type == OFF_SET_LSB) {
        return offSetLsb;
    }
    return offSetDps;
}


void SendConfig_MPU(I2C_HandleTypeDef *hi2c, Config_MPU config_MPU)
{
	uint8_t pData = config_MPU.Data.value;
	HAL_I2C_Master_Seq_Transmit_IT(hi2c, MPU_SLAVE_ADDRESS, &pData, 1, I2C_FIRST_AND_LAST_FRAME);
}

void SendGyroConfig(I2C_HandleTypeDef *hi2c, GyroConfig gyroConfig)
{
	uint8_t pData = gyroConfig.Data.value;
	HAL_I2C_Master_Seq_Transmit_IT(hi2c, MPU_SLAVE_ADDRESS, &pData, 1, I2C_FIRST_AND_LAST_FRAME);
}

void SendAccelConfig(I2C_HandleTypeDef *hi2c, AccelConfig accelConfig)
{

}

void SendAccelConfig2(I2C_HandleTypeDef *hi2c, AccelConfig2 accelConfig2)
{

}

void SendLowPowerAccelerometerODRControl(I2C_HandleTypeDef *hi2c, LowPowerAccelerometerODRControl lowPowerAccelerometerODRControl)
{

}

void SendFIFO_Enable(I2C_HandleTypeDef *hi2c, FIFO_Enable fifo_Enable)
{

}

void Sendi2cMasterControl(I2C_HandleTypeDef *hi2c, I2c_MasterControl i2cMasterControl)
{

}

void SendInterruptEnable(I2C_HandleTypeDef *hi2c, InterruptEnable interruptEnable)
{

}

void SendInterruptStatus(I2C_HandleTypeDef *hi2c, InterruptStatus interruptStatus)
{

}



#endif // USE_MPU_9250
