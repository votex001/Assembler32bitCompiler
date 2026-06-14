#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "global.h"
#include "utils.h"
#include "code.h"




/*cuts from str1 str2*/
char *cutStr(const char *str1,const char *str2){
    char *res;
    size_t strLen1 = strlen(str1);
    size_t strLen2 = strlen(str2);

    if(strLen1 < strLen2){
        return NULL;
    }

    res = malloc(strLen1 - strLen2 + 1);
    strncpy(res,str1,strLen1 - strLen2);
    res[strLen1 - strLen2] = '\0';
    return res;
}

/*Checking if file if .as format*/
bool isCorrectFileName(char *fileName){
    int len = strlen(fileName);
    if(len < 3)
        return FALSE;

    return fileName[len-3] == '.' &&
           fileName[len-2] == 'a' &&
           fileName[len-1] == 's';

}

/*skips at line all spaces and tabs*/
void skipSpaces(char *str, int *i){
    while(str[*i] && (str[*i]=='\t' || str[*i] == ' '))
    (*i)++;
}

bool firstWordIsLabel(const cur_line line,char *firstWord){
    int i,j;
    j = i = 0;
    skipSpaces(line.code,&i);

    for(;line.code[i] && line.code[i] != EOF && line.code[i] != ':' && i <=MAX_LINE_LENGTH;j++,i++){
        firstWord[j] = line.code[i];
    }

    firstWord[j] = '\0';

    if(line.code[i] == ':'){
        return TRUE;
    }
    return FALSE;
    


}


bool isValidLabel(const char *label){

    
    return label[0] && strlen(label) <= 31 && isalpha(label[0]) && isAlphanumeric(label+1) && !isReservedWord(label);

}

bool isAlphanumeric(const char *str){
    int i;

    for(i = 0; str[i];i++){
        if(!isalpha(str[i]) && !isdigit(str[i]))
            return FALSE;
    }

    return TRUE;
}

bool isReservedWord(const char *word){
    funct func;
    opcode op;
    directive dir;
    int size;

    getFuncOp(word,&func,&op);
    getDirectiveByName(word,&dir,&size);
    /*check if derective or instruction command or register*/
    if(op != NONE_OP || dir != NONE_DIR || getRegisterNum(word) != -1)
        return TRUE;

    return FALSE;
}   

