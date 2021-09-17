/*
 * CharArrayToInt.c
 *
 *  Created on: Aug 20, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "CharArrayToInt.h"


int charArrayToInt(char *arr) {
    int i, value, r, flag;

    flag = 1;
    i = value = 0;

    for( i = 0 ; i<strlen(arr) ; ++i){

        // if arr contain negative number
        if( i==0 && arr[i]=='-' ){
            flag = -1;
            continue;
        }

        r = arr[i] - '0';
        value = value * 10 + r;
    }

    value = value * flag;

    return value;

}

