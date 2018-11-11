#include "scanner.h"

// Macros for instruction inside instruction list
// User's functions
#define FUN_DEF 200
#define FUN_END 201
#define FUN_CALL 202 // function return is going to be saved
#define NOFUN_CALL 216 // function return value is not going to be saved
// If calls
#define IF_CALL 203
#define ELSE_CALL 204
#define IF_END 205
// While calls
#define WHILE_CALL 206
#define WHILE_END 207
// Built-in functions
#define INPUTS_CALL 208
#define INPUTI_CALL 209
#define INPUTF_CALL 210
#define PRINT_CALL 211
#define LENGTH_CALL 212
#define SUBSTR_CALL 213
#define ORD_CALL 214
#define CHR_CALL 215


typedef struct tokenList tTList;
typedef struct instructionList tIList;
typedef struct instructionNode tInstr;

// List of token parameters inside function instruction
typedef struct tokenList
{
    tToken param;
    tTList *next;
}tokenList;
// List of instruction which contains list of token of its parameters
typedef struct instructionNode
{
    int instr; // macro of instruction
    tTList *params; // instruction parameters
    tInstr *next; // next instruction
}instructionNode;
// Instruction list (single linked list) which contains head of the list and active member which is used to insert token for current instruction (without searching everytime)
typedef struct instructionList 
{
    tInstr *head; // first intruction pointer
    tInstr *active; // active instruction pointer 
}instructionList;

tIList *ilist;

void init_ilist(tIList *instrs);
void insert_instr(tIList *instrs, int instr);
void insert_param(tIList *instrs, tToken param);
void free_ilist(tIList *instrs);
void generate_code();