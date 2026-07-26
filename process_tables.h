#ifndef PROCCESS_TABLES_H
#define PROCCESS_TABLES_H
#include "global.h"

#define CODE_SINGLE_BLOCK 100

typedef struct symbol
{
    long address;
    char *label;
    bool isData;
    struct symbol *next;
}symbol;

typedef symbol *symbolTable;


typedef struct singleCodeLine {
    unsigned long machineCode; /*always minimum 4byte*/
    bool hasLabel;
    bool isI;/*in second pass we will count steps of I instructions*/
    char *label;
    long IC;
    long lineNum;
    struct singleCodeLine *next;
} singleCodeLine;

typedef singleCodeLine *codeImageTable;

typedef struct singleExtern {
    char *label;
    struct singleExtern *next;
} singleExtern;

typedef singleExtern *codeExternTable;

typedef struct singleEntry {
    char *label;
    long lineNum;
    struct singleEntry *next;
} singleEntry;

typedef singleEntry *codeEntryTable;

typedef struct tableWithLabel {
    char *label;
    void *next;
} tableWithLabel;


void freeSymbolTable();
void freeExternTable();
void freeCodeTable(codeImageTable codeHead);
void freeEntryTable(codeEntryTable entryHead);
bool isSymbolExist(char *name);
void saveSymbols(char *name,bool isData,long address);
void saveJTypeInst(codeImageTable *codeHead,opcode opcode,bool isReg,char *label,unsigned char reg,long IC,long lineNum);
void saveITypeInst(codeImageTable *codeHead,opcode opcode,bool isLabel,char *label,unsigned char rs,unsigned char rt,unsigned short immed,long IC,long lineNum);
void saveRTypeInst(codeImageTable *codeHead,opcode opcode,unsigned char rs,unsigned char rt,unsigned char rd,unsigned char funct,long IC);
bool saveDataCode(unsigned char *dataImg,char *valueToSave,directive dir, int size,long *dc,cur_line line);
bool checkRange(long value, unsigned long bytes);
symbolTable getLabel(char *name);
void saveExtern(char *name);
void saveEntry(codeEntryTable *entryHead,char *label,long lineNum);
bool isExternExist(char *name);




#endif