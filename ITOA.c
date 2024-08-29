/*
 * ITOA.c
 *
 *  Created on: May 12, 2021
 *      Author: karl.yamashita
 * 
 * 
 *      This was made for Microblaze because Standar C library used too much memory. 
 *          Later found out Xilinx has their own Standard C library.
 *
 *
 *      Credit to: https://www.techiedelight.com/implement-itoa-function-in-c/
 *
 */

#include "main.h"
#include "ITOA.h"

static void swap(char *x, char *y);
static char* reverse(char *buffer, int i, int j);

// Function to swap two numbers
static void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}

// Function to reverse `buffer[i�j]`
static char* reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }

    return buffer;
}

/*
 * Description: converts an integer to char array.
 *
 * Input: the value to convert, the array buffer to hold result, the base number to convert to (base: 2, 8, 10 or 16)
 * Output:
 */
char* itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }

    // consider the absolute value of the number
    int n = abs(value);

    int i = 0;
    while (n)
    {
        int r = n % base;

        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }

        n = n / base;
    }

    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }

    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0'; // null terminate string

    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}


