
#include <stdio.h>



FILE *createFile(const char *fileName, const char *extension){
    char *fileWithExt;
    FILE *file;
    fileWithExt = malloc(strlen(fileName) + strlen(extension)+1);

    if(fileWithExt == NULL){
        return NULL;
    }

    sprintf(fileWithExt,"%s.%s",fileName,extension);
    file = fopen(fileWithExt,"w");

    free(fileWithExt);

    return file;
}

void deleteFile(const char *fileName){
    remove(fileName);
}