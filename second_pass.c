#include <stdio.h>

#include "global.h"
#include "process_tables.h"
#include "file_utils.h"



/**
 * Seccond pass:
 * Resolves label addresses, updates instruction code
 * and write final files (.ent, .ext, .ob).
 * @param fileName File name without extension.
 * @param ic Instruction counter.
 * @param dc Data counter.
 * @param codeHead Head or linked list.
 * @param entryHead Head of linked list.
 * @param dataImg Data array.
 * @return Success status.
 */
bool sPassLine(char *fileName,long *ic,long *dc,codeImageTable codeHead,
                                                    codeEntryTable entryHead,unsigned char *dataImg){
    
    bool isSuccess = TRUE;/*success status*/
    FILE *obFile,*extFile,*entFile; /*file pointers*/
    /*heads*/
    labelTable label;
    codeImageTable currentCodeLine = codeHead;
    codeEntryTable currentEntry = entryHead;

    long lineCount = IC_INIT_VAL,calcResult,dcCount; /*needed long numbers*/
    
    /*init files*/
    obFile = writeFile(fileName,".ob");
    extFile = writeFile(fileName,".ext");
    entFile = writeFile(fileName,".ent");

    /*if can't create just pass this function logic*/
    if(obFile == NULL || extFile == NULL ||entFile == NULL ){
        isSuccess = FALSE;
    }


    /*seccond pass logic*/
    if(isSuccess){
        
        /*first processing instruction and write to .ob*/
        fprintf(obFile,"%ld %ld\n",*ic-100,*dc);
        while(currentCodeLine != NULL){
            /*updating all codelines with labels*/
            
            if(currentCodeLine->hasLabel && currentCodeLine->isI){
                label = getLabel(currentCodeLine->label);
                if(label==NULL){
                    printf("%s.as:%ld: error: undefined label '%s'\n",fileName,currentCodeLine->lineNum,currentCodeLine->label);
                    isSuccess = FALSE;
                    lineCount+=4;
                    currentCodeLine= currentCodeLine->next;
                    continue;
                }
                calcResult = (label->address - currentCodeLine->IC);
                if(!checkRange(calcResult,2)){
                    printf("%s.as:%ld: error: branch offset to label '%s' is out of range\n",fileName,currentCodeLine->lineNum,currentCodeLine->label);
                    isSuccess = FALSE;
                    lineCount+=4;
                    currentCodeLine= currentCodeLine->next;
                    continue;
                }
                currentCodeLine->machineCode = currentCodeLine->machineCode | (calcResult & 0xFFFF);
            }/*has label but not I instruction*/
            
            else if(currentCodeLine->hasLabel){
                label = getLabel(currentCodeLine->label);
                /*or it local label or ext or undefined */
                if(label != NULL){
                    /*if is data we adding ic to address*/
                    if(label->isData){
                        calcResult = label->address + *ic;
                    }else{
                        calcResult = label->address;
                    }

                    if (calcResult >= 0 && calcResult < (1L << 25)) {
                            currentCodeLine->machineCode |= (calcResult & 0x1FFFFFF);
                        }
                    else{
                        printf("%s.as:%ld: error: address to label '%s' is out of range\n",fileName,currentCodeLine->lineNum,currentCodeLine->label);
                        isSuccess = FALSE;
                        lineCount+=4;
                        currentCodeLine= currentCodeLine->next;
                        continue;
                    }
                }else if(isExternExist(currentCodeLine->label)){
                    /*extern file printing*/
                     fprintf(extFile,"%s %ld\n",currentCodeLine->label,currentCodeLine->IC);
                }else{
                    printf("%s.as:%ld: error: undefined label '%s'\n",fileName,currentCodeLine->lineNum,currentCodeLine->label);
                    isSuccess = FALSE;
                    lineCount+=4;
                    currentCodeLine= currentCodeLine->next;
                    continue;
                }
               
            }
            fprintf(obFile,"%ld %02lX %02lX %02lX %02lX\n",lineCount,(currentCodeLine->machineCode & 0xFF),
                                                                (currentCodeLine->machineCode >> 8) & 0xFF,
                                                                (currentCodeLine->machineCode >> 16) & 0xFF,
                                                                (currentCodeLine->machineCode >> 24) & 0xFF);
            lineCount+=4;
            currentCodeLine= currentCodeLine->next;
        }

        /*data image printing to .ob*/
        dcCount = 0;
        fprintf(obFile,"%ld ",lineCount);
        while (dcCount < *dc)
        {
            if(dcCount > 0 && (dcCount % 4 ) == 0){
                fprintf(obFile,"\n%ld ",(lineCount + dcCount));
            }
            fprintf(obFile,"%02X ",dataImg[dcCount++]);
        }
        
        /*entry file printing*/
        while (currentEntry != NULL)
        {
            label = getLabel(currentEntry->label);
            if(label == NULL){
                printf("%s.as:%ld: error: undefined label '%s'\n",fileName,currentEntry->lineNum,currentEntry->label);
                isSuccess = FALSE;
                currentEntry = currentEntry->next;
                continue;

            }
            if(label->isData){
                calcResult = label->address + *ic;
            }else{
                calcResult = label->address;
            }
            
            fprintf(entFile,"%s %ld\n",label->label,calcResult);
            currentEntry=currentEntry->next;
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