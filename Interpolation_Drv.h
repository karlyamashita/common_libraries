/*
 * Interpolation.h
 *
 *  Created on: May 22, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_INTERPOLATION_DRV_H_
#define INC_INTERPOLATION_DRV_H_

// un-comment if you want to enable extrapolation. This allows X and Y values to go beyond X2 and Y2 values.
//#define EXTRAPOLATION_ALLOWED

typedef struct
{
	float y; // calculated requested value
	float x; // requested value
	float x1; // typically is zero
	float x2; // target value
	float y1; // typically is zero
	float y2; // calculated value, difference between x2 and y2_offset
	float y2_offset; // DMM measured value
	uint8_t type;
}Interpolation_t;

int Interpolation_Get_Y(Interpolation_t *interpol, float x);
int Interpolation_Set_X1_X2(Interpolation_t *interpol, float _x1, float _x2);
int Interpolation_Set_Y1_Y2(Interpolation_t *interpol, float _y1, float _y2);
int Interpolation_Set_X(Interpolation_t *interpol, float _x);
int Interpolation_GetMsg_X(Interpolation_t *interpol, char *retStr);
int Interpolation_GetMsg_Y(Interpolation_t *interpol, char *retStr);
int Interpolation_SetMsg(Interpolation_t *interpol, char *msg);


#endif /* INC_INTERPOLATION_DRV_H_ */
