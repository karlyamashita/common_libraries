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
    case COMMAND_UNKNOWN:
        strcpy(str, "ERROR-Command Unknown");
        break;
    case VALUE_OUT_OF_RANGE:
        strcpy(str, "ERROR-Value out of range");
        break;
    case VALUE_NOT_VALID:
        strcpy(str, "ERROR-Value not valid");
        break;
    case I2C_ERROR_READ:
    	strcpy(str, "ERROR-I2C Read");
    	break;
    case I2C_ERROR_WRITE:
    	strcpy(str, "ERROR-I2C Write");
    	break;

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
