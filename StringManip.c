/*
 * StringManip.c
 *
 *  Created on: Nov 16, 2021
 *      Author: karl.yamashita
 */

#include "main.h"


/*
 * Description: Removes all spaces from string
 *
 *
 */
void RemoveSpaces(char *str)
{
    // To keep track of non-space character count
    int count = 0;
    int i;

    // Traverse the given string. If current character
    // is not space, then place it at index 'count++'
    for (i = 0; str[i]; i++)
    {
        if (str[i] != ' ')
        {
            str[count++] = str[i]; // here count is
                                   // incremented
        }
    }
    str[count] = '\0';
}

/*
 * Description: checks if character is a digit.
 * Input: character
 * Output: none
 * Return: true or false
 *
 */
bool IsDigit(char chr)
{
    if(chr >= 0x30 && chr <= 0x39)
    {
        return true;
    }
    return false;
}

/*
 * Description: tolower case the whole string
 */
void ToLower(char *str)
{
    int i;

    for(i = 0; i < strlen(str); i++)
    {
        str[i]=tolower(str[i]);
    }
}

/*
 * Description: toupper the whole string
 */
void ToUpper(char *str)
{
    int i;

    for(i = 0; i < strlen(str); i++)
    {
        str[i]=toupper(str[i]);
    }
}

