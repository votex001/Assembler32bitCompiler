#include <stdio.h>
#include <string.h>
#include "global.h"
#include "utils.h"
#include "file_utils.h"
#include "preprocess.h"





void getSecondWord();
void getRestLine();


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
    int i,j; /*pointer for strings*/
    i = j = 0;/*init*/
    char savedWord[MAX_LINE_LENGTH+2];
    char restOfLine[MAX_LINE_LENGTH+2];



    skipSpaces(line.code,&i);

    if(!line.code[i] || line.code[i] == '\n' || line.code[i] == ';')
        return;/*comment or empty string - skip*/

    if(isFirstWordLabel(line,savedWord,&i)){
        /*TODO:check if macro_exists*/
    }

    if(*is_in_macro){
        if(strcmp(&savedWord,"mcroend")==0){
            *is_in_macro = FALSE; 
            
        }else{
            /*appendMacroLine in macro table by *macro_name*/
            appendMacroLine(savedWord,line.code);
        }
        
        return;
    }


    if(strcmp(savedWord,"mcro") == 0){
        *is_in_macro = TRUE;
        
        getSecondWord(line,savedWord);
        getRestLine(line,restOfLine,&i);


        
        /*TODO:save second word in macro_name and end of string*/
        return;
    }

    /*TODO:check if first ord is macro*/
    
}


void getSecondWord(cur_line line,char *secondWord,int *i){
    int j;
    j = 0;
    skipSpaces(line.code,i);
    /*skip first word*/
    while(line.code[*i] != ' ' && *i <= MAX_LINE_LENGTH){(*i)++;}
    skipSpaces(line.code,i);
    /*copy second word*/
    for(;*i <=MAX_LINE_LENGTH && line.code[*i]&& line.code[*i] != ':' && line.code[*i] != ' ';j++,(*i)++){
        secondWord[j] = line.code[*i];
    }

    secondWord[j] = '\0';
};
void getRestLine(cur_line line,char *restOfLine,int *i){
    int j;
    /*copy rest of line*/
    for(j = 0;line.code[*i] != '\n' && *i <= MAX_LINE_LENGTH;j++,(*i)++){
        restOfLine[j]=line.code[*i];
    }

    restOfLine[j]='\0';

};