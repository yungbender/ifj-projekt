#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "code_generator.h"
#include "error.h"

// Unique number used for generating labels
int uniqueCounter;

// These bools, signifies, if builtin functions needs to be generated, so they cannot generate two times, or will not generate at all if thy were not called
bool lengthGenerated = false;
bool substrGenerated = false;
bool ordGenerated = false;
bool chrGenerated = false;

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

/**
 * Function appends parameter inside one instruction.
 * @param instrs Pointer to the instruction list.
 * @param param Parameter of function which is represented by token.
 **/
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
    // Initialize list for freeing pointers which are used by multiple tokens
    tPList *freeList = (tPList *)malloc(sizeof(struct pointerList));
    init_plist(freeList);

    tInstr *temp;
    tTList *tmp;
    // Freeing the one instruction and parameters
    while(instrs->head != NULL)
    {
        // Freeing the parameters inside one instruction
        while(instrs->head->params != NULL)
        {
            tmp = instrs->head->params->next;
            if(instrs->head->params->param.type == STRING || instrs->head->params->param.type == ID || instrs->head->params->param.type == IDF)
            {
                if(search_ptr(freeList, instrs->head->params->param.attr.str) == false)
                {
                    insert_ptr(freeList, instrs->head->params->param.attr.str);
                }
            }
            free(instrs->head->params);
            instrs->head->params = tmp;
        }
        temp = instrs->head->next;
        free(instrs->head);
        instrs->head = temp;
    }
    free(instrs);
    free_plist(freeList);
}

/**
 * Function initialize pointer list, which prevents compiler from double free.
 * @param plist Pointer to the pointer list.
 **/
void init_plist(tPList *plist)
{
    plist->active = NULL;
    plist->head = NULL;
}

/**
 * Function appends pointer, at the end of list.
 * @param plist Pointer to the pointer list.
 * @param freed Pointer to the freed string.
 * @warning This string pointer, is always free, do not reference it.
 **/
void insert_ptr(tPList *plist, string freed)
{
    if(plist == NULL)
    {
        return;
    }
    if(plist->head == NULL)
    {
        plist->head = (tPtr *)malloc(sizeof(struct pointerNode));
        plist->head->freed = freed;
        plist->active = plist->head;
        plist->head->next = NULL;
    }
    else
    {
        plist->active->next = (tPtr *)malloc(sizeof(struct pointerNode));
        plist->active = plist->active->next;
        plist->active->freed = freed;
        plist->active->next = NULL;
    }
}

/**
 * Function searches allocated pointers and returns if pointer was free'd or not.
 * @param plist Pointer to the pointer list.
 * @param freed Pointer to the freed string.
 * @return Function returns true pointer was free'd, false if not.
 **/
bool search_ptr(tPList *plist, string freed)
{
    if(plist == NULL)
    {
        return false;
    }
    tPtr *tmp = plist->head;
    while(tmp != NULL)
    {
        if(tmp->freed.str == freed.str)
        {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

/**
 * Function free's up whole list of pointers.
 * @param plist Pointer to the pointer list.
 **/
void free_plist(tPList *plist)
{
    if(plist == NULL)
    {
        return;
    }
    if(plist->head != NULL)
    {
        tPtr *tmp = plist->head;
        while(tmp != NULL)
        {
            tmp = plist->head->next;
            str_free(&plist->head->freed);
            free(plist->head);
            plist->head = tmp;
        }
    }
    free(plist);
}

/**
 * Function creates output file, insert needed header for ifjcode18 and creates temporary frame for working with "main".
 * @return Function returns pointer to the output file.
 **/
FILE* generate_head()
{
    FILE *f = fopen("prog.out","w");
    fprintf(f,".IFJcode18\n");
    fprintf(f,"CREATEFRAME\n\n");
    return f;
}

/**
 * Function generates while operation inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 **/
void generate_add(FILE *f, tInstr *instruction)
{

}

/**
 * Function generates while operation inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 **/
void generate_sub(FILE *f, tInstr *instruction)
{

}

/**
 * Function generates while operation inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 **/
void generate_mul(FILE *f, tInstr *instruction)
{

}

/**
 * Function generates while operation inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 **/
void generate_div(FILE *f, tInstr *instruction)
{

}

/**
 * Function generates while operation inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 **/
void generate_move(FILE *f, tInstr *instruction)
{

}

/**
 * Function generates while operation inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 **/
void generate_while(FILE *f, tInstr *instruction)
{
    tInstr *actualInstr = instruction;
    int instr;
    int scopeWhile = 0;

    do
    {
        if(actualInstr->instr == DEFVAR)
        {
            fprintf(f, "DEFVAR TF@%s\n", instruction->params->param.attr.str.str);
        }
        else if(actualInstr->instr == WHILE_END)
        {
            scopeWhile--;
        }
        else if(actualInstr->instr == WHILE_CALL)
        {
            scopeWhile++;
        }

        actualInstr = actualInstr->next;
    } while(scopeWhile != 0);

    instr = instruction->next->instr;
    //TODO: while
    while(instr != EQ && instr != NE && instr != LT && instr != GT && instr != GE && instr != LE && instr != FLOAT && instr != INTEGER && instr != STRING)
    {
        generate_instruction(f, instruction);
    }

    switch(instr)
    {
        case EQ:
            fprintf(f, "JUMPIFNEQS $while%d\n", uniqueCounter);
            break;
        case NE:
            fprintf(f, "JUMPIFEQS $while%d\n", uniqueCounter);
            break;
        case LT: //x < y ..... x >= y
            fprintf(f, "DEFVAR TF@result1-%d\n", uniqueCounter);
            fprintf(f, "DEFVAR TF@result2%-d\n", uniqueCounter);
            fprintf(f, "DEFVAR TF@result3%-d\n", uniqueCounter);
            fprintf(f, "DEFVAR TF@x%d\n", uniqueCounter);
            fprintf(f, "DEFVAR TF@y%d\n", uniqueCounter);
            fprintf(f, "DEFVAR TF@x$type%d\n", uniqueCounter);
            fprintf(f, "DEFVAR TF@y$type%d\n", uniqueCounter);
            fprintf(f, "POPS TF@y%d\n", uniqueCounter);
            fprintf(f, "POPS TF@x%d\n", uniqueCounter);
            //checking type
            fprintf(f, "TYPE TF@x%d$type TF@x%d\n", uniqueCounter, uniqueCounter);
            fprintf(f, "TYPE TF@y%d$type TF@Y%d\n", uniqueCounter, uniqueCounter);
            fprintf(f, "JUMPIFNEQ $while_not_string%d string@string TF@y%d$type\n", uniqueCounter, uniqueCounter);
            fprintf(f, "JUMPIFNEQ $while_not_string%d TF@x%d$type string@string\n", uniqueCounter, uniqueCounter);
            fprintf(f, "EXIT int@4\n\n");
            fprintf(f, "LABEL $while_not_string%d\n", uniqueCounter);
            fprintf(f, "JUMPIFEQ $while_ok%d TF@x%d$type TF@y%d$type\n", uniqueCounter, uniqueCounter, uniqueCounter);
            fprintf(f, "JUMPIFEQ $while_retype%d TF@x%d$type string@int\n", uniqueCounter, uniqueCounter);
            fprintf(f, "INT2FLOAT TF@x%d\n", uniqueCounter);
            fprintf(f, "JUMP $while_ok%d\n", uniqueCounter);
            fprintf(f, "LABEL $while_retype%d\n", uniqueCounter);
            fprintf(f, "INT2FLOAT TF@y%d\n", uniqueCounter);
            //condition
            fprintf(f, "LABEL $while_ok%d\n", uniqueCounter);
            fprintf(f, "GT TF@result1-%d TF@x%d TF@y%d\n", uniqueCounter, uniqueCounter, uniqueCounter);
            fprintf(f, "EQ TF@result2-%d TF@x%d TF@y%d\n", uniqueCounter, uniqueCounter, uniqueCounter);
            fprintf(f, "AND TF@result3-%d TF@result1%d TF@result2%d\n", uniqueCounter, uniqueCounter, uniqueCounter);
            fprintf(f, "JUMPIFEQ $while%d TF@result3%d bool@true\n", uniqueCounter, uniqueCounter);
            break;
        case GT:
            break;
        case LE:
            break;
        case GE:
            break;
        case INTEGER:
            break;
        case FLOAT:
            break;
        case STRING:
            break;
    }

    while(instruction->instr != END)
    {
        generate_instruction(f, instruction);
        instruction = instruction->next;
    }

    fprintf(f, "LABEL $while%d\n", uniqueCounter);
    uniqueCounter++;

}

/**
 * Function generates if operation inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 **/
void generate_if(FILE *f, tInstr *instruction)
{

}

/**
 * Function generates function call with return value which is going to be saved, inside source code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 * @param builtin Signifies if called function is builtin or user-defined.
 **/
void generate_funcall(FILE *f, tInstr *instruction, bool builtin)
{
    fprintf(f, "# FUNCTION CALL\n");
    fprintf(f, "PUSHFRAME\n");
    fprintf(f, "CREATEFRAME\n\n");
    int paramsNum = 0;
    tToken ret = instruction->params->param;
    // Expecting builtin function
    tToken funName;
    tTList *params = instruction->params->next;
    // If its user custom function, need to get its name and correct parameters list
    if(builtin != true)
    {
        params = instruction->params->next->next;
        funName = instruction->params->next->param;
    }
    // Define every parameter variable
    while(params != NULL)
    {
        fprintf(f, "DEFVAR TF@$%d\n",paramsNum);
        paramsNum++;
        params = params->next;
    }
    // Define return value
    fprintf(f, "DEFVAR TF@$retval\n");
    fprintf(f, "MOVE TF@$retval nil@nil\n");
    params = instruction->params->next;
    if(builtin != true)
    {
        params = instruction->params->next->next;
    }
    // Move parameters value to the temporary ($0...)
    paramsNum = 0;
    while(params != NULL)
    {
        if(params->param.type == ID)
        {
            fprintf(f, "MOVE TF@$%d LF@%s\n", paramsNum, params->param.attr.str.str);
        }
        else if(params->param.type == INTEGER)
        {
            fprintf(f, "MOVE TF@%d int@%d\n",paramsNum, params->param.attr.i);
        }
        else if(params->param.type == FLOAT)
        {
            fprintf(f, "MOVE TF@%d float@%a\n",paramsNum, params->param.attr.f);
        }
        else if(params->param.type == STRING)
        {
            fprintf(f, "MOVE TF@%d string@%s\n",paramsNum, params->param.attr.str.str);
        }
        paramsNum++;
        params = params->next;
    }
    // now funciton can be called
    // if its not builtin function, call function name label
    if(instruction->instr == FUN_CALL)
    {
        fprintf(f, "CALL $%s\n",funName.attr.str.str);
    }
    // If its builtin function, need to call correct builtin function label
    else
    {
        switch(instruction->instr)
        {
            case LENGTH_CALL:
                fprintf(f, "CALL $length\n");
                break;
            case SUBSTR_CALL:
                fprintf(f, "CALL $substr\n");
                break;
            case ORD_CALL:
                fprintf(f, "CALL $ord\n");
                break;
            case CHR_CALL:
                fprintf(f, "CALL $chr\n");
                break;
        }
    }
    // Function was called and executed, return value
    fprintf(f, "MOVE LF@%s TF@$retval\n",ret.attr.str.str);
    // Everything is done, popping back frame
    fprintf(f, "POPFRAME\n\n");
}

/**
 * Function generates function call which return value is not going to be saved, inside source code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 * @param builtin Signifies if called function is builtin or user-defined.
 **/
void generate_nofuncall(FILE *f, tInstr *instruction, bool builtin)
{
    fprintf(f, "# FUNCTION CALL\n");
    fprintf(f, "PUSHFRAME\n");
    fprintf(f, "CREATEFRAME\n\n");

    int paramsNum = 0;
    tToken funName;
    tTList *params = instruction->params;
    if(builtin != true)
    {
        params = instruction->params->next;
        funName = instruction->params->param;
    }
    // Define every parameter variable
    while(params != NULL)
    {
        fprintf(f, "DEFVAR TF@$%d\n",paramsNum);
        paramsNum++;
        params = params->next;
    }
    // Define return value
    fprintf(f, "DEFVAR TF@$retval\n");
    fprintf(f, "MOVE TF@$retval nil@nil\n");
    // Move parameters value to the temporary ($0...)
    params = instruction->params;
    if(builtin != true)
    {
        params = instruction->params->next;
    }
    paramsNum = 0;
    while(params != NULL)
    {
        if(params->param.type == ID)
        {
            fprintf(f, "MOVE TF@$%d LF@%s\n", paramsNum, params->param.attr.str.str);
        }
        else if(params->param.type == INTEGER)
        {
            fprintf(f, "MOVE TF@$%d int@%d\n",paramsNum, params->param.attr.i);
        }
        else if(params->param.type == FLOAT)
        {
            fprintf(f, "MOVE TF@$%d float@%a\n",paramsNum, params->param.attr.f);
        }
        else if(params->param.type == STRING)
        {
            fprintf(f, "MOVE TF@$%d string@%s\n",paramsNum, params->param.attr.str.str);
        }
        paramsNum++;
        params = params->next;
    }
    // Now function can be called
    // if its not builtin function, call function name label
    if(instruction->instr == NOFUN_CALL)
    {
        fprintf(f, "CALL $%s\n",funName.attr.str.str);
    }
    // If its builtin function, need to call correct builtin function label
    else
    {
        switch(instruction->instr)
        {
            case NOLENGTH_CALL:
                fprintf(f, "CALL $length\n");
                break;
            case NOSUBSTR_CALL:
                fprintf(f, "CALL $substr\n");
                break;
            case NOORD_CALL:
                fprintf(f, "CALL $ord\n");
                break;
            case NOCHR_CALL:
                fprintf(f, "CALL $chr\n");
                break;
        }
    }
    // Function was called and executed, return value
    fprintf(f, "MOVE LF@$noretval TF@$retval\n");
    // Everything is done, popping back frame
    fprintf(f, "POPFRAME\n\n");
}

/**
 * Function generates inputs, inputf, inputi, built-in functions into IFJcode2018.
 * @params f Pointer to the IFJcode2018 source code.
 * @params instruction Pointer to the single instruction from inside code.
 * @params moved Bool which signifies if result of the function is going to be moved.
 **/
void generate_input(FILE *f, tInstr *instruction, bool moved, int datatype)
{
    // input is going to be saved
    if(moved == true)
    {
        // need to check datatype
        switch(datatype)
        {
            case INPUTF_CALL:
                fprintf(f, "READ TF@%s float\n",instruction->params->param.attr.str.str);
                break;
            case INPUTI_CALL:
                fprintf(f, "READ TF@%s int\n",instruction->params->param.attr.str.str);
                break;
            case INPUTS_CALL:
                fprintf(f, "READ TF@%s string\n",instruction->params->param.attr.str.str);
                break;
        }
    }
    // input is not going to be moved, need to save it inside noretval if in function
    else
    {
        // need to check datatype
        switch(datatype)
        {
            case NOINPUTF_CALL:
                fprintf(f, "READ TF@$noretval float\n");
                break;
            case NOINPUTI_CALL:
                fprintf(f, "READ TF@$noretval int\n");
                break;
            case NOINPUTS_CALL:
                fprintf(f, "READ TF@$noretval string\n");
                break;
        }
    }
}


/**
 * Function generates print built-in functions into IFJcode2018.
 * @params f Pointer to the IFJcode2018 source code.
 * @params instruction Pointer to the single instruction from inside code.
 * @params moved Bool which signifies if result of the function is going to be moved.
 **/
void generate_print(FILE *f, tInstr *instruction, bool moved)
{
    tToken where;
    tTList *params = instruction->params;
    // If return value will be saved, save where and change pointer to the parameters to next one 
    if(moved == true)
    {
        where = params->param;
        params = params->next;
    }
    // Function generates write one parameter by one
    while(params != NULL)
    {
        // need to check what user wants to print out, int, float, string, or variable
        // first parameter inside instruction is WHERE is result going to be stored second is WHAT is supposed to print out
        switch(params->param.type)
        {
            case INTEGER:
                fprintf(f, "WRITE int@%d\n",params->param.attr.i);
                break;
            case FLOAT:
                fprintf(f, "WRITE float@%a\n",params->param.attr.f);
                break;
            case STRING:
                fprintf(f, "WRITE string@%s\n",params->param.attr.str.str);
                break;
            case ID:
                fprintf(f, "WRITE TF@%s\n",params->param.attr.str.str);
                break;
        }
        params = params->next;
    }
    // If return value will be saved, save there nil because print returns nil
    if(moved == true)
    {
        fprintf(f, "MOVE TF@%s nil@nil\n",where.attr.str.str);
    }
    // If return value is not going to be saved, but can be returned from function move it to the noretval
    else
    {
        fprintf(f, "MOVE TF@$noretval nil@nil\n");
    }
}

/**
 * Function generates length(s) built-in functions into IFJcode2018.
 * @params f Pointer to the IFJcode2018 source code.
 **/
void generate_length(FILE *f)
{
    fprintf(f, "# DEFINITION OF BUILTIN FUNCTION LENGTH()\n\n");
    // Generate correct function label
    fprintf(f, "LABEL $length\n\n");
    fprintf(f, "DEFVAR TF@string\n");
    fprintf(f, "DEFVAR TF@string$type\n\n");
    fprintf(f, "MOVE TF@string TF@$0\n\n");
    fprintf(f, "TYPE TF@string$type TF@string\n");
    fprintf(f, "JUMPIFEQ $string$OK TF@string$type string@string\n");
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $string$OK\n");
    fprintf(f, "STRLEN TF@$retval TF@string\n\n");
    fprintf(f, "RETURN\n");
    fprintf(f, "# END OF DEFINITION OF BUILTIN FUNCTION LENGTH()\n\n");
}

/**
 * Function generates substr(s, i, n) built-infunction into IFJcode2018.
 * @params f Pointer to the IFJcode2018 source code.
 **/
void generate_substr(FILE *f)
{
    fprintf(f, "# DEFINITION OF BUILTIN FUNCTION SUBSTR()\n\n");
    // Generate correct funcion label
    fprintf(f, "LABEL $substr\n\n");
    fprintf(f, "MOVE TF@$retval string@\n");
    fprintf(f, "DEFVAR TF@string\n");
    fprintf(f, "DEFVAR TF@string$type\n");
    fprintf(f, "DEFVAR TF@from\n");
    fprintf(f, "DEFVAR TF@from$type\n");
    fprintf(f, "DEFVAR TF@from$BOOL\n");
    fprintf(f, "DEFVAR TF@to\n");
    fprintf(f, "DEFVAR TF@to$type\n");
    fprintf(f, "DEFVAR TF@string$length\n");
    fprintf(f, "DEFVAR TF@$tmpstr\n");
    fprintf(f, "DEFVAR TF@$cnt\n");
    fprintf(f, "DEFVAR TF@$max\n\n");
    // Get params
    fprintf(f, "MOVE TF@string TF@$0\n");
    fprintf(f, "MOVE TF@from TF@$1\n");
    fprintf(f, "MOVE TF@to TF@$2\n");
    // Check types
    fprintf(f, "TYPE TF@string$type TF@string\n");
    fprintf(f, "JUMPIFEQ $string$OK TF@string$type string@string\n");
    fprintf(f, "EXIT int@4\n\n");
    // Get length of string
    fprintf(f, "LABEL $string$OK\n");
    fprintf(f, "STRLEN TF@string$length TF@string\n\n");
    // Check types, if int its ok, if float, change to float
    fprintf(f, "TYPE TF@from$type TF@from\n");
    fprintf(f, "JUMPIFEQ $from$FLOAT TF@from$type string@float\n");
    fprintf(f, "JUMPIFEQ $from$LT TF@from$type string@int\n");
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $from$FLOAT\n");
    fprintf(f, "FLOAT2INT TF@from TF@from\n\n");
    fprintf(f, "LABEL $from$LT\n");
    // If i is lower than 0 return nil
    fprintf(f, "LT TF@from$BOOL TF@from int@0\n");
    fprintf(f, "JUMPIFEQ $from$GT TF@from$BOOL bool@false\n");
    fprintf(f, "MOVE TF@$retval nil@nil\n");
    fprintf(f, "RETURN\n\n");
    // If i is higher than length of the string, also return nil
    fprintf(f, "LABEL $from$GT\n");
    fprintf(f, "GT TF@from$BOOL TF@from TF@string$length\n");
    fprintf(f, "JUMPIFEQ $from$OK TF@from$BOOL bool@false\n");
    fprintf(f, "MOVE TF@$retval nil@nil\n");
    fprintf(f, "RETURN\n\n");
    // Check n datatype, if float retype, if string exit, if int is ok
    fprintf(f, "LABEL $from$OK\n");
    fprintf(f, "TYPE TF@to$type TF@to\n");
    fprintf(f, "JUMPIFEQ $to$FLOAT TF@to$type string@float\n");
    fprintf(f, "JUMPIFEQ $LT$length TF@to$type string@int\n");
    fprintf(f, "EXIT int@4\n\n");

    fprintf(f, "LABEL $to$FLOAT\n");
    fprintf(f, "FLOAT2INT TF@to TF@to\n\n");
    // Check if i is less than 1, if yes, return empty string
    fprintf(f, "LABEL $LT$length\n");
    fprintf(f, "LT TF@from$BOOL TF@to int@1\n");
    fprintf(f, "JUMPIFEQ $to$OK TF@from$BOOL bool@false\n");
    fprintf(f, "RETURN\n\n");
    // If n < lenstr(s), max = n, cnt = 0 
    fprintf(f, "LABEL $to$OK\n");
    fprintf(f, "LT TF@from$BOOL TF@to TF@string$length\n");
    fprintf(f, "JUMPIFEQ $if$else TF@from$BOOL bool@false\n");
    fprintf(f, "MOVE TF@$max TF@to\n");
    fprintf(f, "MOVE TF@$cnt int@0\n");
    fprintf(f, "JUMP $endif$substr\n\n");
    // If n => lenstr(s), max = lenstr(s), cnt = i
    fprintf(f, "LABEL $if$else\n");
    fprintf(f, "MOVE TF@$max TF@string$length\n");
    fprintf(f, "MOVE TF@$cnt Tf@from\n\n");
    // End if
    fprintf(f, "LABEL $endif$substr\n\n");
    // While(cnt != max)
    fprintf(f, "LABEL $while$substr\n");
    fprintf(f, "GT TF@from$BOOL TF@$cnt TF@$max\n");
    fprintf(f, "JUMPIFEQ $while$end$substr TF@from$BOOL bool@true\n");
    // tmpstr = Getchar(s, from)
    fprintf(f, "GETCHAR TF@$tmpstr TF@string TF@from\n");
    // retval = retval + tmpstr
    fprintf(f, "CONCAT TF@$retval TF@$retval TF@$tmpstr\n");
    // from++ cnt++
    fprintf(f, "ADD TF@from TF@from int@1\n");
    fprintf(f, "ADD TF@$cnt TF@$cnt int@1\n");
    // GOTO while
    fprintf(f, "JUMP $while$substr\n");
    // End of while
    fprintf(f, "LABEL $while$end$substr\n\n");
    fprintf(f, "RETURN\n\n");
    fprintf(f, "# END OF DEFINITION OF BUILTIN FUNCTION SUBSTR()\n\n");
}

/**
 * Function generates ord(s, i) built-infunction into IFJcode2018.
 * @params f Pointer to the IFJcode2018 source code.
 **/
void generate_ord(FILE *f)
{
    fprintf(f, "# DEFINITION OF BUILTIN FUNCTION ORD()\n\n");
    // Generate correct function label
    fprintf(f, "LABEL $ord\n");
    fprintf(f, "DEFVAR TF@string\n");
    fprintf(f, "DEFVAR TF@int\n");
    fprintf(f, "DEFVAR TF@string$type\n");
    fprintf(f, "DEFVAR TF@int$type\n\n");
    fprintf(f, "MOVE TF@string TF@$0\n");
    fprintf(f, "MOVE TF@int TF@$1\n\n");
    fprintf(f, "TYPE TF@string$type TF@string\n");
    fprintf(f, "TYPE TF@int$type TF@int\n");
    fprintf(f, "JUMPIFEQ $string$OK TF@string$type string@string\n");
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $string$OK\n");
    // If int everything is allright
    fprintf(f, "JUMPIFEQ $int$OK TF@int$type string@int\n");
    // If float, need to FLOAT2INT
    fprintf(f, "JUMPIFEQ $int$FLOAT TF@int$type string@float\n");
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $int$FLOAT\n");
    fprintf(f, "FLOAT2INT TF@int TF@int\n\n");
    fprintf(f, "LABEL $int$OK\n");
    fprintf(f, "DEFVAR TF@string$length\n");
    fprintf(f, "STRLEN TF@string$length TF@string\n\n");
    fprintf(f, "DEFVAR TF@int$BOOL\n");
    fprintf(f, "LF TF@int$BOOL TF@int TF@string$length\n");
    fprintf(f, "JUMPIFEQ $int$TRUE TF@int$BOOL bool@true\n");
    fprintf(f, "MOVE TF@$retval nil@nil\n");
    fprintf(f, "RETURN\n\n");
    fprintf(f, "LABEL $int$TRUE\n");
    fprintf(f, "STRI2INT TF@$retval TF@string TF@int\n");
    fprintf(f, "RETURN\n\n");
    fprintf(f, "# END OF DEFINITION OF BUILTIN FUNCTION ORD()\n\n");
}

/**
 * Function generates chr(i) built-infunction into IFJcode2018.
 * @params f Pointer to the IFJcode2018 source code.
 **/
void generate_chr(FILE *f)
{
    fprintf(f, "# DEFINITION OF BUILTIN FUNCTION CHR()\n\n");
    // Generate correct funcion label
    fprintf(f, "LABEL $chr\n\n");
    fprintf(f, "DEFVAR TF@int\n");
    fprintf(f, "DEFVAR TF@int$type\n");
    fprintf(f, "DEFVAR TF@int$BOOL\n");
    fprintf(f, "MOVE TF@int TF@$0\n\n");
    fprintf(f, "TYPE TF@int$type TF@int\n");
    fprintf(f, "JUMPIFEQ $int$LT TF@int$type string@int\n");
    fprintf(f, "JUMPIFEQ $int$FLOAT TF@int$type string@float\n");
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $int$FLOAT\n");
    fprintf(f, "FLOAT2INT TF@int TF@int\n\n");
    fprintf(f, "LABEL $int$LT\n");
    fprintf(f, "LT TF@int$BOOL TF@int int@256\n");
    fprintf(f, "JUMPIFEQ $int$GT TF@int$BOOL bool@true\n");
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $int$GT\n");
    fprintf(f, "GT TF@int$BOOL TF@int int@-1\n");
    fprintf(f, "JUMPIFEQ $int$OK TF@int$BOOL bool@true\n");
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $int$OK\n");
    fprintf(f, "INT2CHAR TF@$retval TF@int\n");
    fprintf(f, "RETURN\n\n");
    fprintf(f, "# END OF DEFINITION OF BUILTIN FUNCTION CHR()\n\n");
}

/**
 * Function generates ONE instruction only.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 **/
void generate_instruction(FILE *f, tInstr *instruction)
{
    switch(instruction->instr)
    {
        // In this case cannot be case for function definition
        case WHILE_CALL:
            generate_while(f,instruction);
            break;
        case IF_CALL:
            generate_if(f,instruction);
            break;
        case FUN_CALL:
            generate_funcall(f,instruction,false);
            break;
        case LENGTH_CALL:
        case SUBSTR_CALL:
        case ORD_CALL:
        case CHR_CALL:
            generate_funcall(f,instruction,true);
            break;
        case NOFUN_CALL:
            generate_nofuncall(f,instruction,false);
            break;
        case NOSUBSTR_CALL:
        case NOORD_CALL:
        case NOCHR_CALL:
        case NOLENGTH_CALL:
            generate_nofuncall(f,instruction, true);
            break;
        // Built in function which can be generated inside main
        case INPUTF_CALL:
        case INPUTI_CALL:
        case INPUTS_CALL:
            generate_input(f,instruction,true, instruction->instr);
            break;
        case NOINPUTF_CALL:
        case NOINPUTI_CALL:
        case NOINPUTS_CALL:
            generate_input(f,instruction,false, instruction->instr);
            break;
        case PRINT_CALL:
            generate_print(f,instruction,true);
            break;
        case NOPRINT_CALL:
            generate_print(f,instruction,false);
            break;
        case ADD:
            generate_add(f, instruction);
            break;
        case SUB:
            generate_sub(f, instruction);
            break;
        case MUL:
            generate_sub(f, instruction);
            break;
        case DIV:
            generate_div(f, instruction);
            break;
        case MOVE:
            generate_move(f, instruction);
            break;
        case NOP:
            break;
    }
}

/**
 * Function generates ALL defvars inside main.
 * @params f Pointer to the IFJcode2018 source code.
 * @params instruction Pointer to the list of instructions.
 **/
void generate_defvar_main(FILE *f, tInstr *instruction)
{
    while(instruction != NULL)
    {
        // If funciton definition is found, skip the whole list until you find end
        if(instruction->instr == FUN_DEF)
        {
            while(instruction->instr != FUN_END)
            {
                instruction = instruction->next;
            }
            instruction = instruction->next;
        }
        if(instruction->instr == DEFVAR)
        {
            fprintf(f, "DEFVAR TF@%s\n", instruction->params->param.attr.str.str);
            // replace defvar with nop to skip this instruction next time
            instruction->instr = NOP;
        }
        instruction = instruction->next;
    }
}

/**
 * Function generates ALL defvars inside function.
 * @params f Pointer to the IFJcode2018 source code.
 * @params instruction Pointer to the list of instructions.
 **/
void generate_defvar_fun(FILE *f, tInstr *instruction)
{
    // generating special variable noretval which saves return of nonreturn function, so nonreturn function can return value if was called inside return function
    fprintf(f, "DEFVAR TF@$noretval\n");
    fprintf(f, "MOVE TF@$noretval nil@nil\n\n");
    while(instruction->instr != FUN_END)
    {
        if(instruction->instr == DEFVAR)
        {
            fprintf(f, "DEFVAR TF@%s\n", instruction->params->param.attr.str.str);
            // replace defvar with nop to skip this instruction next time
            instruction->instr = NOP;
        }
        instruction = instruction->next;
    }
}

/**
 * Function generates instruction one by one but ignores function definitions and everything inside it until end macro.
 * @param f Pointer to the IFJcode2018 source code.
 **/
void generate_main(FILE *f)
{
    tInstr *begin = ilist->head;
    // DEFVARS must go first!
    // generating special variable noretval which saves return of nonreturn function, so nonreturn function can return value if was called inside return function
    fprintf(f, "DEFVAR TF@$noretval\n\n");
    fprintf(f, "MOVE TF@$noretval nil@nil\n\n");
    generate_defvar_main(f,begin);
    while(begin != NULL)
    {
        // If funciton definition is found, skip the whole list until you find end
        if(begin->instr == FUN_DEF)
        {
            while(begin->instr != FUN_END)
            {
                begin = begin->next;
            }
            begin = begin->next;
        }
        else
        {
            generate_instruction(f,begin);
            begin = begin->next;
        }
    }
    fprintf(f, "EXIT int@0\n\n");
    fprintf(f, "# END OF MAIN\n");
}

/**
 * Function chooses, which value(token) to return, based on last instruction.
 * @param instruction Pointer to the last instruction of the funcion.
 **/
tToken choose_return(tInstr *instruction)
{
    tToken nil;
    nil.type = NOP;
    tToken noretval;
    str_init(&noretval.attr.str);
    str_copy_const_string(&(noretval.attr.str),"$noretval");
    switch(instruction->instr)
    {
        case DEFVAR:
        case IF_CALL:
        case ELSE_CALL:
        case IF_END:
        case WHILE_CALL:
        case WHILE_END:
        case FUN_END:
        case PRINT_CALL:
        case NOPRINT_CALL:
            break;
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case MOVE:
        case FUN_CALL:
        case INPUTF_CALL:
        case INPUTI_CALL:
        case INPUTS_CALL:
        case LENGTH_CALL:
        case SUBSTR_CALL:
        case ORD_CALL:
        case CHR_CALL: 
            // return where the function result was saved
            str_free(&noretval.attr.str);
            return (instruction->params->param);
        case NOFUN_CALL:
        case NOINPUTF_CALL:
        case NOINPUTI_CALL:
        case NOINPUTS_CALL:
        case NOLENGTH_CALL:
        case NOSUBSTR_CALL:
        case NOORD_CALL:
        case NOCHR_CALL:
            // return $noretval
            noretval.type = NORETVAL;
            return noretval;
    }
    str_free(&noretval.attr.str);
    return nil;
}

/**
 * Function finds function definition macro and generates everything until end of the function and continues until end of the list.
 * @param f Pointer to the IFJcode2018 source code.
 **/
void generate_fundef(FILE *f)
{
    tInstr *begin = ilist->head;
    tTList *params = NULL;
    tToken ret;
    int paramsNum = 0;
    while(begin != NULL)
    {
        if(begin->instr == FUN_DEF)
        {
            // Print out label of function
            fprintf(f, "# START OF FUNCTION %s\n\n",begin->params->param.attr.str.str);
            fprintf(f, "LABEL $%s\n",begin->params->param.attr.str.str);
            // Create parameters and move there calling parameters
            params = begin->params->next;
            while(params != NULL)
            {
                fprintf(f, "DEFVAR TF@%s\n",params->param.attr.str.str);
                fprintf(f, "MOVE TF@%s TF@$%d\n",params->param.attr.str.str, paramsNum);
                paramsNum++;
                params = params->next;
            }
            paramsNum = 0;
            // Get next function instruction
            begin = begin->next;
            // Generate all defvars inside function
            generate_defvar_fun(f, begin);
            // Generate function body until you hit LAST body instruction
            while(begin->instr != FUN_END && begin->next->instr != FUN_END)
            {
                generate_instruction(f,begin);
                begin = begin->next;
            }
            // Generate the last instruction
            generate_instruction(f,begin);
            // Based on last instruction, generator will return value
            ret = choose_return(begin);
            if(ret.type != NOP)
            {
                fprintf(f, "MOVE TF@$retval TF@%s\n",ret.attr.str.str);
            }
            fprintf(f, "RETURN\n\n");
        }
        // If built in function was called, must check if it was generated already, if it was not generate it, else do nothing
        else if((begin->instr == LENGTH_CALL || begin->instr == NOLENGTH_CALL) && lengthGenerated == false)
        {
            generate_length(f);
            // function was generated, set bool to true so function will not be generated again
            lengthGenerated = true;
        }
        else if((begin->instr == SUBSTR_CALL || begin->instr == NOSUBSTR_CALL) && substrGenerated == false)
        {
            generate_substr(f);
            substrGenerated = true;
        }
        else if((begin->instr == ORD_CALL || begin->instr == NOORD_CALL) && ordGenerated == false)
        {
            generate_ord(f);
            ordGenerated = true;
        }
        else if((begin->instr == CHR_CALL || begin->instr == NOCHR_CALL) && chrGenerated == false)
        {
            generate_chr(f);
            chrGenerated = true;
        }
        begin = begin->next;
    }
    if(ret.type == NORETVAL) // If we stored string in ret token, we need to free it
    {
        str_free(&ret.attr.str);
    }
}


/**
 * Function which generates parsed inside code to final IFJcode2018 code.
 **/
void generate_code()
{
    if(ilist == NULL)
    {
        return;
    }
    FILE *f = generate_head();
    generate_main(f);
    generate_fundef(f);
    fclose(f);
    // This part is segfaulting, will fix
    free_ilist(ilist);
}
