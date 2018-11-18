#include "scanner.h"

// This constant is numerical difference between builtin functions with return value moved or not
#define NOCALL_CALL_DIFFERENCE 22
// Macros for instruction inside instruction list
#define DEFVAR 228
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
#define NOINPUTS_CALL 230
#define NOINPUTI_CALL 231
#define NOINPUTF_CALL 232
#define NOPRINT_CALL 233
#define NOLENGTH_CALL 234
#define NOSUBSTR_CALL 235
#define NOORD_CALL 236
#define NOCHR_CALL 237
// Operators
#define ADD 217 // '+'
#define SUB 218 // '-'
#define MUL 219 // '*'
#define DIV 220 // '/'
#define MOVE 221 // '='
#define EQ  222 // '=='
#define LT  223 // '<'
#define LE  224 // '<='
#define GT  225 // '>'
#define GE  226 // '>='
#define NE  227 // '!='
// Data stack instructions
#define ADDS 241
#define SUBS 242
#define MULS 243
#define DIVS 244
#define IDIVS 245
#define LTS 246
#define GTS 247
#define EQS 248
#define PUSHS 249
#define POPS 250
// Empty instruction
#define NOP 240

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
void generate_instruction(FILE *f, tInstr *instruction);