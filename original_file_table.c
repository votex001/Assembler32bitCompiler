#include "original_file_table.h"
#include "utils.h"
#include "string.h"



lineTable lineTableHead;

/*TODO: delete table*/

void saveLineInfo(cur_line line){
    lineTable current;
    lineTable newLine;

    newLine = mallocWithCheck(sizeof(*newLine));
    strcpy(newLine->line,line.code);
    newLine->num = line.num;
    newLine->next = NULL;
    current = lineTableHead;
    if(lineTableHead == NULL){
        lineTableHead = newLine;
        return;
    }

    while (current->next!=NULL)
    {
        current=current->next;
    }

    current->next = newLine;
    
}

long getLineNum(char *str){
    lineTable current = lineTableHead;

    if(lineTableHead == NULL){
        return -1;
    }

    while (current->next != NULL && strcmp(current->line,str) != 0){
        current=current->next;
    }
    
    return (strcmp(current->line,str) == 0)?current->num:-1;
    
}




