

#include "main.h"
#include "IsHex.h"


/*
 Description: Checks to see if value is a string hex number and converts to a decimal number. 
 * Input: The string to check
 * Output: The decimal number
 * Return: False if not a hex number and save integer value to address pointer.
 *         True if hex number and save integer value to address pointer
 */
bool IsHex(char *msg, uint32_t *dataOut)
{
    char array[16] = {0};
    
    if(strncmp(msg, "0x", strlen("0x")) == 0)
    {
        msg += strlen("0x");
        
        sprintf(array, "%s", msg);        
        ConvertHex2Int(array, dataOut);
                
        return true;
    }
    else
    {
        *dataOut = atoi(msg);
    }
    
    return false;
}

/*
 Description: Converts string hex number to a uint32_t number
 * Input: String hex value, should include "0x" preceeding hex value
 * Output: 32bit number
 * Return: error status
 */
int ConvertHex2Int(char *hex, uint32_t *valOut) 
{
    while (*hex) {
        // get current character then increment
        uint8_t byte = *hex++;
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
        else
        {
        	return VALUE_NOT_VALID; // error
        }
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        *valOut = (*valOut << 4) | (byte & 0xF);
    }
    return NO_ERROR;
}

