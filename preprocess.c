#include <stdio.h>
#include <string.h>
#include "global.h"









/*if preprocces is succes we going to make .am and start procces 1 and 2 else we skip file*/
bool preprocess_file(FILE *file,char *fileName){
    bool is_success = true;
    char temp_str[MAX_LINE_LENGTH + 2];
    char c;
    cur_line line;
    line.code = temp_str;
    /*we going in file line by line*/
    for(line.num = 1;fgets(temp_str,MAX_LINE_LENGTH+2,file)!=NULL;line.num++){
        
        /*checking if line is to long*/
        if(strchr(temp_str,'\n')==NULL){
            printf("%s:%ld: error: line is too long.Maximum line length is %d",fileName,line.num,MAX_LINE_LENGTH);
            is_success = false;
            /*skip extra symbols*/
            while((c = fgetc(file))!='\n' && c!= EOF);
        }
        

    }


    return is_success;
}


/*bool expandMacros(cur_line line){
    int i;*//*pointer for string*/
/*}*/


