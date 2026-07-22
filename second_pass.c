#include <stdio.h>
#include "global.h"
#include "process_tables.h"
#include "file_utils.h"


bool sPassLine(char *fileName,long *ic,long *dc,codeImageTable *codeHead,
                                                    codeExternTable externHead,codeEntryTable entryHead,unsigned char *dataImg){
    
    bool isSuccess = TRUE;
    FILE *obFile,*extFile,*entFile;
    codeImageTable currentCodeLine = *codeHead;
    symbolTable symbol;
    long lineCount = IC_INIT_VAL,iResult;
    
    obFile = writeFile(fileName,".ob");
    extFile = writeFile(fileName,".ext");
    entFile = writeFile(fileName,".ent");
    if(obFile == NULL || extFile == NULL ||entFile == NULL ){
        isSuccess = FALSE;
    }


    if(isSuccess){
        fprintf(obFile,"%ld %ld\n",*ic-100,*dc);

        /*first processing instruction and write to .ob*/
        while(currentCodeLine != NULL){

            if(currentCodeLine->hasLabel && currentCodeLine->isI){
                symbol = getSymbol(currentCodeLine->label);
                if(symbol==NULL){
                    printf("%s.as:%ld: error: undefined label '%s'\n",fileName,currentCodeLine->lineNum,currentCodeLine->label);
                    isSuccess = FALSE;
                    continue;
                }
                iResult = (symbol->address - currentCodeLine->IC);
                if(!checkRange(iResult,2)){
                    printf("%s.as:%ld: error: branch offset to label '%s' is out of range\n",fileName,currentCodeLine->lineNum,currentCodeLine->label);
                    isSuccess = FALSE;
                    continue;
                }
                currentCodeLine->machineCode = currentCodeLine->machineCode | (iResult & 0xFFFF);
            }
            if(currentCodeLine->hasLabel && !currentCodeLine->isI){
                printf("%ld\n",lineCount);/**/
            }




            fprintf(obFile,"%ld %02X %02X %02X %02X\n",lineCount,(currentCodeLine->machineCode & 0xFF),
                                                                (currentCodeLine->machineCode >> 8) & 0xFF,
                                                                (currentCodeLine->machineCode >> 16) & 0xFF,
                                                                (currentCodeLine->machineCode >> 24) & 0xFF);
            lineCount+=4;
            currentCodeLine= currentCodeLine->next;
        }

    }



















    fclose(obFile);
    fclose(extFile);
    fclose(entFile);
    if(!isSuccess){
        deleteFile(fileName,".ob");
        deleteFile(fileName,".ext");
        deleteFile(fileName,".ent");
    }

    return isSuccess;
}