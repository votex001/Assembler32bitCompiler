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




void processFiles(char *fullName){
    long ic = IC_INIT_VAL,dc = 0;
    cur_line line;
    char temp_str[MAX_LINE_LENGTH + 2];
    bool isSuccess = TRUE;
    codeImageTable codeHead = NULL;
    codeExternTable externHead = NULL;
    codeEntryTable entryHead = NULL;
    unsigned char *dataImg = mallocWithCheck(CODE_SINGLE_BLOCK); /*1 byte per cell*/
    
    FILE *amFile;
    line.code = temp_str;
    line.num = getLineNum(line.code);
    line.fileName = cutStr(fullName,".as");
    amFile = readFile(line.fileName,".am");
    
    if(amFile == NULL){
        return;
    }
    /*goes line by line*/
    while(fgets(temp_str,MAX_LINE_LENGTH+2,amFile)!=NULL){
        line.num = getLineNum(line.code);
        if(!fPassLine(line,&ic,&dc,&codeHead,&externHead,dataImg,&entryHead)){
            isSuccess = FALSE;
        }
    }
    if(isSuccess){
         if(sPassLine(line.fileName,&ic,&dc,&codeHead,externHead,entryHead,dataImg)){
            printf("SUCCESS\n");
         }
    }
    deleteMacroTable();
    fclose(amFile);
    if(!isSuccess){
        deleteFile(line.fileName,".am");
    }
    free(line.fileName);
}
