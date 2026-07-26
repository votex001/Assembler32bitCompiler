#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "file_utils.h"
#include "process_files.h"
#include "global.h"
#include "first_pass.h"
#include "macro_table.h"
#include "process_tables.h"
#include "original_file_table.h"
#include "second_pass.h"



/**
 * Function performs first and second pass of assembler.
 * Read preprocessed file .am line by line to save 
 * tables of code in first proccess.
 * If no caught errors starts second pass that write out files.
 * @param fileName File name with extension .as.
 */
void processFiles(char *fileName){

    long ic = IC_INIT_VAL,dc = 0; /*init start value of ic and dc*/
    cur_line line;/*line information*/
    char temp_str[MAX_LINE_LENGTH + 2];/*to save line from .as*/
    bool isSuccess = TRUE;/*to check errors*/

    /*data heads*/
    codeImageTable codeHead = NULL;
    codeEntryTable entryHead = NULL;
    unsigned char *dataImg = mallocWithCheck(CODE_SINGLE_BLOCK); /*1 byte per cell*/
    
    /*work with files*/
    FILE *amFile;
    line.code = temp_str;
    line.num = 0;
    line.fileName = cutStr(fileName,".as");
    amFile = readFile(line.fileName,".am");
    /*printing error int read file so we need just to exit*/
    if(amFile == NULL){
        return;
    }
    /*goes line by line*/
    while(fgets(temp_str,MAX_LINE_LENGTH+2,amFile)!=NULL){
        line.num = getLineNum(line.code);
        if(!fPassLine(line,&ic,&dc,&codeHead,dataImg,&entryHead)){
            isSuccess = FALSE;
        }
    }
    if(isSuccess){
         if(!sPassLine(line.fileName,&ic,&dc,codeHead,entryHead,dataImg)){
            isSuccess = FALSE;
         }
    }
    deleteMacroTable();
    fclose(amFile);
    if(!isSuccess){
        deleteFile(line.fileName,".am");
    }
    free(line.fileName);
    free(dataImg);
    freeSymbolTable();
    freeExternTable();
    freeEntryTable(entryHead);
    freeCodeTable(codeHead);

}
