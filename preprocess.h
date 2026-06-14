#ifndef PREPROCESS_H
#define PREPROCESS_H
#include <stdio.h>
#include "global.h"


bool expandMacros(cur_line line);
bool preprocessFile(FILE *file,char *fileName);

#endif