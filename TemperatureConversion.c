/*
 * TemperatureConversion.c
 *
 *  Created on: Oct 14, 2025
 *      Author: karl.yamashita
 */

#include "main.h"


int Celsius_Fahrenheit(float *celsius, float *fahrenheit)
{
	int status = NO_ERROR;

	*fahrenheit = ((*celsius * 1.8) + 32);

	return status;
}

int Fahrenheit_Celsius(float *fahrenheit, float *celsius)
{
	int status = NO_ERROR;

	*celsius = (*fahrenheit -32) / 1.8 ;

	return status;
}
