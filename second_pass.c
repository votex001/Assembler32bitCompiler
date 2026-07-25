#include <stdio.h>
#include "global.h"
#include "process_tables.h"
#include "file_utils.h"


bool sPassLine(char *fileName,long *ic,long *dc,codeImageTable *codeHead,
                                                    codeEntryTable entryHead,unsigned char *dataImg){
    
    bool isSuccess = TRUE;
    FILE *obFile,*extFile,*entFile;
    codeImageTable currentCodeLine = *codeHead;
    symbolTable label;
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
                label = getSymbol(currentCodeLine->label);
                if(label==NULL){
                    printf("%s.as:%ld: error: undefined label '%s'\n",fileName,currentCodeLine->lineNum,currentCodeLine->label);
                    isSuccess = FALSE;
                    continue;
                }
                iResult = (label->address - currentCodeLine->IC);
                if(!checkRange(iResult,2)){
                    printf("%s.as:%ld: error: branch offset to label '%s' is out of range\n",fileName,currentCodeLine->lineNum,currentCodeLine->label);
                    isSuccess = FALSE;
                    continue;
                }
                currentCodeLine->machineCode = currentCodeLine->machineCode | (iResult & 0xFFFF);
            }/*has label but not I instruction*/
            else if(currentCodeLine->hasLabel){
                printf("label = %s ", currentCodeLine->label);
                printf("line - %ld\n",currentCodeLine->lineNum);
                label = getSymbol(currentCodeLine->label);
               
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