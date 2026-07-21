#ifndef ORIGINAL_FILE_TABLE_H
#define ORIGINAL_FILE_TABLE_H
#include "global.h"

typedef struct singleLineInfo
{
    char line[MAX_LINE_LENGTH +2];
    long num;
    struct singleLineInfo *next;
}singleLineInfo;

typedef singleLineInfo *lineTable;

void saveLineInfo(cur_line line);
long getLineNum(char *str);
#endif