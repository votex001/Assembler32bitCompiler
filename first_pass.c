
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "first_pass.h"
#include "utils.h"
#include "code.h"
#include "process_tables.h"





bool fpassLine(cur_line line,long *ic,long *dc){
    int i = 0;/*char 0 in line*/
    char firstWord[MAX_LINE_LENGTH + 2];
    char nexWord[MAX_LINE_LENGTH + 2];
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
    /*first word will be label or not but it will be saved in firstWord*/
    if(isNextWordLabel(line,firstWord,&i)){
        
        if(!isValidLabel(firstWord)){
                printf("%s.as:%ld: error: invalid label '%s'. ",line.fileName,line.num,firstWord);
                printf("Rules: 1-31 chars, starts with letter, not a reserved word\n");
                return FALSE;
        }
        /*getting word after */
        getNextWord(line,nexWord,&i);

        if(!isReservedWord(nexWord)){
            printf("%s.as:%ld: error: invalid comand '%s'\n",line.fileName,line.num,nexWord);
            return FALSE;
        }

        /*getting now info about comman from second word*/
        getDirectiveByName(nexWord,&directive,&size);
        getFuncOp(nexWord,&funct,&opcode);
        if(directive != NONE_DIR){
            saveSymbols(firstWord,TRUE,*dc);
        }else if(opcode != NONE_OP){
            saveSymbols(firstWord,FALSE,*ic);
        }

        /*after we saved label we need to move first word to second one (command)*/
        strcpy(firstWord,nexWord);
    }
    

    if(!isReservedWord(firstWord)){
        printf("%s.as:%ld: error: invalid comand '%s'\n",line.fileName,line.num,firstWord);
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
            saveJTypeInst(opcode,0,NULL,0,*ic);
            *ic+=4;
            return TRUE;
        }


        /*get first param*/
        getNextWord(line,nexWord,&i);
        fParam = getRegisterNum(nexWord);

        /*J instructions only 1 param*/
        if(opcode >=30){

            /*separate check jmp with register*/ 
            if(opcode == JMP_OP && fParam != -1){
                if(isTextAfterCommand(line,&i,firstWord))return FALSE;
                /*save*/
                saveJTypeInst(opcode,TRUE,NULL,fParam,*ic);
                *ic+=4;
                return TRUE;
            }
            
            /*any jump with no register*/
            else{
                if(isTextAfterCommand(line,&i,firstWord))return FALSE;

                saveJTypeInst(opcode,FALSE,nexWord,0,*ic);
                *ic+=4;
                return TRUE;
            }
        }
        /*every command after this stage need to be minimum with 2 params and first is register*/
        if(!isCommaNext(line,&i,firstWord,nexWord))return FALSE;
        if(fParam == -1){
            printf("%s.as:%ld: error: argument %s is not register as aspected.\n",line.fileName,line.num,nexWord);
            return FALSE;
        }

        /*get second param*/
        getNextWord(line,nexWord,&i);
        sParam = getRegisterNum(nexWord);
        
        
        /*instruction with 2 params*/
        if(opcode == MOVE_OP){
            /*second argument might be register and no text after*/
            if(sParam == -1){
                printf("%s.as:%ld: error: argument %s is not register as aspected.\n",line.fileName,line.num,nexWord);
                return FALSE;
            }
            if(isTextAfterCommand(line,&i,firstWord))return FALSE;

            saveRTypeInst(opcode,fParam,0,sParam,funct,*ic);
            *ic+=4;
            return TRUE;
        }

        /*every command after this stage need to be minimum with 3 params*/
        if(!isCommaNext(line,&i,firstWord,nexWord))return FALSE;
        /*is first 2 need to be registers*/
        if(sParam == -1 && (opcode == ADD_OP || (opcode >=15 && opcode <= 18))){
                printf("%s.as:%ld: error: argument %s is not register as aspected.\n",line.fileName,line.num,nexWord);
                return FALSE;
            }
        /*second param might be integer 2-byte size*/
        if((opcode >= 10 && opcode <= 14) || (opcode >= 19 && opcode <= 24)){
            immed = strtol(nexWord,NULL,10);
            if(!is_int(nexWord)){
                printf("%s.as:%ld: error: %s argument might be integer.\n",line.fileName,line.num,nexWord);
                return FALSE;
            }else if(!checkRange(immed,2)){
                printf("%s.as:%ld: error: value %s is out of range for 2-byte integer.\n",line.fileName,line.num,nexWord);
                return FALSE;
            }
        }
        /*get third param*/
        getNextWord(line,nexWord,&i);
        tParam = getRegisterNum(nexWord);


        /*instructions with 3 params and first 2 is registers*/
        if(opcode == ADD_OP || (opcode >=15 && opcode <= 18)){
            /*every mathemathical without immed*/
            if(opcode == ADD_OP){
                /*for this command 3rd param also register*/
                if(tParam == -1){
                    printf("%s.as:%ld: error: argument %s is not register as aspected.\n",line.fileName,line.num,nexWord);
                    return FALSE;
                }
                if(isTextAfterCommand(line,&i,firstWord))return FALSE;
                saveRTypeInst(opcode,fParam,sParam,tParam,funct,*ic);
                *ic+=4;
                return TRUE;
            }else{
                /*third param need to bee label and not reserved word*/
                if(isReservedWord(nexWord)){
                    printf("%s.as:%ld: error: argument %s is reserved word.\n",line.fileName,line.num,nexWord);
                    return FALSE;
                }
                if(!isValidLabel(nexWord)){
                printf("%s.as:%ld: error: invalid label '%s'. \n",line.fileName,line.num,firstWord);
                printf("Rules: 1-31 chars, starts with letter, not a reserved word\n");
                return FALSE;
                }
                if(isTextAfterCommand(line,&i,firstWord))return FALSE;

                saveITypeInst(opcode,TRUE,nexWord,fParam,sParam,0,*ic);
                *ic+=4;
                return TRUE;
            }
        }
        /*I instruction with immed - 3rd param is register*/
        if((opcode >= 10 && opcode <= 14) || (opcode >= 19 && opcode <= 24)){

            
            if(tParam == -1){
                printf("%s.as:%ld: error: register '%s' is no valid might be $0-31\n",line.fileName,line.num,nexWord);
                return FALSE;
            }
            
            /*check extra text*/
            skipSpaces(line.code,&i);
            if(!isEmptyStr(line.code,i)){
                printf("%s.as:%ld: error: extra text after command %s\n",line.fileName,line.num,firstWord);
                return FALSE;
            }
            saveITypeInst(opcode,FALSE,NULL,fParam,tParam,immed,*ic);
            *ic+=4;
            return TRUE;
        }
            
    }
    
    if(directive >= DH_DIR && directive <= ASCIZ_DIR){
        while(!isEmptyStr(line.code,i)){
            /*get word*/
            getNextWord(line,nexWord,&i);
            skipSpaces(line.code,&i);
            /*saving and adding dc*/
            if(!saveDataCode(nexWord,directive,size,dc,line)) return FALSE;
            /*check if end of line*/
            if(isEmptyStr(line.code,i))return TRUE;
            /*if not end of line check if has comma*/
            if(!isCommaNext(line,&i,firstWord,nexWord))return FALSE;
        }
    }
    return TRUE;
}