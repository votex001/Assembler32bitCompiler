#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

typedef struct macro_table
{
    char *name;

    char *content;

    struct macro_table *next;
} macro_table;



#endif