#ifndef GLOBAL_H
#define GLOBAL_H






typedef enum booleans {
    false = 0,
    true = 1
} bool;

#define MAX_LINE_LENGTH 80

typedef struct cur_line
{
    /*to print where we has errors*/
    long num;

    /*line string*/
    char *code;
    
} cur_line;


#endif