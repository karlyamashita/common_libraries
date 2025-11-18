/*
 * Linear_5_11.h
 *
 *  Created on: Nov 22, 2021
 *      Author: karl.yamashita
 */

#ifndef LINEAR_5_11_H_
#define LINEAR_5_11_H_


float L5_11_to_Float(uint16_t wordValue);
uint16_t Float_to_L11(float input_val);

float L16_to_Float(int exp, uint16_t input_val);
uint16_t Float_to_L16(int exp, float input_val);


#endif /* LINEAR_5_11_H_ */
