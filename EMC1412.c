/*
 * Description: Dual-Channel temperature sensor
 * 
 */

#include "main.h"
#include "EMC1412.h"


int EMC1412_Init(void)
{
    int status = NO_ERROR;
    
    status = EMC1412_SetTemperatureRange(1); // set for extended temperature
    
    return status;
}

/*
 * Description: Set Alert pin for Interrupt or Comparator
 * Input: Mode, 0 = Interrupt, 1 = Comparator
 * 
 */
int EMC1412_SetAlertComp(bool mode)
{
   int status = NO_ERROR;
   ConfigurationRegister config;
   uint8_t data[1];
   
   status = EMC1412_GetRegisterData(CONFIGURATION , data);
   if(status != NO_ERROR)
   {
       return status;
   }
   config.Bytes.byte0 = data[0];
   
   config.Status.AlertComp = mode;
   status = EMC1412_SetRegisterData(CONFIGURATION, config.Bytes.byte0);
   
   return status;
}

/*
 * Description: Set to Run or Stop
 * 
 */
int EMC1412_SetRunStop(bool mode)
{
    int status = NO_ERROR;
   ConfigurationRegister config;
   uint8_t data[1];
   
   // get register data
   status = EMC1412_GetRegisterData(CONFIGURATION , data);
   if(status != NO_ERROR)
   {
       return status;
   }
   config.Bytes.byte0 = data[0];
   
   // modify data
   config.Status.RunStop = mode;
   status = EMC1412_SetRegisterData(CONFIGURATION, config.Bytes.byte0);
   
   return status;    
}

/*
 * Description: Set Dynamic Averaging (DAVG) disable
 * Input: 0 = Enabled, 1 = disabled
 * 
 */
int EMC1412_SetDAVG_DIS(bool mode)
{
    int status = NO_ERROR;
   ConfigurationRegister config;
   uint8_t data[1];
   
   // get register data
   status = EMC1412_GetRegisterData(CONFIGURATION , data);
   if(status != NO_ERROR)
   {
       return status;
   }
   config.Bytes.byte0 = data[0];
   
   // modify data
   config.Status.DavgDis = mode;
   status = EMC1412_SetRegisterData(CONFIGURATION, config.Bytes.byte0);
   
   return status;    
}

/*
 * Description: Set Temperature range
 * Input: 0 = 0 to 127.875 (default), 1 = -64 to +191.875
 * 
 */
int EMC1412_SetTemperatureRange(bool mode)
{
    int status = NO_ERROR;
    ConfigurationRegister config;
    uint8_t data[1];

    // get register data
    status = EMC1412_GetRegisterData(CONFIGURATION , data);
    if(status != NO_ERROR)
    {
       return status;
    }
    config.Bytes.byte0 = data[0];

    // modify data
    config.Status.Range = mode;
    status = EMC1412_SetRegisterData(CONFIGURATION, config.Bytes.byte0);

    return status;    
}

/*
 * Description: Set Conversion rate
 * Input: 0 = 0 to 127.875 (default), 1 = -64 to +191.875
 * 
 */
int EMC1412_SetConversionRate(uint8_t value)
{
    int status = NO_ERROR;
   ConversionRateRegister config;
   uint8_t data[1];
   
   // get register data
   status = EMC1412_GetRegisterData(CONVERSION_RATE , data);
   if(status != NO_ERROR)
   {
       return status;
   }
   config.Bytes.byte0 = data[0];
   
   // modify data
   config.Status.conv = value;
   status = EMC1412_SetRegisterData(CONVERSION_RATE, config.Bytes.byte0);
   
   return status;    
}

/*
 * Description: Set Consecutive Alert
 * Input: 0 = alerts 1 (default), 1 = alerts 2, 3 = alerts 3, 7 = 4 alerts 
 * 
 */
int EMC1412_SetConsecutiveAlert(uint8_t value)
{
    int status = NO_ERROR;
    ConsecutiveAlertRegister config;
    uint8_t data[1];

    // get register data
    status = EMC1412_GetRegisterData(CONSECUTIVE_ALERT , data);
    if(status != NO_ERROR)
    {
        return status;
    }
    config.Bytes.byte0 = data[0];

    // modify data
    config.Status.cAlrt = value;
    status = EMC1412_SetRegisterData(CONSECUTIVE_ALERT, config.Bytes.byte0);

    return status;    
}

/*
 * Description: Set Consecutive Thermal
 * Input: 0 = alerts 1, 1 = alerts 2, 3 = alerts 3, 7 = 4 alerts (default) 
 * 
 */
int EMC1412_SetConsecutiveTherm(uint8_t value)
{
    int status = NO_ERROR;
   ConsecutiveAlertRegister config;
   uint8_t data[1];
   
   // get register data
   status = EMC1412_GetRegisterData(CONSECUTIVE_ALERT , data);
   if(status != NO_ERROR)
   {
       return status;
   }
   config.Bytes.byte0 = data[0];
   
   // modify data
   config.Status.cTherm = value;
   status = EMC1412_SetRegisterData(CONSECUTIVE_ALERT, config.Bytes.byte0);
   
   return status;    
}

/*
 * Description: Set External Diode Ideality Factor
 * Input:  
 * 
 */
int EMC1412_SetExternalDiodeIdealityFactor(uint8_t value)
{
    int status = NO_ERROR;
    ExternalDiodeIdealityFactorRegister config;
    uint8_t data[1];

    // get register data
    status = EMC1412_GetRegisterData(EXTERNAL_DIODE_IDEALITY_FACTOR , data);
    if(status != NO_ERROR)
    {
        return status;
    }
    config.Bytes.byte0 = data[0];

    // modify data
    config.Status.ideality = value;
    status = EMC1412_SetRegisterData(EXTERNAL_DIODE_IDEALITY_FACTOR, config.Bytes.byte0);

    return status;    
}

/*
 * Description: Set Filter Control
 * Input:  
 * 
 */
int EMC1412_SetFilterControl(uint8_t value)
{
    int status = NO_ERROR;
    FilterControlRegister config;
    uint8_t data[1];

    // get register data
    status = EMC1412_GetRegisterData(FILTER_CONTROL , data);
    if(status != NO_ERROR)
    {
        return status;
    }
    config.Bytes.byte0 = data[0];

    // modify data
    config.Status.filter = value;
    status = EMC1412_SetRegisterData(FILTER_CONTROL, config.Bytes.byte0);

    return status;    
}

/*
 * Description: Get Internal Diode High and Low Limit. There are no low byte option.
 * 
 */
int EMC1412_GetInternalLimitData(uint8_t registerAddress, uint8_t *retVal)
{
    int status = NO_ERROR;
    
    // get high byte
    status = I2C_Master_Read_Register_Generic(I2C_BASE, EMC1412_SLAVE_ADDRESS, registerAddress, retVal, 1);
    if(status != 0)
    {
        return status;
    }

    return status;
}

/*
 * Description: Get Temperature and limits. There are High and Low bytes to get.
 * 
 */
int EMC1412_GetTemperatureLimitData(uint8_t registerAddress, float *retVal)
{
    int status = NO_ERROR;
    uint8_t regVal[1] = {0};
    uint16_t tempData = 0;
    
    // get high byte
    status = I2C_Master_Read_Register_Generic(I2C_BASE, EMC1412_SLAVE_ADDRESS, registerAddress, regVal, 1);
    if(status != 0)
    {
        return status;
    }
    tempData = regVal[0] << 3;
    
    // get low byte
    status = I2C_Master_Read_Register_Generic(I2C_BASE, EMC1412_SLAVE_ADDRESS, registerAddress, regVal, 1);
    if(status != 0)
    {
        return status;
    }
    tempData |= regVal[0] >> 5;

    *retVal = (float) (tempData * 0.125);
    
    return status;
}

/*
 * Description: Set Temperature or limits. There are High and Low bytes to set.
 * 
 */
int EMC1412_SetTemperatureLimitData(uint8_t registerAddress, float value)
{
    int status = NO_ERROR;
    uint8_t data[1];
    uint16_t tempData = 0;
    
    tempData = (uint16_t)(value / 0.125);

    // high byte
    data[0] = tempData >> 3;
    status = EMC1412_SetRegisterData(registerAddress , data[0]);
    if(status != NO_ERROR)
    {
       return status;
    }
    
    // low byte
    data[0] = tempData << 5;
    status = EMC1412_SetRegisterData(registerAddress, data[0]);

    return status;
}

int EMC1412_GetRegisterData(uint8_t registerAddress, uint8_t *retVal)
{
    return I2C_Master_Read_Register_Generic(I2C_BASE, EMC1412_SLAVE_ADDRESS, registerAddress, retVal, 1);
}

int EMC1412_SetRegisterData(uint8_t registerAddress, uint8_t regData)
{
    int status = NO_ERROR;
    uint8_t data[1];
    
    data[0] = registerAddress;
    status = I2C_Master_Transmit_Generic(I2C_BASE, EMC1412_SLAVE_ADDRESS, data, 1);
    if(status != NO_ERROR)
    {
        return status;
    }
    
    data[0] = regData;
    status = I2C_Master_Transmit_Generic(I2C_BASE, EMC1412_SLAVE_ADDRESS, data, 1);
    
    return status;
}

/*
 * Description: Mainly to see if communication works. Default value 0x5D
 */
int EMC1412_GetManufacturer_ID(uint8_t *retVal)
{  
    return EMC1412_GetRegisterData(MANUFACTURER_ID, retVal);
}


