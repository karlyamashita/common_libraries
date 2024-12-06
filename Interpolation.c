/*
 * Interpolation.c
 *
 *  Created on: May 22, 2024
 *      Author: karl.yamashita
 *
 *
 *      An example to how to use this.
 *      Lets say we have a potentiometer that is monitored by ADC channel. The range is typically 0-4095 (12 bit ADC).
 *      Lets say you want to change a device register value by the potentiometer value but the register only goes between 0-32,
 *      like for a volume adjustment. You would set x1=0 and x2=4095 and set y1=0 and y2=32. Then by passing x (ADC) value,
 *      you can get the y value for the register that is proportional to the ADC value.
 *
 */


#include "main.h"

/*
 * Description: Calculates the y value.
 * 				x1, x2, y1 and y2 must be initiated with values by calling Interpolation_Set_X1_X2
 * 				and Interpolation_Set_Y1_Y2. Before getting y value, Interpolation_Set_X must be called.
 * 	Input: Interpolation data structure and X value
 * 	Output: The y value is updated in the data structure.
 * 	Return: Error status.
 */
int Interpolation_Get_Y(Interpolation_t *interpol, float x)
{
	int status = NO_ERROR;

#ifndef EXTRAPOLATION_ALLOWED
	if(x < interpol->x1 || x > interpol->x2)
    {
		return INTERPOLATION_ERROR;
    }
#endif
	interpol->x = x;
	interpol->y = (interpol->x - interpol->x1) * (interpol->y2 - interpol->y1) / (interpol->x2 - interpol->x1) + interpol->y1;

	return status;
}

/*
 * Description: User must initiate the x1 and x2 value ranges.
 * Input: the interpolation data structure. The x1 and x2 values.
 * Return: Error status
 */
int Interpolation_Set_X1_X2(Interpolation_t *interpol, float _x1, float _x2)
{
	if(_x1 == 0 && _x2 == 0)
	{
		return INTERPOLATION_ERROR;
	}

	interpol->x1 = _x1;
	interpol->x2 = _x2;

	return 0;
}

/*
 * Description: User must initiate the y1 and y2 value ranges.
 * Input: the interpolation data structure. The y1 and y2 values.
 * Return: Error status
 */
int Interpolation_Set_Y1_Y2(Interpolation_t *interpol, float _y1, float _y2)
{
	if(_y1 == 0 && _y2 == 0)
	{
		return INTERPOLATION_ERROR;
	}

	interpol->y1 = _y1;
	interpol->y2 = _y2;

	return 0;
}

int Interpolation_GetMsg_Y(Interpolation_t *interpol, char *retStr)
{
	int status = NO_ERROR;

	sprintf(retStr, "%lf", interpol->y);

	return status;
}

/*
 * Description: Function to parse string message and set the appropriate x or y Set function
 * Input: The interpolation data structure. The string message containing the command and values.
 */
int Interpolation_SetMsg(Interpolation_t *interpol, char *msg)
{
	int status = NO_ERROR;
	char *token; // the x or y commands
	char *token2; // the x value
	char *token3; // the y value
	char *rest = msg;
	char delim[] = ":,";

	token = strtok_r(rest, delim, &rest);
	token2 = strtok_r(rest, delim, &rest);
	token3 = strtok_r(rest, delim, &rest);

	if(strncmp(token, "x1x2", strlen("x1x2")) == 0)
	{
		status = Interpolation_Set_X1_X2(interpol, atof(token2), atof(token3));
	}
	else if(strncmp(token, "y1y2", strlen("y1y2")) == 0)
	{
		status = Interpolation_Set_Y1_Y2(interpol, atof(token2), atof(token3));
	}
	else if(strncmp(token, "_x", strlen("_x")) == 0)
	{
		status = Interpolation_Set_X(interpol, atof(token2));
	}
	else
	{
		return COMMAND_UNKNOWN;
	}

	return status;
}
