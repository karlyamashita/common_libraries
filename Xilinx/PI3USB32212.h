/*
 * PI3USB32212.h
 *
 *  Created on: Jun 28, 2021
 *      Author: karl.yamashita
 */

#ifndef INC_PI3USB32212_H_
#define INC_PI3USB32212_H_


typedef struct{
	uint8_t function;
	bool newFunctionFlag;
	unsigned :7;
}PI3USB32212_struct;

enum{
	SSMUX1_OE,
	SSMUX1_PD,
	SSMUX1_SEL1,
	SSMUX1_SEL2,

	SSMUX2_OE,
	SSMUX2_PD,
	SSMUX2_SEL1,
	SSMUX2_SEL2,
};

void PI3USB32212_SetMux(uint32_t function);
void PI3USB32212_SetDemux(uint32_t function);


#endif /* INC_PI3USB32212_H_ */
