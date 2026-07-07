#ifndef PREPROCESS_H
#define PREPROCESS_H
#include <stdio.h>
#include "global.h"


void expandMacros(cur_line line,bool *is_success,bool *if_in_macro,char *macro_name);
bool preprocessFile(FILE *file,char *fileName);

#endif