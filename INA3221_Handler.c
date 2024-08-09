/*
 * INA3221_Handler.c
 *
 *  Created on: Aug 17, 2023
 *      Author: karl.yamashita
 */


#include "main.h"

extern TelemetryStruct telemetryData;

/*
 * Description: Call this every 10ms. There are 6 reads for voltage and current on 3 channels.
 *               So will take ~60ms to complete a round and telemetry will send every 100ms
 */
void INA3221_Read(void)
{
    static uint32_t mode = 0;

    ShuntBusVoltage_struct shuntBusVoltage;
    uint32_t i2c_base = INA3221_I2C_PORT_DEFAULT;
    uint8_t slaveAddress = INA3221_SLAVE_ADDRESS;
    INA3221_Voltage ina3221_Voltage = {0};

    switch(mode)
    {
    case 0:
        shuntBusVoltage.channel = 1;
        shuntBusVoltage.shuntOrBus = SHUNT_SEL;
        INA3221_ReadVoltage(i2c_base, slaveAddress, &shuntBusVoltage, &ina3221_Voltage.voltage);
        TelemetryUpdateCurrent(&telemetryData, shuntBusVoltage.channel, ina3221_Voltage.voltage * SHUNT_VOLTAGE_LSB);
        ++mode;
        break;
    case 1:
        shuntBusVoltage.channel = 1;
        shuntBusVoltage.shuntOrBus = BUS_SEL;
        INA3221_ReadVoltage(i2c_base, slaveAddress, &shuntBusVoltage, &ina3221_Voltage.voltage);
        TelemetryUpdateVoltage(&telemetryData, shuntBusVoltage.channel, ina3221_Voltage.voltage * BUS_VOLTAGE_LSB);
        ++mode;
        break;
    case 2:
        shuntBusVoltage.channel = 2;
        shuntBusVoltage.shuntOrBus = SHUNT_SEL;
        INA3221_ReadVoltage(i2c_base, slaveAddress, &shuntBusVoltage, &ina3221_Voltage.voltage);
        TelemetryUpdateCurrent(&telemetryData, shuntBusVoltage.channel, ina3221_Voltage.voltage * SHUNT_VOLTAGE_LSB_VBAT); // changed define 2-1-2024
        ++mode;
        break;
    case 3:
        shuntBusVoltage.channel = 2;
        shuntBusVoltage.shuntOrBus = BUS_SEL;
        INA3221_ReadVoltage(i2c_base, slaveAddress, &shuntBusVoltage, &ina3221_Voltage.voltage);
        TelemetryUpdateVoltage(&telemetryData, shuntBusVoltage.channel, ina3221_Voltage.voltage * BUS_VOLTAGE_LSB);
        ++mode;
        break;
    case 4:
        shuntBusVoltage.channel = 3;
        shuntBusVoltage.shuntOrBus = SHUNT_SEL;
        INA3221_ReadVoltage(i2c_base, slaveAddress, &shuntBusVoltage, &ina3221_Voltage.voltage);
        TelemetryUpdateCurrent(&telemetryData, shuntBusVoltage.channel, ina3221_Voltage.voltage * SHUNT_VOLTAGE_LSB);
        ++mode;
        break;
    case 5:
        shuntBusVoltage.channel = 3;
        shuntBusVoltage.shuntOrBus = BUS_SEL;
        INA3221_ReadVoltage(i2c_base, slaveAddress, &shuntBusVoltage, &ina3221_Voltage.voltage);
        TelemetryUpdateVoltage(&telemetryData, shuntBusVoltage.channel, ina3221_Voltage.voltage * BUS_VOLTAGE_LSB);
        mode = 0;
        break;
    default:
        mode = 0;
        break;
    }
}

