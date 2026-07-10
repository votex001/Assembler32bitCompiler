#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>

FILE *writefile(const char *fileName, const char *ext);
void deleteFile(const char *name, const char *ext);



#endif