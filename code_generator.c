#include <stdio.h>
#include <stdlib.h>
#include "code_generator.h"

/**
 * Function initializes instruction list for first use.
 * @params instrs Pointer to the instruction list.
 **/
void init_ilist(tIList *instrs)
{
    instrs->active = NULL;
    instrs->head = NULL;
}

/**
 * Function inserts instruction code (macro) to the instruction list, creates instruction node for it.
 * @params @instrs Pointer to the instruction list.
 * @params instr Number of instruction to insert (macro).
 **/
void insert_instr(tIList *instrs, int instr)
{
    // This situation cannot occur, but just in case there is no SEGFAULT
    if(instrs == NULL)
    {
        return;
    }
    // If the list is empty, insert new instruction and make it active because it is last instruction in the list
    if(instrs->head == NULL)
    {
        instrs->head = (tInstr *)malloc(sizeof(struct instructionNode));
        instrs->head->instr = instr;
        instrs->head->params = NULL;
        instrs->head->next = NULL;
        instrs->active = instrs->head;
        return;
    }
    // If the list is not empty, active member is always the last member, so just allocate next member (active->next) and save data
    instrs->active->next = (tInstr *)malloc(sizeof(struct instructionNode));
    instrs->active = instrs->active->next;
    instrs->active->instr = instr;
    instrs->active->params = NULL;
    instrs->active->next = NULL;
}

void insert_param(tIList *instrs, tToken param)
{
    // This situation cannot occur, but just in case there is no SEGFAULT
    if(instrs == NULL)
    {
        return;
    }
    else if(instrs->active == NULL)
    {
        return;
    }
    // If there are no parameters, create new one
    if(instrs->active->params == NULL)
    {
        instrs->active->params = (tTList *)malloc(sizeof(struct tokenList));
        instrs->active->params->param = param;
        instrs->active->params->next = NULL;
        return;
    }
    // If there are parameters, find the last one and create new one after it
    tTList *temp = instrs->active->params;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = (tTList *)malloc(sizeof(struct tokenList));
    temp->next->param = param;
    temp->next->next = NULL;
}

/**
 * Function free's up the whole instruction list, instructions, tokens.
 * @params instrs Instruction list with instructions which is supposed to get free'd.
 **/
void free_ilist(tIList *instrs)
{
    if(instrs == NULL)
    {
        return;
    }

    tInstr *temp;
    tTList *tmp;
    // Freeing the one instruction and parameters
    while(instrs->head != NULL)
    {
        // Freeing the parameters inside one instruction
        while(instrs->head->params != NULL)
        {
            tmp = instrs->head->params->next;
            free(instrs->head->params);
            instrs->head->params = tmp;
        }
        temp = instrs->head->next;
        free(instrs->head);
        instrs->head = temp;
    }
    free(instrs);
}

void generate_code()
{
    
}
