/*
 * StringManip.h
 *
 *  Created on: Nov 16, 2021
 *      Author: karl.yamashita
 */

#ifndef STRINGMANIP_H_
#define STRINGMANIP_H_

void RemoveSpaces(char *str);
int Get_char_Index(const char *str, char c);
void Replace_Char(char *str, char findChar, char replaceChar);
void ReplaceWord(char* str, char* oldWord, char* newWord);
bool IsDigit(char chr);
void ToLower(char *str);
void ToUpper(char *str);


#endif /* STRINGMANIP_H_ */
