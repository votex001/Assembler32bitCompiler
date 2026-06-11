#include <stdio.h>
#include "global.h"
#include "utils.h"

void compileFile(char *file);


int main(int argc, char *files[]){
    int i;

    for(i = 1;i<argc;i++)
        compileFile(files[i]);

    return 0;
}


void compileFile(char *fileName){

    FILE *file_address;
    
    if(!isCorrectFileName(fileName)){
        printf("Error: can't open file %s - incorrect format.Skipped.\n",fileName);
        return;
    }    
    
    file_address = fopen(fileName,"r");

    if(file_address == NULL){
        printf("Error: file %s is inaccessible for reading.Skipped.\n",fileName);
        return;
    }

    
    

}
