#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>

FILE *writeFile(const char *fileName, const char *ext);
void deleteFile(const char *name, const char *ext);
FILE *readFile(const char *fileName, const char *ext);


#endif