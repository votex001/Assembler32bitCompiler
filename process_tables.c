#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "process_tables.h"
#include "global.h"
#include "utils.h"
/*TODO: description*/

/*global table*/
labelTable labelHead;
codeExternTable externHead;


long arrCounter = CODE_SINGLE_BLOCK;/*cheack in func if dc == arrCounter realoc dataImg  +100*/

/*halper functions */
static void saveByte(unsigned char *dataImg,unsigned long value, long *dc);
static void saveNumber(unsigned char *dataImg,long value, directive dir, long *dc);
static void saveInstructionCode(codeImageTable *codeHead,unsigned long machineCode,bool withLabel,char *label,long IC,long lineNum,bool isI);



/**
 * Saving extern declaration.
 * @param name Name of extern.
 */
void saveExtern(char *name){
    codeExternTable current = externHead;
    codeExternTable newExtern;

    /*page 41*/
    if(isExternExist(name)){
        return;
    }
    
    newExtern = mallocWithCheck(sizeof(*newExtern));
    newExtern->label = mallocWithCheck(strlen(name)+1);
    strcpy(newExtern->label,name);
    newExtern->next = NULL;

    if(externHead == NULL){
        externHead = newExtern;
        return;
    }

    while(current->next !=NULL){
        current = current->next;
    }
    current->next = newExtern;
}

/**
 * Saving entry declaration place.
 * @param entryHead Head of entry linked list.
 * @param label label to entry.
 * @param lineNum Number of line where declared entry.
 */
void saveEntry(codeEntryTable *entryHead,char *label,long lineNum){
    codeEntryTable current = *entryHead;

    /*init new entry*/
    codeEntryTable newEntry = mallocWithCheck(sizeof(*newEntry));
    newEntry->label = mallocWithCheck(strlen(label)+1);
    strcpy(newEntry->label,label);
    newEntry->lineNum = lineNum;
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


/**
 * Saving Label information.
 * @param name Label name.
 * @param isData In second pass we need to know it.
 * @param address If is not data it IC else it DC.
 */
void saveLabels(char *name,bool isData,long address){
    labelTable current;

    /*init new label*/
    label *newLabel = mallocWithCheck(sizeof(*newLabel));
    newLabel->address = address;
    newLabel->label = mallocWithCheck(strlen(name) + 1);
    newLabel->isData = isData;
    strcpy(newLabel->label, name);
    newLabel->next = NULL;;
    
    /*we need to check if label exist separate from this func to return ERROR*/

    /*1st half of list will be isData second instructions*/
    if(labelHead == NULL || labelHead->address > address){
        newLabel->next = labelHead;
        labelHead = newLabel;
        return;
    }

    current = labelHead;

    /*finding our label by name*/
    while(current->next != NULL && current->next->address < address)
        current = current->next;

    /*we adding or in the end of list or in the middle*/
    if(current->next == NULL){
        current->next = newLabel;
    }else{
        newLabel->next = current->next;
        current->next = newLabel;
    }
}

/**
 * Saving jump type instructions.
 * @param codeHead Head of code linked list.
 * @param opcode opcode number of instruction.
 * @param isReg If it register.
 * @param label Label name if it not register.
 * @param reg Number of register if it is.
 * @param IC Instruction counter.
 * @param lineNum Number of line where declared instruction.
 */
void saveJTypeInst(codeImageTable *codeHead,opcode opcode,bool isReg,
                        char *label,unsigned char reg,long IC,long lineNum){
    unsigned long machineCode = 0;
    /*saving hlt separate because of with label false + is reg false*/
    if(opcode == HLT_OP){
        machineCode = ((opcode & 0x3f) << 26);
        saveInstructionCode(codeHead,machineCode,FALSE,label,IC,lineNum,FALSE);
    }
    else if(isReg){
        machineCode = ((opcode & 0x3f) << 26) | (1 << 25) | reg;
        saveInstructionCode(codeHead,machineCode,FALSE,NULL,IC,0,FALSE);
    }else{
        machineCode = ((opcode & 0x3f) << 26);
        /*saving line num to tell in second pass were label that undeclarated*/
        saveInstructionCode(codeHead,machineCode,TRUE,label,IC,lineNum,FALSE);
    }
}

/**
 * Saving immediate type instructions.
 * @param codeHead Head of code linked list.
 * @param opcode opcode number of instruction.
 * @param isLabel If instruction has label.
 * @param label Label name if it exist.
 * @param rs Number of register.
 * @param rt Number of register.
 * @param immed Immediate number.
 * @param IC Instruction counter.
 * @param lineNum Number of line where declared instruction.
 */
void saveITypeInst(codeImageTable *codeHead,opcode opcode,bool isLabel,char *label,
                    unsigned char rs,unsigned char rt,unsigned short immed,long IC,long lineNum){
    unsigned long machineCode = ((opcode & 0x3f) << 26) |
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

/**
 * Saving register type instructions.
 * @param codeHead Head of code linked list.
 * @param opcode opcode number of instruction.
 * @param rs Number of register.
 * @param rt Number of register.
 * @param rd Number of register.
 * @param funct funct number of instruction.
 * @param IC Instruction counter.
 */
void saveRTypeInst(codeImageTable *codeHead,opcode opcode,unsigned char rs,
                            unsigned char rt,unsigned char rd,unsigned char funct,long IC)
{
    unsigned long machineCode = ((opcode & 0x3f) << 26) |
                                ((rs & 0x1f) << 21) |
                                ((rt & 0x1f) << 16) |
                                ((rd & 0x1f) << 11) |
                                ((funct & 0x1f) << 6);
        
        saveInstructionCode(codeHead,machineCode,FALSE,NULL,IC,0,FALSE);
}

/**
 * Saving .data type instruction.
 * @param dataImg Data image array to save data.
 * @param valueToSave Machine code to save.
 * @param dir Directive type (.asciz,.dw,.dh,.db).
 * @param size Byte to use for saving.
 * @param dc Data counter.
 * @param line Line information.
 * @return Success status.
 */
bool saveDataCode(unsigned char *dataImg,char *valueToSave,directive dir, int size,long *dc,cur_line line){

    long value;
    int i;


    if(dataImg == NULL){
        dataImg = mallocWithCheck(arrCounter);
    }


    
    if(dir == ASCIZ_DIR){

            /*page 23 2^25 max size*/
            if (((*dc) + strlen(valueToSave) + 1 ) >=(1 << 25))
            {
                printf("Memory overflow\n");
                exit(1);
            }

            /*new block of memory*/
            if((*dc) + strlen(valueToSave) + 1  >= arrCounter){
                while((*dc) + strlen(valueToSave) + 1 >= arrCounter){
                    arrCounter += CODE_SINGLE_BLOCK;
                }
                dataImg = reallocWithCheck(dataImg,arrCounter);
            }

        /* skipping " */
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
        /*new block of memory*/
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

/**
 * Get label information by name.
 * @param name Name of label to search.
 * @return Original label from linked list.
 */
labelTable getLabel(char *name){
    labelTable current = labelHead;

    while (current != NULL) {
        if (strcmp(current->label, name) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

/**
 * Function to delete table.
 */
void freeLabelTable(){
    labelTable next;
     while (labelHead != NULL)
    {
        next = labelHead->next;
        free(labelHead->label);
        free(labelHead);
        labelHead = next;
    }

}

/**
 * Function to delete table.
 */
void freeExternTable(){
    codeExternTable next;

    while (externHead != NULL)
    {
        next = externHead->next;
        free(externHead->label);
        free(externHead);
        externHead = next;
    }
}

/**
 * Function to delete table.
 * @param codeHead Head of linked list.
 */
void freeCodeTable(codeImageTable codeHead){
    codeImageTable next;

    while (codeHead != NULL)
    {
        next = codeHead->next;
        free(codeHead->label);
        free(codeHead);
        codeHead = next;
    }
    
}

/**
 * Function to delete table.
 * @param entryHead Head of linked list.
 */
void freeEntryTable(codeEntryTable entryHead){
    codeEntryTable next;

    while (entryHead != NULL)
    {
        next = entryHead->next;
        free(entryHead->label);
        free(entryHead);
        entryHead = next;
    }

}

/**
 * Function that checks if Extern is declared.
 * @param name Name of Extern.
 * @return Found status.
 */
bool isExternExist(char *name){
    codeExternTable current = externHead;
    if(current == NULL){
        return FALSE;
    }
    while(current->next != NULL && strcmp(current->label,name) != 0){
        current = current->next;
    }
    return (strcmp(current->label,name) == 0)?TRUE:FALSE;
}

/**
 * Function that checks if value is in range of bytes.
 * @param value Value to check range.
 * @param bytes Byte range (1-4).
 * @return Success status.
 */
bool checkRange(long value, unsigned long bytes)
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

/*From here helper functions*/

/**
 * Function to save structured instruction code.
 * @param codeHead Head of code linked list.
 * @param machineCode Structure machine code
 * @param withLabel If it with label we need to know it.
 * @param label Label declared in code.
 * @param IC Instruction counter.
 * @param lineNum Number of line where code was declared.
 * @param isI If it immed instruction.
 */
static void saveInstructionCode(codeImageTable *codeHead,unsigned long machineCode,bool withLabel,char *label,long IC,long lineNum,bool isI){
    codeImageTable current = *codeHead;
    codeImageTable newLine = mallocWithCheck(sizeof(*newLine));
    newLine->hasLabel = withLabel;
    if(label){
        newLine->label = mallocWithCheck(strlen(label) + 1);
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

/**
 * Function that saving (.db, .dw, .dh) data numbers.
 * @param dataImg Data array.
 * @param value Value to save.
 * @param dir Type of data (.db, .dw, .dh).
 * @param dc Data counter.
 */
static void saveNumber(unsigned char *dataImg,long value, directive dir, long *dc)
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

/**
 * Function that saving value per byte in data image.
 * @param dataImg Data array.
 * @param value Value to save.
 * @param dc Data counter.
 */
static void saveByte(unsigned char *dataImg,unsigned long value, long *dc)
{
    dataImg[(*dc)++] = value & 0xff;
}