/*
 * Interpolation.c
 *
 *  Created on: May 22, 2024
 *      Author: karl.yamashita
 */


#include "main.h"


int Interpolation_Get_Y(Interpolation_t *interpol)
{
	if(interpol->x1 == 0 && interpol->x2 == 0)
	{
		return INTERPOLATION_ERROR_VALUE_INVALID;
	}
	else if(interpol->y1 == 0 && interpol->y2 == 0)
	{
		return INTERPOLATION_ERROR_VALUE_INVALID;
	}

	interpol->y = (interpol->x - interpol->x1) * (interpol->y2 - interpol->y1) / (interpol->x2 - interpol->x1) + interpol->y1;

	return 0;
}

int Interpolation_Set_X1_X2(Interpolation_t *interpol, double _x1, double _x2)
{
	interpol->x1 = _x1;
	interpol->x2 = _x2;

	return 0;
}

int Interpolation_Set_Y1_Y2(Interpolation_t *interpol, double _y1, double _y2)
{
	interpol->y1 = _y1;
	interpol->y2 = _y2;

	return 0;
}

int Interpolation_Set_X(Interpolation_t *interpol, double _x)
{
	interpol->x = _x + 0.000001;

	return 0;
}

int Interpolation_SetMsg(Interpolation_t *interpol, char *msg)
{
	int status = NO_ERROR;
	char *token;
	char *token2;
	char *token3;
	char *rest = msg;

	token = strtok_r(rest, ":,", &rest);
	token2 = strtok_r(rest, ":,", &rest);
	token3 = strtok_r(rest, ":,", &rest);

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
