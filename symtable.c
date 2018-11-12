#include <stdio.h>
#include <malloc.h>
#include "symtable.h"

/**
 * Function symbol table. 
 * @param symTable Symbol table which is supposed to initalize.
 **/
void init_table(tSymTable *symTable)
{
    symTable->root = NULL;
}

/**
 * Function initialize stack.
 * @param stack Stack with symbol tables.
 **/
void init_stack(tStack *stack)
{
    stack->head = NULL;
    stack->next = NULL;
}

/**
 * Function returns first member of stack. If stack is empty, returns NULL.
 * @param stack Stack which head user wants to know.
 * @return Function returns head of stack.
 **/
tToken* head_stack(tStack *stack)
{
    return stack->head;
}

/**
 * Function pops out the first BST from stack. If stack is empty, function returns NULL..
 * @param stack Stack from which is popped first BST.
 * @return Function returns popped symtable.
 * @warning If the pop is not cathced inside variable, there will be memory leak, because the pointer will get lost.
 **/
tToken* pop_stack(tStack *stack)
{
    tToken *temp;
    tStack *tmp;

    if(stack->next == NULL)
    {
        temp = stack->head;
        stack->head = NULL;
        return temp;
    }
    tmp = stack->next;
    temp = stack->head;
    stack->head = stack->next->head;
    stack->next = stack->next->next;
    free(tmp);
    return temp;
}

/**
 * Function push BST on stack as first member of stack.
 * @param stack Stack where is BST pushed.
 * @param new Which BST is supposed to be pushed.
 * @return Function returns pushed symbolic table.
 **/
void push_stack(tStack *stack, tToken *new)
{
    tStack *temp = (tStack*)malloc(sizeof(tStack));
    temp->head = stack->head;
    temp->next = stack->next;
    stack->head = new;
    stack->next = temp;
}

/**
 * Function creates variable node and returns its pointer.
 * @param id Token with informations about variable.
 * @param dataType Information about which dataType does variable have.
 * @return Function returns pointer to the new node.
 **/
tNode* create_var(tToken id, int dataType)
{
    tNode *temp = (tNode *)malloc(sizeof(struct node));
    //TODO unsucc malloc

    str_copy_string(&(temp->id),&(id.attr.str)); // must be string because its variable identificator
    temp->dataType = dataType;
    temp->wasDefined = true;
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
tNode* insert_var(tNode *root, tToken id, int dataType)
{
    if(root == NULL)
    {
        return create_var(id, dataType);
    }
    else if(str_cmp_string(&(id.attr.str),&(root->id)) < 0)
    {
        root->lptr = insert_var(root->lptr, id, dataType);
    }
    else if(str_cmp_string(&(id.attr.str),&(root->id)) > 0)
    {
        root->rptr = insert_var(root->rptr, id, dataType);
    }
    return root;
}

/**
 * Function creates function node and returns its pointer.
 * @param id Token with informations about function.
 * @param paramsNum Number of parameters function takes.
 * @param wasDefined Bool which means if the function was defined or is expecitng to get defined.
 * @return Function returns pointer to the new node.
 **/
tNode *create_fun(tToken id, int paramsNum, bool wasDefined)
{
    tNode *temp = (tNode *)malloc(sizeof(struct node));
    //TODO unsucc malloc

    str_copy_string(&(temp->id),&(id.attr.str)); // must be string because its function identificator
    temp->dataType = 0;
    temp->wasDefined = wasDefined;
    temp->paramsNum = paramsNum;
    temp->lptr = NULL;  
    temp->rptr = NULL;

    return temp;
}

/**
 * Function inserts new function into symbol table.
 * @param root Root of the symbol table (BST).
 * @param id Token with informations about function.
 * @param paramsNum Number of parameters function takes.
 **/ 
tNode* insert_fun(tNode *root, tToken id, int paramsNum, bool wasDefined)
{
    if(root == NULL)
    {
        return create_fun(id, paramsNum, wasDefined);
    }
    else if(str_cmp_string(&(id.attr.str),&(root->id)) < 0)
    {
        root->lptr = insert_fun(root->lptr, id, paramsNum, wasDefined);
    }
    else if(str_cmp_string(&(id.attr.str),&(root->id)) > 0)
    {
        root->rptr = insert_fun(root->rptr, id, paramsNum, wasDefined);
    }
    return root;
}

/**
 * Function searches for the variable/function inside symbol table tree (BST) and returns its pointer.
 * @param root Root of symbol table (BST) where variable or function is searched.
 * @param id Identificator of searched symbol.
 * @return Function returns pointer to the searched variable/function, if it is not found returns NULL.
 **/
tNode *search_table(tNode *root, string id)
{
    if(root == NULL)
    {
        return NULL;
    }
    else if(str_cmp_string(&(id),&(root->id)) < 0)
    {
        return search_table(root->lptr, id);
    }
    else if(str_cmp_string(&(id),&(root->id)) > 0)
    {
        return search_table(root->rptr, id);
    }
    else
    {
        return root;
    }
}

/**
 * Function frees whole tree (every node).
 * @param root Pointer to the tree root node.
 **/
void free_tree(tNode *root)
{
    if(root == NULL)
    {
        return;
    }
    if(root->lptr != NULL)
    {
        free_tree(root->lptr);
    }
    if(root->rptr != NULL)
    {
        free_tree(root->rptr);
    }
    if(root->lptr == NULL && root->rptr == NULL)
    {
        if(root->id.str != NULL)
        {
            str_free(&(root->id));
        }
        free(root);
    }
}

/**
 * Function frees symTable (every BST).
 * @param symTable Pointer to the table.
 **/
void free_symtable(tSymTable *symTable)
{
    free_tree(symTable->root);
    if(symTable->root != NULL)
    {
        str_free(&(symTable->root->id));
        free(symTable->root);
    }
    free(symTable);    
}

/**
 * Function free's up the whole BST stack.
 * @params stack Stack with symbol tables(BST).
 **/
void free_stack(tStack *stack)
{
    if(stack == NULL)
    {
        return;
    }
    tStack *temp;
    while(temp != NULL)
    {
        temp = stack->next;
        free(stack->head);
        if(temp == NULL)
        {
            break;
        }
        stack->head = temp->head;
    }
    free(stack);
}