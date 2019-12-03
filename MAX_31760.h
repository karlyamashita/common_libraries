/*
 * MAX_31760.h
 *
 *  Created on: Nov 28, 2019
 *      Author: Karl
 */
#include "LSP.h"
#ifdef USE_MAX_31760
#ifndef MAX_31760_H_
#define MAX_31760_H_

#define MAX31760_SLAVE_ADDRESS_WRITE 0xA0
#define MAX31760_SLAVE_ADDRESS_READ 0xA1

#define REG_CR1 0x00 // Control Register 1
#define REG_CR2 0x01 // Control Register 2
#define REG_CR3 0x02 // Control Register 3
#define REG_FFDC 0x03 // Fan Fault Duty Cycle
#define REG_MASK 0x04 // Alert Mask Register
#define REG_IFR 0x05 // Ideality Factor Register
#define REG_RHSH 0x06 // Remote High Set-Point Register, MSB
#define REG_RHSL 0x07 // Remote High Set-Point Register, LSB
#define REG_LOTSH 0x08 // Local Overtemperature Set-Point Register, MSB
#define REG_LOTSL 0x09 // Local Overtemperature Set-Point Register, LSB
#define REB_ROTSH 0x0A // Remote Overtemperature Set-Point Register, MSB
#define REG_ROTSL 0x0B // Remote Overtemperature Set-Point Register, LSB
#define REG_LHSH 0x0C // Local High Set-Point Register, MSB
#define REG_LHSL 0x0D // Local High Set-Point Register, LSB
#define REG_TCTH 0x0E // TACH Count Threshold Register, MSB
#define REG_TCTL 0x0F // TACH Count Threshold Register, LSB
#define REG_USER 0x10 // General-Purpose User EEPROM 10h–17h
#define REG_LUT 0x20 //  48-Byte Lookup Table 20h–4Fh
#define REG_PWMR 0x50 // Direct Duty-Cycle Control Register
#define REG_PWMV 0x51 // Current PWM Duty-Cycle Register
#define REG_TC1H 0x52 // TACH1 Count Register, MSB
#define REG_TC1L 0x53 // TACH1 Count Register, LSB
#define REG_TC2H 0x54 // TACH2 Count Register, MSB
#define REG_TC2L 0x55 // TACH2 Count Register, LSB
#define REG_RTH 0x56 // Remote Temperature Reading Register, MSB
#define REG_RTL 0x57 // Remote Temperature Reading Register, LSB
#define REG_LTH 0x58 // Local Temperature Reading Register, MSB
#define REG_LTL 0x59 // Local Temperature Reading Register, LSB
#define REG_SR 0x5A // Status Register
#define REG_EEX 0x5B // EEPROM LOAD/WRITE Register

typedef union CNTL_REG_1 {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned TIS:1;
		unsigned MTI:1;
		unsigned PPS:1;
		unsigned DRV:2;
		unsigned HYST:1;
		unsigned POR:1;
		unsigned ALTMSK:1;
	}Status;
}CNTL_REG_1;

typedef union CNT_REG_2 {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned DFC:1; // 0 = Direct fan control disabled (default). 1 = Direct fan control enabled.
		unsigned FSST:1; // 0 = Fan tachometer provides square-wave pulses (default). 1 = Fan provides rotation detection (RD) signal.
		unsigned RDPS:1; // 0 = RD is at low level when the fan is running (default). 1 = RD is at high level when the fan is running.
		unsigned FS_ENABLE:1; // 0 = Externally driving FF/FS has no effect on duty cycle. (default). 1 = Externally driving FF/FS low forces 100% duty cycle
		unsigned FF_MODE:1; // 0 = FF/FS output functions as an interrupt. 1 = FF/FS output functions as a “comparator mode” fault indicator (default)
		unsigned SPEN:1; // 0 = No spin-up when the fan starts up (default). 1 = Spin-up enabled. The PWM duty cycle is 100% until the end criterion is met (default).
		unsigned ALERTS:1; // 0 = ALERT function as an interrupt (default). 1 = ALERT functions as a “comparator mode” fault indicator
		unsigned STBY:1; // 0 = Normal operating mode (default). 1 = Standby mode.
	}Status;
}CNTL_REG_2;

typedef union CNTL_REG_3 {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned TACH1E:1;
		unsigned TACH2E:1;
		unsigned PSEN:1;
		unsigned TACHFULL:1;
		unsigned RAMP:2;
		unsigned FF_0:1;
		unsigned CLR_FAIL:1;
	}Status;
}CNTL_REG_3;

//  Fan Fault Duty Cycle
typedef union FFDC {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned FFDC0:1;
		unsigned FFDC1:1;
		unsigned FFDC2:1;
		unsigned FFDC3:1;
		unsigned FFDC4:1;
		unsigned FFDC5:1;
		unsigned FFDC6:1;
		unsigned FFDC7:1;
	}Status;
}FFDC;

//Alert Mask Register (MASK)
typedef union MASK {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned TACH1AM:1;
		unsigned TACH2AM:1;
		unsigned ROTAM:1;
		unsigned RHAM:2;
		unsigned LOTAM:1;
		unsigned LHAM:1;
		unsigned :2;
	}Status;
}MASK;

//  Ideality Factor Register (IFR)
typedef union IFR {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned IF0:1;
		unsigned IF1:1;
		unsigned IF2:1;
		unsigned IF3:1;
		unsigned IF4:1;
		unsigned IF5:1;
	}Status;
}IFR;

// TACH Count Threshold Register, MSB (TCTH)
typedef union TCTH {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned TCT_8:1;
		unsigned TCT_9:1;
		unsigned TCT_10:1;
		unsigned TCT_11:1;
		unsigned TCT_12:1;
		unsigned TCT_13:1;
		unsigned TCT_14:1;
		unsigned TCT_15:1;
	}Status;
}TCTH;

// TACH Count Threshold Register, LSB (TCTL)
typedef union TCTL {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned TCT_0:1;
		unsigned TCT_1:1;
		unsigned TCT_2:1;
		unsigned TCT_3:1;
		unsigned TCT_4:1;
		unsigned TCT_5:1;
		unsigned TCT_6:1;
		unsigned TCT_7:1;
	}Status;
}TCTL;


typedef union PWMR {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned PWMR0:1;
		unsigned PWMR1:1;
		unsigned PWMR2:1;
		unsigned PWMR3:1;
		unsigned PWMR4:1;
		unsigned PWMR5:1;
		unsigned PWMR6:1;
		unsigned PWMR7:1;
	}Status;
}PWMR;

// Current PWM Duty-Cycle Register (PWMV)
typedef union PWMV {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned PWMV0:1;
		unsigned PWMV1:1;
		unsigned PWMV2:1;
		unsigned PWMV3:1;
		unsigned PWMV4:1;
		unsigned PWMV5:1;
		unsigned PWMV6:1;
		unsigned PWMV7:1;
	}Status;
}PWMV;

// TACH1 Count Register, MSB (TC1H)
typedef union TC1H {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned TC1_8:1;
		unsigned TC1_9:1;
		unsigned TC1_10:1;
		unsigned TC1_11:1;
		unsigned TC1_12:1;
		unsigned TC1_13:1;
		unsigned TC1_14:1;
		unsigned TC1_15:1;
	}Status;
}TC1H;

// TACH1 Count Register, LSB (TC1L)
typedef union TC1L {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned TC1_0:1;
		unsigned TC1_1:1;
		unsigned TC1_2:1;
		unsigned TC1_3:1;
		unsigned TC1_4:1;
		unsigned TC1_5:1;
		unsigned TC1_6:1;
		unsigned TC1_7:1;
	}Status;
}TC1L;

// TACH2 Count Register, MSB (TC2H)
typedef union TC2H {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned TC2_8:1;
		unsigned TC2_9:1;
		unsigned TC2_10:1;
		unsigned TC2_11:1;
		unsigned TC2_12:1;
		unsigned TC2_13:1;
		unsigned TC2_14:1;
		unsigned TC2_15:1;
	}Status;
}TC2H;

// TACH2 Count Register, LSB (TC2L)
typedef union TC2L {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned TC2_0:1;
		unsigned TC2_1:1;
		unsigned TC2_2:1;
		unsigned TC2_3:1;
		unsigned TC2_4:1;
		unsigned TC2_5:1;
		unsigned TC2_6:1;
		unsigned TC2_7:1;
	}Status;
}TC2L;

// Status Register (SR)
typedef union SR {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned TACH1A:1;
		unsigned TACH2A:1;
		unsigned ROTA:1;
		unsigned RHA:1;
		unsigned LOTA:1;
		unsigned LHA:1;
		unsigned RDFA:1;
		unsigned PC_BIT:1;
	}Status;
}SR;

//  EEPROM LOAD/WRITE Register (EEX)
typedef union EEX {
	struct {
		uint8_t byte0;
	}Byte;
	struct {
		unsigned _00h_0Fh:1;
		unsigned _10h_1Fh:1;
		unsigned _20h_2Fh:1;
		unsigned _30h_3Fh:1;
		unsigned _40h_4Fh:1;
		unsigned RESERVED:2;
		unsigned LW:1;
	}Status;
}EEX;



HAL_StatusTypeDef SendMax31760(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t regData);


#endif /* MAX_31760_H_ */
#endif // USE_MAX_31760
