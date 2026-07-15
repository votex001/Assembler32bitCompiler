#include "global.h"
#include <string.h>
#include <ctype.h>



struct cmdInstructionEl {
    char *name;
    funct fun;
    opcode op;
};

struct cmdInstructionEl instTable[] = {
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

struct cmdDirectiveEl {
    char *name;
    directive dir;
    int size;
};

struct cmdDirectiveEl dirTable[] = {
    {".db",DB_DIR,1},/*per number*/
    {".dh",DH_DIR,2},/*per number*/
    {".dw",DW_DIR,4},/*per number*/
    {".asciz",ASCIZ_DIR,1},/*per symbol*/
    {".extern",EXTERN_DIR,0},
    {".entry",ENTRY_DIR,0},
    {NULL,NONE_DIR,0}
};


void getFuncOp(char *name, funct *func_out, opcode *op_out){
    struct cmdInstructionEl *el;
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

int getRegisterNum(const char *reg){
    int digit;
    if(reg[0] == '$' && isdigit(reg[1]) && reg[2] == '\0'){
        digit = reg[1] - '0';/*convert*/
        if(digit >= 0 && digit <= 31)
            return digit;
    }
    /*not a register*/
    return -1;


}


void getDirectiveByName(const char *name,directive *dir_out,int *size_out){
    struct cmdDirectiveEl *el;

    *dir_out = NONE_DIR;
    *size_out = 0;

    for(el = dirTable; el->name != NULL;el++){
        if(strcmp(el->name,name) == 0){
            *dir_out = el->dir;
            *size_out = el->size;
            return;
        }
    }
    
}