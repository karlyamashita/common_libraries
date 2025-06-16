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

int Get_char_Index(const char *str, char c)
{
    char *ptr = strchr(str, c);
    if (ptr == NULL)
    {
        return -1; // Character not found
    }

    return ptr - str; // Calculate the index
}

void Replace_Char(char *str, char findChar, char replaceChar)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == findChar)
        {
            str[i] = replaceChar;
        }
    }
}

void ReplaceWord(char* str, char* oldWord, char* newWord)
{
    char *pos, temp[1000];
    int index = 0;
    int owlen;

    owlen = strlen(oldWord);

    // Repeat This loop until all occurrences are replaced.

    while ((pos = strstr(str, oldWord)) != NULL) {
        // Bakup current line
        strcpy(temp, str);

        // Index of current found word
        index = pos - str;

        // Terminate str after word found index
        str[index] = '\0';

        // Concatenate str with new word
        strcat(str, newWord);

        // Concatenate str with remaining words after
        // oldword found index.
        strcat(str, temp + index + owlen);
    }
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

