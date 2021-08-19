/*
 * StringToFloat.c
 *
 *  Created on: Aug 5, 2021
 *      Author: karl.yamashita
 */


#include "main.h"
#include "StringToFloat.h"

/*
 * Description: Convert char array to float value
 * Input: char array must be a number with no Alpha numeric characters. Currently only for x.x digits or max 9.x in the ones place with one decimal number.
 * Output: return 0 if no errors.
 */
int StringToFloat(char *array, float *value_OUT){
	float value = 0;

	value = GetASCII_ByteValue(*array);
	array++; // this should be the decimal index
	if(*array != '.'){
		*value_OUT = value; // no decimal number
	}
	else
	{
		array++; // this should be the value before decimal place

		value += (GetASCII_ByteValue(*array) *.1);
		*value_OUT = value;
	}

	return 0;
}

