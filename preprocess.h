#ifndef PREPROCESS_H
#define PREPROCESS_H
#include <stdio.h>
#include "global.h"


bool expandMacros(cur_line line,bool *skip_current_macro,bool *is_in_macro,char *macro_name,FILE *amFile);
bool preprocessFile(char *fileName);

#endif