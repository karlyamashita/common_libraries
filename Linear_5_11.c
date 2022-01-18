/*
 * Linear_5_11.c
 *
 *  Created on: Nov 22, 2021
 *      Author: karl.yamashita
 *
 *
 *      Conversion of Linear 5_11 and Linear 16
 *
 *
 */

#include "main.h"
#include "Linear_5_11.h"


/*
 * Convert a LinearFloat5_11 formatted word
 * into a floating point value
 */
float L5_11_to_Float(uint16_t wordValue)
{
    float floatValue;
     int8_t exponent = wordValue>>11; // extract exponent as MS 5 bits
     int16_t mantissa = wordValue & 0x7ff; // extract mantissa as LS 11 bits
     double powValue;

     // sign extend exponent from 5 to 8 bits
     if( exponent > 0x0F ){
         exponent |= 0xE0;
     }
     // sign extend mantissa from 11 to 16 bits
     if( mantissa > 0x03FF ){
         mantissa |= 0xF800;
     }

     powValue = pow(2, exponent); // compute value as mantissa * 2^(exponent)

     floatValue = mantissa * powValue;
     return floatValue;
}

/*
 * Convert a floating point value into a
 * LinearFloat5_11 formatted word
 */
uint16_t Float_to_L11(float input_val)
{
     uint16_t uExponent;
     uint16_t uMantissa;

     // set exponent to -16
     int exponent = -16;
     // extract mantissa from input value
     int mantissa = (int)(input_val / pow(2.0, exponent));
     // Search for an exponent that produces
     // a valid 11-bit mantissa
     do
     {
         if((mantissa >= -1024) && (mantissa <= +1023)){
             break; // stop if mantissa valid
         }
         exponent++;
         mantissa = (int)(input_val / pow(2.0, exponent));
     } while (exponent < +15);

     // Format the exponent of the L11
     uExponent = exponent << 11;
     // Format the mantissa of the L11
     uMantissa = mantissa & 0x07FF;
     // Compute value as exponent | mantissa
     return uExponent | uMantissa;
}


/*
 * Convert a LinearFloat16 formatted word
 * into a floating point value
 */
float L16_to_Float(int exp, uint16_t input_val)
{
     int8_t exponent = exp;
     int16_t mantissa = input_val;
     // sign extend exponent
     if( exponent > 0x0F ) exponent |= 0xE0;

     // sign extend mantissa
     // if( mantissa > 0x03FF ) mantissa |= 0xF800;

     return mantissa * pow(2,exponent); // compute value as mantissa * 2^(exponent)
}

/*
 * Convert a floating point value into a
 * LinearFloat16 formatted word
 */
uint16_t Float_to_L16(float input_val)
{
     // The length of the L16 value
     // Read the VOUT_MODE register of your
     // particular device for the value to use
     // LTC3880 = -12
     // LTC2978 = -13
     int L16_Length = -12;
     // set exponent to 2^L16_Length
     float exponent = pow(2.0, L16_Length);
     // convert value to uint16 and return
     return (uint16_t)(input_val / exponent);
}

