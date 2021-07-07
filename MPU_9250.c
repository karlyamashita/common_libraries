#include "LSP.h"
#ifdef USE_MPU_9250
#include "main.h"
#include "MPU_9250.h"
#include "math.h"



//mpu9250
uint8_t Gscale = GFS_250DPS, Ascale = AFS_2G, Mscale = MFS_16BITS, Mmode = M_100Hz, sampleRate = 0x04;
float aRes, gRes, mRes;      // scale resolutions per LSB for the sensors


uint8_t data[8];

// use extern to access these values
uint16_t xAccelVal, yAccelVal, zAccelVal;
uint16_t xGyroVal, yGyroVal, zGyroVal;

int32_t OffSetGyroGet(GyroOffsUsr gyroValue, int8_t type)
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

void SendConfig_MPU(Config_MPU *configMPU)
{

}

void GyroConfigSend(GyroConfig *gyroConfig)
{

}

void AccelConfigSend(AccelConfig *accelConfig)
{

}

void AccelConfig2Send(AccelConfig2 *accelConfig2)
{

}

void LowPowerAccelerometerODRControlSend(LowPowerAccelerometerODRControl *lowPowerAccelerometerODRControl)
{

}

void FIFO_EnableSend(FIFO_Enable *fifoEnable)
{

}

void I2C_MasterControlSend(I2c_MasterControl *i2cMasterControl)
{

}

void InterruptEnableSend(InterruptEnable *interruptEnable)
{

}

void InterruptStatusSend(InterruptStatus *interruptStatus)
{

}

float GetGres(uint8_t Gscale) {
	float _gRes;
	switch (Gscale)
	{
	// Possible gyro scales (and their register bit settings) are:
	// 250 DPS (00), 500 DPS (01), 1000 DPS (10), and 2000 DPS  (11).
	case GFS_250DPS:
		_gRes = 250.0/32768.0;
		break;
	case GFS_500DPS:
		_gRes = 500.0/32768.0;
		break;
	case GFS_1000DPS:
		_gRes = 1000.0/32768.0;
		break;
	case GFS_2000DPS:
		_gRes = 2000.0/32768.0;
		break;
	}
	return _gRes;
}

float GetAres(uint8_t Ascale) {
	float _aRes;
	switch (Ascale)
	{
	// Possible accelerometer scales (and their register bit settings) are:
	// 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11).
	// Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
	case AFS_2G:
		_aRes = 2.0f/32768.0f;
		break;
	case AFS_4G:
		_aRes = 4.0f/32768.0f;
		break;
	case AFS_8G:
		_aRes = 8.0f/32768.0f;
		break;
	case AFS_16G:
		_aRes = 16.0f/32768.0f;
		break;
	}
	return _aRes;
}

int MPU9250Init(void) {
	int errorCode = 0;
	aRes = GetAres(Ascale);
	gRes = GetGres(Gscale);

	errorCode = InitMPU9250((uint8_t)MPU9250_SLAVE_ADDRESS, Ascale, Gscale, sampleRate);

	return errorCode;
}

int InitMPU9250(uint8_t devAddr, uint8_t Ascale, uint8_t Gscale, uint8_t sampleRate)
{
	int errorCode = 0;
	uint8_t c;

	// wake up device
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, PWR_MGMT_1_ADDR, 0); // Clear sleep mode bit (6), enable all sensors
	if(errorCode != 0){
		return errorCode;
	}

	HAL_Delay(100); // Wait for all registers to reset

	// get stable time source
	errorCode = MPU_WriteByte((uint8_t)MPU9250_SLAVE_ADDRESS, PWR_MGMT_1_ADDR, 1); // Auto select clock source to be PLL gyroscope reference if ready else
	if(errorCode != 0){
		return errorCode;
	}

	HAL_Delay(200); // Wait for all registers to reset

	// Configure Gyro and Thermometer
	// Disable FSYNC and set thermometer and gyro bandwidth to 41 and 42 Hz, respectively;
	// minimum delay time for this setting is 5.9 ms, which means sensor fusion update rates cannot
	// be higher than 1 / 0.0059 = 170 Hz
	// DLPF_CFG = bits 2:0 = 011; this limits the sample rate to 1000 Hz for both
	// With the MPU9250, it is possible to get gyro sample rates of 32 kHz (!), 8 kHz, or 1 kHz
	errorCode = MPU_WriteByte((uint8_t)MPU9250_SLAVE_ADDRESS, CONFIG_ADDR, 3);
	if(errorCode != 0){
		return errorCode;
	}
	// Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
	// Use a 200 Hz rate; a rate consistent with the filter update rate, determined inset in CONFIG above
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, SMPLRT_DIV_ADDR, sampleRate);
	if(errorCode != 0){
		return errorCode;
	}
	// Set gyroscope full scale range
	// Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3

	if(errorCode != 0){
		return errorCode;
	}
	c = data[0];
	// c = c & ~0xE0; // Clear self-test bits [7:5]
	c = c & ~0x02; // Clear Fchoice bits [1:0]
	c = c & ~0x18; // Clear AFS bits [4:3]
	c = c | Gscale << 3; // Set full scale range for the gyro
	// c =| 0x00; // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of GYRO_CONFIG
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, GYRO_CONFIG_ADDR, c); // Write new GYRO_CONFIG value to register
	if(errorCode != 0){
		return errorCode;
	}
	// Set accelerometer full-scale range configuration
	errorCode = MPU_ReadBytes(MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG_ADDR, data, 1); // get current ACCEL_CONFIG register value
	if(errorCode != 0){
		return errorCode;
	}
	c = data[0];
	// c = c & ~0xE0; // Clear self-test bits [7:5]
	c = c & ~0x18;  // Clear AFS bits [4:3]
	c = c | Ascale << 3; // Set full scale range for the accelerometer
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG_ADDR, c); // Write new ACCEL_CONFIG register value
	if(errorCode != 0){
		return errorCode;
	}
	// Set accelerometer sample rate configuration
	// It is possible to get a 4 kHz sample rate from the accelerometer by choosing 1 for
	// accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 kHz
	errorCode = MPU_ReadBytes(MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG_2_ADDR, data, 1); // get current ACCEL_CONFIG2 register value
	if(errorCode != 0){
		return errorCode;
	}
	c = data[0];
	c = c & ~0x0F; // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
	c = c | 0x03;  // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG_2_ADDR, c); // Write new ACCEL_CONFIG2 register value
	if(errorCode != 0){
		return errorCode;
	}
	// The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
	// but all these rates are further reduced by a factor of 5 to 200 Hz because of the SMPLRT_DIV setting

	return errorCode;
}

int CalibrateMPU9250(uint8_t devAddr, float * dest1, float * dest2)
{
	int errorCode = 0;
	uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
	uint16_t ii, packet_count, fifo_count;
	int32_t gyro_bias[3]  = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

	// reset device
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, PWR_MGMT_1_ADDR, 0x80); // Write a one to bit 7 reset bit; toggle reset device
	if(errorCode != 0){
		return errorCode;
	}
	HAL_Delay(100);

	// get stable time source; Auto select clock source to be PLL gyroscope reference if ready
	// else use the internal oscillator, bits 2:0 = 001
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, PWR_MGMT_1_ADDR, 0x01);
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, PWR_MGMT_2_ADDR, 0x00);
	if(errorCode != 0){
		return errorCode;
	}
	HAL_Delay(200);

	// Configure device for bias calculation
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, INT_ENABLE_ADDR, 0x00);   // Disable all interrupts
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, FIFO_EN_ADDR, 0x00);      // Disable FIFO
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, PWR_MGMT_1_ADDR, 0x00);   // Turn on internal clock source
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, I2C_MST_CTRL_ADDR, 0x00); // Disable I2C master
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, USER_CTRL_ADDR, 0x00);    // Disable FIFO and I2C master modes
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, USER_CTRL_ADDR, 0x0C);    // Reset FIFO and DMP

	HAL_Delay(100);

	// Configure MPU6050 gyro and accelerometer for bias calculation
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, CONFIG_ADDR, 0x01);      // Set low-pass filter to 188 Hz
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, SMPLRT_DIV_ADDR, 0x00);  // Set sample rate to 1 kHz
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, GYRO_CONFIG_ADDR, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG_ADDR, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity

	uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
	uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g

	// Configure FIFO to capture accelerometer and gyro data for bias calculation
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, USER_CTRL_ADDR, 0x40);   // Enable FIFO
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, FIFO_EN_ADDR, 0x78);     // Enable gyro and accelerometer sensors for FIFO  (max size 512 bytes in MPU-9150)
	if(errorCode != 0){
		return errorCode;
	}
	HAL_Delay(100); // accumulate 40 samples in 40 milliseconds = 480 bytes

	// At end of sample accumulation, turn off FIFO sensor read
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, FIFO_EN_ADDR, 0x00);        // Disable gyro and accelerometer sensors for FIFO
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_ReadBytes(MPU9250_SLAVE_ADDRESS, FIFO_COUNTH_ADDR, &data[0], 2); // read FIFO sample count
	if(errorCode != 0){
		return errorCode;
	}
	fifo_count = ((uint16_t)data[0] << 8) | data[1];
	packet_count = fifo_count/12;// How many sets of full gyro and accelerometer data for averaging

	for (ii = 0; ii < packet_count; ii++) {
		int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
		errorCode = MPU_ReadBytes(MPU9250_SLAVE_ADDRESS, FIFO_R_W_ADDR, &data[0], 12); // read data for averaging
		if(errorCode != 0){
			return errorCode;
		}
		accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  // Form signed 16-bit integer for each sample in FIFO
		accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
		accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;
		gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
		gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
		gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;

		accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
		accel_bias[1] += (int32_t) accel_temp[1];
		accel_bias[2] += (int32_t) accel_temp[2];
		gyro_bias[0]  += (int32_t) gyro_temp[0];
		gyro_bias[1]  += (int32_t) gyro_temp[1];
		gyro_bias[2]  += (int32_t) gyro_temp[2];
	}
	accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
	accel_bias[1] /= (int32_t) packet_count;
	accel_bias[2] /= (int32_t) packet_count;
	gyro_bias[0]  /= (int32_t) packet_count;
	gyro_bias[1]  /= (int32_t) packet_count;
	gyro_bias[2]  /= (int32_t) packet_count;

	if(accel_bias[2] > 0L) {accel_bias[2] -= (int32_t) accelsensitivity;}  // Remove gravity from the z-axis accelerometer bias calculation
	else {accel_bias[2] += (int32_t) accelsensitivity;}

	// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
	data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
	data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
	data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
	data[3] = (-gyro_bias[1]/4)       & 0xFF;
	data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
	data[5] = (-gyro_bias[2]/4)       & 0xFF;

	// Push gyro biases to hardware registers
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, XG_OFFSET_H_ADDR, data[0]);
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, XG_OFFSET_L_ADDR, data[1]);
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, YG_OFFSET_H_ADDR, data[2]);
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, YG_OFFSET_L_ADDR, data[3]);
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, ZG_OFFSET_H_ADDR, data[4]);
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_WriteByte(MPU9250_SLAVE_ADDRESS, ZG_OFFSET_L_ADDR, data[5]);
	if(errorCode != 0){
		return errorCode;
	}

	// Output scaled gyro biases for display in the main program
	dest1[0] = (float) gyro_bias[0]/(float) gyrosensitivity;
	dest1[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
	dest1[2] = (float) gyro_bias[2]/(float) gyrosensitivity;

	// Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
	// factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
	// non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
	// compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
	// the accelerometer biases calculated above must be divided by 8.

	int32_t accel_bias_reg[3] = {0, 0, 0}; // A place to hold the factory accelerometer trim biases
	errorCode = MPU_ReadBytes(MPU9250_SLAVE_ADDRESS, XA_OFFSET_H_ADDR, &data[0], 2); // Read factory accelerometer trim values
	if(errorCode != 0){
		return errorCode;
	}
	accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
	errorCode = MPU_ReadBytes(MPU9250_SLAVE_ADDRESS, YA_OFFSET_H_ADDR, &data[0], 2);
	if(errorCode != 0){
		return errorCode;
	}
	accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
	if(errorCode != 0){
		return errorCode;
	}
	errorCode = MPU_ReadBytes(MPU9250_SLAVE_ADDRESS, ZA_OFFSET_H_ADDR, &data[0], 2);
	if(errorCode != 0){
		return errorCode;
	}
	accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);

	uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
	uint8_t mask_bit[3] = {0, 0, 0}; // Define array to hold mask bit for each accelerometer bias axis

	for(ii = 0; ii < 3; ii++) {
		if((accel_bias_reg[ii] & mask)) mask_bit[ii] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
	}

	// Construct total accelerometer bias, including calculated average accelerometer bias from above
	accel_bias_reg[0] -= (accel_bias[0]/8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
	accel_bias_reg[1] -= (accel_bias[1]/8);
	accel_bias_reg[2] -= (accel_bias[2]/8);

	data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
	data[1] = (accel_bias_reg[0])      & 0xFF;
	data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
	data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
	data[3] = (accel_bias_reg[1])      & 0xFF;
	data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
	data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
	data[5] = (accel_bias_reg[2])      & 0xFF;
	data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

	// Output scaled accelerometer biases for display in the main program
	dest2[0] = (float)accel_bias[0]/(float)accelsensitivity;
	dest2[1] = (float)accel_bias[1]/(float)accelsensitivity;
	dest2[2] = (float)accel_bias[2]/(float)accelsensitivity;

	return errorCode;
}

/*
 *
 *
 */
int GyroOutGet(uint8_t regAddr, uint8_t size) {
	int status = 0;
	data[0] = 0;
	status = MPU_ReadBytes(MPU9250_SLAVE_ADDRESS, regAddr, data, size);
	if(status != 0) return status;

	xAccelVal = (data[0] << 8 | data[1]);
	yAccelVal = (data[2] << 8 | data[3]);
	zAccelVal = (data[4] << 8 | data[5]);
	return 0;
}

int AccelOutGet(uint8_t regAddr, uint8_t size)
{
	int status = 0;
	data[1] = 0;
	status = MPU_ReadBytes(MPU9250_SLAVE_ADDRESS, regAddr, data, size);
	if(status != 0) return status;

	xGyroVal = (data[0] << 8 | data[1]);
	yGyroVal = (data[2] << 8 | data[3]);
	zGyroVal = (data[4] << 8 | data[4]);
	return 0;
}

/*
 * These two functions were designed for an STM32. For other uC,
 * use the appropriate I2C function to write and read bytes.
 *
 */

/*
 *
 *
 *
 * Note: I2C_Write_Data() is a generic function that you should create specific for your Micro.
 *
 */
int MPU_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
	HAL_StatusTypeDef status;
	uint8_t dataBuf[2];
	int errorCode = 0;

	dataBuf[0] = regAddr;
	dataBuf[1] = data;

	errorCode = I2C_Write_Data(I2C_PORT_0, devAddr, dataBuf, 2);
	if(errorCode == XST_SUCCESS){
		status = HAL_OK;
	}
	else if(errorCode == XST_IIC_BUS_BUSY){
		status = HAL_BUSY;
	}

	if(status != HAL_OK){
		return -1;
	}
	return 0;
}


/*
 * Description:
 *
 *
 *
 * Note: I2C_Write_Data() is a generic function that you should create specific for your Micro.
 *
 */
int MPU_ReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *data_Out, uint8_t size) {
	HAL_StatusTypeDef status;
	int errorCode = 0;

	errorCode = I2C_Write_Data(I2C_PORT_0, devAddr, &regAddr, 2);
	if(errorCode == XST_SUCCESS){
		status = HAL_OK;
	}
	else if(errorCode == XST_IIC_BUS_BUSY){
		status = HAL_BUSY;
	}
	if(status != HAL_OK) {
		return -1;
	}

	errorCode = I2C_Read_Data(I2C_PORT_0, devAddr, regAddr, data_Out, size);
	if(errorCode == XST_SUCCESS){
		status = HAL_OK;
	}
	else if(errorCode == XST_IIC_BUS_BUSY){
		status = HAL_BUSY;
	}

	if(status != HAL_OK) {
		return -1;
	}
	return 0;
}


#endif // USE_MPU_9250
