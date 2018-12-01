#ifndef _SYM_H
#define _SYM_H
/************************************************************************
 * 
 * Compiler implementation for imperative programming language IFJ18
 * 
 * Autors:
 * Sasák Tomáš - xsasak01
 * Venkrbec Tomáš - xvenkr01
 * Krajči Martin - xkrajc21
 * Dižová Natália - xdizov00 
 * 
 ***********************************************************************/
#include <stdbool.h>
#include "code_generator.h"
#include "stack.h"

typedef struct node tNode;
typedef struct symTable tSymTable;

typedef struct node
{
    // Variable
    string id; // name of the identifier + type
    // Function
    int paramsNum; // number of parameters of function
    bool wasDefined; // bool which means if the function was defined because IFJ18 supports function calls before definition
    // BST nodes pointers
    tNode *lptr; // pointer to the next identifier 
    tNode *rptr; // pointer to the next identifier
}node;

typedef struct symTable
{
    tNode *root; // root node of BST
}symTable;

void init_table(tSymTable *symTable);
tNode *create_var(tToken id);
tNode *insert_var(tNode *root, tToken id);
tNode *create_fun(tToken id, int paramsNum, bool wasDefined);
tNode *insert_fun(tNode* root, tToken id, int paramsNum, bool wasDefined);
tNode *search_table(tNode *root, string id);
void free_tree(tNode *root);
void free_symtable(tSymTable *symTable);
#endif // _SYM_H
