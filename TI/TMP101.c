/*
 * TM101.c
 *
 *  Created on: Mar 3, 2021
 *      Author: karl.yamashita
 *
 *      Texas Instruments TMP101 temperature sensor
 */

#include "main.h"
#include "TMP101.h"


/*
 * Description: initialize TMP101 sensors
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef InitTMP101(void){
    HAL_StatusTypeDef status;
    TMP101_Configuration TMP101_Configuration;
    int16_t tempHigh, tempLow;
    uint8_t pui8DataTx[3]= {0};
    uint16_t size = 2;

    TMP101_Configuration.Status.SD = 0;
    TMP101_Configuration.Status.TM = 0;
    TMP101_Configuration.Status.POL = 0;
    TMP101_Configuration.Status.F0 = 0;
    TMP101_Configuration.Status.F1 = 1;
    TMP101_Configuration.Status.R0 = 1;
    TMP101_Configuration.Status.R1 = 1;
    TMP101_Configuration.Status.OS_ALERT = 0;
    // = 0x70

    pui8DataTx[0] = TMP101_CONFIGURATION_REGISTER;
    pui8DataTx[1] = TMP101_Configuration.Byte.byte0;

    // #1
    status = TMP101_WriteRegister(I2C0_BASE, TMP101_SLAVE_ADDRESS_00, pui8DataTx, size);
    if(status != HAL_OK){
        return status;
    }
    // #2
    status = TMP101_WriteRegister(I2C0_BASE, TMP101_SLAVE_ADDRESS_01, pui8DataTx, size);
    if(status != HAL_OK){
        return status;
    }

    // 0x0F = 1°C;
    // c to f = (c * 1.8) + 32
    // f to c = (f -32) / 1.8
    tempLow = 0x3B0;
    tempHigh = 0x3C0; // 60 Celsius

    status = SetTMP101_00_Low(tempLow);
    if(status != HAL_OK){
        return status;
    }
    status = SetTMP101_01_Low(tempLow);
    if(status != HAL_OK){
        return status;
    }
    status = SetTMP101_00_High(tempHigh);
    if(status != HAL_OK){
        return status;
    }
    status = SetTMP101_01_High(tempHigh);
    if(status != HAL_OK){
        return status;
    }
    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef SetTMP101_00_High(uint16_t temp){
    HAL_StatusTypeDef status;
    const uint8_t size = 3;
    uint8_t pui8DataTx[3];

    temp = temp << 4;

    // ****** High Temp ********
    pui8DataTx[0] = TMP101_TEMP_HIGH_REGISTER;
    pui8DataTx[1] = temp >> 8;
    pui8DataTx[2] = temp;

    status = I2C_Master_Transmit(I2C0_BASE, TMP101_SLAVE_ADDRESS_00, pui8DataTx, size);
    if (status != HAL_OK)
    {

    }
    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef SetTMP101_01_High(uint16_t temp){
    HAL_StatusTypeDef status;
    const uint8_t size = 3;
    uint8_t pui8DataTx[3];

    temp = temp << 4;

    // ****** High Temp ********
    pui8DataTx[0] = TMP101_TEMP_HIGH_REGISTER;
    pui8DataTx[1] = temp >> 8;
    pui8DataTx[2] = temp;

    status = TMP101_WriteRegister(I2C0_BASE, TMP101_SLAVE_ADDRESS_01, pui8DataTx, size);
    if (status != HAL_OK)
    {

    }
    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef SetTMP101_00_Low(uint16_t temp){
    HAL_StatusTypeDef status;
    const uint8_t size = 3;
    uint8_t pui8DataTx[3];

    temp = temp << 4;

    // ****** Low Temp ********
    pui8DataTx[0] = TMP101_TEMP_LOW_REGISTER;
    pui8DataTx[1] = temp >> 8;
    pui8DataTx[2] = temp;

    status = TMP101_WriteRegister(I2C0_BASE, TMP101_SLAVE_ADDRESS_00, pui8DataTx, size);
    if (status != HAL_OK)
    {

    }
    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef SetTMP101_01_Low(uint16_t temp){
    HAL_StatusTypeDef status;
    const uint8_t size = 3;
    uint8_t pui8DataTx[3];

    temp = temp << 4;

    // ****** Low Temp ********
    pui8DataTx[0] = TMP101_TEMP_LOW_REGISTER;
    pui8DataTx[1] = temp >> 8;
    pui8DataTx[2] = temp;

    status = TMP101_WriteRegister(I2C0_BASE, TMP101_SLAVE_ADDRESS_01, pui8DataTx, size);
    if (status != HAL_OK)
    {

    }
    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef GetTMP101Limit00_High(void){
    UartCharBufferTxStruct uartTx;
    HAL_StatusTypeDef status;
    uint8_t pui8DataTx[3];
    uint8_t tmp101_LimitValue[2];
    uint16_t tmp101Value;
    char str[64] = {0}; // main string, increase if needed
    char str2[32] = {0};

    pui8DataTx[0] = TMP101_TEMP_HIGH_REGISTER;
    // set to read tmp101 slave 00
    status = TMP101_WriteRegister(I2C0_BASE, TMP101_SLAVE_ADDRESS_00, pui8DataTx, 1);
    if (status == HAL_OK)
    {
        // read temp sensor 1
        status = TMP101_ReadRegister(I2C0_BASE, TMP101_SLAVE_ADDRESS_00, tmp101_LimitValue, 2);
        if(status != HAL_OK){
           return status;
        }

        tmp101Value = (uint16_t)( (tmp101_LimitValue[0] >> 4) & 0x0F) << 8;
        tmp101Value |= (uint16_t) ( (tmp101_LimitValue[0] & 0x0F) << 4 | (tmp101_LimitValue[1] & 0xF0) >> 4 );

        strcpy(str, "CMD9:H1=");

        GetStringForTemp(tmp101Value, str2);

        strcat(str, str2);
        strcat(str, "\r\n");

        UartCopyStrToCharBufferTxStruct(UART_PORT_3, &uartTx, str);
        UartAddTxMessageBuffer(&uartTx);
    }

    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef GetTMP101Limit01_High(void){
    UartCharBufferTxStruct uartTx;
    HAL_StatusTypeDef status;
    uint8_t pui8DataTx[3];
    uint8_t tmp101_LimitValue[2];
    uint16_t tmp101Value;
    char str[64] = {0}; // main string, increase if needed
    char str2[32] = {0};

    pui8DataTx[0] = TMP101_TEMP_HIGH_REGISTER;
    // set to read tmp101 slave 01
    status = TMP101_WriteRegister(I2C0_BASE, TMP101_SLAVE_ADDRESS_01, pui8DataTx, 1);
    if (status == HAL_OK)
    {
        // read temp sensor 1
        status = TMP101_ReadRegister(I2C0_BASE, TMP101_SLAVE_ADDRESS_01, tmp101_LimitValue, 2);
        if(status != HAL_OK){
           return status;
        }

        tmp101Value = (uint16_t)( (tmp101_LimitValue[0] >> 4) & 0x0F) << 8;
        tmp101Value |= (uint16_t) ((tmp101_LimitValue[0] & 0x0F) << 4 | (tmp101_LimitValue[1] & 0xF0) >> 4);

        strcpy(str, "CMD9:H2=");

        GetStringForTemp(tmp101Value, str2);

        strcat(str, str2);
        strcat(str, "\r\n");

        UartCopyStrToCharBufferTxStruct(UART_PORT_3, &uartTx, str);
        UartAddTxMessageBuffer(&uartTx);
    }

    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef GetTMP101Limit00_Low(void){
    UartCharBufferTxStruct uartTx;
    HAL_StatusTypeDef status;
    uint8_t pui8DataTx[3];
    uint8_t tmp101_LimitValue[2];
    uint16_t tmp101Value;
    char str[64] = { 0 }; // main string, increase if needed
    char str2[32] = {0};

    pui8DataTx[0] = TMP101_TEMP_LOW_REGISTER;
    // set to read tmp101 slave 00
    status = TMP101_WriteRegister(I2C0_BASE, TMP101_SLAVE_ADDRESS_00, pui8DataTx, 1);
    if (status == HAL_OK)
    {
        return status;
    }

    // read temp sensor 1
    status = TMP101_ReadRegister(I2C0_BASE, TMP101_SLAVE_ADDRESS_00, tmp101_LimitValue, 2);
    if(status != HAL_OK){
       return status;
    }

    tmp101Value = (uint16_t)( (tmp101_LimitValue[0] >> 4) & 0x0F) << 8;
    tmp101Value |= (uint16_t) ((tmp101_LimitValue[0] & 0x0F) << 4 | (tmp101_LimitValue[1] & 0xF0) >> 4);

    strcpy(str, "CMD9:L1=");

    GetStringForTemp(tmp101Value, str2);

    strcat(str, str2);
    strcat(str, "\r\n");

    UartCopyStrToCharBufferTxStruct(UART_PORT_3, &uartTx, str);
    UartAddTxMessageBuffer(&uartTx);

    return status;
}

/*
 * Description:
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef GetTMP101Limit01_Low(void){
    UartCharBufferTxStruct uartTx;
    HAL_StatusTypeDef status;
    uint8_t pui8DataTx[3];
    uint8_t tmp101_LimitValue[2];
    uint16_t tmp101Value;
    char str[64] = { 0 }; // main string, increase if needed
    char str2[32] = {0};

    pui8DataTx[0] = TMP101_TEMP_LOW_REGISTER;
    // set to read tmp101 slave 01
    status = TMP101_WriteRegister(I2C0_BASE, TMP101_SLAVE_ADDRESS_01, pui8DataTx, 1);
    if (status == HAL_OK)
    {
        // read temp sensor 1
        status = TMP101_ReadRegister(I2C0_BASE, TMP101_SLAVE_ADDRESS_01, tmp101_LimitValue, 2);
        if(status != HAL_OK){
           return status;
        }

        tmp101Value = (uint16_t)( (tmp101_LimitValue[0] >> 4) & 0x0F) << 8;
        tmp101Value |= (uint16_t) ((tmp101_LimitValue[0] & 0x0F) << 4 | (tmp101_LimitValue[1] & 0xF0) >> 4);

        strcpy(str, "CMD9:L2=");

        GetStringForTemp(tmp101Value, str2);

        strcat(str, str2);
        strcat(str, "\r\n");

        UartCopyStrToCharBufferTxStruct(UART_PORT_3, &uartTx, str);
        UartAddTxMessageBuffer(&uartTx);
    }

    return status;
}

/*
 *
 *
 *
 */
HAL_StatusTypeDef TMP101_ReadTempReg(uint8_t sensorSlaveAddress, uint8_t *value){
    uint8_t pui8DataTx[3];
    HAL_StatusTypeDef status;

    pui8DataTx[0] = TMP101_TEMPERATURE_REGISTER;
    // set to read tmp101 slave 01
    status = TMP101_WriteRegister(I2C0_BASE, sensorSlaveAddress, pui8DataTx, 1);
    if (status != HAL_OK){
        return status;
    }
    // read temp sensor 2
    status = TMP101_ReadRegister(I2C0_BASE, sensorSlaveAddress, value, 2);
    if(status != HAL_OK){
        return status;
    }
    return HAL_OK;
}


/*
 * Description:
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef TMP101_WriteRegister(uint32_t i2c_base, uint8_t DevAddress, uint8_t *data, uint8_t size){
    return I2C_Master_Transmit(i2c_base, DevAddress, data, size);
}



/*
 * Description:
 *
 * Input:
 * Output:
 */
HAL_StatusTypeDef TMP101_ReadRegister(uint32_t i2c_base, uint8_t DevAddress, uint8_t *data, uint8_t size){
    return I2C_Master_Receive(i2c_base, DevAddress, data, size);
}





