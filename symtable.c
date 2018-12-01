/************************************************************************
 * 
 * Compiler implementation for imperative programming language IFJ18
 * 
 * Autors:
 * Sasák Tomáš - xsasak01
 * Venkrbec Tomáš - xvenkr01
 * Krajči Martin - xkrajc21
 * Natália Dižová - xdizov00 
 * 
 ***********************************************************************/
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
 * Function creates variable node and returns its pointer.
 * @param id Token with informations about variable.
 * @param dataType Information about which dataType does variable have.
 * @return Function returns pointer to the new node.
 **/
tNode* create_var(tToken id)
{
    tNode *temp = (tNode *)malloc(sizeof(struct node));
    //TODO unsucc malloc
    temp->id = (&id)->attr.str;
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
tNode* insert_var(tNode *root, tToken id)
{
    if(root == NULL)
    {
        return create_var(id);
    }
    else if(str_cmp_string(&(id.attr.str),&(root->id)) < 0)
    {
        root->lptr = insert_var(root->lptr, id);
    }
    else if(str_cmp_string(&(id.attr.str),&(root->id)) > 0)
    {
        root->rptr = insert_var(root->rptr, id);
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
    temp->id = (&id)->attr.str;
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
    free_tree(root->rptr);
    free_tree(root->lptr);
    free(root);
}

/**
 * Function frees symTable (every BST).
 * @param symTable Pointer to the table.
 **/
void free_symtable(tSymTable *symTable)
{
    free_tree(symTable->root);

    free(symTable);    
}