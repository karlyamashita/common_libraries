#include "LSP.h"
#ifdef USE_MPU_9250
#ifndef MPU_9250_H
#define MPU_9250_H

#include "stdint.h"


#define MPU_SLAVE_ADDRESS 0xD0

// read/write
// The value in this register indicates the self test output generated
// during manufacturing tests. This value is to be used to check
// against subsequent self test outputs performed by the end user.
#define SELF_TEST_X_GYRO_ADDR 0x00 // 
#define SELF_TEST_Y_GYRO_ADDR 0x01
#define SELF_TEST_Z_GYRO_ADDR 0x02
#define SELF_TEST_X_ACCEL_ADDR 0x0D
#define SELF_TEST_Y_ACCEL_ADDR 0x0E
#define SELF_TEST_Z_ACCEL_ADDR 0x0F

#define XG_OFFSET_H_ADDR 0x13
#define XG_OFFSET_L_ADDR 0x14
#define YG_OFFSET_H_ADDR 0x15
#define YG_OFFSET_L_ADDR 0x16
#define ZG_OFFSET_H_ADDR 0x17
#define ZG_OFFSET_L_ADDR 0x18
#define SMPLRT_DIV_ADDR 0x19

#define CONFIG_ADDR 0x1A
#define GYRO_CONFIG_ADDR 0x1B
#define ACCEL_CONFIG_ADDR 0x1C
#define ACCEL_CONFIG_2_ADDR 0x1D
#define LP_ACCEL_ODR_ADDR 0x1E
#define WOM_THR_ADDR 0x1F
#define FIFO_EN_ADDR 0x23
#define I2C_MST_CTRL_ADDR 0x24
#define I2C_SLV0_ADDR 0x25
#define I2C_SLV0_REG_ADDR 0x26
#define I2C_SLV0_CTRL_ADDR 0x27

#define I2C_SLV1_ADDR_ADDR 0x28
#define I2C_SLV1_REG_ADDR 0x29
#define I2C_SLV1_CTRL_ADDR 0x2A

#define I2C_SLV2_ADDR_ADDR 0x2B
#define I2C_SLV2_REG_ADDR 0x2C
#define I2C_SLV2_CTRL_ADDR 0x2D

#define I2C_SLV3_ADDR_ADDR 0x2E
#define I2C_SLV3_REG_ADDR 0x2F
#define I2C_SLV3_CTRL_ADDR 0x30

#define I2C_SLV4_ADDR_ADDR 0x31
#define I2C_SLV4_REG_ADDR 0x32
#define I2C_SLV4_DO_ADDR 0x33
#define I2C_SLV4_CTRL_ADDR 0x34

#define I2C_SLV4_DI_ADDR 0x35
#define I2C_MST_STATUS_ADDR 0x36
#define INT_PIN_CFG_ADDR 0x37
#define INT_ENABLE_ADDR 0x38
#define INT_STATUS_ADDR 0x3A

// read only
// accel
#define ACCEL_XOUT_H_ADDR 0x3B
#define ACCEL_XOUT_L_ADDR 0x3C

#define ACCEL_YOUT_H_ADDR 0x3D
#define ACCEL_YOUT_L_ADDR 0x3E

#define ACCEL_ZOUT_H_ADDR 0x3F
#define ACCEL_ZOUT_L_ADDR 0x40

// temp
#define TEMP_OUT_H_ADDR 0x41
#define TEMP_OUT_L_ADDR 0x42

//gyro
#define GRYO_XOUT_H_ADDR 0x43
#define GRYO_XOUT_L_ADDR 0x44

#define GRYO_YOUT_H_ADDR 0x45
#define GRYO_YOUT_L_ADDR 0x46

#define GRYO_ZOUT_H_ADDR 0x47
#define GRYO_ZOUT_L_ADDR 0x48

// ext sen data, read only
#define EXT_SENS_DATA_00 0x49
//    more registers in between
#define EXT_SENS_DATA_23 0x60

// read/write
#define I2C_SLV0_DO_ADDR 0x63
#define I2C_SLV1_DO_ADDR 0x64
#define I2C_SLV2_DO_ADDR 0x65
#define I2C_SLV3_DO_ADDR 0x66
#define I2C_MST_DELAY_CTRL_ADDR 0x67
#define SIGNAL_PATH_RESET_ADDR 0x68

#define MOT_DETECT_CTRL_ADDR 0x69
#define USER_CTRL_ADDR 0x6A
#define PWR_MGMT_1_ADDR 0x6B // reset is 0x01
#define PWR_MGMT_2_ADDR 0x6C
#define FIFO_COUNTH_ADDR 0x72
#define FIFO_COUNTL_ADDR 0x73
#define FIFO_R_W_ADDR 0x74
#define WHO_AM_I_ADDR 0x75 // reset is 0x71, read only

#define XA_OFFSET_H_ADDR 0x77
#define XA_OFFSET_L_ADDR 0x78
#define YA_OFFSET_H_ADDR 0x7A
#define YA_OFFSET_L_ADDR 0x7B
#define ZA_OFFSET_H_ADDR 0x7D
#define ZA_OFFSET_L_ADDR 0x7E

#define FS_SEL 1
#define OFF_SET_LSB 0
#define OFF_SET_DPS 1

// custom defines
#define X_REG 0
#define Y_REG 1
#define Z_REG 2
#define ACCEL_DATA_RDY 0
#define GRYO_DATA_RDY 1
#define MAG_DATA_RDY 2

// Set initial input parameters
#define  AFS_2G  0
#define  AFS_4G  1
#define  AFS_8G  2
#define  AFS_16G 3

#define  GFS_250DPS  0
#define  GFS_500DPS  1
#define  GFS_1000DPS 2
#define  GFS_2000DPS 3

#define  MFS_14BITS  0 // 0.6 mG per LSB
#define  MFS_16BITS  1    // 0.15 mG per LSB

#define M_8Hz   0x02
#define M_100Hz 0x06


typedef union{
    struct{
        int16_t value;
    }Value;
    struct{
        int8_t lowByte;
        int8_t highByte;
    }Bytes;
}GyroOffsUsr;

typedef union{
    struct{
        int8_t value;
    }Data;
    struct{
    	unsigned DlpfCfg:3;
    	unsigned extSyncSet:3; // Enables the FSYNC pin data to be sampled.
    	unsigned fifoMode:1;// when set to 1, when the fifo is full, additional writes will not be written to fifo. If set to 0,
    	// when the fifo is full, additional writes will be written to the fifo, replacing the oldest data
    	unsigned :1;
    }Status;
}Config_MPU;

typedef union{
    struct{
        uint8_t value;
    }Data;
    struct{
    	unsigned fChoiceB:2;
    	unsigned :1;
    	unsigned gyroFsSel:2;
    	unsigned zGyroCten:1;
    	unsigned yGyroCten:1;
    	unsigned xGyroCten:1;
    }Status;
}GyroConfig;

typedef union{
    struct{
        uint8_t value;
    }Data;
    struct
    {
    	unsigned :3;
    	unsigned accelFsSel:2; // accel full scale select
    	unsigned azStEn:1;
    	unsigned ayStEn:1;
    	unsigned axStEn:1; // x Accel self-test
    }Status;
}AccelConfig;

typedef union{
    struct{
        uint8_t value;
    }Data;
    struct
    {
    	unsigned a_dlpfCfg:2; // Accel full scale select
        unsigned accel_fChoiceB:1;
        unsigned :4;
    }Status;
}AccelConfig2;

typedef union {
	struct {
		uint8_t value;
	}Data;
	struct {
		unsigned posc_clksel:4;
		unsigned :4;
	}Status;
}LowPowerAccelerometerODRControl;

typedef union {
	struct {
		uint8_t value;
	}Data;
	struct {
		unsigned slv_0:1;
		unsigned slv_1:1;
		unsigned slv_2:1;
		unsigned accel:1;
		unsigned gyro_zout:1;
		unsigned gyro_yout:1;
		unsigned gyro_xout:1;
		unsigned temp_out:1;
	}Status;
}FIFO_Enable;

typedef union {
	struct {
		uint8_t value;
	}Data;
	struct {
		unsigned i2cMstClk:4;
		unsigned i2cMst_P_NsrR:1;
		unsigned slv3FifoEn:1;
		unsigned waitForEs:1;
		unsigned multMstEn:1;
	}Status;
}I2c_MasterControl;

typedef union {
	struct {
		uint8_t value;
	}Data;
	struct {
		unsigned rawRdyEn:1;
		unsigned :2;
		unsigned fsyncIntEn:1;
		unsigned fifoOverflowEn:1;
		unsigned :1;
		unsigned womEn:1;
		unsigned :1;
	}Status;
}InterruptEnable;

typedef union {
	struct {
		uint8_t value;
	}Data;
	struct {
		unsigned rawDataRdyInt:1;
		unsigned :2;
		unsigned fsyncInt:1;
		unsigned fifoOverflowInt:1;
		unsigned :1;
		unsigned womInt:1;
		unsigned :1;
	}Status;
}InterruptStatus;

typedef union {
	struct {
		uint16_t value;
	}Data;
	struct {
		uint8_t lowByte;
		uint8_t highByte;
	}Status;
}GyroOut;


//prototypes
int32_t OffSetGyroGet(GyroOffsUsr gyroValue, int8_t type);
void Config_MPUSend(Config_MPU *configMPU) ;
void GyroConfigSend(GyroConfig *gyroConfig);
void AccelConfigSend(AccelConfig *accelConfig);
void AccelConfig2Send(AccelConfig2 *accelConfig2);
void FIFO_EnableSend(FIFO_Enable *fifoEnable);
void I2C_MasterControlSend(I2c_MasterControl *i2cMasterControl);
void InterruptEnableSend(InterruptEnable *interruptEnable);
int GyroOutGet(uint8_t regAddr, uint8_t size);
int AccelOutGet(uint8_t regAddr, uint8_t size);
int MPU9250Init(void);

// these were ported over from a C++ project from https://github.com/kriswiner/MPU9250/blob/master/Dual_MPU9250/MPU9250.cpp
int InitMPU9250(uint8_t devAddr, uint8_t Ascale, uint8_t Gscale, uint8_t sampleRate);
int CalibrateMPU9250(uint8_t devAddr, float * dest1, float * dest2);
float GetGres(uint8_t Gscale);
float GetAres(uint8_t Ascale);

// place these 2 function calls where needed
int MPU_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
int MPU_ReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint8_t size);


#endif // MPU_9250_H
#endif // USE_MPU_9250
