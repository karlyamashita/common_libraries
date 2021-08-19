/*
 * INA3221.h
 *
 *  Created on: Jun 28, 2021
 *      Author: karl.yamashita
 */

#ifndef INC_INA3221_H_
#define INC_INA3221_H_


#define INA3221_SLAVE_ADDRESS 0b1000000
// register addresses
#define CONFIG_REG_ADDR 0
#define CHAN1_SHUNT_VOLTAGE_ADDR 1
#define CHAN1_BUS_VOLTAGE_ADDR 2
#define CHAN2_SHUNT_VOLTAGE_ADDR 3
#define CHAN2_BUS_VOLTAGE_ADDR 4
#define CHAN3_SHUNT_VOLTAGE_ADDR 5
#define CHAN3_BUS_VOLTAGE_ADDR 6
#define CHAN1_CRITICAL_ALERT_ADDR 7
#define CHAN1_WARNING_ALART_ADDR 8
#define CHAN2_CRITICAL_ALERT_ADDR 9
#define CHAN2_WARNING_ALART_ADDR 0xA
#define CHAN3_CRITICAL_ALERT_ADDR 0xB
#define CHAN3_WARNING_ALART_ADDR 0XC
#define SHUNT_VOLTAGE_SUM_ADDR 0xD
#define SHUNT_VOLTAGE_SUM_LIMIT_ADDR 0xE
#define MASK_ENABLE_ADDR 0xF
#define POWER_VALID_UPPER_LIMIT_ADDR 0x10
#define POWER_VALID_LOWER_LIMIT_ADDR 0x11
#define MANUFACTURER_ID_ADDR 0xFE
#define DIE_ID_ADDR	0xFF


#define SHUNT_SEL 0 // shunt selected
#define BUS_SEL 1

// Configuration register defines
#define avg2_0_sample_1 0
#define avg2_0_sample_4 1
#define avg2_0_sample_16 2
#define avg2_0_sample_64 3
#define avg2_0_sample_128 4;
#define avg2_0_sample_256 5
#define avg2_0_sample_512 6
#define avg2_0_sample_1024 7
#define vBusCT2_0_time_140us 0
#define vBusCT2_0_time_204us 1
#define vBusCT2_0_time_332us 2
#define vBusCT2_0_time_588us 3
#define vBusCT2_0_time_1_1ms 4
#define vBusCT2_0_time_2_116ms 5
#define vBusCT2_0_time_4_156ms 6
#define vBusCT2_0_time_8_244ms 7
// note: vShCT2_0 same as vBusCT2_0
#define mode3_1_Mode_PowerDown 0
#define mode3_1_Mode_ShuntSingle 1
#define mode3_1_Mode_BusSingle 2
#define mode3_1_Mode_ShuntBusSingle 3
#define mode3_1_Mode_PowerDown2 4
#define mode3_1_Mode_ShuntCont 5
#define mode3_1_Mode_BusCont 6
#define mode3_1_Mode_ShuntBusCont 7



typedef union{
	struct{
		uint8_t bytes[2];
	}Bytes;
	struct{
		unsigned mode3_1:3;
		unsigned vShCT2_0:3; // uses same conversion time as vBus_CT2_0
		unsigned vBus_CT2_0:3;
		unsigned avg2_0:3;
		unsigned ch3_En:1;
		unsigned ch2_En:1;
		unsigned ch1_En:1;
		unsigned rst:1;
	}Status;
}INA3221_ConfigRegStruct;


// Channel Shunt/Bus-Voltage defines
#define SHUNT_VOLTAGE_FULL  0.1638 // mV
#define SHUNT_VOLTAGE_LSB  0.00004 // uV
#define BUS_VOLTAGE_FULL  32.76 // volts
#define BUS_VOLTAGE_LSB  0.008 // mV
typedef struct{
	uint8_t channel;
	uint8_t shuntOrBus; // shunt = 0, bus = 1
	float voltage;
}ShuntBusVoltage;


// prototypes
int INA3221_Init(void);

int INA3221_SetConfigReg(INA3221_ConfigRegStruct *config);
int INA3221_ChannelShuntBusVoltage(ShuntBusVoltage *shuntBusVoltage);
int INA3221_Read(ShuntBusVoltage *shuntBusVoltage, uint16_t *value_OUT);


#endif /* INC_INA3221_H_ */
