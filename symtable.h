#include "scanner.h"
#include "code_generator.h"

typedef struct node tNode;
typedef struct paramList tParamList;
typedef struct symTable tSymTable;

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
    tParamList *params; // list of name of parameters
    tIList *instrs; // list of instruction of function
    // BST nodes pointers
    tNode *lptr; // pointer to the next identifier (left side are variables)
    tNode *rptr; // pointer to the next identifier (right side are function)
}node;

typedef struct symTable
{
    tNode *root; // root node of BST
    tSymTable *next; // next BST in stack
}symTable;

void stack_init(tSymTable *symTable);
tNode* stack_head(tSymTable *symTable);
tSymTable* stack_pop(tSymTable *symTable);
tSymTable* stack_push(tSymTable *symTable, tNode *new);
tNode* create_var(tToken id, int dataType);
tNode* insert_var(tNode *root, tToken id, int dataType);
tNode* create_fun(tToken id, int paramsNum);
tNode* insert_fun(tNode* root, tToken id, int paramsNum);
tNode *search_local_table(tNode *root, string id);
tNode *search_global_table(tSymTable *symTable, string id);
tParamList* insert_param(tParamList *params, string id);
void free_params(tParamList *params);
void free_tree(tNode *root);
void free_symtable(tSymTable *symTable);