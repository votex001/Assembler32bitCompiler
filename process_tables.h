#ifndef PROCCESS_TABLES_H
#define PROCCESS_TABLES_H
#include "global.h"



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
    char *label;
    long IC;
    struct singleCodeLine *next;
} singleCodeLine;

typedef singleCodeLine *codeImageTable;

typedef struct singleExtern {
    char *label;
    struct singleExtern *next;
} singleExtern;

typedef singleExtern *codeExternTable;

typedef struct singleExternCallPlace {
    char *label;
    struct singleExternCallPlace *next;
} singleExternCallPlace;

typedef singleExtern *codeExternCallTable;

bool isSymbolExist(char *name);
void saveSymbols(char *name,bool isData,long address);
void saveJTypeInst(opcode opcode,bool isReg,char *label,unsigned char reg,long IC);
void saveITypeInst(opcode opcode,bool isLabel,char *label,unsigned char rs,unsigned char rt,unsigned short immed,long IC);
void saveRTypeInst(opcode opcode,unsigned char rs,unsigned char rt,unsigned char rd,unsigned char funct,long IC);
bool saveDataCode(char *valueToSave,directive dir, int size,long *dc,cur_line line);
bool checkRange(long value, unsigned int bytes);
symbolTable getSymbol(char *name);
void saveExtern(char *name);
bool isExternExist(char *name);




#endif