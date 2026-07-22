#ifndef PROCCESS_TABLES_H
#define PROCCESS_TABLES_H
#include "global.h"

#define CODE_SINGLE_BLOCK 100

typedef struct symbol
{
    long address;
    char *name;
    bool isData;
    struct symbol *next;
}symbol;

typedef symbol *symbolTable;


typedef struct singleCodeLine {
    unsigned int machineCode;
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
    struct singleEntry *next;
} singleEntry;

typedef singleEntry *codeEntryTable;


bool isSymbolExist(char *name);
void saveSymbols(char *name,bool isData,long address);
void saveJTypeInst(codeImageTable *codeHead,opcode opcode,bool isReg,char *label,unsigned char reg,long IC,long lineNum);
void saveITypeInst(codeImageTable *codeHead,opcode opcode,bool isLabel,char *label,unsigned char rs,unsigned char rt,unsigned short immed,long IC,long lineNum);
void saveRTypeInst(codeImageTable *codeHead,opcode opcode,unsigned char rs,unsigned char rt,unsigned char rd,unsigned char funct,long IC);
bool saveDataCode(unsigned char *dataImg,char *valueToSave,directive dir, int size,long *dc,cur_line line);
bool checkRange(long value, unsigned int bytes);
symbolTable getSymbol(char *name);
void saveExtern(codeExternTable *externHead,char *name);
void saveEntry(codeEntryTable *entryHead,char *label);
bool isExternExist(codeExternTable *externHead,char *name);




#endif