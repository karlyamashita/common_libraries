/*
 * ASCII_to_ByteValue.c
 *
 *  Created on: Jun 11, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "ChartoByte.h"


/*
 * Description: Get byte value of a single character specifically for CMDx where x can equal 0-9 and A-Z
 *              A = 10, B = 11...Y = 34, Z = 35.
 *
 * Input: ASCII character
 * Output: integer value of character
 *
 */
int GetASCII_ByteValue(char chr){
    int msgPointer = 0;
    if(chr >= 0x30 && chr <= 0x39){
        msgPointer = chr - 0x30;
    }
    else if(chr >= 0x41 && chr <= 0x5A){
        msgPointer = (chr - 0x40) + 9;
    }
    else
    {
        return -1;
    }
    return msgPointer;
}

