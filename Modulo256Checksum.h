/*
 * Modulo256Checksum.h
 *
 *  Created on: May 13, 2021
 *      Author: karl.yamashita
 */

#ifndef MODULO256CHECKSUM_H_
#define MODULO256CHECKSUM_H_


bool ValidateChkSum(uint8_t *data, uint8_t len);
void CalculateChkSum(uint8_t *data, uint8_t len);

#endif /* MODULO256CHECKSUM_H_ */
