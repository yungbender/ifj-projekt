#include <stdio.h>
#include <malloc.h>
#include "symtable.h"

/**
 * Function initalizes stack. 
 * @param symTable Symbol table which is supposed to initalize.
 **/
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
 * Function returns first member of stack. If stack is empty, returns NULL.
 * @param symTable Symbol table which head user wants to know.
 * @return Function returns head of symbol table.
 **/
tNode* stack_head(tSymTable *symTable)
{
    return symTable->root;
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

/**
 * Function creates symbol table and names it 'r'.
 * @param symTable Pointer where symTable is created.
 **/
void create_table(tSymTable *symTable)
{
    tNode *temp = (tNode *)malloc(sizeof(struct node));
    //TODO unsucc malloc
    temp->dataType = NULL;
    str_add_char(&(temp->id),'r');
    temp->instrs = NULL;
    temp->params = NULL;
    temp->paramsNum = 0;
    temp->lptr = NULL; // this way are variables located
    temp->rptr = NULL; // this way are function located

    symTable->root = temp;
    symTable->next = NULL;
      
}

/**
 * Function creates variable node and returns its pointer.
 * @param id Token with informations about variable.
 * @param dataType Information about which dataType does variable have.
 **/
tNode* create_var(tToken id, int dataType)
{
    tNode *temp = (tNode *)malloc(sizeof(struct node));
    //TODO unsucc malloc

    str_copy_string(&(temp->id),&(id.attr.str)); // must be string because its variable identificator
    temp->dataType = dataType;
    temp->instrs = NULL;
    temp->params = NULL;
    temp->paramsNum = 0;
    temp->lptr = NULL;
    temp->rptr = NULL;

    return temp;
}

/**
 * Function inserts new variable into symbol table.
 * @param root Root of the symbol table (BST).
 * @param id Token with informations about variable.
 * @param dataType Information about which dataType does variable have.
 **/ 
void insert_var(tNode *root, tToken id, int dataType)
{
    if(root == NULL)
    {
        tNode *temp;
        temp = create_var(id, dataType);
        root = temp;
    }
    else if(str_cmp_string(&(id.attr.str),&(root->id)) < 0)
    {
        insert_var(root->lptr, id, dataType);
    }
    else if(str_cmp_string(&(id.attr.str),&(root->id)) > 0)
    {
        insert_var(root->rptr, id, dataType);
    }
}

void insert_fun(tNode* root, tToken id, tIList *ilist)
{
    
}

/**
 * Function searches for the variable/function inside symbol table tree (BST) and returns its pointer.
 * @warning This function is supposed to be used ONLY for searching inside Function! (local symbol table) 
 * @param root Root of symbol table (BST) where variable or function is searched.
 * @param id Identificator of searched symbol.
 * @return Function returns pointer to the searched variable/function, if it is not found returns NULL.
 **/
tNode *search_local_table(tNode *root, string id)
{
    if(root == NULL)
    {
        return NULL;
    }
    else if(str_cmp_string(&(id),&(root->id)) < 0)
    {
        search_local_table(root->lptr, id);
    }
    else if(str_cmp_string(&(id),&(root->id)) > 0)
    {
        search_local_table(root->rptr, id);
    }
    else if(str_cmp_string(&(id),&(root->id)) == 0)
    {
        return root;
    }
}

/**
 * Function searches for the variable/function inside symbol table and returns its pointer.
 * @warning This function is supposed to be used ONLY for searching inside MAIN (searches WHOLE table, not single tree) 
 * @param root Root of symbol table (BST) where variable or function is searched.
 * @param id Identificator of searched symbol.
 * @return Function returns pointer to the searched variable/function, if it is not found returns NULL.
 **/
tNode *search_global_table(tSymTable *symTable, string id)
{
    tNode *result;
    if(symTable == NULL)
    {
        return NULL;
    }
    while(symTable != NULL)
    {
        result = search_local_table(symTable->root,id);
        symTable = symTable->next;
    }
    return result;
}





