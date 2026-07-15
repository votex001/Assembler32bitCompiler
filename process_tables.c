#include <stdio.h>
#include "process_tables.h"
#include "global.h"
#include "utils.h"
#include "string.h"
#include "stdlib.h"
#include "limits.h"


/*global tables*/
symbolTable symbolHead;
codeImageTable codeHead;

long arrCounter = 100;/*cheack in func if dc == arrCounter realoc dataImg  +100*/
unsigned char *dataImg; /*1 byte per cell*/


/*TODO: save labels*/
/*TODO: save machinCode func that check if it codeImg or dataImg*/

void saveByte(unsigned int value, long *dc)
{
    dataImg[(*dc)++] = value & 0xFF;
}

bool checkRange(long value, directive dir)
{
    switch(dir)
    {
        case DB_DIR:
            return value >= SCHAR_MIN && value <= SCHAR_MAX;

        case DH_DIR:
            return value >= SHRT_MIN && value <= SHRT_MAX;

        case DW_DIR:
            return value >= INT_MIN &&
                   value <= INT_MAX;
        default:
            break;
    }

    return FALSE;
}

void saveNumber(long value, directive dir, long *dc)
{
    /*need to be little endian page 23*/
    switch(dir)
    {
        case DB_DIR:
            saveByte(value, dc);
            break;


        case DH_DIR:
            saveByte(value, dc);
            saveByte(value >> 8, dc);
            break;


        case DW_DIR:
            saveByte(value, dc);
            saveByte(value >> 8, dc);
            saveByte(value >> 16, dc);
            saveByte(value >> 24, dc);
            break;
        default:
            break;
    }
}

bool saveDataCode(char *code,directive dir, int size,long *dc,char *error){

    long value;
    int i;
    

    if(dataImg == NULL){
        dataImg = mallocWithCheck(arrCounter);
    }
    /*page 23 2^25 max size*/
    if ((*dc) >=(1 << 25))
    {
        printf("Memory overflow\n");
        exit(1);
    }

    
    
    if(((*dc) + strlen(code)) >= arrCounter){
        dataImg = reallocWithCheck(dataImg,arrCounter + 100);
    }
    if(dir == ASCIZ_DIR){
        i = 0;
        while (i<MAX_LINE_LENGTH&& code[i] && code[i]!='\0')
        {
            saveByte(code[i],dc);
            i++;
        }
        saveByte('\0', dc);
    }else if(dir == DB_DIR || dir == DH_DIR || dir == DW_DIR){
        if(!is_int(code)){
            strcpy(error,"not a num");
            return FALSE;
        }
        value = strtol(code,NULL,10);
        if(!checkRange(value,dir)){
            strcpy(error,"number overflow");
            return FALSE;
        }
        saveNumber(value,dir,dc);
    } 
    return TRUE;
}

singleCodeLine *createCodeLine(unsigned int machine_word,char *label,struct singleCodeLine *next){
    singleCodeLine *newCodeLine = mallocWithCheck(sizeof(*newCodeLine));
    newCodeLine->machine_word = machine_word;
    if(label){
        newCodeLine->label = mallocWithCheck(strlen(label)+1);
        strcpy(newCodeLine->label,label);
    }
    else{
        newCodeLine->label = NULL;
    }
    newCodeLine->next = NULL;

    return newCodeLine;
}

symbol *createSymbol(unsigned int address,char *name, bool isData ){
    symbol *newSymbol = mallocWithCheck(sizeof(*newSymbol));
    newSymbol->address = address;
    newSymbol->name = mallocWithCheck(strlen(name) + 1);
    newSymbol->isData = isData;
    strcpy(newSymbol->name, name);
    newSymbol->next = NULL;

    return newSymbol;
}