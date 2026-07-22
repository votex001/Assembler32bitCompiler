#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include "process_tables.h"


bool sPassLine(char *fileName,long *ic,long *dc,codeImageTable *codeHead,
                                                    codeExternTable externHead,codeEntryTable entryHead,unsigned char *dataImg);

#endif