#ifndef PROCCESS_TABLES_H
#define PROCCESS_TABLES_H


typedef struct label
{
    long address;
    char *label;
}label;

typedef label *labelTable;


typedef struct singleCodeLine {
    unsigned int address;
    unsigned int opcode;
    unsigned int funct;
    unsigned int rs;
    unsigned int rt;
    unsigned int rd;
    unsigned int funct;
    unsigned int label;
} singleCodeLine;

typedef singleCodeLine *codeImage;

typedef struct singleDataLine{
    long address;
    char *code;
}singleDataLine;

typedef singleDataLine *dataImage;




#endif