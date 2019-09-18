#include "LSP.h"
#ifdef USE_ASCII_TO_HEX
#include "main.h"
#include "ascii_to_hex.h"

const uint8_t hex[] = {0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf};
uint16_t AscII2hex(uint8_t input)
{
	switch (input-48){
		// input is "0-9" in ascii
		case 0:			case 1:			case 2:			case 3: 		case 4:	
		case 5:			case 6:			case 7: 		case 8:			case 9:
			return hex[input-48];
		//	break;
		// input is "A-F" in ascii
		case 17:		case 18:		case 19:
		case 20:		case 21:		case 22:
			return hex[input-55];
		//	break;
		// input is "a-f" in ascii
		case 49:		case 50:		case 51:
		case 52:		case 53:		case 54:
			return hex[input-87];
		//	break;
		// mask error bits onto the return
		default:
		//	return 0x8080;
			break;
	}	
	// error return type
	return 0x8080;
}

#endif // USE_ASCII_TO_HEX

