#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include "global.h"
#include "process_tables.h"



bool fPassLine(cur_line line,long *ic,long *dc,
    codeImageTable *codeHead,codeExternTable *externHead,unsigned char *dataImg,codeEntryTable *entryHead);


#endif