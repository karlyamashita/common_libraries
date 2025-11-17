/*
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Karl Yamashita
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */

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

    // for debug
    ERR_1 = 200,
    ERR_2,
    ERR_3,
    ERR_4
};



void GetErrorString(int errorCode, char *str);

#endif /* ERRORSTATUS_H_ */
