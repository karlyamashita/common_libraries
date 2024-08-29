/*
 * ErrorStatus.h
 *
 *  Created on: Nov 24, 2021
 *      Author: karl.yamashita
 */

#ifndef ERRORSTATUS_H_
#define ERRORSTATUS_H_


// error status
enum {
    RETURN_DATA = -2,
    NO_ACK = -1,
    NO_ERROR,
    COMMAND_UNKNOWN,
    VALUE_OUT_OF_RANGE,
    VALUE_NOT_VALID,
	I2C_ERROR_READ,
	I2C_ERROR_WRITE,


    // for debug
    ERR_1 = 200,
    ERR_2,
    ERR_3,
    ERR_4
};



void GetErrorString(int errorCode, char *str);

#endif /* ERRORSTATUS_H_ */
