#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "macro_table.h"
#include "utils.h"




/*global table*/
macro_table macro_head;









void appendMacroLine(char *name,char *content){

    macro *macro_ptr,*current;
    char *temp_content;

    

    
    /*we dont have macros until now*/
    if(macro_head == NULL){
        macro_ptr = (macro *) mallocWithCheck(sizeof(*macro));
        macro_ptr->name = (char *) mallocWithCheck(strlen(name)+1);
        macro_ptr->content = (char *) mallocWithCheck(strlen(content)+1);
        macro_ptr->next = NULL;
        strcpy(macro_ptr->name,name);
        strcpy(macro_ptr->content,content);
        macro_head = macro_ptr;
        return;
    }

    current = macro_head;
    /*finding our macro*/
    while(current->next != NULL && strcmp(current->name, name) != 0)current=current->next;

    /*if we find name adding content else we adding new macro in the end of arr*/
    if(strcmp(current->name,name)==0){
        current->content = reallocWithCheck(current->content,
                                            strlen(current->content) + 
                                            strlen(content) + 1);
        strcat(current->content,content);                                    
    }else{
        macro_ptr = (macro *) mallocWithCheck(sizeof(*macro));
        macro_ptr->name = (char *) mallocWithCheck(strlen(name)+1);
        macro_ptr->content = (char *) mallocWithCheck(strlen(content)+1);
        macro_ptr->next = NULL;
        strcpy(macro_ptr->name,name);
        strcpy(macro_ptr->content,content);
        current->next = macro_ptr;
    }
}



bool isMacroExist(char *name){
    macro_table cur = head;

    while (cur != NULL)
    {
        if(strcmp(cur->name,name) == 0){
            return TRUE;
        }
        cur = cur->next;
    }
    return FALSE;
}