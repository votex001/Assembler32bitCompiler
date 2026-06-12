#include <stdio.h>
#include <string.h>
#include "global.h"
#include "utils.h"
#include "file_utils.h"






bool expandMacros(cur_line line);


/*if preproces is success we going to make .am and start process 1 and 2 else we skip file*/
bool preprocess_file(FILE *file,char *fileName){
    bool is_success = TRUE;
    FILE *amFile
    char temp_str[MAX_LINE_LENGTH + 2];
    char c;
    cur_line line;
    line.code = temp_str;
    line.fileName = cutStr(fileName,".as");
    amFile = createFile(line.fileName,".am");

    if(fileName == NULL){
        printf("%s.as: error: failed to create output file '%s.am'\n",line.fileName,line.fileName);
        return FALSE;
    }
    
    /*we going in file line by line*/
    for(line.num = 1;fgets(temp_str,MAX_LINE_LENGTH+2,file)!=NULL;line.num++){
        
        /*checking if line is to long*/
        if(strchr(temp_str,'\n')==NULL){
            printf("%s.as:%ld: error: line is too long.Maximum line length is %d",fileName,line.num,MAX_LINE_LENGTH);
            is_success = FALSE;
            /*skip extra symbols*/
            while((c = fgetc(file))!='\n' && c!= EOF);
            /*TODO: create expandMacros*/
        }else if(!expandMacros(line)){
            is_success = FALSE;
        }
        

    }


    return is_success;
}


bool expandMacros(cur_line line){
    int i; /*pointer for string*/
    char firstWord[MAX_LINE_LENGTH];


    skipSpaces(line.code,&i);

    if(!line.code[i] || line.code[i] == '\n' || line.code[i] == EOF || line.code[i] == ';')
        return TRUE;/*comment or empty string - skip*/
    /*TODO: add every legal line to amFile if get false delete amFIle*/
    if(firstWordIsLabel(line,firstWord)){

        /*TODO: in utils create is_legal_label*/
        if(!is_legal_label(firstWord)){

            printf("%s.as:%ld: error: illegal label - "
                "label name must start with an alphabetic character "
                "and cannot be a reserved word",
                line.fileName,
                line.num);

            return FALSE;
        /*TODO: in tables create macroExists*/
        }else if(macroExists(firstWord)){

            printf("%s.as:%ld: error: label '%s' conflicts with an existing macro name",line.fileName,line.num,firstWord);

            return FALSE;
        }
    }
    /*TODO: if it macro dont add to file and save it in tables*/
    if(strcmp(firstWord,"mcro")){

    }
    
        return TRUE;/*end func no errors found*/
    
}


