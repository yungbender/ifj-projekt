#include "symtable.h"

typedef struct parserdata
{
    tToken token;
    tSymTable *local;
    tSymTable *global;
}tPData;