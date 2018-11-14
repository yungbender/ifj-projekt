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
 **/
void generate_while()
{

}

/**
 * Function generates if operation inside code.
 **/
void generate_if()
{

}

// FUN_CALL WHERE_RETURNED_VALUE FUNCTION_NAME PARAMETER0 ...
void generate_funcall(FILE *f, tInstr *instruction)
{
    fprintf(f, "# FUNCTION CALL\n");
    fprintf(f, "PUSHFRAME\n");
    fprintf(f, "CREATEFRAME\n\n");
    int paramsNum = 0;
    tToken ret = instruction->params->param;
    tToken funName = instruction->params->next->param;
    tTList *params = instruction->params->next->next;
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
    params = instruction->params->next->next;
    paramsNum = 0;
    while(params != NULL)
    {
        fprintf(f, "MOVE TF@$%d LF@%s\n", paramsNum, params->param.attr.str.str);
        paramsNum++;
        params = params->next;
    }
    // Now function can be called
    fprintf(f, "CALL $%s\n",funName.attr.str.str);
    // Function was called and executed, return value
    fprintf(f, "MOVE LF@%s TF@$retval\n",ret.attr.str.str);
    // Everything is done, popping back frame
    fprintf(f, "POPFRAME\n\n");
}

// FUN_CALL FUNCTION_NAME PARAMETER0 ...
void generate_nofuncall(FILE *f, tInstr *instruction)
{
    fprintf(f, "# FUNCTION CALL\n");
    fprintf(f, "PUSHFRAME\n");
    fprintf(f, "CREATEFRAME\n\n");
    int paramsNum = 0;
    tToken funName = instruction->params->param;
    tTList *params = instruction->params->next;
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
    params = instruction->params->next;
    paramsNum = 0;
    while(params != NULL)
    {
        fprintf(f, "MOVE TF@$%d LF@%s\n", paramsNum, params->param.attr.str.str);
        paramsNum++;
        params = params->next;
    }
    // Now function can be called
    fprintf(f, "CALL $%s\n",funName.attr.str.str);
    // Function was called and executed, return value
    fprintf(f, "MOVE LF@$noretval TF@$retval\n");
    // Everything is done, popping back frame
    fprintf(f, "POPFRAME\n\n");
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
            generate_while();
            break;
        case IF_CALL:
            generate_if();
            break;
        case FUN_CALL:
            generate_funcall(f,instruction);
            break;
        case NOFUN_CALL:
            generate_nofuncall(f,instruction);
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
            while(instruction->instr != FUN_DEF)
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
            while(begin->instr != FUN_DEF)
            {
                begin = begin->next;
            }
            begin = begin->next;
        }
        generate_instruction(f,begin);
        begin = begin->next;
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
        // these returns last instruction where was result calculated
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case MOV:
            // return token with name of WHERE was result saved
            return (instruction->params->param);
        case DEFVAR:
        case IF_CALL:
        case ELSE_CALL:
        case IF_END:
        case WHILE_CALL:
        case WHILE_END:
        case FUN_END:
            return nil;
        case FUN_CALL:
            // return where the function result was saved
            return (instruction->params->param);
        case NOFUN_CALL:
            // return $noretval
            return noretval;
    }
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
                fprintf(f, "DEFVAR %s\n",params->param.attr.str.str);
                fprintf(f, "MOVE %s $%d\n",params->param.attr.str.str, paramsNum);
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
            tToken ret = choose_return(begin);
            if(ret.type != NOP)
            {
                fprintf(f, "MOVE TF@$retval TF@%s\n",ret.attr.str.str);
            }
            fprintf(f, "RETURN\n\n");
        }
        begin = begin->next;
    }
    
}


/**
 * Function which generates parsed inside code to final IFJcode2018 code.
 **/
void generate_code()
{
    
    FILE *f = generate_head();
    generate_main(f);
    generate_fundef(f);
    fclose(f);
    free_ilist(ilist);
}
