/*
 * ErrorStatus.h
 *
 *  Created on: Nov 24, 2021
 *      Author: karl.yamashita
 */

#ifndef ERRORSTATUS_H_
#define ERRORSTATUS_H_

#ifdef css

// error status
enum {
    RETURN_DATA = -2,
    NO_ACK = -1,
    NO_ERROR = 0,
    ERROR, // not used
    BUSY_UART,
    TIMEOUT,
    COMMAND_UNKNOWN,
    VALUE_OUT_OF_RANGE,
    VALUE_NOT_VALID,
    LTM46xx_BUSY,
    LTM46xx_NVM,
    LTM46xx_ALERT,
    I2C_ERROR_WRITE,
    I2C_ERROR_READ,
    ERROR_LTM46xx_READ_ONLY,
    ERROR_DUTY_CYCLE_OUT_OF_RANGE,

    // for debug
    ERR_1 = 200,
    ERR_2,
    ERR_3,
    ERR_4
};
#endif

#ifdef _XC_H
enum {
    NO_ACK = -1,
    NO_ERROR = 0,
    ERROR,
    UART_BUSY,
    MCDP6200_BUSY,
    TIMEOUT,
    COMMAND_UNKNOWN,
    VALUE_OUT_OF_RANGE,
    VALUE_NOT_VALID,
    NOT_HEX_VALUE,
    LTM46xx_BUSY,
    LTM46xx_NVM,
    LTM46xx_ALERT,
    LTM46xx_READ_ONLY,
    I2C_ERROR_WRITE,
    I2C_ERROR_READ,
    I2C_ERROR_DIRECT_PARSE,
    I2C_PERIPHERAL_NOT_INIT,
    I2C_BASE_NOT_VALID,
    I2C_BUSY,
    I2C_BUS_BUSY,
    I2C_MCS_ERROR,
    I2C_ERROR_TIMEOUT,
    I2C_ERROR_RETRIES_EXCEEDED,
    I2C_ERROR_MESSAGE_FAIL,
    I2C_ERROR_STUCK_START,
    ERROR_READ_ONLY,
    ERROR_WRITE_ONLY,
    ERROR_ADC_VALUE,
    ERROR_SIB_REV,
    ERROR_SIB_ID,
    TMP101_ALERT,
    ERROR_EOM_TIMEOUT,
    ERROR_MCDP6200_LANE_VALUE,
    ERROR_FLASH_ERASE,
    RETURN_DATA,

    // for debug
    ERR_1 = 200,
    ERR_2,
    ERR_3,
    ERR_4
};

#endif // XC


void GetErrorString(int errorCode, char *str);

#endif /* ERRORSTATUS_H_ */
