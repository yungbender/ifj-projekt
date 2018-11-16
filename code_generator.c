#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "code_generator.h"
#include "error.h"

// this counter is used, every time that new temporary hidden variable is made, its to prevent DEFVAR for already DEFVARed variable
int uniqueCounter = 0;

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
    // Now function can be called
    fprintf(f, "CALL $%s\n",funName.attr.str.str);
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
            case INPUTF_CALL:
                fprintf(f, "READ TF@$noretval float\n");
                break;
            case INPUTI_CALL:
                fprintf(f, "READ TF@$noretval int\n");
                break;
            case INPUTS_CALL:
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
                fprintf(f, "WRITE %d\n",params->param.attr.i);
                break;
            case FLOAT:
                fprintf(f, "WRITE %a\n",params->param.attr.f);
                break;
            case STRING:
                fprintf(f, "WRITE %s\n",params->param.attr.str.str);
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
 * Function generates length built-in functions into IFJcode2018.
 * @params f Pointer to the IFJcode2018 source code.
 * @params instruction Pointer to the single instruction from inside code.
 * @params moved Bool which signifies if result of the function is going to be moved.
 **/
void generate_length(FILE *f, tInstr *instruction, bool moved)
{
    tToken where;
    tTList *params = instruction->params;
    // If return value will be saved, save where and change pointer to the parameters to next one 
    if(moved == true)
    {
        where = params->param;
        params = params->next;
    }
    switch(params->param.type)
    {
        // Cannot strlen number or float
        case INTEGER:
        case FLOAT:
            fprintf(stderr, "Semantic error, wrong datatype as function parameter \"length()\"!\n");
            exit(DATA_ERR);
            break;
        // String is everytime allright
        case STRING:
            if(moved == true)
            {
                fprintf(f, "STRLEN TF@%s string@%s\n",where.attr.str.str, params->param.attr.str.str);
            }
            else
            {
                fprintf(f, "STRLEN TF@$noretval string@%s\n", params->param.attr.str.str);
            }
            break;
        // Need to check if its really string
        case ID:
            fprintf(f, "DEFVAR TF@%s$%d$type\n",params->param.attr.str.str, uniqueCounter);
            fprintf(f, "TYPE TF@%s$%d$type TF@%s\n",params->param.attr.str.str, uniqueCounter, params->param.attr.str.str);
            fprintf(f, "JUMPIFEQ $%s$%d$OK TF@%s$%d$type string@string\n", params->param.attr.str.str, uniqueCounter, params->param.attr.str.str, uniqueCounter);
            fprintf(f, "EXIT int@4\n");
            fprintf(f, "LABEL $%s$%d$OK\n", params->param.attr.str.str, uniqueCounter);
            if(moved == true)
            {
                fprintf(f, "STRLEN TF@%s TF@%s\n",where.attr.str.str, params->param.attr.str.str);
            }
            else
            {
                fprintf(f, "STRLEN TF@$noretval TF@%s\n", params->param.attr.str.str);
            }
            uniqueCounter++;
            break;
    }
}

void generate_substr(FILE *f, tInstr *instruction, bool moved)
{

}

void generate_ord(FILE *f, tInstr *instruction, bool moved)
{
    
}

void generate_chr(FILE *f, tInstr *instruction, bool moved)
{
    
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
            generate_funcall(f,instruction);
            break;
        case NOFUN_CALL:
            generate_nofuncall(f,instruction);
            break;
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
        case LENGTH_CALL:
            generate_length(f,instruction,true);
            break;
        case NOLENGTH_CALL:
            generate_length(f,instruction,false);
            break;
        case SUBSTR_CALL:
            generate_substr(f,instruction,true);
            break;
        case NOSUBSTR_CALL:
            generate_substr(f,instruction,false);
            break;
        case ORD_CALL:
            generate_ord(f,instruction,true);
            break;
        case NOORD_CALL:
            generate_ord(f,instruction,false);
            break;
        case CHR_CALL:
            generate_chr(f,instruction,true);
            break;
        case NOCHR_CALL:
            generate_chr(f,instruction,false);
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
        case PRINT_CALL:
            return nil;
        case INPUTF_CALL:
        case INPUTI_CALL:
        case INPUTS_CALL:
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
                fprintf(f, "DEFVAR TF@%s\n",params->param.attr.str.str);
                fprintf(f, "MOVE TF@%s $%d\n",params->param.attr.str.str, paramsNum);
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
    if(ilist == NULL)
    {
        return;
    }
    FILE *f = generate_head();
    generate_main(f);
    generate_fundef(f);
    fclose(f);
    free_ilist(ilist);
}
