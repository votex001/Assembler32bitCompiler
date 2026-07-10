
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_utils.h"
#include "utils.h"

/*get name and extension and creates file*/
FILE *writeFile(const char *fileName, const char *ext){
    char *fileWithExt;
    FILE *file;
    fileWithExt = mallocWithCheck((strlen(fileName) + strlen(ext)) + 1);

    if(fileWithExt == NULL){
        printf("Can't create or rewrite to file %s.",fileWithExt);
        return NULL;
    }

    sprintf(fileWithExt,"%s%s",fileName,ext);
    file = fopen(fileWithExt,"w");
    free(fileWithExt);

    return file;
}

FILE *readFile(const char *fileName, const char *ext){
    char *fileWithExt;
    FILE *file;
    fileWithExt = mallocWithCheck((strlen(fileName) + strlen(ext)) + 1);

    if(fileWithExt == NULL){
        printf("Can't read file %s.",fileWithExt);
        return NULL;
    }

    sprintf(fileWithExt,"%s%s",fileName,ext);
    file = fopen(fileWithExt,"r");
    free(fileWithExt);

    return file;
}

/*get name and extenssion and deletes file NEED TO CLOSE FILE BEFORE!*/
void deleteFile(const char *name, const char *ext){
    char *fileWithExt;

    fileWithExt = mallocWithCheck((strlen(name) + strlen(ext)) + 1);

    if (fileWithExt == NULL){

        return;
    }
    
    sprintf(fileWithExt, "%s%s", name, ext);
    remove(fileWithExt);
    free(fileWithExt);
}