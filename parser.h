#include "symtable.h"

typedef struct parserdata
{
    // Currently loaded token
    tToken token;
    // Pointer to the local table (inside functions, scopes, conditions and cycles)
    tSymTable *local;
    // Pointer to the global table (inside main, scopes, conditions and cycles)
    tSymTable *global;
    // Pointer to the list of instructions (from whole source code)
    tIList *instrs;
}tPData;

void start();