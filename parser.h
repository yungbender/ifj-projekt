#include <stdbool.h>
#include "symtable.h"
#include "error.h"

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
    // Bool variable which tells the parser if he is parsing funcion or main
    bool inDefinition;
}tPData;

void start();
void parser_init();