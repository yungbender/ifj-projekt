#include <stdbool.h>
#include "code_generator.h"

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
    int dataType; // current datatype
    // Function
    int paramsNum; // number of parameters of function
    bool wasDefined; // bool which means if the function was defined because IFJ18 supports function calls before definition
    //tParamList *params; // list of name of parameters
    //tIList *instrs; // list of instruction of function
    // BST nodes pointers
    tNode *lptr; // pointer to the next identifier (left side are variables)
    tNode *rptr; // pointer to the next identifier (right side are function)
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
void pop_stack(tStack *stack);
void push_stack(tStack *stack, tToken new);
void init_table(tSymTable *symTable);
tNode* create_var(tToken id, int dataType);
tNode* insert_var(tNode *root, tToken id, int dataType);
tNode *create_fun(tToken id, int paramsNum, bool wasDefined);
tNode* insert_fun(tNode* root, tToken id, int paramsNum, bool wasDefined);
tNode *search_table(tNode *root, string id);
void free_stack(tStack *stack);
void free_params(tParamList *params);
void free_tree(tNode *root);
void free_symtable(tSymTable *symTable);