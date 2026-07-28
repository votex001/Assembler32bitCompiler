#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "utils.h"
#include "file_utils.h"
#include "macro_table.h"
#include "preprocess.h"
#include "original_file_table.h"







/**
 * Preprocesses file with .as extension.
 * Opening all mcros and making .am file to pass one and two.
 * @param fileName File name with extension.
 * @return Success status.
 */
bool preprocessFile(char *fileName){
    bool is_success = TRUE;
    bool is_in_macro = FALSE,skip_current_macro = FALSE;
    char macro_name[MAX_LINE_LENGTH];
    char temp_str[MAX_LINE_LENGTH + 2];
    FILE *amFile;
    FILE *asFile;
    cur_line line;
    line.code = temp_str;
    line.fileName = cutStr(fileName,".as");

    
    
    asFile = readFile(line.fileName,".as");

    if(asFile == NULL){
        printf("Error: file %s is inaccessible for reading.Skipped.\n",fileName);
        return FALSE;
    }
    
    if(line.fileName == NULL){
        printf("Error: cannot cut filename\n");
        return FALSE;
    }
    amFile = writeFile(line.fileName,".am");
  
    if(amFile == NULL){
        printf("%s.as: Error: failed to create output file '%s.am'\n",line.fileName,line.fileName);
        return FALSE;
    }
    
    /*we going in file line by line*/
    for(line.num = 1;fgets(temp_str,MAX_LINE_LENGTH+2,asFile)!=NULL;line.num++){
        if (strchr(temp_str, '\n') == NULL && !feof(asFile)) {
			printf("%s.as: error: Line too long to process. Maximum line length should be %d.",line.fileName,MAX_LINE_LENGTH);
            is_success = FALSE;
        }
        if(!expandMacros(line,&skip_current_macro,&is_in_macro,macro_name,amFile)){
            is_success = FALSE;
        }
    }
    fclose(amFile);
    if(!is_success){
        deleteFile(line.fileName,".am");
    }
    free(line.fileName);
     
    
    return is_success;
}


/**
 * Checks whether the current line contains a macro call or declaration.
 * Expands existing macros and write .am (file with no macro lines).
 * @param line Code line with information about file and num of line.
 * @param skip_current_macro If found some problems inside macro skipin it.
 * @param is_in_macro If in macro just saving lines of code.
 * @param macro_name When in macro checking the name of it.
 * @param amFile Output file.
 * @return Success status per line.
 */
bool expandMacros(cur_line line,bool *skip_current_macro,bool *is_in_macro,char *macro_name,FILE *amFile){
    int i,j; /*pointer for strings*/
    char savedWord[MAX_LINE_LENGTH+2];
    char restOfLine[MAX_LINE_LENGTH+2];
    const char *macroContent;
    i = j = 0;/*init*/
    
    /*saving number of line*/
    saveLineNum(line);

    /*skip white chars*/
    skipSpaces(line.code,&i);

    /*skip empti string no errors or code*/
    if(isEmptyStr(line.code,i)){
        fputs(line.code,amFile);
        return TRUE;/*comment or empty string - skip*/
    }

    /*Check if we need to append macro in code*/
    if(!isNextWordLabel(line,savedWord,&i) && isMacroExist(savedWord)){
        macroContent = getMacro(savedWord);
        fputs(macroContent,amFile);
        return TRUE;
    }
    /*if found mcro in command but text before*/
    if(strstr(line.code, "mcro") != NULL && strcmp(savedWord,"mcro") != 0 && strcmp(savedWord,"mcroend") == 1){
        printf("%s.as:%ld: error: text before mcro declaration.\n",line.fileName,line.num);
        *skip_current_macro = TRUE;
        return FALSE;
    }

    /*searching for mcro start line*/
    if(strcmp(savedWord,"mcro") == 0){
        *is_in_macro = TRUE;/*from now we are copy macro code until we dont found mcroend*/
        
        
        getNextWord(line,savedWord,&i);/*name of macro*/
        strcpy(macro_name, savedWord);/*copy it to macro name to still know the name of it*/
        skipSpaces(line.code,&i);/*white chars*/

        /*page 32*/
        if(line.code[i] != '\n'){
            printf("%s.as:%ld: error: text after mcro declaration.\n",line.fileName,line.num);
            *skip_current_macro = TRUE;
            return FALSE;
        }
        if(!isReservedWord(macro_name) && !isMacroExist(macro_name)){
            appendMacroLine(macro_name,restOfLine);
        }else{
            printf("%s.as:%ld: error: mcro %s declareted already.\n",line.fileName,line.num,macro_name);
            *skip_current_macro = TRUE;
            return FALSE;
        }
        /*getting out in the end of macro line*/
        return TRUE;
    }
    /*skippin macro with error*/
    if(*is_in_macro && *skip_current_macro){
        if(strcmp(savedWord,"mcroend")==0){
            *is_in_macro = FALSE;
            *skip_current_macro = FALSE;

            /*page 32*/
            skipSpaces(line.code,&i);
            if(!isEmptyStr(line.code,i)){
                printf("%s.as:%ld: error: text after mcro end.\n",line.fileName,line.num);
                return FALSE;
            }
            
        }
            return TRUE;
    }

    /*copying macro line or exit macro cicle*/
    if(*is_in_macro){
        if(strcmp(savedWord,"mcroend")==0){
            *is_in_macro = FALSE;
            
            /*page 32*/
            skipSpaces(line.code,&i);
            if(!isEmptyStr(line.code,i)){
                printf("%s.as:%ld: error: text after mcro end.\n",line.fileName,line.num);
                return FALSE;
            }

        }else{
            /*appendMacroLine in macro table by *macro_name*/
            appendMacroLine(macro_name,line.code);
        }
        return TRUE;
    }

    /*else just save line in .am*/
    fputs(line.code,amFile);
    return TRUE;
}




