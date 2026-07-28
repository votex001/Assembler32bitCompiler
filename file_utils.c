
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utils.h"
#include "utils.h"

/**
 * Function to get link of file and write on it.
 * @param fileName Name without extension.
 * @param ext Extension - format is (.ext).
 */
FILE *writeFile(const char *fileName, const char *ext){
    char *fileWithExt;
    FILE *file;
    /*allocate memory for extension*/
    fileWithExt = mallocWithCheck((strlen(fileName) + strlen(ext)) + 1);


    if(fileWithExt == NULL){
        printf("Can't create or rewrite to file %s, skip file.\n",fileWithExt);
        return NULL;
    }

    sprintf(fileWithExt,"%s%s",fileName,ext);
    file = fopen(fileWithExt,"w");
    free(fileWithExt);

    return file;
}


/**
 * Function to get link of read only file file.
 * @param fileName Name without extension.
 * @param ext Extension - format is (.ext).
 */
FILE *readFile(const char *fileName, const char *ext){
    char *fileWithExt;
    FILE *file;
    /*allocate memory for extension*/
    fileWithExt = mallocWithCheck((strlen(fileName) + strlen(ext)) + 1);

    if(fileWithExt == NULL){
        printf("Error: cant open %s, skip file.\n",fileWithExt);
        return NULL;
    }

    sprintf(fileWithExt,"%s%s",fileName,ext);
    file = fopen(fileWithExt,"r");
    free(fileWithExt);

    return file;
}

/**
 * Function delete file - need to be closed before.
 * @param name Name without extension.
 * @param ext Extension - format is (.ext).
 */
void deleteFile(const char *name, const char *ext){
    char *fileWithExt;
    /*allocate memory for extension*/
    fileWithExt = mallocWithCheck((strlen(name) + strlen(ext)) + 1);

    if (fileWithExt == NULL){

        return;
    }
    
    sprintf(fileWithExt, "%s%s", name, ext);
    remove(fileWithExt);
    free(fileWithExt);
}