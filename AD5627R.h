/*
 * AD5627R.h
 *
 *  Created on: Jul 1, 2021
 *      Author: karl.yamashita
 */

#ifndef INC_AD5627R_H_
#define INC_AD5627R_H_

#define AD5627R_SLAVE_ADDRESS 0x0E

#define AD_RESOLUTION 0.0012210012210012

// CommandDefines
enum {
	WRITE_TO_INPUT_REG,
	UPDATE_DAC_REG,
	WRITE_TO_INPUT_REG_UPDATE_ALL,
	WRITE_TO_AND_UPDATE_DAC_CHANNEL,
	POWERUP_POWERDOWN,
	RESET,
	LDAC_REG_SETUP,
	INTERNAL_REF_SETUP
};

// DAC_AddrCommands
enum {
	DAC_A,
	DAC_B,
	DAC_BOTH = 7
};

// PowerDownModes
enum {
	NORMAL_OPERATION,
	PULLDOWN_1K,
	PULLDOWN_100K,
	THREE_STATE
};

/*
 * Description: The input shift register is 24 bits wide.
 *
 *
 */
typedef union{
	struct {
		uint8_t data[3];
	}Bytes;
	struct {
		unsigned dacAddr :3; // see DAC Addr commands enum
		unsigned command :3; // see command defines enum
		unsigned byteSelection :1; // 1 = initial command, 0 = command byte each block
		unsigned :1; // should be zero
		struct{
			uint8_t dacLowByte;
			uint8_t dacHighByte;
		}bytes;
	}InputShift_Struct;
	struct{
		unsigned :3;
		unsigned command:3; // see command defines enum
		unsigned :1;
		unsigned :1;

		unsigned :8;
		unsigned dac_A:1;
		unsigned dac_B:1;
		unsigned :6;
	}LDAC_Setup_Struct;
	struct{
		unsigned :3;
		unsigned command:3; // see command defines enum
		unsigned :1;
		unsigned :1;

		unsigned :8;
		unsigned resetBit:1;
		unsigned :7;
	}SW_ResetMode_Struct;
	struct{
		unsigned :3;
		unsigned command:3; // see command defines enum
		unsigned :1;
		unsigned :1;

		unsigned :2;
		unsigned :8;
		unsigned dac_A:1;
		unsigned dac_B:1;
		unsigned :2;
		unsigned powerDownMode:2; // see powerDown modes enum
	}PowerDownCommand_Struct;
	struct{
		unsigned :3;
		unsigned command:3; // see command defines enum
		unsigned :1;
		unsigned :1;

		unsigned :8;
		unsigned refMode:1;
		unsigned :7;
	}ReferenceSetup_Struct;
}AD5627R_Data_Struct;

// prototypes
int AD5627R_Init(void);
int AD5627R_ClearRef(void);
int AD5627R_SetDAC_Value(uint8_t command, float voltage);

int AD5627R_Write(uint8_t *data_IN, uint8_t byteCount);
int AD5627R_Read(uint8_t *data_OUT, uint8_t byteCount);

void GPIO_Pin_CLR_Set(bool status);
void GPIO_Pin_LDAC_Set(bool status);

#endif /* INC_AD5627R_H_ */
