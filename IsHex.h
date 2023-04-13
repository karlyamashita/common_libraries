/* 
 * File:   IsHex.h
 * Author: karl.yamashita
 *
 * Created on March 14, 2023, 12:24 PM
 */

#ifndef ISHEX_H
#define	ISHEX_H

#ifdef	__cplusplus
extern "C" {
#endif

bool IsHex(char *msg, uint32_t *dataOut);
int ConvertHex2Int(char *hex, uint32_t *valOut);

#ifdef	__cplusplus
}
#endif

#endif	/* ISHEX_H */

