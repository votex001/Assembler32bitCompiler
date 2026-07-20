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

/*halper functions */
void saveByte(unsigned int value, long *dc);
bool checkRange(long value, unsigned int bytes);
void saveNumber(long value, directive dir, long *dc);
void saveInstructionCode(unsigned int machineCode,bool withLabel,char *label,long IC);
symbol *createSymbol(long address,char *name, bool isData );



/*TODO: add delete table of LABELS*/
/*TODO: add delete table of INST CODE*/
/*TODO: add delete table of DATA CODE*/



/*saving symbol to Table NOT CHECKING IF LABEL EXIST*/
void saveSymbols(char *name,bool isData,long address){
    symbolTable current;
    symbol *newSymbol = createSymbol(address,name,isData);
    
    /*we need to check if label exist separate from this func to return ERROR*/

    
    /*1st half of list will be isData second instructions*/
    if(symbolHead == NULL || symbolHead->address > address){
        newSymbol->next = symbolHead;
        symbolHead = newSymbol;
        return;
    }

    current = symbolHead;

    /*finding our label by name*/
    while(current->next != NULL && current->next->address < address)
        current = current->next;

    /*we adding or in the end of list or in the middle*/
    if(current->next == NULL){
        current->next = newSymbol;
    }else{
        newSymbol->next = current->next;
        current->next = newSymbol;
    }
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

void saveITypeInst(opcode opcode,bool isLabel,char *label,unsigned char rs,unsigned char rt,unsigned short immed,long IC){
    unsigned int machineCode = ((opcode & 0x3f) << 26) |
                               ((rs & 0x1f) << 21) |
                               ((rt & 0x1f) << 16);
    
    if(!isLabel){   
        machineCode = machineCode | immed; /*<- immed 16 bit in 1st 16 bits*/
        saveInstructionCode(machineCode,FALSE,NULL,IC);
    }else{
        saveInstructionCode(machineCode,TRUE,label,IC);
    }
        

}

void saveRTypeInst(opcode opcode,unsigned char rs,
                            unsigned char rt,unsigned char rd,unsigned char funct,long IC)
{
    unsigned int machineCode = 0;
    if(opcode == 0){
        machineCode = ((opcode & 0x3f) << 26) |
                      ((rs & 0x1f) << 21) |
                      ((rt & 0x1f) << 16) |
                      ((rd & 0x1f) << 11) |
                      ((funct & 0x1f) << 6);
        saveInstructionCode(machineCode,FALSE,NULL,IC);
    }
}



bool saveDataCode(char *valueToSave,directive dir, int size,long *dc,cur_line line){

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

    if(((*dc) + strlen(valueToSave)) >= arrCounter){
        dataImg = reallocWithCheck(dataImg,arrCounter + 100);
    }
    if(dir == ASCIZ_DIR){
        
        /*skippin " at start and in the end of"*/
        i = 1;
        while (i<MAX_LINE_LENGTH&& valueToSave[i] &&valueToSave[i]!='"' &&valueToSave[i]!='\0')
        {
            saveByte(valueToSave[i],dc);
            i++;
        }
        saveByte('\0', dc);
    }else if(dir == DB_DIR || dir == DH_DIR || dir == DW_DIR){
        if(!is_int(valueToSave)){
            printf("%s.as:%ld: error: value %s might be integer.\n",line.fileName,line.num,valueToSave);
            return FALSE;
        }
        value = strtol(valueToSave,NULL,10);
        if(!checkRange(value,size)){
            printf("%s.as:%ld: error: value %s is out of %d-byte range.\n",line.fileName,line.num,valueToSave,size);
            return FALSE;
        }
        saveNumber(value,dir,dc);
    } 
    return TRUE;
}



/*from here helper functions*/


bool isSymbolExist(char *name){
    symbolTable current = symbolHead;

    if(current == NULL){
        return FALSE;
    }

    while(current != NULL){
        if(strcmp(current->name,name) == 0){
            return TRUE;
        }
        current = current->next;
    }

    
    return FALSE;
}

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

symbol *createSymbol(long address,char *name, bool isData ){
    symbol *newSymbol = mallocWithCheck(sizeof(*newSymbol));
    newSymbol->address = address;
    newSymbol->name = mallocWithCheck(strlen(name) + 1);
    newSymbol->isData = isData;
    strcpy(newSymbol->name, name);
    newSymbol->next = NULL;

    return newSymbol;
}





void saveInstructionCode(unsigned int machineCode,bool withLabel,char *label,long IC){
    codeImageTable current = codeHead;

    codeImageTable newLine = mallocWithCheck(sizeof(*newLine));
    newLine->hasLabel = withLabel;
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
            return FALSE;
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
