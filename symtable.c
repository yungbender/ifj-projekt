#include <stdio.h>
#include <malloc.h>
#include "symtable.h"

void stack_init(tSymTable *symTable)
{
    symTable->next = NULL;
    symTable->root = NULL;
}

/**
 * Function pops out the first BST from stack. If stack is empty, function returns nothing. 
 * @param symTable Symbol table from which is popped first BST.
 * @warning This function is not supposed to pop without memory leak or catch the popped BST.
 * @return Function returns popped BST.
 **/
tNode* stack_pop(tSymTable *symTable)
{
    if(symTable->root == NULL)
    {
        return NULL;
    }
    else
    {
        tNode *temp;
        temp = symTable->root;
        symTable = symTable->next;
        return temp;
    }
}

/**
 * Function pops out the first BST from stack. If stack is empty, function returns nothing. But this one frees the BST.
 * @param symTable Symbol table from which is popped first BST.
 **/
void stack_popFree(tSymTable *symTable)
{
    if(symTable->root == NULL)
    {
        return;
    }
    else
    {
        tNode *temp;
        temp = symTable->root;
        symTable = symTable->next;
        free(temp);
    }
}

/**
 * Function push BST on stack as first member of stack.
 * @param symTable Symbol table where is BST pushed.
 * @param new Which BST is supposed to be pushed.
 **/
void stack_push(tSymTable *symTable, tNode *new)
{
    tSymTable *temp;
    temp->root = new;
    temp->next = symTable;
    symTable = temp;
}

