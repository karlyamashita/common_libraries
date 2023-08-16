
#include "main.h"
#include "Chksum16bit.h"

/*
 * Description: For validating Telemetry data. 
 *              The checksum is the first two bytes of the telemetry data.
 *              Data should be in 2 bytes increments.
 * Input: Pointer to the data array, the total packet length and if to invert checksum value
 * Output: none
 * Return: True if checksum matches, false if checksum does not match calculated value.
 *                     
 */
bool ChkSum16_Validate(uint8_t *data, uint8_t len, bool invert)
{
    uint16_t chksum = 0;
    uint32_t i = 0;
    
    for(i = 2; i < len; i += 2)
    {
        chksum += data[i] | (data[i+1] << 8);
    }
    if(invert)
    {
        chksum ^= 0xFFFF; // invert
    }
    if(chksum != (data[0] | (data[1] << 8)))
    {
        return false;
    }
    return true;
}

/*
 * Description: Takes a Telemetry packet and calculates the 16 bit checksum and places into the first 2 bytes of data packet.
 * Input: The Pointer to the data array, the packet length and to invert checksum value
 * Output: The data packet with added checksum
 */
void ChkSum16_Calculate(uint8_t *data, uint8_t len, bool invert)
{
    uint16_t chksum = 0;
    uint32_t i = 0;
    
    for (i = 2; i < len; i += 2)
    {
        chksum += data[i] | (data[i+1] << 8);
    }
    
    if(invert)
    {
        chksum ^= 0xFFFF; // invert
    }
    
    data[0] = chksum;
    data[1] = chksum >> 8;
}


