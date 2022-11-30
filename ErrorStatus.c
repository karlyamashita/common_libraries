/*
 * ErrorStatus.c
 *
 *  Created on: Mar 21, 2022
 *      Author: karl.yamashita
 */

#include "main.h"
#include "ErrorStatus.h"



/*
 * Description: Returns string based off error code.
 *
 *
 */
void GetErrorString(int errorCode, char *str){
    switch(errorCode){
    case NO_ERROR:
        strcpy(str, "OK");
        break;
    case ERROR:
        strcpy(str, "ERROR");
        break;
    case UART_BUSY:
        strcpy(str, "ERROR-UART Busy");
        break;
    case MCDP6200_BUSY:
        strcpy(str, "ERROR-MCDP6200 Busy");
        break;
    case TIMEOUT:
        strcpy(str, "ERROR-Timeout");
        break;
    case COMMAND_UNKNOWN:
        strcpy(str, "ERROR-Command Unknown");
        break;
    case VALUE_OUT_OF_RANGE:
        strcpy(str, "ERROR-Value out of range");
        break;
    case VALUE_NOT_VALID:
        strcpy(str, "ERROR-Value not valid");
        break;
    case NOT_HEX_VALUE:
        strcpy(str, "ERROR-Not Hex value");
        break;
    case LTM46xx_BUSY:
        strcpy(str, "ERROR-LTM4675 busy");
        break;
    case LTM46xx_NVM:
        strcpy(str, "ERROR-LTM4675 NVM unlock not successful");
        break;
    case I2C_ERROR_WRITE:
        strcpy(str, "ERROR-I2C no data sent");
        break;
    case I2C_ERROR_READ:
        strcpy(str, "ERROR-I2C no data read");
        break;
    case I2C_ERROR_DIRECT_PARSE:
        strcpy(str, "ERROR-Possible parameters error");
        break;
    case I2C_PERIPHERAL_NOT_INIT:
        strcpy(str, "ERROR-I2C peripheral not initialized");
        break;
    case I2C_BASE_NOT_VALID:
        strcpy(str, "ERROR-I2C base not valid");
        break;
    case I2C_BUSY:
        strcpy(str, "ERROR-I2C busy");
        break;
    case I2C_BUS_BUSY:
        strcpy(str, "ERROR-I2C bus busy");
        break;
    case I2C_ERROR_MESSAGE_FAIL:
        strcpy(str, "ERROR-I2C ");
        break;
    case ERROR_READ_ONLY:
        strcpy(str, "ERROR-Read Only");
        break;
    case ERROR_WRITE_ONLY:
        strcpy(str, "ERROR-Write Only");
        break;
    case ERROR_ADC_VALUE:
        strcpy(str, "ERROR-ADC value not valid");
        break;
    case ERROR_SIB_REV:
        strcpy(str, "ERROR-SIB Revision not valid");
        break;
    case ERROR_SIB_ID:
        strcpy(str, "ERROR-SIB ID not valid");
        break;
    case TMP101_ALERT:
        strcpy(str, "ERROR-TMP101 Alert");
        break;
    case LTM46xx_ALERT:
        strcpy(str, "ERROR-LTM4675 Alert");
        break;
    case LTM46xx_READ_ONLY:
        strcpy(str, "ERROR-LTM46xx_Read Only");
        break;
    case ERROR_EOM_TIMEOUT:
        strcpy(str, "ERROR-EOM timed out");
        break;
    case ERROR_MCDP6200_LANE_VALUE:
        strcpy(str, "ERROR-MCDP6200 Lane Value unknown");
        break;
    case ERROR_FLASH_ERASE:
        strcpy(str, "ERROR-Flash Erase Failed");
        break;
        // for debug
    case ERR_1:
        strcpy(str, "ERROR-DEBUG 1");
        break;
    case ERR_2:
        strcpy(str, "ERROR-DEBUG 2");
        break;
    case ERR_3:
        strcpy(str, "ERROR-DEBUG 3");
        break;
    case ERR_4:
        strcpy(str, "ERROR-DEBUG 4");
        break;
    default:
        strcpy(str, "ERROR-returned code not a valid error");
        break;
    }
}
