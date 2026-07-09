
#include <stdio.h>


/*get name and extension and creates file*/
FILE *createFile(const char *fileName, const char *ext){
    char *fileWithExt;
    FILE *file;
    fileWithExt = malloc(strlen(fileName) + strlen(ext)+1);

    if(fileWithExt == NULL){
        return NULL;
    }

    sprintf(fileWithExt,"%s.%s",fileName,ext);
    file = fopen(fileWithExt,"w");

    free(fileWithExt);

    return file;
}
/*get name and extenssion and deletes file NEED TO CLOSE FILE BEFORE!*/
void deleteFile(const char *name, const char *ext){
    char fileWithExt[strlen(name) + strlen(ext) + 1];

    sprintf(fileWithExt, "%s%s", name, ext);

    remove(fileWithExt);
}