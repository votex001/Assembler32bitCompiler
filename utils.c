#include "global.h"
#include <string.h>
#include "utils.h"



/*Checking if file if .as format*/
bool isCorrectFileName(char *fileName){
    int len = strlen(fileName);
    if(len < 3)
        return false;

    return fileName[len-3] == '.' &&
           fileName[len-2] == 'a' &&
           fileName[len-1] == 's';

}