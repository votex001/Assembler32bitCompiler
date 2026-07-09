#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

typedef struct macro
{
    char *name;

    char *content;

    struct macro *next;
} macro;

typedef macro *macro_table;

void appendMacroLine(char *name,char *content);
bool isMacroExist(char *name);

#endif