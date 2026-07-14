#ifndef PREPROCESS_H
#define PREPROCESS_H
#include <stdio.h>
#include "global.h"


void expandMacros(cur_line line,bool *is_success,bool *skip_current_macro,bool *is_in_macro,char *macro_name,FILE *amFile);
bool preprocessFile(FILE *file,char *fileName);

#endif