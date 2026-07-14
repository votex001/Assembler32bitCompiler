#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "utils.h"
#include "file_utils.h"
#include "macro_table.h"
#include "preprocess.h"





void getSecondWord(cur_line line,char *secondWord,int *i);


/*if preproces is success we going to make .am and start process 1 and 2 else we skip file*/
bool preprocessFile(FILE *file,char *fileName){
    bool is_success = TRUE;
    bool is_in_macro = FALSE,skip_current_macro = FALSE;
    char macro_name[MAX_LINE_LENGTH];
    char temp_str[MAX_LINE_LENGTH + 2];
    
    /*creating am file and sending it to expand macros func*/ 
    FILE *amFile;
    cur_line line;
    line.code = temp_str;
    line.fileName = cutStr(fileName,".as");
    if(line.fileName == NULL){
        printf("error: cannot cut filename\n");
        return FALSE;
    }
    amFile = writeFile(line.fileName,".am");
  
    if(amFile == NULL){
        printf("%s.as: error: failed to create output file '%s.am'\n",line.fileName,line.fileName);
        return FALSE;
    }
    
    /*we going in file line by line*/
    for(line.num = 1;fgets(temp_str,MAX_LINE_LENGTH+2,file)!=NULL;line.num++){
        expandMacros(line,&is_success,&skip_current_macro,&is_in_macro,macro_name,amFile);
    }
    fclose(amFile);
    if(!is_success){
        deleteFile(line.fileName,".am");
    }
    free(line.fileName);
     
    
    return TRUE;
}


void expandMacros(cur_line line,bool *is_success,bool *skip_current_macro,bool *is_in_macro,char *macro_name,FILE *amFile){
    int i,j; /*pointer for strings*/
    char savedWord[MAX_LINE_LENGTH+2];
    char restOfLine[MAX_LINE_LENGTH+2];
    const char *macroContent;
    i = j = 0;/*init*/



    skipSpaces(line.code,&i);

    if(!line.code[i] || line.code[i] == '\n' || line.code[i] == ';')
        return;/*comment or empty string - skip*/

    
    if(!isFirstWordLabel(line,savedWord,&i) && isMacroExist(savedWord)){
        macroContent = getMacro(savedWord);
        fputs(macroContent,amFile);
        return;
    }
    if(strstr(line.code, "mcro") != NULL && strcmp(savedWord,"mcro") == 1){
        printf("%s.as:%ld: error: text before mcro declaration.\n",line.fileName,line.num);
        *is_success = FALSE;
        *skip_current_macro = TRUE;
        return;
    }

    /*searching for mcro start line*/
    if(strcmp(savedWord,"mcro") == 0){
        *is_in_macro = TRUE;
        
      
        getSecondWord(line,savedWord,&i);
        strcpy(macro_name, savedWord);
        skipSpaces(line.code,&i);
        if(line.code[i] != '\n'){
            printf("%s.as:%ld: error: text after mcro declaration.\n",line.fileName,line.num);
            *is_success = FALSE;
            *skip_current_macro = TRUE;
            return;
        }
        if(!isReservedWord(macro_name) && getMacro(macro_name) == NULL){

            
            appendMacroLine(macro_name,restOfLine);
        }else{
            printf("%s.as:%ld: error: mcro %s declareted already.\n",line.fileName,line.num,macro_name);
            *is_success = FALSE;
            *skip_current_macro = TRUE;
            return;
        }

        return;
    }
    /*skippin macro with error*/
    if(*is_in_macro && *skip_current_macro){
        if(strcmp(savedWord,"mcroend")==0){
            *is_in_macro = FALSE; 
        }else{
            return;
        }
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
        line.code[*i] != '\t' && 
        line.code[*i] != '\n' &&
        line.code[*i] != '\r')
    {
        secondWord[j++] = line.code[*i];
        (*i)++;
    }

    secondWord[j] = '\0';
}


