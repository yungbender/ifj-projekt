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
#include <stdlib.h>
#include "stack.h"
/**
 * Function initialize stack.
 * @param stack Stack with symbol tables.
 **/
void init_stack(tStack *stack)
{
    stack->head.type = EMPTY;
    stack->next = NULL;
}

/**
 * Function returns first member of stack. If stack is empty, returns NULL.
 * @param stack Stack which head user wants to know.
 * @return Function returns head of stack.
 **/
tToken head_stack(tStack *stack)
{
    return stack->head;
}

/**
 * Function pops out the first BST from stack. If stack is empty, function returns NULL..
 * @param stack Stack from which is popped first BST.
 * @return Function returns popped symtable.
 * @warning If the pop is not cathced inside variable, there will be memory leak, because the pointer will get lost.
 **/
void pop_stack(tStack *stack)
{
    tStack *tmp;

    if(stack == NULL)
    {
        return;
    }
    else if(stack->head.type == EMPTY)
    {
        return;
    }

    tmp = stack->next;
    stack->head = stack->next->head;
    stack->next = stack->next->next;
    free(tmp);
}

/**
 * Function push BST on stack as first member of stack.
 * @param stack Stack where is BST pushed.
 * @param new Which BST is supposed to be pushed.
 * @return Function returns pushed symbolic table.
 **/
void push_stack(tStack *stack, tToken new)
{
    tStack *temp = (tStack*)malloc(sizeof(tStack));
    temp->head = stack->head;
    temp->next = stack->next;
    stack->head = new;
    stack->next = temp;
}

/**
 * Function clears whole stack until finds EMPTY flag.
 * @param stack Stack which will get cleaned.
 **/
void clear_stack(tStack *stack)
{
    tStack *result = stack;
    while(result->head.type != EMPTY)
    {
        pop_stack(stack);
        result = stack;
    }
}

/**
 * Function free's up the whole BST stack.
 * @params stack Stack with symbol tables(BST).
 **/
void free_stack(tStack *stack)
{
    if(stack == NULL)
    {
        return;
    }
    tStack *temp = stack;
    while(temp != NULL)
    {
        temp = stack->next;
        free(stack);
        stack = temp;
    }
}