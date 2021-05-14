/*
 * ITOA.h
 *
 *  Created on: May 12, 2021
 *      Author: karl.yamashita
 */

#ifndef ITOA_H_
#define ITOA_H_

enum Base_struct{
    BASE_2 = 2,
    BASE_8 = 8,
    BASE_10 = 10,
    BASE_16 = 16
};

char* itoa(int value, char* buffer, int base);

#endif /* ITOA_H_ */
