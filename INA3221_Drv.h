/*
 * INA3221_Drv.h
 *
 *  Created on: Jun 15, 2024
 *      Author: codek
 */

#ifndef INC_INA3221_DRV_H_
#define INC_INA3221_DRV_H_

// Do not modify below
#define SHUNT_VOLTAGE_FULL  0.1638 // mV
#define SHUNT_VOLTAGE_LSB  0.00004 // uV
#define BUS_VOLTAGE_FULL  32.76 // volts
#define BUS_VOLTAGE_LSB  0.008 // mV

enum
{
	INA3221_SHUNT,
	INA3221_BUS,
	INA3221_CRITICAL,
	INA3221_WARNING,
	INA3221_SUM,
	INA3221_SUM_LIMIT,
	INA3221_UPPER_LIMIT,
	INA3221_LOWER_LIMIT
};

enum INA3221_EnableChannels
{
	INA3221_CH1 = 1,
	INA3221_CH2 = 2,
	INA3221_CH3 = 4
};

enum RegisterAddress
{
	INA3221_CONFIG,
	INA3221_CH1_SHUNT,
	INA3221_CH1_BUS,
	INA3221_CH2_SHUNT,
	INA3221_CH2_BUS,
	INA3221_CH3_SHUNT,
	INA3221_CH3_BUS,
	INA3221_CH1_CRIT_ALERT,
	INA3221_CH1_WARN_ALERT,
	INA3221_CH2_CRIT_ALERT,
	INA3221_CH2_WARN_ALERT,
	INA3221_CH3_CRIT_ALERT,
	INA3221_CH3_WARN_ALERT,
	INA3221_SHUNT_VOLT_SUM,
	INA3221_SHUNT_VOLT_SUM_LIMIT,
	INA3221_MASK_ENABLE,
	INA3221_POWER_VALID_UPPER,
	INA3221_POWER_VALID_LOWER,
	INA3221_MANUFACT_ID = 0xFE,
	INA3221_DIE_ID
};

typedef union
{
	struct
	{
		uint8_t data[2];
	}Bytes;
	struct
	{
		unsigned mode3_1:3;
		unsigned vShCT2_0:3; // uses same conversion time as vBus_CT2_0
		unsigned vBus_CT2_0:3;
		unsigned avg2_0:3;
		unsigned ch_En:3;
		unsigned rst:1;
	}Status;
}INA3221_ConfigRegDef;

typedef union
{
	struct
	{
		uint8_t data[2];
	}Bytes;
	struct
	{
		uint32_t :3;
		uint32_t shuntBusVoltage:12;
		uint32_t sign:1;
	}Status;
}INA3221_ShuntBusVoltDef;

typedef union
{
	struct
	{
		uint8_t data[2];
	}Bytes;
	struct
	{
		uint32_t :3;
		uint32_t critWarnAlert:12;
		uint32_t sign:1;
	}Status;
}INA3221_CriticalWarnAlertDef;

typedef union
{
	struct
	{
		uint8_t data[2];
	}Bytes;
	struct
	{
		uint32_t :1;
		uint32_t critWarnAlert:14;
		uint32_t sign:1;
	}Status;
}INA3221_ShuntVoltSumDef;

typedef union
{
	struct
	{
		uint8_t data[2];
	}Bytes;
	struct
	{
		uint32_t CVRF:1;
		uint32_t TCF:1;
		uint32_t PVF:1;
		uint32_t WF3:1;
		uint32_t WF2:1;
		uint32_t WF1:1;
		uint32_t SF:1;
		uint32_t CF3:1;
		uint32_t CF2:1;
		uint32_t CF1:1;
		uint32_t CEN:1;
		uint32_t WEN:1;
		uint32_t SCC3:1;
		uint32_t SCC2:1;
		uint32_t SCC1:1;
		uint32_t :1;
	}Status;
}INA3221_MaskEnableDef;

typedef union
{
	struct
	{
		uint8_t data[2];
	}Bytes;
	struct
	{
		uint32_t :3;
		uint32_t powerValidLim:12;
		uint32_t sign:1;
	}Status;
}INA3221_PowerValidLimitDef;

typedef struct __INA3221_HandleTypeDef
{
	INA3221_ConfigRegDef config;
	INA3221_ShuntBusVoltDef ch1_shunt;
	INA3221_ShuntBusVoltDef ch1_bus;
	INA3221_ShuntBusVoltDef ch2_shunt;
	INA3221_ShuntBusVoltDef ch2_bus;
	INA3221_ShuntBusVoltDef ch3_shunt;
	INA3221_ShuntBusVoltDef ch3_bus;
	INA3221_CriticalWarnAlertDef ch1_critical;
	INA3221_CriticalWarnAlertDef ch1_warning;
	INA3221_CriticalWarnAlertDef ch2_critical;
	INA3221_CriticalWarnAlertDef ch2_warning;
	INA3221_CriticalWarnAlertDef ch3_critical;
	INA3221_CriticalWarnAlertDef ch3_warning;
	INA3221_ShuntVoltSumDef shuntVoltSum;
	INA3221_ShuntVoltSumDef shuntVoltSumLimit;
	INA3221_MaskEnableDef maskEnable;
	INA3221_PowerValidLimitDef powerValidUpperLim;
	INA3221_PowerValidLimitDef powerValidLowerLim;
	uint16_t manufacturerID;
	uint16_t dieID;
}INA3221_HandleTypeDef;


int INA3221_Read(I2C_GenericDef *i2c, uint16_t reg, void (*callback)(struct __I2C_GenericDef_ *i2c));
int INA3221_Write(I2C_GenericDef *i2c, uint16_t reg, void (*callback)(struct __I2C_GenericDef_ *i2c));

int INA3221_EnableCh(I2C_GenericDef *i2c, uint32_t channels);

void INA3221_CopyRegisterToDataPtr(I2C_GenericDef *i2c, uint8_t *regData);

#endif /* INC_INA3221_DRV_H_ */
