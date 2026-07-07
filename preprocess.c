#include <stdio.h>
#include <string.h>
#include "global.h"
#include "utils.h"
#include "file_utils.h"
#include "preprocess.h"









/*if preproces is success we going to make .am and start process 1 and 2 else we skip file*/
bool preprocessFile(FILE *file,char *fileName){
    bool is_success = TRUE;
    bool is_in_macro = FALSE;
    char *macro_name[MAX_LINE_LENGTH];
    char temp_str[MAX_LINE_LENGTH + 2];
    char c;
    
    /*creating am file and sending it to expand macrof func*/ 
    FILE *amFile;
    cur_line line;
    line.code = temp_str;
    line.fileName = cutStr(fileName,".as");
    amFile = createFile(line.fileName,".am");
  
    if(amFile == NULL){
        printf("%s.as: error: failed to create output file '%s.am'\n",line.fileName,line.fileName);
        return FALSE;
    }
    
    /*we going in file line by line*/
    for(line.num = 1;fgets(temp_str,MAX_LINE_LENGTH+2,file)!=NULL;line.num++){
        /*In this stage if we have some problem with writing am we just deleting it and move on*/
        if(!is_success){
            fclose(amFile);
            deleteFile(amFile);
            return FALSE;
        }


        /*checking if line is to long*/
        if(strchr(temp_str,'\n')==NULL){
            printf("%s.as:%ld: error: line is too long.Maximum line length is %d",fileName,line.num,MAX_LINE_LENGTH);
            is_success = FALSE;
            /*skip extra symbols*/
            while((c = fgetc(file))!='\n' && c!= EOF);
            
        }else {
            expandMacros(line,&is_success,&is_in_macro,macro_name);
        }
        

    }

    fclose(amFile);
    return is_success;
}


void expandMacros(cur_line line,bool *is_success,bool *is_in_macro,char *macro_name){
    int i = 0; /*pointer for string*/
    char firstWord[MAX_LINE_LENGTH+1];



    skipSpaces(line.code,&i);

    if(!line.code[i] || line.code[i] == '\n' || line.code[i] == EOF || line.code[i] == ';')
        return TRUE;/*comment or empty string - skip*/

    if(isFirstWordLabel(line,&firstWord)){
        /*check if macro_exists*/
    }

    if(*is_in_macro){
        if(strcmp(&firstWord,"mcroend")){
            *is_in_macro = FALSE; 
            
        }else{

            /*appendMacroLine in macro table by *macro_name*/
        }
        
        return;
    }


    if(strcmp(firstWord,"mcro")){
        *is_in_macro = TRUE;
        /*save second word in macro_name*/
        return;
    }








    
}


