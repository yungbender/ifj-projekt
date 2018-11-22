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
#include "symtable.h"
#include "error.h"

#define OK 0

/**
 * Structure which represents data of parser.
 */
typedef struct parserdata
{
    tToken token; //< Currently parsed token.
    tSymTable *local; //< Pointer to the local symbol table, storec variables.
    tSymTable *global; //< Pointer to the global symbol table, stores functions.
    tIList *instrs; //< Pointer to the inside mid-instruction list.
    tStack *stack; //< Stack with tokens, which can be used next time, used if parser must "eat" one more token to choose correct derivation tree, or used for expression parsing.
    int scopes; //< Counter of how many scopes parser is inside.
    bool inDefinition; //< Bool which signifies if parser is inside function definition.
}tPData;

void parse();
void start();
void parser_init();
void if_condition();
void end_of_line();
void while_loop();
void end_of_file();
void function_call(bool moved, bool pushed);
void function_declaration();
void params(tNode *function);
void parse_function_definition();
void validate_variable(string name);
void validate_calls(tNode *root);
void validate_params(tNode *root);
int validate_symbol(string name);
void error(int id);