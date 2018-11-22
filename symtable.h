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

#define EMPTY 999

typedef struct node tNode;
typedef struct paramList tParamList;
typedef struct symTable tSymTable;
typedef struct tokenStack tStack;

typedef struct paramList
{
    string id;
    tParamList *next;
}paramList;

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

typedef struct tokenStack
{
    tToken head; // root of current pushed token
    tStack *next; // pointer to the next member of the stack
}tokenStack;

void init_stack(tStack *stack);
tToken head_stack(tStack *stack);
void clear_stack(tStack *stack);
void pop_stack(tStack *stack);
void push_stack(tStack *stack, tToken new);
void init_table(tSymTable *symTable);
tNode* create_var(tToken id);
tNode* insert_var(tNode *root, tToken id);
tNode *create_fun(tToken id, int paramsNum, bool wasDefined);
tNode* insert_fun(tNode* root, tToken id, int paramsNum, bool wasDefined);
tNode *search_table(tNode *root, string id);
void free_stack(tStack *stack);
void free_params(tParamList *params);
void free_tree(tNode *root);
void free_symtable(tSymTable *symTable);