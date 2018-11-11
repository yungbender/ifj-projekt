#include <stdbool.h>
#include "symtable.h"
#include "error.h"

#define OK 0

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
    // Stack with local symbol tables (used for function calls, recursions, function parsing)
    tStack *stack;
    // Int variable which signals in how many scopes parser is (function,while,if,etc) this is for "end" keyword to determine, if end token is correct
    int scopes;
    // Bool variable which signals if parser is inside function definition body
    bool inDefinition;
}tPData;

void parse();
void start();
void parser_init();