#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

typedef struct macro
{
    char *name;

    char *content;

    struct macro *next;
} macro;

typedef macro *macroTable;

void appendMacroLine(const char *name,const char *content);
bool isMacroExist(const char *name);
void deleteMacroTable();
const char *getMacro(const char *name);


#endif