#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "utils.h"
#include "file_utils.h"
#include "macro_table.h"
#include "preprocess.h"





void getSecondWord(cur_line line,char *secondWord,int *i);
void getRestLine(cur_line line,char *restOfLine,int *i);


/*if preproces is success we going to make .am and start process 1 and 2 else we skip file*/
bool preprocessFile(FILE *file,char *fileName){
    bool is_success = TRUE;
    bool is_in_macro = FALSE;
    char macro_name[MAX_LINE_LENGTH];
    char temp_str[MAX_LINE_LENGTH + 2];
    int c;
    
    /*creating am file and sending it to expand macros func*/ 
    FILE *amFile;
    cur_line line;
    line.code = temp_str;
    line.fileName = cutStr(fileName,".as");
    if(line.fileName == NULL){
        printf("error: cannot cut filename\n");
        return FALSE;
    }
    amFile = writefile(line.fileName,".am");
  
    if(amFile == NULL){
        printf("%s.as: error: failed to create output file '%s.am'\n",line.fileName,line.fileName);
        return FALSE;
    }
    
    /*we going in file line by line*/
    for(line.num = 1;fgets(temp_str,MAX_LINE_LENGTH+2,file)!=NULL;line.num++){
        /*In this stage if we have some problem with writing am we just deleting it and move on*/
        if(!is_success){
            fclose(amFile);
            deleteFile(line.fileName,".am");
            return FALSE;
        }


        /*checking if line is to long*/
        if(strchr(temp_str,'\n')==NULL && !feof(file)){
            printf("%s.as:%ld: error: line is too long.Maximum line length is %d\n",line.fileName,line.num,MAX_LINE_LENGTH);
            is_success = FALSE;
            /*skip extra symbols*/
            while((c = fgetc(file))!='\n' && c!= EOF);
        }else {
            expandMacros(line,&is_success,&is_in_macro,macro_name,amFile);
        }
        

    }
    /*free all mallocs*/
    deleteMacroTable();
    free(line.fileName);
    fclose(amFile);
    return is_success;
}


void expandMacros(cur_line line,bool *is_success,bool *is_in_macro,char *macro_name,FILE *amFile){
    int i,j; /*pointer for strings*/
    char savedWord[MAX_LINE_LENGTH+2];
    char restOfLine[MAX_LINE_LENGTH+2];
    const char *macroContent;
    i = j = 0;/*init*/



    skipSpaces(line.code,&i);

    if(!line.code[i] || line.code[i] == '\n' || line.code[i] == ';')
        return;/*comment or empty string - skip*/

    /*if label has same name with macro*/
    if(isFirstWordLabel(line,savedWord,&i)){
        if(isMacroExist(savedWord)){
            printf("%s.as:%ld: error: label '%s' conflicts with macro name",line.fileName,line.num,savedWord);
            *is_success = FALSE;
            deleteMacroTable();
            return;
        }
    }else if(isMacroExist(savedWord)){
        macroContent = getMacro(savedWord);
        fputs(macroContent,amFile);
        return;
    }

    /*searching for mcro start line*/
    if(strcmp(savedWord,"mcro") == 0){
        *is_in_macro = TRUE;
        
      
        getSecondWord(line,savedWord,&i);
        strcpy(macro_name, savedWord);
        getRestLine(line,restOfLine,&i);

        appendMacroLine(macro_name,restOfLine);

        return;
    }

    /*copying macro line or exit macro cicle*/
    if(*is_in_macro){
        if(strcmp(savedWord,"mcroend")==0){
            *is_in_macro = FALSE; 
        }else{
            /*appendMacroLine in macro table by *macro_name*/
            appendMacroLine(macro_name,line.code);
        }
        return;
    }

    /*else just save line in .am*/
    fputs(line.code,amFile);
    
}


void getSecondWord(cur_line line,char *secondWord,int *i){
    int j;
    j = 0;
   
    skipSpaces(line.code,i);
    /*copy second word*/
    while(line.code[*i] &&line.code[*i] != ' ' &&
          line.code[*i] != '\t' && line.code[*i] != '\n')
    {
        secondWord[j++] = line.code[*i];
        (*i)++;
    }

    secondWord[j] = '\0';
}


void getRestLine(cur_line line,char *restOfLine,int *i){
    int j;
    /*copy rest of line*/
    for(j = 0;*i <= MAX_LINE_LENGTH && line.code[*i] != '\n';j++,(*i)++){
        restOfLine[j]=line.code[*i];
    }

    restOfLine[j]='\0';

}