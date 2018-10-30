#include <stdio.h>
#include <stdlib.h>
#include "str.h"
#include "code_generator.h"

void ilist_init(tIList *instrs)
{
    instrs->instr = NULL;
    instrs->next = NULL;
}

tInstr* create_instr()
{

}

tIList* insert_inst(tIList *instrs)
{

}

void free_ilist(tIList *instrs)
{
    if(instrs == NULL)
    {
        return;
    }

    tInstr *temp;
    while(temp != NULL)
    {
        temp = instrs->next;
        free(instrs->instr);
        instrs->instr = temp;
    }
    free(instrs);
}