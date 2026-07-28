#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "macro_table.h"
#include "utils.h"




/*global table*/
macroTable head;





/**
 * Getting name of macro and adding command line to it.
 * @param name Name of macro to add functions.
 * @param content Lines of code.
 * @return Macro structure.
 */
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

/**
 * Function to append macro lines.
 * @param name Name of macro to add functions.
 * @param content Lines of code.
 */
void appendMacroLine(const char *name,const char *content){

    macroTable current;
    
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

/**
 * Function thats searching by name of macto lines of code.
 * @param name Name of macro.
 * @return Lines of code saved in Macro.
 */
char *getMacro(const char *name){
    macroTable cur = head;

    while(cur != NULL && strcmp(cur->name, name) != 0)
        cur = cur->next;

    if(cur == NULL){
        return NULL;
    }

    return cur->content;
}


/**
 * Boolean fuction that check if macro exist.
 * @param name Name of macro.
 * @return True if macro exist.
 */
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

/**
 * Deletes table and all mallocs.
 */
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