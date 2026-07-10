#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "macro_table.h"
#include "utils.h"




/*global table*/
macroTable head;






macro *createMacro(const char *name, const char *content)
{
    macro *newMacro = mallocWithCheck(sizeof(*newMacro));
    newMacro->name = mallocWithCheck(strlen(name) + 1);
    newMacro->content = mallocWithCheck(strlen(content) + 1);
    strcpy(newMacro->name, name);
    strcpy(newMacro->content, content);
    newMacro->next = NULL;

    return newMacro;
}


void appendMacroLine(const char *name,const char *content){

    macro *current;
    
    /*we dont have macros until now*/
    if(head == NULL){
       
        head = createMacro(name,content);
        return;
    }

    current = head;

    /*finding our macro*/
    while(current->next != NULL && strcmp(current->name, name) != 0)
        current = current->next;

    /*if we find name adding content else we adding new macro in the end of arr*/
    if(strcmp(current->name,name)==0){
        
        current->content = reallocWithCheck(current->content,
                                            strlen(current->content) + 
                                            strlen(content) + 1);
        strcat(current->content,content);                                    
    }else{
        current->next = createMacro(name,content);
    }
}

const char *getMacro(const char *name){
    macroTable cur = head;

    while(cur != NULL && strcmp(cur->name, name) != 0)
        cur = cur->next;

    if(cur == NULL){
        return NULL;
    }

    return cur->content;
}



bool isMacroExist(const char *name){
    macroTable cur = head;
    while (cur != NULL)
    {
        if(strcmp(cur->name,name) == 0){
            return TRUE;
        }
        cur = cur->next;
    }
    return FALSE;
}


void deleteMacroTable(){
    macroTable prev = head;
    macroTable cur;

    while (prev != NULL)
    {
        cur = prev->next;
        free(prev->content);
        free(prev->name);
        free(prev);
        prev = cur;
    }
    head = NULL;
}