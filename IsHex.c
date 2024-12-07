

#include "main.h"
#include "IsHex.h"


/*
 * Description: Checks to see if string is a hex value and converts to a decimal number.
 * Input: The string to check
 * Output: The decimal number
 * Return: False if not a hex number and save integer value to address pointer.
 *         True if hex number and save integer value to address pointer
 */
bool IsHex(char *msg, uint32_t *dataOut)
{
    if(strncmp(msg, "0x", strlen("0x")) == 0)
    {
        msg += strlen("0x");
        *dataOut = strtol(msg, NULL, 16);
                
        return true;
    }
    else
    {
        *dataOut = atoi(msg);
    }
    
    return false;
}
