/*
 * FloatToASCII.c
 *
 *  Created on: May 21, 2021
 *      Author: karl.yamashita
 *
 *      This was made for Microblaze because Standar C library used too much memory. 
 *          Later found out Xilinx has their own Standard C library.
 * 
 *
 *  Credit: https://stackoverflow.com/questions/7228438/convert-double-float-to-string
 *
 */

#include "main.h"


static int n_tu(int number, int count);

/** Number on countu **/
static int n_tu(int number, int count)
{
    int result = 1;
    while(count-- > 0)
        result *= number;

    return result;
}

/*
 * Description: Convert float to string
 *
 * Input: The float number to convert and how decimal places
 * Output: The char array to save the string to
 */
void Float_to_string(float f, uint8_t places, char strOut[])
{
    long long int length, length2, i, number, position, sign;
    float number2;

    sign = -1;   // -1 == positive number
    if (f < 0)
    {
        sign = '-';
        f *= -1;
    }

    number2 = f;
    number = f;
    length = 0;  // Size of decimal part
    length2 = 0; // Size of tenth

    /* Calculate length2 tenth part */
    while( (number2 - (float)number) != 0.0 && !((number2 - (float)number) < 0.0)  && (length2 < places))
    {
         number2 = f * (n_tu(10.0, length2 + 1));
         number = number2;

         length2++;
    }

    /* Calculate length decimal part */
    for (length = (f > 1) ? 0 : 1; f > 1; length++)
        f /= 10;

    position = length;
    length = length + 1 + length2;
    number = number2;
    if (sign == '-')
    {
        length++;
        position++;
    }

    for (i = length; i >= 0 ; i--)
    {
        if (i == (length))
            strOut[i] = '\0';
        else if(i == (position))
            strOut[i] = '.';
        else if(sign == '-' && i == 0)
            strOut[i] = '-';
        else
        {
            strOut[i] = (number % 10) + '0';
            number /=10;
        }
    }
}

