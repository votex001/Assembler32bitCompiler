#ifndef PROCCESS_TABLES_H
#define PROCCESS_TABLES_H


typedef struct symbol
{
    unsigned int address;
    char *label;
    bool isData;
    struct symbol next;
}symbol;

typedef symbol *symbolTable;


typedef struct singleCodeLine {
    unsigned int machine_word;
    char *label;
    struct singleCodeLine next;
} singleCodeLine;

typedef singleCodeLine *codeImage;





#endif