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


void compileFile(char *file){

    FILE *file_addres;
    
    if(!isCorrectFileName(file)){
        printf("Can't open file %s - incorrect format",file);
        return;
    }    
    
    file_addres = fopen(file,"r");

    if(file_addres == NULL){


    }

    
    

}
