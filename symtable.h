#include "scanner.h"
#include "code_generator.h"

typedef struct node tNode;
typedef struct paramList tParamList;
typedef struct symTable tSymTable;

typedef struct paramList // if the identifier in tree is function, this is list of parameters
{
    string funName; // name of the parameter
    tParamList *next; // pointer to the next parameter
}paramList;

typedef struct node
{
    string id; // name of the identifier + type
    int dataType; // current datatype
    tParamList *params; // list of parameters names
    int paramsNum; // number of parameters of function
    tIList *instrs; // list of instruction of function
    tNode *lptr; // pointer to the next identifier (left side are variables)
    tNode *rptr; // pointer to the next identifier (right side are function)
}node;

typedef struct symTable
{
    tNode *root; // root node of BST
    tSymTable *next; // next node in stack
}symTable;