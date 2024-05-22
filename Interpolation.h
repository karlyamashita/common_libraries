/*
 * Interpolation.h
 *
 *  Created on: May 22, 2024
 *      Author: karl.yamashita
 */

#ifndef INC_INTERPOLATION_H_
#define INC_INTERPOLATION_H_


typedef struct
{
	double y;
	double x;
	double x1;
	double x2;
	double y1;
	double y2;
}Interpolation_t;

int Interpolation_Get_Y(Interpolation_t *interpol);
int Interpolation_Set_X1_X2(Interpolation_t *interpol, double _x1, double _x2);
int Interpolation_Set_Y1_Y2(Interpolation_t *interpol, double _y1, double _y2);
int Interpolation_Set_X(Interpolation_t *interpol, double _x);
int Interpolation_SetMsg(Interpolation_t *interpol, char *msg);


#endif /* INC_INTERPOLATION_H_ */
