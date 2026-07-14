#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "file_utils.h"
#include "process_files.h"
#include "global.h"
#include "first_pass.h"
#include "macro_table.h"




void processFiles(char *fullName){
    long ic = IC_INIT_VAL,dc = 0;
    cur_line line;
    char temp_str[MAX_LINE_LENGTH + 2];
    
 
    FILE *amFile;
    FILE *obFile;
    line.code = temp_str;
    line.fileName = cutStr(fullName,".as");
    amFile = readFile(line.fileName,".am");
    if(amFile == NULL){
        return;
    }
    obFile = writeFile(line.fileName,".ob");
    if(obFile == NULL){
        return;
    }

    for(line.num = 1;fgets(temp_str,MAX_LINE_LENGTH+2,amFile)!=NULL;line.num++){
        if(!fpassLine(line,&ic,&dc,obFile)){
            fclose(amFile);
            fclose(obFile);
            deleteFile(line.fileName,".am");
            deleteFile(line.fileName,".ob");
            return;
        }
    }


    deleteMacroTable();
    free(line.fileName);
    fclose(amFile);
    fclose(obFile);
}