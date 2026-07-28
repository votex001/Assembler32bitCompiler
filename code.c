#include <string.h>
#include <ctype.h>

#include "global.h"

/** Structure for instruction format*/
typedef struct  cmdInstructionEl{
    char *name;
    funct fun;
    opcode op;
}cmdInstructionEl;

/* Table of instructions.*/
cmdInstructionEl instTable[] = {
    {"add",  ADD_FUNCT,  ADD_OP},
    {"sub",  SUB_FUNCT,  SUB_OP},
    {"and",  AND_FUNCT,  AND_OP},
    {"or",   OR_FUNCT,   OR_OP},
    {"nor",  NOR_FUNCT,  NOR_OP},

    {"move", MOVE_FUNCT, MOVE_OP},
    {"mvhi", MVHI_FUNCT, MVHI_OP},
    {"mvlo", MVLO_FUNCT, MVLO_OP},

    {"addi", NONE_FUNCT, ADDI_OP},
    {"subi", NONE_FUNCT, SUBI_OP},
    {"andi", NONE_FUNCT, ANDI_OP},
    {"ori",  NONE_FUNCT, ORI_OP},
    {"nori", NONE_FUNCT, NORI_OP},
    {"bne",  NONE_FUNCT, BNE_OP},
    {"beq",  NONE_FUNCT, BEQ_OP},
    {"blt",  NONE_FUNCT, BLT_OP},
    {"bgt",  NONE_FUNCT, BGT_OP},
    {"lb",   NONE_FUNCT, LB_OP},
    {"sb",   NONE_FUNCT, SB_OP},
    {"lw",   NONE_FUNCT, LW_OP},
    {"sw",   NONE_FUNCT, SW_OP},
    {"lh",   NONE_FUNCT, LH_OP},
    {"sh",   NONE_FUNCT, SH_OP},
    {"jmp",  NONE_FUNCT, JMP_OP},
    {"la",   NONE_FUNCT, LA_OP},
    {"call", NONE_FUNCT, CALL_OP},
    {"hlt",  NONE_FUNCT, HLT_OP},

    {NULL, NONE_FUNCT, NONE_OP}
};

/* Structure for directive format*/
typedef struct cmdDirectiveEl{
    char *name;
    directive dir;
    int size;
}cmdDirectiveEl;


/* Table of directives.*/
cmdDirectiveEl dirTable[] = {
    {".db",DB_DIR,1},/*per number*/
    {".dh",DH_DIR,2},/*per number*/
    {".dw",DW_DIR,4},/*per number*/
    {".asciz",ASCIZ_DIR,1},/*per symbol*/
    {".extern",EXTERN_DIR,0},
    {".entry",ENTRY_DIR,0},
    {NULL,NONE_DIR,0}
};

/**
 * Checking if current word is one of instruction.
 * @param name Name of instruction.
 * @param func_out Returning func of current instruction.
 * @param op_out Returning opcode of current instruction
 */
void getFuncOp(char *name, funct *func_out, opcode *op_out){
    cmdInstructionEl *el;
    *op_out = NONE_OP;
    *func_out = NONE_FUNCT;
    /*iterate instruction table until it NONE and if we found compare by name we stop*/
    /*saved info in opcode and func outs*/
    for(el = instTable; el->name !=NULL;el++){
        if(strcmp(el->name, name) == 0){
            *func_out = el->fun;
            *op_out = el->op;
            return;
        }
    }

}


/**
 * Function is getting $0-31 and return number of register.
 * @return Number of register $0-31 or -1 if not register.
 */
int getRegisterNum(const char *reg)
{
    int num = 0;
    int i = 1; /*pass $*/


    /*register should start with $*/
    if (reg[0] != '$')
        return -1;

    /*util it number and it should be max 2 numbers */
    while (isdigit(reg[i]) && i<3)
    {
        num = num * 10 + (reg[i] - '0');
        i++;
    }

    /*after num need to be only \0*/
    if (reg[i] != '\0')
        return -1;

    /*there is 32 registers only*/
    if (num >= 0 && num <= 31)
        return num;

    return -1;
}

/**
 * Checking if word is one of directive.
 * @param name Directive name.
 * @param dir_out Directive.
 * @param size_out Size per simbol to save.
 */
void getDirectiveByName(const char *name,directive *dir_out,int *size_out){
    cmdDirectiveEl *el;

    *dir_out = NONE_DIR;/*starting searchin with non directive value*/
    *size_out = 0;

    for(el = dirTable; el->name != NULL;el++){
        if(strcmp(el->name,name) == 0){
            *dir_out = el->dir;
            *size_out = el->size;
            return;
        }
    }
    
}