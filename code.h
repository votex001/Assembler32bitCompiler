#ifndef CODE_H
#define CODE_H

#include "global.h"

void getFuncOp(const char *cmd, funct *func_out, opcode *op_out);
int getRegisterNum(const char *str);
void getDirectiveByName(const char *name,directive *dir_out,int *size_out);

#endif
