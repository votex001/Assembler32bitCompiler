#ifndef GLOBAL_H
#define GLOBAL_H






typedef enum booleans {
    FALSE = 0,
    TRUE = 1
} bool;

#define MAX_LINE_LENGTH 80

typedef struct cur_line
{
    /*to print where we has errors*/
    long num;

    /*line string*/
    char *code;
    
    char *fileName;
    
} cur_line;


#endif