/************************************************************************
 * 
 * Compiler implementation for imperative programming language IFJ18
 * 
 * Autors:
 * Sasák Tomáš - xsasak01
 * Venkrbec Tomáš - xvenkr01
 * Krajči Martin - xkrajc21
 * Natália Dižová - xdizov00 
 * 
 ***********************************************************************/
#include "scanner.h"
#define EMPTY 999

typedef struct tokenStack tStack;

typedef struct tokenStack
{
    tToken head; // root of current pushed token
    tStack *next; // pointer to the next member of the stack
}tokenStack;

void free_stack(tStack *stack);
void init_stack(tStack *stack);
tToken head_stack(tStack *stack);
void clear_stack(tStack *stack);
void pop_stack(tStack *stack);
void push_stack(tStack *stack, tToken new);