/* 
 * File:   EMC1412.h
 * Author: karl.yamashita
 *
 * Created on July 5, 2022, 12:05 PM
 */

#ifndef EMC1412_H
#define	EMC1412_H

#ifdef	__cplusplus
extern "C" {
#endif

    
#define EMC1412_SLAVE_ADDRESS 0b1001100
#define I2C_BASE 1
    
    
enum EMC1412_Registers{
    INTERNAL_DIODE_DATA_HIGH_BYTE,    
    EXTERNAL_DIODE_DATA_HIGH_BYTE,
    STATUS,
    CONFIGURATION,
    CONVERSION_RATE,
    INTERNAL_DIODE_HIGH_LIMIT,
    INTERNAL_DIODE_LOW_LIMIT,
    EXTERNAL_DIODE_HIGH_LIMIT_HIGH_BYTE,
    EXTERNAL_DIODE_LOW_LIMIT_HIGH_BYTE,
    ONE_SHOT = 0x0F,
    EXTERNAL_DIODE_DATA_LOW_BYTE,
    SCRATCHPAD_11,
    SCRATCHPAD_12,
    EXTERNAL_DIODE_HIGH_LIMIT_LOW_BYTE,
    EXTERNAL_DIODE_LOW_LIMIT_LOW_BYTE,
    EXTERNAL_DIODE_THERM_LIMIT = 0x19,
    CHANNEL_MASK_REGISTER = 0x1F,
    INTERNAL_DIODE_THERM_LIMIT,
    THERM_HYSTERESIS,
    CONSECUTIVE_ALERT,
    EXTERNAL_DIODE_BETA_CONFIGURATION = 0x25,
    EXTERNAL_DIODE_IDEALITY_FACTOR = 0x27,
    INTERNAL_DIODE_DATA_LOW_BYTE = 0x29,
    FILTER_CONTROL = 0x40,
    PRODUCT_ID = 0xFD,
    MANUFACTURER_ID,
    REVISION 
};


/* Temperature Data Registers. See Table 7.2 in data sheet.
 * 
 * All temperatures are stored as an 11-bit value with the high byte representing the integer value
 * and the low byte representing the fractional value left justified to occupy the MSBits
 * 
 * 
 */

typedef union
{
    struct
    {
        uint8_t byte0;
    }Bytes;
    struct
    {
        unsigned iTherm:1;
        unsigned eTherm:1;
        unsigned Fault:1;
        unsigned eLow:1;
        unsigned eHigh:1;
        unsigned iLow:1;
        unsigned iHigh:1;
        unsigned busy:1;
    }Status;
}StatusRegister;

typedef union
{
    struct
    {
        uint8_t byte0;
    }Bytes;
    struct
    {
        unsigned :1;
        unsigned DavgDis:1;
        unsigned Range:1;
        unsigned :1;
        unsigned RecD:1;
        unsigned AlertComp:1;
        unsigned RunStop:1;
        unsigned MaskAll:1;
    }Status;
}ConfigurationRegister;

typedef union
{
    struct
    {
        uint8_t byte0;
    }Bytes;
    struct
    {
        unsigned conv:4; // default 0x06 = 4/sec
        unsigned :4;
    }Status;
}ConversionRateRegister; // see table 7.6 in data sheet

typedef union
{
    struct
    {
        uint8_t byte0; 
    }Bytes;
    struct
    {
        unsigned intMask:1;
        unsigned eMask:1;
        unsigned :6;
    }Status;
}ChannelMaskRegister;

typedef union
{
    struct
    {
        uint8_t byte0;
    }Bytes;
    struct
    {
        unsigned :1;
        unsigned cAlrt:3;
        unsigned cTherm:3;
        unsigned timeOut:1;
    }Status;
}ConsecutiveAlertRegister;

typedef union
{
    struct
    {
        uint8_t byte0;
    }Bytes;
    struct
    {
        unsigned beta:3;
        unsigned enable:1;
        unsigned :4;
    }Status;
}BetaConfigurationRegister;

typedef union
{
    struct
    {
       uint8_t byte0; 
    }Bytes;
    struct
    {
        unsigned ideality:6;
        unsigned :2;
    }Status;
}ExternalDiodeIdealityFactorRegister;

typedef union
{
   struct
    {
       uint8_t byte0; 
    }Bytes;
    struct
    {
        unsigned filter:2;
        unsigned :6;
    }Status; 
}FilterControlRegister;


int EMC1412_Init(void);

int EMC1412_SetAlertComp(bool mode);
int EMC1412_SetRunStop(bool mode);
int EMC1412_SetDAVG_DIS(bool mode);
int EMC1412_SetTemperatureRange(bool mode);
int EMC1412_SetConversionRate(uint8_t value);
int EMC1412_SetConsecutiveAlert(uint8_t value);
int EMC1412_SetConsecutiveTherm(uint8_t value);
int EMC1412_SetExternalDiodeIdealityFactor(uint8_t value);
int EMC1412_SetFilterControl(uint8_t value);

int EMC1412_GetInternalLimitData(uint8_t registerAddress, uint8_t *retVal);
int EMC1412_GetTemperatureLimitData(uint8_t registerAddress, float *retVal);

int EMC1412_GetRegisterData(uint8_t registerAddress, uint8_t *retVal);
int EMC1412_SetRegisterData(uint8_t registerAddress, uint8_t data);

int EMC1412_GetManufacturer_ID(uint8_t *retVal);

#ifdef	__cplusplus
}
#endif

#endif	/* EMC1412_H */

