/*
 * I2C0_Config.c
 *
 *  Created on: Jul 12, 2024
 *      Author: karl.yamashita
 */


#include "main.h"


I2C_GenericDef i2c0 =
{
 .i2c_base = I2C0_BASE
};


/*
 * Description: Master for MCDP6200, INA3221 and TMP108 sensors
 *
 *
 *
 */
void I2C0_Config(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0))
    {
    }
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }

    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    // init the master module clock and 100kbps
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
}
