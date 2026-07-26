#include <stdio.h>
#include "global.h"
#include "utils.h"
#include "preprocess.h"
#include "process_files.h"



/**
 * Processes a single source file.
 * Checking format of files start preprocess and proccess.
 * @param fileName File name with extension.
 */
void compileFile(char *fileName);

/**
 * Entry point - 32bit assembler.Assembly language specified in booklet 2026b.
 */
int main(int argc, char *files[]){
    int i;

    for(i = 1;i<argc;i++)
        compileFile(files[i]);

    return 0;
}


void compileFile(char *fileName){

    
    /*if we created .am we can start do first pas and second*/
    if(preprocessFile(fileName)){
       processFiles(fileName);
    };

}
