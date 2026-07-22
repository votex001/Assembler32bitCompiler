
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "first_pass.h"
#include "utils.h"
#include "code.h"
#include "process_tables.h"
#include "macro_table.h"

bool isLabelBefore = FALSE;
char savedLabelName[MAX_LINE_LENGTH+2];



bool fpassLine(cur_line line,long *ic,long *dc){
    int i = 0;/*char 0 in line*/
    char firstWord[MAX_LINE_LENGTH + 2];
    char nextWord[MAX_LINE_LENGTH + 2];
    /*instruction*/
    opcode opcode;
    funct funct;
    int fParam,sParam,tParam;
    long immed;
    

    /*data*/
    directive directive;
    int size;
    

    
    
    /*skip every empty char*/
    skipSpaces(line.code,&i);

    /*comment line or empty line skip*/
    if(isEmptyStr(line.code,i)){
        return TRUE;
    }
    if(isLabelBefore){
        isLabelBefore = FALSE;
        if(isNextWordLabel(line,firstWord,&i)){
            printf("%s.as:%ld: error: consecutive labels are not allowed.\n",line.fileName,line.num);
            return FALSE;
        }
        /*need to be reserved word but not register*/
        if(!isReservedWord(firstWord)){
            printf("%s.as:%ld: error: invalid command '%s'\n",line.fileName,line.num,firstWord);
            return FALSE;
        }else if(getRegisterNum(firstWord) != -1){
            printf("%s.as:%ld: error: invalid command '%s'\n",line.fileName,line.num,firstWord);
            return FALSE;
        }
        getDirectiveByName(savedLabelName,&directive,&size);
        getFuncOp(savedLabelName,&funct,&opcode);
        if(directive >= DH_DIR  && directive <= ASCIZ_DIR){
            saveSymbols(savedLabelName,TRUE,*dc);
        }else if(opcode != NONE_OP){
            saveSymbols(savedLabelName,FALSE,*ic);
        }
        
        i=0;/*reseting i after cheking if it label or not*/
    }
    /*first word will be label or not but it will be saved in firstWord*/
    if(isNextWordLabel(line,firstWord,&i)){
        
        if(!isValidLabel(firstWord)){
                printf("%s.as:%ld: error: invalid label '%s'. ",line.fileName,line.num,firstWord);
                printf("Rules: 1-31 chars, starts with letter, not a reserved word\n");
                return FALSE;
        }
        if(getSymbol(firstWord)!=NULL){
            printf("%s.as:%ld: error: label '%s' is already declared.\n",line.fileName,line.num,firstWord);
            return FALSE;
        }
        if(isExternExist(firstWord)){
            printf("%s.as:%ld: error: external label '%s' is already declared ",line.fileName, line.num, firstWord);
            printf("and cannot be defined locally.\n");
            return FALSE;
        }
        if(isMacroExist(firstWord)){
            printf("%s.as:%ld: error: label %s conflicts with an existing macro name.\n",line.fileName,line.num,firstWord);
            return FALSE;
        }
       

        if(isEmptyStr(line.code,i)){
            strcpy(savedLabelName,firstWord);
            isLabelBefore = TRUE;
            return TRUE;
        }

        /*getting word after */
        getNextWord(line,nextWord,&i);
        
        /*need to be reserved word but not register*/
        if(!isReservedWord(nextWord)){
            printf("%s.as:%ld: error: invalid command '%s'\n",line.fileName,line.num,nextWord);
            return FALSE;
        }else if(getRegisterNum(nextWord)!=-1){
            printf("%s.as:%ld: error: invalid command '%s'\n",line.fileName,line.num,nextWord);
            return FALSE;
        }

        /*getting now info about comman from second word*/
        getDirectiveByName(nextWord,&directive,&size);
        getFuncOp(nextWord,&funct,&opcode);
        /*if non dererctive or extern or entry just pass*/
        if(directive >= DH_DIR  && directive <= ASCIZ_DIR){
            saveSymbols(firstWord,TRUE,*dc);
        }else if(opcode != NONE_OP){
            saveSymbols(firstWord,FALSE,*ic);
        }

        if(directive == EXTERN_DIR || directive == ENTRY_DIR){
            printf("%s.as:%ld: warning: label '%s' before directive '%s' is ignored.\n",line.fileName, line.num, firstWord, nextWord);
        }

        /*after we saved label we need to move first word to second one (command)*/
        strcpy(firstWord,nextWord);
    }
    
    /*need to be reserved word but not register*/
    if(!isReservedWord(firstWord)){
        printf("%s.as:%ld: error: invalid command '%s'\n",line.fileName,line.num,firstWord);
        return FALSE;
    }else if(getRegisterNum(firstWord)!=-1){
        printf("%s.as:%ld: error: invalid command '%s'\n",line.fileName,line.num,firstWord);
        return FALSE;
    }
    /*getting now info about command from first word*/
    getDirectiveByName(firstWord,&directive,&size);
    getFuncOp(firstWord,&funct,&opcode);

    /*if instruction we check what kind of command and save*/
    if(opcode != NONE_OP){

        /*HLT without params*/
        if(opcode == HLT_OP){
            /*check extra text*/
            if(isTextAfterCommand(line,&i,firstWord))return FALSE;
            saveJTypeInst(opcode,0,NULL,0,*ic,line.num);
            *ic+=4;
            return TRUE;
        }


        /*get first param*/
        getNextWord(line,nextWord,&i);
        fParam = getRegisterNum(nextWord);

        
        

        /*J instructions only 1 param*/
        if(opcode >=30){

            /*separate check jmp with register*/ 
            if(opcode == JMP_OP && fParam != -1){
                if(isTextAfterCommand(line,&i,firstWord))return FALSE;
                /*save*/
                saveJTypeInst(opcode,TRUE,NULL,fParam,*ic,line.num);
                *ic+=4;
                return TRUE;
            }
            
            /*any jump with no register*/
            else{
                if(isTextAfterCommand(line,&i,firstWord))return FALSE;
                if(!isValidLabel(nextWord)){
                    printf("%s.as:%ld: error: invalid label '%s'. ",line.fileName,line.num,nextWord);
                    printf("Rules: 1-31 chars, starts with letter, not a reserved word\n");
                    return FALSE;
                }

                saveJTypeInst(opcode,FALSE,nextWord,0,*ic,line.num);
                *ic+=4;
                return TRUE;
            }
        }
        /*every command after this stage need to be minimum with 2 params and first is register*/
        if(!isCommaNext(line,&i,firstWord,nextWord))return FALSE;
        if(fParam == -1){
            printf("%s.as:%ld: error: argument %s is not register as aspected.\n",line.fileName,line.num,nextWord);
            return FALSE;
        }

        /*get second param*/
        getNextWord(line,nextWord,&i);
        sParam = getRegisterNum(nextWord);
        
        
        /*instruction with 2 params*/
        if(opcode == MOVE_OP){
            /*second argument might be register and no text after*/
            if(sParam == -1){
                printf("%s.as:%ld: error: argument %s is not register as aspected.\n",line.fileName,line.num,nextWord);
                return FALSE;
            }
            if(isTextAfterCommand(line,&i,firstWord))return FALSE;

            saveRTypeInst(opcode,fParam,0,sParam,funct,*ic);
            *ic+=4;
            return TRUE;
        }

        /*every command after this stage need to be minimum with 3 params*/
        if(!isCommaNext(line,&i,firstWord,nextWord))return FALSE;
        /*is first 2 need to be registers*/
        if(sParam == -1 && (opcode == ADD_OP || (opcode >=15 && opcode <= 18))){
                printf("%s.as:%ld: error: argument %s is not register as aspected.\n",line.fileName,line.num,nextWord);
                return FALSE;
            }
        /*second param might be integer 2-byte size*/
        if((opcode >= 10 && opcode <= 14) || (opcode >= 19 && opcode <= 24)){
            immed = strtol(nextWord,NULL,10);
            if(!is_int(nextWord)){
                printf("%s.as:%ld: error: %s argument might be integer.\n",line.fileName,line.num,nextWord);
                return FALSE;
            }else if(!checkRange(immed,2)){
                printf("%s.as:%ld: error: value %s is out of range for 2-byte integer.\n",line.fileName,line.num,nextWord);
                return FALSE;
            }
        }
        /*get third param*/
        getNextWord(line,nextWord,&i);
        tParam = getRegisterNum(nextWord);


        /*instructions with 3 params and first 2 is registers*/
        if(opcode == ADD_OP || (opcode >=15 && opcode <= 18)){
            /*every mathemathical without immed*/
            if(opcode == ADD_OP){
                /*for this command 3rd param also register*/
                if(tParam == -1){
                    printf("%s.as:%ld: error: argument %s is not register as aspected.\n",line.fileName,line.num,nextWord);
                    return FALSE;
                }
                if(isTextAfterCommand(line,&i,firstWord))return FALSE;
                saveRTypeInst(opcode,fParam,sParam,tParam,funct,*ic);
                *ic+=4;
                return TRUE;
            }else{
                /*third param need to bee label and not reserved word*/
                if(isReservedWord(nextWord)){
                    printf("%s.as:%ld: error: argument %s is reserved word.\n",line.fileName,line.num,nextWord);
                    return FALSE;
                }
                if(!isValidLabel(nextWord)){
                    printf("%s.as:%ld: error: invalid label '%s'.",line.fileName,line.num,nextWord);
                    printf("Rules: 1-31 chars, starts with letter, not a reserved word\n");
                    return FALSE;
                }
                if(isTextAfterCommand(line,&i,firstWord))return FALSE;

                saveITypeInst(opcode,TRUE,nextWord,fParam,sParam,0,*ic,line.num);
                *ic+=4;
                return TRUE;
            }
        }
        /*I instruction with immed - 3rd param is register*/
        if((opcode >= 10 && opcode <= 14) || (opcode >= 19 && opcode <= 24)){

            
            if(tParam == -1){
                printf("%s.as:%ld: error: register '%s' is no valid might be $0-31\n",line.fileName,line.num,nextWord);
                return FALSE;
            }
            
            /*check extra text*/
            skipSpaces(line.code,&i);
            if(!isEmptyStr(line.code,i)){
                printf("%s.as:%ld: error: extra text after command %s\n",line.fileName,line.num,firstWord);
                return FALSE;
            }
            saveITypeInst(opcode,FALSE,NULL,fParam,tParam,immed,*ic,line.num);
            *ic+=4;
            return TRUE;
        }
            
    }
    
    if(directive >= DH_DIR && directive <= ASCIZ_DIR){
        while(!isEmptyStr(line.code,i)){
            /*get word*/
            getNextWord(line,nextWord,&i);
            skipSpaces(line.code,&i);
            /*saving and adding dc*/
            if(!saveDataCode(nextWord,directive,size,dc,line)) return FALSE;
            /*check if end of line*/
            if(isEmptyStr(line.code,i))return TRUE;
            /*if not end of line check if has comma*/
            if(!isCommaNext(line,&i,firstWord,nextWord))return FALSE;
        }
    }

    if(directive == EXTERN_DIR){
        getNextWord(line,nextWord,&i);
        if(!isValidLabel(nextWord)){
                printf("%s.as:%ld: error: invalid label '%s'. ",line.fileName,line.num,nextWord);
                printf("Rules: 1-31 chars, starts with letter, not a reserved word\n");
                return FALSE;
        }
        if(getSymbol(nextWord) != NULL){
            printf("%s.as:%ld: error: label '%s' is already declared",line.fileName, line.num, nextWord);
            printf("and cannot be declared as external.\n");
            return FALSE;
        }
        saveExtern(nextWord);
        return TRUE;
    }

    

    
    return TRUE;
}