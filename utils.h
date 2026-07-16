#ifndef UTILS_H
#define UTILS_H


#include "global.h"

bool isCorrectFileName(char *fileName); 
void skipSpaces(char *str, int *i);
bool isFirstWordLabel(const cur_line line,char *nextWord,int *i);
char *cutStr(const char *str1,const char *str2);
bool isValidLabel(const char *label);
bool isAlphanumeric(const char *str);
bool isReservedWord(const char *word);
void *mallocWithCheck(long size);
void *reallocWithCheck(void *ptr,long size);
bool isEmptyStr(char *str,int i);
bool is_int(char *string);
/*TODO: getNextWord()*/
#endif