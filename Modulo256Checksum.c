/*
 * Modulo256Checksum.c
 *
 *  Created on: May 13, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "Modulo256Checksum.h"

/*
 * Description: Validate checksum using modulo 256
 * Input: the data array and the length of array including the checksum
 * Output: true if valid checksum else false
 *
 */
bool ValidateChkSum(uint8_t *data, uint8_t len){
    uint32_t chksum, i = 0;
    for(i = 0; i < len - 1; i++){
        chksum += *data++;
    }
    if((chksum % 256) != *data){
        return false;
    }
    return true;
}

/*
 * Description: Calculate checksum using modulo 256
 * Input: The data array and the length of array minus the checksum index
 * Output: The checksum value. Return true if successful.
 *
 */
void CalculateChkSum(uint8_t *data, uint8_t len){
    uint32_t chksum = 0;
    uint32_t i = 0;

    for (i = 0; i < len - 1; i++)
    {
        chksum += *data++;
    }
    *data = (chksum % 256); // copy checksum to last index
}

