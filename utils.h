#ifndef UTILS_H
#define UTILS_H


#include "global.h"

bool isCorrectFileName(char *fileName); 
void skipSpaces(char *str, int *i);
bool isNextWordLabel(const cur_line line,char *nextWord,int *i);
char *cutStr(const char *str1,const char *str2);
bool isValidLabel(const char *label);
bool isAlphanumeric(const char *str);
bool isReservedWord(const char *word);
void *mallocWithCheck(long size);
void *reallocWithCheck(void *ptr,long size);
bool isEmptyStr(char *str,int i);
bool is_int(char *string);
void getNextWord(cur_line line,char *nextWord,int *i);
bool isCommaNext(cur_line line,int *i,char *firstWord,char *nexWord);
bool isTextAfterCommand(cur_line line,int *i,char *commandName);


#endif