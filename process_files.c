#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "file_utils.h"
#include "process_files.h"
#include "global.h"
#include "first_pass.h"
#include "macro_table.h"
#include "process_tables.h"




void processFiles(char *fullName){
    long ic = IC_INIT_VAL,dc = 0;
    cur_line line;
    char temp_str[MAX_LINE_LENGTH + 2];
    bool isSuccess = TRUE;
    
 
    FILE *amFile;
    line.code = temp_str;
    line.fileName = cutStr(fullName,".as");
    amFile = readFile(line.fileName,".am");
    if(amFile == NULL){
        printf("Error: cant open %s.am, skip file.",line.fileName);
        return;
    }

    /*goes line by line*/
    for(line.num = 1;fgets(temp_str,MAX_LINE_LENGTH+2,amFile)!=NULL;line.num++){
        if(!fpassLine(line,&ic,&dc)){
            isSuccess = FALSE;
        }
    }
    if(isSuccess){
        printf("IC: %ld, DC: %ld",ic,dc);
    }
    deleteMacroTable();
    free(line.fileName);
    fclose(amFile);
}
