#ifndef UTILS_H
#define UTILS_H


#include "global.h"

bool isCorrectFileName(char *fileName); 
void skipSpaces(char *str, int *i);
bool firstWordIsLabel(cur_line line,char *firstWord);
char *cutStr(const char *str1,const char *str2);

#endif