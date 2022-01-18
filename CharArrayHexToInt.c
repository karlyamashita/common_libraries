/*
 * HexToInt.c
 *
 *  Created on: Dec 16, 2021
 *      Author: karl.yamashita
 *
 *
 *      Credit: https://stackoverflow.com/questions/10156409/convert-hex-string-char-to-int
 *
 */

#include "main.h"
#include "CharArrayHexToInt.h"


/**
 * hex2int
 * take a hex string and convert it to a 32bit number (max 8 hex digits)
 */
uint32_t Hex2Int(char *hex, uint32_t *valOut) {
    while (*hex) {
        // get current character then increment
        uint8_t byte = *hex++;
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
        else
        {
        	return 1; // error
        }
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        *valOut = (*valOut << 4) | (byte & 0xF);
    }
    return 0;
}
