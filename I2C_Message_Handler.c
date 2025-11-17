/*
 * I2C_Bus_Reset.c
 *
 *  Created on: Oct 20, 2025
 *      Author: karl.yamashita
 */

#include "main.h"

int I2C_Handler(char *msg, char *retStr)
{
	int status = NO_ERROR;
	char *result;
	char getChar[] = "?"; // the char that indicates a get command

	RemoveSpaces(msg);

	// check if get command (?)
	result = strstr(msg, getChar);
	if(result)
	{
		return WRITE_ONLY_ERROR;
	}
	// no ?, so is a set command
	status = I2C_Set(msg, retStr); //set

	return status;
}

int I2C_Set(char *msg, char *retStr)
{
	int status = NO_ERROR;
	char *token; // command
	char *rest = msg;
	char delim[] = "?:,\r";

	token = strtok_r(rest, delim, &rest); // command

	if(strncmp(token, "busreset", strlen("busreset")) == 0)
	{
		HAL_I2C_DeInit(&hi2c2);

		HAL_I2C_Init(&hi2c2);
	}
	else
	{
		status = COMMAND_UNKNOWN;
	}

	return status;
}
