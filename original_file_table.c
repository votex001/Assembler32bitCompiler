#include <stdlib.h>
#include <string.h>

#include "original_file_table.h"
#include "utils.h"



lineTable lineTableHead;



/**
 * Saving line Number.
 * @param line Line information.
 */
void saveLineNum(cur_line line){
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
/**
 * Function to get correct line Number from .as file
 * @param line Text of line to search it.
 * @return Line number or -1 if not found it.
 */
long getLineNum(char *line){
    lineTable current = lineTableHead;

    if(lineTableHead == NULL){
        return -1;
    }

    while (current->next != NULL && strcmp(current->line,line) != 0){
        current=current->next;
    }
    
    return (strcmp(current->line,line) == 0)?current->num:-1;
    
}

/**
 * Deletes table after use.
 */
void deleteLineTable(){
    lineTable cur = lineTableHead;
    lineTable next;

    while (cur != NULL)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }
    lineTableHead = NULL;
}


