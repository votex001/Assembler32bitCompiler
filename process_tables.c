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
        if(!checkRange(value,size)){
            strcpy(error,"number overflow");
            return FALSE;
        }
        saveNumber(value,dir,dc);
    } 
    return TRUE;
}



/*from here helper functions*/


singleCodeLine *createCodeLine(unsigned int machine_word,char *label,struct singleCodeLine *next){
    singleCodeLine *newCodeLine = mallocWithCheck(sizeof(*newCodeLine));
    newCodeLine->machineCode = machine_word;
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



void  saveJTypeInst(opcode opcode,bool is_reg,char *label,unsigned char reg,long IC){
    unsigned int machineCode = 0;
    if(is_reg){
        machineCode = ((opcode & 0x3f) << 26) | (1 << 25) | reg;
        saveInstructionCode(machineCode,FALSE,NULL,IC);
    }else{
        machineCode = ((opcode & 0x3f) << 26);
        saveInstructionCode(machineCode,TRUE,label,IC);
    }
}

void saveITypeInst(opcode opcode,unsigned char rs,unsigned char rt,unsigned short immed,long IC){
    unsigned int machineCode = 0;

    /*saving  I*/
        machineCode = ((opcode & 0x3f) << 26) |
                      ((rs & 0x1f) << 21) |
                      ((rt & 0x1f) << 16) | immed; /*<- immed 16 bit in 1st 16 bits*/
        saveInstructionCode(machineCode,FALSE,NULL,IC);


}

void saveRTypeInst(opcode opcode,unsigned char rs,
                            unsigned char rt,unsigned char rd,unsigned char funct,long IC)
{
    unsigned int machineCode = 0;
    if(opcode == 0){
        machineCode = ((rs & 0x1f) << 21) |
                      ((rt & 0x1f) << 16) |
                      ((rd & 0x1f) << 11) |
                      ((funct & 0x1f) << 6);
        saveInstructionCode(machineCode,FALSE,NULL,IC);
    }else if(opcode == 1){
        machineCode = ((opcode & 0x3f) << 26) |
                      ((rs & 0x1f) << 21) |
                      ((rd & 0x1f) << 11) |
                      ((funct & 0x1f) << 6);
        saveInstructionCode(machineCode,FALSE,NULL,IC);
    }
}

void saveInstructionCode(unsigned int machineCode,bool is_jump,char *label,long IC){
    codeImageTable current = codeHead;

    codeImageTable newLine = mallocWithCheck(sizeof(*newLine));
    newLine->is_jmp = is_jump;
    if(label){
        newLine->label = mallocWithCheck(strlen(label)+1);
        strcpy(newLine->label,label);
    }else{
        newLine->label = NULL;
    }
    newLine->machineCode = machineCode;
    newLine->IC = IC;
    newLine->next = NULL;

    /*no codeLines before*/
    if(current == NULL || IC < codeHead->IC){
        codeHead = newLine;
        return;
    }

    /*saving before head*/
    if(newLine->IC < codeHead->IC){
        newLine->next = codeHead;
        codeHead = newLine;
        return;
    }
    /*sorting by IC*/
    while (current->next != NULL && current->next->IC < newLine->IC)
    {
        current = current->next;
    }
    /*save in the end of list*/
    if(current->next == NULL){
        current->next = newLine;
        return;
    }
    /*cutting list and adding newLine between*/
    newLine->next = current->next;
    current->next = newLine;
}

bool checkRange(long value, unsigned int bytes)
{   /*fast checking if value can be stored in needed bytes*/
    long min;
    long max;

    switch(bytes){
        case 1:
            min = SCHAR_MIN;
            max = SCHAR_MAX;
            break;
        case 2:
            min = SHRT_MIN;
            max = SHRT_MAX;
            break;
        case 4:
            min = INT_MIN;
            max = INT_MAX;
            break;
        default:
            return false;
    }

    return value >= min && value <= max;
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



void saveByte(unsigned int value, long *dc)
{
    dataImg[(*dc)++] = value & 0xff;
}
