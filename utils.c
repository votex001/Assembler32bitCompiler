#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "global.h"
#include "utils.h"
#include "code.h"

/**
 * Allocates memory and check if it successful.
 * @param size Size of memory ro allocate.
 * @return Pointer to allocated memory. 
 */
void *mallocWithCheck(long size){
    void *ptr = malloc(size);
    if(ptr == NULL){
        printf("Error: fatal memory allocation failed.");
        exit(1);
    }
    return ptr;
}

/**
 * Reallocates memory and check if it successful.
 * @param pointer to previous location of memory.
 * @param size Size of memory ro reallocate.
 * @return Pointer to reallocated memory. 
 */
void *reallocWithCheck(void *ptr,long size){

    void *new_ptr = realloc(ptr, size);

    if(new_ptr == NULL){
        printf("Error: fatal memory allocation failed.");
        exit(1);
    }

    return new_ptr;
}

/**
 * Cuts from end of str1 - str2.
 * @param str1 String to cut from.
 * @param str2 String to cut.
 * @return Result.
 */
char *cutStr(const char *str1,const char *str2){
    char *res;
    size_t strLen1 = strlen(str1);
    size_t strLen2 = strlen(str2);

    if(strLen1 < strLen2){
        return NULL;
    }

    res = mallocWithCheck(strLen1 - strLen2 + 1);
    strncpy(res,str1,strLen1 - strLen2);
    res[strLen1 - strLen2] = '\0';
    return res;
}

/**
 * Checking if file if .as format
 * @param fileName Full name of file to check
 */
bool isCorrectFileName(char *fileName){
    int len = strlen(fileName);
    if(len < 3)
        return FALSE;

    return fileName[len-3] == '.' &&
           fileName[len-2] == 'a' &&
           fileName[len-1] == 's';

}

/**
 * Skips at line all spaces and tabs
 * @param str String line.
 * @param i Counter after function will be at first non white char.
 */
void skipSpaces(char *str, int *i){
    while(str[*i] && (str[*i]=='\t' || str[*i] == ' ' || str[*i]=='\r'))
    (*i)++;
}

/**
 * Check if word is label
 * @param line Line information.
 * @param nextWord Word output.
 * @param i Counter will on first char after word.
 * @return If word is label.
 */
bool isNextWordLabel(const cur_line line,char *nextWord,int *i){
    int j;
    j = 0;
    skipSpaces(line.code,i);

    for(;*i <=MAX_LINE_LENGTH && line.code[*i]&& 
        line.code[*i] != ':' && 
        line.code[*i] != ' '&& 
        line.code[*i] != '\n' && 
        line.code[*i] != '\r';j++,(*i)++){
        nextWord[j] = line.code[*i];
    }

    nextWord[j] = '\0';
    
    if(line.code[*i] == ':'){
        (*i)++;/*skip symbol*/
        return TRUE;
    }
    return FALSE;


}

/**
 * Validate label
 * @param label Label word.
 * @return If it valid.
 */
bool isValidLabel(const char *label){

    /*page 35*/
    return label[0] && strlen(label) <= 31 && isalpha(label[0]) && isAlphanumeric(label+1) && !isReservedWord(label);

}

/**
 * Checks if word is build from characters or numbers.
 * @param str Word.
 * @return If it Alphanumeric.
 */
bool isAlphanumeric(const char *word){
    int i;

    for(i = 0; word[i];i++){
        if(!isalpha(word[i]) && !isdigit(word[i]))
            return FALSE;
    }

    return TRUE;
}

/**
 * Checks if it register instruction or directive
 * @param word Word to check.
 * @return Is reserved word.
 */
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

/**
 * Checks if in string white characters only.
 * @param str string to check.
 * @param i Start place to check from.
 * @return if no found something else.
 */
bool isEmptyStr(char *str,int i){
    if(!str[i] || str[i] == '\n' || str[i] == ';' || str[i] == '\r'){
        return TRUE;
    }
    return FALSE;
}

/**
 * Check if string is number
 * @param string String to check.
 * @return If only numbers there.
 */
bool is_int(char *string) {
	int i = 0;
    /*page 35*/
	if (string[0] == '-' || string[0] == '+') string++; 
	for (; string[i]; i++) {
		if (!isdigit(string[i])) {
			return FALSE;
		}
	}
	return i > 0; 
}


/**
 * Saving word after i from line.
 * @param line Line info.
 * @param nextWord Output word.
 * @param i Counter when to start copy word from.
 */
void getNextWord(cur_line line,char *nextWord,int *i){
    int j;
    j = 0;
   
    skipSpaces(line.code,i);
    /*copy second word*/
    while(j<MAX_LINE_LENGTH + 1 && line.code[*i]&& line.code[*i] != ',' &&line.code[*i] != ' ' &&
        line.code[*i] != '\t' && 
        line.code[*i] != '\n' &&
        line.code[*i] != '\r')
    {
        nextWord[j++] = line.code[*i];
        (*i)++;
    }

    nextWord[j] = '\0';
}

/**
 * Checking next non white char if comma.
 * @param line Line information.
 * @param i Counter to start searching from it.
 * @param commandName Command name.
 * @param lastParam Parameter before expected comma.
 * @return If found comma.
 */
bool isCommaNext(cur_line line,int *i,char *commandName,char *lastParam){
    skipSpaces(line.code,i);
    if(isEmptyStr(line.code,*i) || line.code[*i] != ','){
        printf("%s.as:%ld: error: miss parameters for ",line.fileName,line.num);
        printf("command '%s', expected ',' after %s.\n",commandName,lastParam);
        return FALSE;
    }    
    /*we got comma and skip it*/
    (*i)++;
    return TRUE;       
}
/**
 * Checking if there some nonwhite characters after counter.
 * @param line Line info.
 * @param i Counter.
 * @param commandName Name of command.
 * @return If found nonwhite character.
 */
bool isTextAfterCommand(cur_line line,int *i,char* commandName){
    skipSpaces(line.code,i);
    if(!isEmptyStr(line.code,*i)){
        printf("%s.as:%ld: error: extra text after command %s\n",line.fileName,line.num,commandName);
        return TRUE;
    }
    return FALSE;
}