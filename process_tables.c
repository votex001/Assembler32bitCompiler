#include <stdio.h>
#include "process_tables.h"
#include "global.h"
#include "utils.h"
#include "string.h"
#include "stdlib.h"
#include "limits.h"


/*global table*/
symbolTable symbolHead;


long arrCounter = CODE_SINGLE_BLOCK;/*cheack in func if dc == arrCounter realoc dataImg  +100*/

/*halper functions */
void saveByte(unsigned char *dataImg,unsigned int value, long *dc);
bool checkRange(long value, unsigned int bytes);
void saveNumber(unsigned char *dataImg,long value, directive dir, long *dc);
void saveInstructionCode(codeImageTable *codeHead,unsigned int machineCode,bool withLabel,char *label,long IC,long lineNum,bool isI);
symbol *createSymbol(long address,char *name, bool isData );



/*TODO: delete table of LABELS*/
/*TODO: delete table of INST CODE*/
/*TODO: delete table of DATA CODE*/
/*TODO: delete table of extern*/
/*TODO: delete table of externCall*/
/*TODO: delete table of entry*/

/*second pass*/

/*TODO: get data */
/*TODO: get instruction */
/*TODO: get entry */
/*TODO: getExternCall*/
/*TODO: save entry */


symbolTable getSymbol(char *name){
    symbolTable newExtern;
    symbolTable current = symbolHead;

    if(current == NULL){
        return NULL;
    }

    while(current->next != NULL && strcmp(current->name,name) != 0){
        current = current->next;
    }

    if(strcmp(current->name,name) != 0){
        return NULL;
    }else{
        newExtern = mallocWithCheck(sizeof(*newExtern));
        newExtern->name = mallocWithCheck(strlen(name)+1);
        strcpy(newExtern->name,current->name);
        newExtern->address = current->address;
        newExtern->next = NULL;
        return newExtern;
    }
}

bool isExternExist(codeExternTable *externHead,char *name){
    codeExternTable current = *externHead;
    if(current == NULL){
        return FALSE;
    }
    while(current->next != NULL && strcmp(current->label,name) != 0){
        current = current->next;
    }
    return (strcmp(current->label,name) == 0)?TRUE:FALSE;
}



void saveExtern(codeExternTable *externHead,char *name){
    codeExternTable current = *externHead;
    codeExternTable newExtern;

    /*page 41*/
    if(isExternExist(externHead,name)){
        return;
    }
    
    newExtern = mallocWithCheck(sizeof(*newExtern));
    newExtern->label = mallocWithCheck(strlen(name));
    strcpy(newExtern->label,name);
    newExtern->next = NULL;

    if(*externHead == NULL){
        *externHead = newExtern;
        return;
    }

    while(current->next !=NULL){
        current = current->next;
    }
    current->next = newExtern;
}

void saveEntry(codeEntryTable *entryHead,char *label){
    codeEntryTable current = *entryHead;
    codeEntryTable newEntry;

    
    
    newEntry = mallocWithCheck(sizeof(*newEntry));
    newEntry->label = mallocWithCheck(strlen(label));
    strcpy(newEntry->label,label);
    newEntry->next = NULL;

    if(*entryHead == NULL){
        *entryHead = newEntry;
        return;
    }

    while(current->next !=NULL){
        current = current->next;
    }
    current->next = newEntry;
}

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


void saveJTypeInst(codeImageTable *codeHead,opcode opcode,bool isReg,char *label,unsigned char reg,long IC,long lineNum){
    unsigned int machineCode = 0;
    if(isReg){
        machineCode = ((opcode & 0x3f) << 26) | (1 << 25) | reg;
        saveInstructionCode(codeHead,machineCode,FALSE,NULL,IC,0,FALSE);
    }else{
        machineCode = ((opcode & 0x3f) << 26);
        /*saving line num to tell in second pass were label that undeclarated*/
        saveInstructionCode(codeHead,machineCode,TRUE,label,IC,lineNum,FALSE);
    }
}

void saveITypeInst(codeImageTable *codeHead,opcode opcode,bool isLabel,char *label,unsigned char rs,unsigned char rt,unsigned short immed,long IC,long lineNum){
    unsigned int machineCode = ((opcode & 0x3f) << 26) |
                               ((rs & 0x1f) << 21) |
                               ((rt & 0x1f) << 16);
    
    if(!isLabel){   
        machineCode = machineCode | immed; /*<- immed 16 bit in 1st 16 bits*/
        saveInstructionCode(codeHead,machineCode,FALSE,NULL,IC,0,TRUE);
    }else{
        /*saving line num to tell in second pass were label that undeclarated*/
        saveInstructionCode(codeHead,machineCode,TRUE,label,IC,lineNum,TRUE);
    }
        

}

void saveRTypeInst(codeImageTable *codeHead,opcode opcode,unsigned char rs,
                            unsigned char rt,unsigned char rd,unsigned char funct,long IC)
{
    unsigned int machineCode = 0;
    printf("opcode=%d rs=%d rt=%d rd=%d funct=%d\n",
       opcode, rs, rt, rd, funct);
    if(opcode == 0){
        machineCode = ((opcode & 0x3f) << 26) |
                      ((rs & 0x1f) << 21) |
                      ((rt & 0x1f) << 16) |
                      ((rd & 0x1f) << 11) |
                      ((funct & 0x1f) << 6);
        saveInstructionCode(codeHead,machineCode,FALSE,NULL,IC,0,FALSE);
    }
}



bool saveDataCode(unsigned char *dataImg,char *valueToSave,directive dir, int size,long *dc,cur_line line){

    long value;
    int i;


    if(dataImg == NULL){
        dataImg = mallocWithCheck(arrCounter);
    }


    
    if(dir == ASCIZ_DIR){

            /*page 23 2^25 max size*/
            if (((*dc) + strlen(valueToSave)) >=(1 << 25))
            {
                printf("Memory overflow\n");
                exit(1);
            }

            if((*dc) + strlen(valueToSave) >= arrCounter){
                while((*dc) + strlen(valueToSave) >= arrCounter){
                    arrCounter += CODE_SINGLE_BLOCK;
                }
                dataImg = reallocWithCheck(dataImg,arrCounter);
            }

        /*skippin " at start and in the end of"*/
        i = 1;
        while (i<MAX_LINE_LENGTH&& valueToSave[i] &&valueToSave[i]!='"' &&valueToSave[i]!='\0')
        {
            saveByte(dataImg,valueToSave[i],dc);
            i++;
        }
        saveByte(dataImg,'\0', dc);
    }else if(dir == DB_DIR || dir == DH_DIR || dir == DW_DIR){

        /*page 23 2^25 max size*/
        if (((*dc) + size) >=(1 << 25)){
            printf("Memory overflow\n");
            exit(1);
        }
        if((*dc) + size >= arrCounter){
            while((*dc) + size >= arrCounter){
                arrCounter += CODE_SINGLE_BLOCK;
            }
            dataImg = reallocWithCheck(dataImg,arrCounter);   
        }

        if(!is_int(valueToSave)){
            printf("%s.as:%ld: error: value %s might be integer.\n",line.fileName,line.num,valueToSave);
            return FALSE;
        }

        value = strtol(valueToSave,NULL,10);

        if(!checkRange(value,size)){
            printf("%s.as:%ld: error: value %s is out of %d-byte range.\n",line.fileName,line.num,valueToSave,size);
            return FALSE;
        }

        saveNumber(dataImg,value,dir,dc);
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





void saveInstructionCode(codeImageTable *codeHead,unsigned int machineCode,bool withLabel,char *label,long IC,long lineNum,bool isI){
    codeImageTable current = *codeHead;
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
    newLine->isI = isI;/*in second pass we will count steps of I instructions*/
    newLine->lineNum = lineNum;
    newLine->next = NULL;

    /*no codeLines before*/
    if(*codeHead == NULL){
        *codeHead = newLine;
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


void saveNumber(unsigned char *dataImg,long value, directive dir, long *dc)
{
    /*need to be little endian page 23*/
    switch(dir)
    {
        case DB_DIR:
            saveByte(dataImg,value, dc);
            break;


        case DH_DIR:
            saveByte(dataImg,value, dc);
            saveByte(dataImg,value >> 8, dc);
            break;


        case DW_DIR:
            saveByte(dataImg,value, dc);
            saveByte(dataImg,value >> 8, dc);
            saveByte(dataImg,value >> 16, dc);
            saveByte(dataImg,value >> 24, dc);
            break;
        default:
            break;
    }
}



void saveByte(unsigned char *dataImg,unsigned int value, long *dc)
{
    dataImg[(*dc)++] = value & 0xff;
}
