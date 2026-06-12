#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>

FILE *createFile(const char *sourceName, const char *extension);
void deleteFile(const char *FileName);



#endif