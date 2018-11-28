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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "code_generator.h"
#include "error.h"

/**
 * Counter for making unique labels.
 */
int uniqueCounter = 0;
bool inExpression = false;

/** @name These bools, signifies, if builtin functions needs to be generated, so they cannot generate two times, or will not generate at all if thy were not called
*/
//@{
bool lengthRequest = false;
bool substrRequest = false;
bool ordRequest = false;
bool chrRequest = false;
//@}

/**
 * Function initializes instruction list for first use.
 * @param instrs Pointer to the instruction list.
 */
void init_ilist(tIList *instrs)
{
    instrs->active = NULL;
    instrs->head = NULL;
}

/**
 * Function inserts instruction code (macro) to the instruction list, creates instruction node for it.
 * @param instrs Pointer to the instruction list.
 * @param instr Number of instruction to insert (macro).
 */
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
 */
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
 * @param instrs Instruction list with instructions which is supposed to get free'd.
 */
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
 */
void init_plist(tPList *plist)
{
    plist->active = NULL;
    plist->head = NULL;
}

/**
 * Function appends pointer, at the end of list.
 * @param plist Pointer to the pointer list.
 * @param freed Pointer to the freed string.
 */
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
 * @return Function returns true pointer was found, if not false.
 */
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
 */
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
 */
FILE* generate_head()
{
    FILE *f = stdout;
    fprintf(f,".IFJcode18\n");
    fprintf(f,"CREATEFRAME\n\n");
    return f;
}

/**
 * Function generates add operation inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 */
void generate_adds(FILE *f, tInstr *instruction)
{
    fprintf(f, "POPS TF@op1\n");
    fprintf(f, "POPS TF@op2\n");
    fprintf(f, "TYPE TF@op1$type TF@op1\n");
    fprintf(f, "TYPE TF@op2$type TF@op2\n");
    fprintf(f, "JUMPIFNEQ $add_nil$op1$%d TF@op1$type string@nil\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $add_nil$op1$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $add_nil$op2$%d TF@op2$type string@nil\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $add_nil$op2$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFEQ $add_ok%d TF@op1$type TF@op2$type\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $checktype1%d TF@op1$type string@string\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $checktype1%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $checktype2%d TF@op2$type string@string\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $checktype2%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $add-retype1%d TF@op1$type string@int\n", uniqueCounter);
    fprintf(f, "INT2FLOAT TF@op1 TF@op1\n");
    fprintf(f, "LABEL $add-retype1%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $add_ok%d TF@op2$type string@int\n", uniqueCounter);
    fprintf(f, "INT2FLOAT TF@op2 TF@op2\n");
    fprintf(f, "LABEL $add_ok%d\n", uniqueCounter);
    fprintf(f, "JUMPIFEQ $concat%d TF@op1$type string@string\n", uniqueCounter);

    fprintf(f, "PUSHS TF@op2\n");
    fprintf(f, "PUSHS TF@op1\n");
    fprintf(f, "ADDS\n");
    fprintf(f, "JUMP $add_end%d\n", uniqueCounter);

    fprintf(f, "LABEL $concat%d\n", uniqueCounter);
    fprintf(f, "CONCAT TF@op2 TF@op2 TF@op1\n");
    fprintf(f, "PUSHS TF@op2\n");
    fprintf(f, "LABEL $add_end%d\n", uniqueCounter);

    instruction->instr = NOP;
    uniqueCounter++;
    
}

/**
 * Function generates sub operation inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 */
void generate_subs(FILE *f, tInstr *instruction)
{
    fprintf(f, "POPS TF@op1\n");
    fprintf(f, "POPS TF@op2\n");
    fprintf(f, "TYPE TF@op1$type TF@op1\n");
    fprintf(f, "TYPE TF@op2$type TF@op2\n");
    fprintf(f, "JUMPIFNEQ $sub_nil$op1$%d TF@op1$type string@nil\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $sub_nil$op1$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $sub_nil$op2$%d TF@op2$type string@nil\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $sub_nil$op2$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $checktype1%d TF@op1$type string@string\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $checktype1%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $checktype2%d TF@op2$type string@string\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $checktype2%d\n", uniqueCounter);
    fprintf(f, "JUMPIFEQ $sub_ok%d TF@op1$type TF@op2$type\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $sub-retype1%d TF@op1$type string@int\n", uniqueCounter);
    fprintf(f, "INT2FLOAT TF@op1 TF@op1\n");
    fprintf(f, "LABEL $sub-retype1%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $sub_ok%d TF@op2$type string@int\n", uniqueCounter);
    fprintf(f, "INT2FLOAT TF@op2 TF@op2\n");
    fprintf(f, "LABEL $sub_ok%d\n", uniqueCounter);
    fprintf(f, "PUSHS TF@op2\n");
    fprintf(f, "PUSHS TF@op1\n");
    fprintf(f, "SUBS\n");

    instruction->instr = NOP;
    uniqueCounter++;
}

/**
 * Function generates mul operation inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 */
void generate_muls(FILE *f, tInstr *instruction)
{
    fprintf(f, "POPS TF@op1\n");
    fprintf(f, "POPS TF@op2\n");
    fprintf(f, "TYPE TF@op1$type TF@op1\n");
    fprintf(f, "TYPE TF@op2$type TF@op2\n");
    fprintf(f, "JUMPIFNEQ $mul_nil$op1$%d TF@op1$type string@nil\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $mul_nil$op1$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $mul_nil$op2$%d TF@op2$type string@nil\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $mul_nil$op2$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $checktype1%d TF@op1$type string@string\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $checktype1%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $checktype2%d TF@op2$type string@string\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $checktype2%d\n", uniqueCounter);
    fprintf(f, "JUMPIFEQ $mul_ok%d TF@op1$type TF@op2$type\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $mul-retype1%d TF@op1$type string@int\n", uniqueCounter);
    fprintf(f, "INT2FLOAT TF@op1 TF@op1\n");
    fprintf(f, "LABEL $mul-retype1%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $mul_ok%d TF@op2$type string@int\n", uniqueCounter);
    fprintf(f, "INT2FLOAT TF@op2 TF@op2\n");
    fprintf(f, "LABEL $mul_ok%d\n", uniqueCounter);
    fprintf(f, "PUSHS TF@op2\n");
    fprintf(f, "PUSHS TF@op1\n");
    fprintf(f, "MULS\n");

    instruction->instr = NOP;
    uniqueCounter++;
}

/**
 * Function generates div operation inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 */
void generate_divs(FILE *f, tInstr *instruction)
{
    fprintf(f, "POPS TF@op2\n");
    fprintf(f, "POPS TF@op1\n\n");
    fprintf(f, "TYPE TF@op1$type TF@op1\n");
    fprintf(f, "TYPE TF@op2$type TF@op2\n\n");
    fprintf(f, "JUMPIFNEQ $div_nil$op1$%d TF@op1$type string@nil\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $div_nil$op1$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $div_nil$op2$%d TF@op2$type string@nil\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $div_nil$op2$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $div$op1$ok$%d TF@op1$type string@string\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $div$op1$ok$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $div$ok$type$%d TF@op2$type string@string\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $div$ok$type$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $div$retype$%d TF@op1$type TF@op2$type\n\n", uniqueCounter);
    fprintf(f, "LABEL $div$ok$%d\n", uniqueCounter);
    fprintf(f, "TYPE TF@op1$type TF@op1\n");
    fprintf(f, "JUMPIFEQ $idivs$%d TF@op1$type string@float\n\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $noti$op20$%d TF@op2 int@0\n", uniqueCounter);
    fprintf(f, "EXIT int@9\n\n");
    fprintf(f, "LABEL $noti$op20$%d\n", uniqueCounter);
    fprintf(f, "PUSHS TF@op1\n");
    fprintf(f, "PUSHS TF@op2\n");
    fprintf(f, "IDIVS\n");
    fprintf(f, "JUMP $end$div$%d\n\n", uniqueCounter);
    fprintf(f, "LABEL $idivs$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $notf$op20$%d TF@op2 float@0x0p+0\n", uniqueCounter);
    fprintf(f, "EXIT int@9\n\n");
    fprintf(f, "LABEL $notf$op20$%d\n", uniqueCounter);
    fprintf(f, "PUSHS TF@op1\n");
    fprintf(f, "PUSHS TF@op2\n");
    fprintf(f, "DIVS\n");
    fprintf(f, "JUMP $end$div$%d\n", uniqueCounter);
    fprintf(f, "LABEL $div$retype$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFEQ $div$retype1$%d TF@op1$type string@int\n", uniqueCounter);
    fprintf(f, "INT2FLOAT TF@op2 TF@op2\n");
    fprintf(f, "JUMP $div$ok$%d\n\n", uniqueCounter);
    fprintf(f, "LABEL $div$retype1$%d\n", uniqueCounter);
    fprintf(f, "INT2FLOAT TF@op1 TF@op1\n");
    fprintf(f, "JUMP $div$ok$%d\n\n", uniqueCounter);
    fprintf(f, "LABEL $end$div$%d\n", uniqueCounter);

    uniqueCounter++;
    instruction->instr = NOP;
}

void generate_nots(FILE *f, tInstr *instruction)
{
    fprintf(f, "NOTS\n");

    instruction->instr = NOP;
}

void generate_lts(FILE *f, tInstr *instruction)
{
    fprintf(f, "POPS TF@op2\n");
    fprintf(f, "POPS TF@op1\n\n");
    fprintf(f, "TYPE TF@op1$type TF@op1\n");
    fprintf(f, "TYPE TF@op2$type TF@op2\n\n");
    fprintf(f, "JUMPIFNEQ $lt_nil$op1$%d TF@op1$type string@nil\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $lt_nil$op1$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $lt_nil$op2$%d TF@op2$type string@nil\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $lt_nil$op2$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFEQ $lt$string$%d TF@op1$type string@string\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $lt$retype$%d TF@op1$type TF@op2$type\n\n", uniqueCounter);
    fprintf(f, "JUMPIFEQ $lt$ok$%d TF@op1$type TF@op2$type\n", uniqueCounter);
    fprintf(f, "LABEL $lt$string$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFEQ $lt$ok$%d TF@op2$type string@string\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $lt$ok$%d\n", uniqueCounter);
    fprintf(f, "PUSHS TF@op1\n");
    fprintf(f, "PUSHS TF@op2\n");
    fprintf(f, "LTS\n");
    fprintf(f, "JUMP $end$lt$%d\n\n", uniqueCounter);
    fprintf(f, "LABEL $lt$retype$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $lt$notstring$%d TF@op2$type string@string\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $lt$notstring$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFEQ $lt$retype1$%d TF@op1$type string@int\n", uniqueCounter);
    fprintf(f, "INT2FLOAT TF@op2 TF@op2\n");
    fprintf(f, "JUMP $lt$ok$%d\n", uniqueCounter);
    fprintf(f, "LABEL $lt$retype1$%d\n", uniqueCounter);
    fprintf(f, "INT2FLOAT TF@op1 TF@op1\n");
    fprintf(f, "JUMP $lt$ok$%d\n\n", uniqueCounter);
    fprintf(f, "LABEL $end$lt$%d\n", uniqueCounter);

    uniqueCounter++;
    instruction->instr = NOP;
}

void generate_gts(FILE *f, tInstr *instruction)
{
    fprintf(f, "POPS TF@op2\n");
    fprintf(f, "POPS TF@op1\n\n");
    fprintf(f, "TYPE TF@op1$type TF@op1\n");
    fprintf(f, "TYPE TF@op2$type TF@op2\n\n");
    fprintf(f, "JUMPIFNEQ $gt_nil$op1$%d TF@op1$type string@nil\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $gt_nil$op1$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $gt_nil$op2$%d TF@op2$type string@nil\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $gt_nil$op2$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFEQ $gt$string$%d TF@op1$type string@string\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $gt$retype$%d TF@op1$type TF@op2$type\n\n", uniqueCounter);
    fprintf(f, "JUMPIFEQ $gt$ok$%d TF@op1$type TF@op2$type\n", uniqueCounter);
    fprintf(f, "LABEL $gt$string$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFEQ $gt$ok$%d TF@op2$type string@string\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $gt$ok$%d\n", uniqueCounter);
    fprintf(f, "PUSHS TF@op1\n");
    fprintf(f, "PUSHS TF@op2\n");
    fprintf(f, "GTS\n");
    fprintf(f, "JUMP $end$gt$%d\n\n", uniqueCounter);
    fprintf(f, "LABEL $gt$retype$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFNEQ $gt$notstring$%d TF@op2$type string@string\n", uniqueCounter);
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $gt$notstring$%d\n", uniqueCounter);
    fprintf(f, "JUMPIFEQ $gt$retype1$%d TF@op1$type string@int\n", uniqueCounter);
    fprintf(f, "INT2FLOAT TF@op2 TF@op2\n");
    fprintf(f, "JUMP $gt$ok$%d\n", uniqueCounter);
    fprintf(f, "LABEL $gt$retype1$%d\n", uniqueCounter);
    fprintf(f, "INT2FLOAT TF@op1 TF@op1\n");
    fprintf(f, "JUMP $gt$ok$%d\n\n", uniqueCounter);
    fprintf(f, "LABEL $end$gt$%d\n", uniqueCounter);

    uniqueCounter++;
    instruction->instr = NOP;
}

void generate_eqs(FILE *f, tInstr *instruction)
{
    fprintf(f, "POPS TF@op2\n");
    fprintf(f, "POPS TF@op1\n\n");
    fprintf(f, "TYPE TF@op1$type TF@op1\n");
    fprintf(f, "TYPE TF@op2$type TF@op2\n\n");
    fprintf(f, "JUMPIFEQ $eq$ok$%d TF@op1$type TF@op2$type\n", uniqueCounter);
    fprintf(f, "PUSHS bool@false\n\n");
    fprintf(f, "JUMP $end$eq$%d\n", uniqueCounter);
    fprintf(f, "LABEL $eq$ok$%d\n", uniqueCounter);
    fprintf(f, "PUSHS TF@op1\n");
    fprintf(f, "PUSHS TF@op2\n");
    fprintf(f, "EQS\n");
    fprintf(f, "LABEL $end$eq$%d\n", uniqueCounter);

    uniqueCounter++;
    instruction->instr = NOP;
}

void generate_expression(FILE *f, tInstr *instruction)
{
    // Initialize internal variables
    fprintf(f, "# MATHEMATICAL EXPRESSION START\n");
    fprintf(f, "PUSHFRAME\n");
    fprintf(f, "CREATEFRAME\n\n");
    fprintf(f, "PUSHS nil@nil\n");
    fprintf(f, "DEFVAR TF@op1\n");
    fprintf(f, "DEFVAR TF@op2\n");
    fprintf(f, "DEFVAR TF@op1$type\n");
    fprintf(f, "DEFVAR TF@op2$type\n");

    instruction = instruction->next;

    while(instruction->instr != EXPRESSION_END)
    {
        generate_instruction(f, instruction);
        instruction = instruction->next;
    }
    
    // Expression is completed and result is on stack
    fprintf(f, "POPFRAME\n");
    fprintf(f, "# MATHEMATHICAL EXPRESSION FINISH\n");
}

void generate_concatenation(FILE *f, tInstr *instruction)
{
    int defvarnum = uniqueCounter;
    tToken where = instruction->params->param;
    fprintf(f, "DEFVAR TF@$tmpconcat$type$%d\n", defvarnum);
    fprintf(f, "DEFVAR TF@$tmpconcat$%d\n", defvarnum);
    fprintf(f, "MOVE TF@$tmpconcat$%d string@\n", defvarnum);
    while(instruction->instr != CONCAT_END)
    {
        if(instruction->instr == CONCAT)
        {
            // if its constant string, no need to control type because its string
            if(instruction->params->param.type == STRING)
            {
                fprintf(f, "CONCAT TF@$tmpconcat$%d TF@$tmpconcat$%d string@%s\n\n", defvarnum, defvarnum, instruction->params->param.attr.str.str);
            }
            // It is ID need to semantic check
            else if(instruction->params->param.type == ID)
            {
                fprintf(f, "TYPE TF@$tmpconcat$type$%d TF@%s\n", defvarnum, instruction->params->param.attr.str.str);
                fprintf(f, "JUMPIFEQ $concat$%d TF@$tmpconcat$type$%d string@string\n", uniqueCounter, defvarnum);
                fprintf(f, "EXIT int@4\n");
                fprintf(f, "LABEL $concat$%d\n", uniqueCounter);
                fprintf(f, "CONCAT TF@$tmpconcat$%d TF@$tmpconcat$%d TF@%s\n\n", defvarnum, defvarnum, instruction->params->param.attr.str.str);
            }
        }
        uniqueCounter++;
        if(instruction->next->instr == CONCAT_END)
        {
            break;
        }
        instruction = instruction->next;
    }
    // Here instruction is move, if NULL the value is not saved, need to save it just to retval
    fprintf(f, "MOVE TF@%s TF@$tmpconcat$%d\n", where.attr.str.str, defvarnum);
}

/**
 * Function generates push instruction inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 */
void generate_pushs(FILE *f, tInstr *instruction)
{
    switch(instruction->params->param.type)
    {
        case INTEGER:
            fprintf(f, "PUSHS int@%d\n", instruction->params->param.attr.i);
            break;
        case FLOAT:
            fprintf(f, "PUSHS float@%a\n", instruction->params->param.attr.f);
            break;
        case STRING:
            fprintf(f, "PUSHS string@%s\n", instruction->params->param.attr.str.str);
            break;
        case ID:
            fprintf(f, "PUSHS LF@%s\n", instruction->params->param.attr.str.str);
            break;
        case NIL:
            fprintf(f, "PUSHS nil@nil\n");
            break;
    }
    instruction->instr = NOP;
}

/**
 * Function generates pop instruction inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 */
void generate_pops(FILE *f, tInstr *instruction)
{
    if(instruction->params->param.type == ID)
    {
        fprintf(f, "POPS TF@%s\n", instruction->params->param.attr.str.str);
    }
}

/**
 * Function generates while operation inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 */
void generate_while(FILE *f, tInstr *instruction)
{
    tInstr *actualInstr = instruction;
    int scopeWhile = 0;
    int tempUniqueCounter = uniqueCounter;

    do
    {
        if(actualInstr->instr == DEFVAR)
        {
            fprintf(f, "DEFVAR TF@%s\n", instruction->params->param.attr.str.str);
            fprintf(f, "DEFVAR TF@%s nil@nil\n", instruction->params->param.attr.str.str);
            actualInstr->instr = NOP;
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

    fprintf(f, "DEFVAR TF@condition%d\n", tempUniqueCounter);
    fprintf(f, "DEFVAR TF@condition$type%d\n", tempUniqueCounter);
    fprintf(f, "LABEL $while_start%d\n", tempUniqueCounter);

    instruction = instruction->next;

    while(instruction->instr != WHILE_COND_END)
    {
        generate_instruction(f, instruction);
        instruction = instruction->next;
    }

    fprintf(f, "POPS TF@condition%d\n", tempUniqueCounter);
    fprintf(f, "TYPE TF@condition$type%d TF@condition%d\n", tempUniqueCounter, tempUniqueCounter);
    fprintf(f, "JUMPIFEQ $while%d TF@condition$type%d string@nil\n", tempUniqueCounter, tempUniqueCounter);
    fprintf(f, "JUMPIFNEQ $while_ok%d TF@condition$type%d string@bool\n", tempUniqueCounter, tempUniqueCounter);
    fprintf(f, "JUMPIFEQ $while%d TF@condition%d bool@false\n", tempUniqueCounter, tempUniqueCounter);
    fprintf(f, "LABEL $while_ok%d\n", tempUniqueCounter);

    instruction = instruction->next;
    uniqueCounter++;

    while(instruction->instr != WHILE_END)
    {
        generate_instruction(f, instruction);
        instruction->instr = NOP;
        instruction = instruction->next;
    }

    fprintf(f, "JUMP $while_start%d\n", tempUniqueCounter);
    fprintf(f, "LABEL $while%d\n", tempUniqueCounter);
    uniqueCounter++;

}

/**
 * Function generates if operation inside code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 */
void generate_if(FILE *f, tInstr *instruction)
{
    tInstr *actualInstr = instruction;
    int scopeIf = 0;
    int tempUniqueCounter = uniqueCounter;

    do
    {
        if(actualInstr->instr == DEFVAR)
        {
            fprintf(f, "DEFVAR TF@%s\n", instruction->params->param.attr.str.str);
            fprintf(f, "DEFVAR TF@%s nil@nil\n", instruction->params->param.attr.str.str);
            actualInstr->instr = NOP;
        }
        else if(actualInstr->instr == IF_END)
        {
            scopeIf--;
        }
        else if(actualInstr->instr == IF_CALL)
        {
            scopeIf++;
        }

        actualInstr = actualInstr->next;
    } while(scopeIf != 0);

    instruction = instruction->next;

    while(instruction->instr != IF_COND_END)
    {
        generate_instruction(f, instruction);
        instruction = instruction->next;
    }

    fprintf(f, "DEFVAR TF@condition%d\n", tempUniqueCounter);
    fprintf(f, "DEFVAR TF@condition$type%d\n", tempUniqueCounter);
    fprintf(f, "POPS TF@condition%d\n", tempUniqueCounter);
    fprintf(f, "TYPE TF@condition$type%d TF@condition%d\n", tempUniqueCounter, tempUniqueCounter);
    fprintf(f, "JUMPIFEQ $else%d TF@condition$type%d string@nil\n", tempUniqueCounter, tempUniqueCounter);
    fprintf(f, "JUMPIFNEQ $if_ok%d TF@condition$type%d string@bool\n", tempUniqueCounter, tempUniqueCounter);
    fprintf(f, "JUMPIFEQ $else%d TF@condition%d bool@false\n", tempUniqueCounter, tempUniqueCounter);
    fprintf(f, "LABEL $if_ok%d\n", tempUniqueCounter);

    instruction = instruction->next;
    uniqueCounter++;

    while(instruction->instr != ELSE_CALL)
    {
        generate_instruction(f, instruction);
        instruction->instr = NOP;
        instruction = instruction->next;
    }

    fprintf(f, "JUMP $else_end%d\n", tempUniqueCounter);
    fprintf(f, "LABEL $else%d\n", tempUniqueCounter);
    uniqueCounter++;
    instruction = instruction->next;

    while(instruction->instr != IF_END)
    {
        generate_instruction(f, instruction);
        instruction->instr = NOP;
        instruction = instruction->next;
    }

    fprintf(f, "LABEL $else_end%d\n", tempUniqueCounter);
    uniqueCounter++;
}

/**
 * Function generates function call with return value which is going to be saved, inside source code.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 * @param builtin Signifies if called function is builtin or user-defined.
 */
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
        else if(params->param.type == NIL)
        {
            fprintf(f, "MOVE TF@$%d nil@nil\n", paramsNum);
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
                lengthRequest = true;
                break;
            case SUBSTR_CALL:
                fprintf(f, "CALL $substr\n");
                substrRequest = true;
                break;
            case ORD_CALL:
                fprintf(f, "CALL $ord\n");
                ordRequest = true;
                break;
            case CHR_CALL:
                fprintf(f, "CALL $chr\n");
                chrRequest = true;
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
 */
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
        else if(params->param.type == NIL)
        {
            fprintf(f, "MOVE TF@$%d nil@nil\n", paramsNum);
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
                // User called length, need to request code generator, to define it later
                lengthRequest = true;
                break;
            case NOSUBSTR_CALL:
                fprintf(f, "CALL $substr\n");
                substrRequest = true;
                break;
            case NOORD_CALL:
                fprintf(f, "CALL $ord\n");
                ordRequest = true;
                break;
            case NOCHR_CALL:
                fprintf(f, "CALL $chr\n");
                chrRequest = true;
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
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 * @param moved Bool which signifies if result of the function is going to be moved.
 */
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
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 * @param moved Bool which signifies if result of the function is going to be moved.
 */
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
 * @param f Pointer to the IFJcode2018 source code.
 */
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
 * @param f Pointer to the IFJcode2018 source code.
 **/
void generate_substr(FILE *f)
{
    fprintf(f, "# DEFINITION OF BUILTIN FUNCTION SUBSTR()\n\n");
    fprintf(f, "LABEL $substr\n\n");
    fprintf(f, "MOVE TF@$retval string@\n");
    fprintf(f, "DEFVAR TF@string\n");
    fprintf(f, "DEFVAR TF@start\n");
    fprintf(f, "DEFVAR TF@lenght\n");
    fprintf(f, "DEFVAR TF@string$type\n");
    fprintf(f, "DEFVAR TF@start$type\n");
    fprintf(f, "DEFVAR TF@lenght$type\n");
    fprintf(f, "DEFVAR TF@string$lenght\n");
    fprintf(f, "DEFVAR TF@lenght_control1\n");
    fprintf(f, "DEFVAR TF@is_bigger\n");
    fprintf(f, "DEFVAR TF@is_bigger2\n");
    fprintf(f, "DEFVAR TF@max_lenght\n");
    fprintf(f, "DEFVAR TF@temp_char\n");
    fprintf(f, "DEFVAR TF@negative_start\n");
    fprintf(f, "DEFVAR TF@greater_start\n");
    fprintf(f, "DEFVAR TF@max\n");
    //Getting params
    fprintf(f, "MOVE TF@string TF@$0\n");
    fprintf(f, "MOVE TF@start TF@$1\n");
    fprintf(f, "MOVE TF@lenght TF@$2\n");
    //Getting type of params
    fprintf(f, "TYPE TF@string$type TF@string\n");
    fprintf(f, "TYPE TF@start$type TF@start\n");
    fprintf(f, "TYPE TF@lenght$type TF@lenght\n");
    //Controling if is first parameter string
    fprintf(f, "JUMPIFEQ $is_string TF@string$type string@string\n");
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $is_string\n");
    //Controling if is secong parameter int or float, if it is float, retyping
    fprintf(f, "JUMPIFEQ $is_int1 TF@start$type string@int\n");
    fprintf(f, "JUMPIFEQ $retype1 TF@start$type string@float\n");
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $retype1\n");
    fprintf(f, "FLOAT2INT TF@start TF@start\n");
    fprintf(f, "LABEL $is_int1\n");
    //Controling if is third parameter int or float, if it is float, retyping
    fprintf(f, "JUMPIFEQ $is_int2 TF@lenght$type string@int\n");
    fprintf(f, "JUMPIFEQ $retype2 TF@start$type string@float\n");
    fprintf(f, "EXIT int@4\n");
    fprintf(f, "LABEL $retype2\n");
    fprintf(f, "FLOAT2INT TF@start TF@start\n");
    fprintf(f, "LABEL $is_int2\n");
    //Controling if is secong paramter greater then lenght of string and lower then 0
    fprintf(f, "STRLEN TF@string$lenght TF@string\n");
    fprintf(f, "LT TF@negative_start TF@start int@0\n");
    fprintf(f, "JUMPIFNEQ $not_negative TF@negative_start bool@true\n");
    fprintf(f, "MOVE TF@$retval nil@nil\n");
    fprintf(f, "RETURN\n");
    fprintf(f, "LABEL $not_negative\n");
    fprintf(f, "SUB TF@string$lenght TF@string$lenght int@1\n");
    fprintf(f, "GT TF@greater_start TF@start TF@string$lenght\n");
    fprintf(f, "JUMPIFNEQ $not_greater TF@greater_start bool@true\n");
    fprintf(f, "MOVE TF@$retval nil@nil\n");
    fprintf(f, "RETURN\n");
    fprintf(f, "LABEL $not_greater\n");
    fprintf(f, "ADD TF@string$lenght TF@string$lenght int@1\n");
    //Controling how long substring gonna be
    fprintf(f, "STRLEN TF@string$lenght TF@string\n");
    fprintf(f, "SUB TF@lenght_control1 TF@string$lenght TF@start\n");
    fprintf(f, "GT TF@is_bigger TF@lenght TF@lenght_control1\n");
    //fprintf(f, "EQ TF@is_bigger2 TF@string$lenght TF@lenght_control1\n");
    //fprintf(f, "OR TF@is_bigger TF@is_bigger TF@is_bigger2 \n");
    fprintf(f, "JUMPIFNEQ $not_bigger TF@is_bigger bool@true\n");
    fprintf(f, "SUB TF@max_lenght TF@string$lenght TF@start\n");
    fprintf(f, "JUMP $okok\n");
    fprintf(f, "LABEL $not_bigger\n");
    fprintf(f, "MOVE TF@max_lenght TF@lenght\n");
    fprintf(f, "LABEL $okok\n");
    //getting substring
    fprintf(f, "ADD TF@max TF@start TF@max_lenght\n");
    //fprintf(f, "SUB TF@max TF@max int@1\n");
    fprintf(f, "LABEL $for\n");
    fprintf(f, "JUMPIFEQ $for_end TF@max TF@start\n");
    fprintf(f, "GETCHAR TF@temp_char TF@string TF@start\n");
    fprintf(f, "CONCAT TF@$retval TF@$retval TF@temp_char\n");
    fprintf(f, "ADD TF@start TF@start int@1\n");
    fprintf(f, "JUMP $for\n");
    fprintf(f, "LABEL $for_end\n");
    fprintf(f, "RETURN\n");
    fprintf(f, "# END OF DEFINITION OF BUILTIN FUNCTION SUBSTR()\n\n");
}

/**
 * Function generates ord(s, i) built-infunction into IFJcode2018.
 * @param f Pointer to the IFJcode2018 source code.
 */
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
    fprintf(f, "JUMPIFEQ $string$OKKK TF@string$type string@string\n");
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $string$OKKK\n");
    // If int everything is allright
    fprintf(f, "JUMPIFEQ $int$OKO TF@int$type string@int\n");
    // If float, need to FLOAT2INT
    fprintf(f, "JUMPIFEQ $int$FLOATO TF@int$type string@float\n");
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $int$FLOATO\n");
    fprintf(f, "FLOAT2INT TF@int TF@int\n\n");
    fprintf(f, "LABEL $int$OKO\n");
    fprintf(f, "DEFVAR TF@string$length\n");
    fprintf(f, "STRLEN TF@string$length TF@string\n\n");
    fprintf(f, "DEFVAR TF@int$BOOL\n");
    fprintf(f, "LT TF@int$BOOL TF@int TF@string$length\n");
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
 * @param f Pointer to the IFJcode2018 source code.
 */
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
    fprintf(f, "JUMPIFEQ $int$FLOATC TF@int$type string@float\n");
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $int$FLOATC\n");
    fprintf(f, "FLOAT2INT TF@int TF@int\n\n");
    fprintf(f, "LABEL $int$LT\n");
    fprintf(f, "LT TF@int$BOOL TF@int int@256\n");
    fprintf(f, "JUMPIFEQ $int$GT TF@int$BOOL bool@true\n");
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $int$GT\n");
    fprintf(f, "GT TF@int$BOOL TF@int int@-1\n");
    fprintf(f, "JUMPIFEQ $int$OKC TF@int$BOOL bool@true\n");
    fprintf(f, "EXIT int@4\n\n");
    fprintf(f, "LABEL $int$OKC\n");
    fprintf(f, "INT2CHAR TF@$retval TF@int\n");
    fprintf(f, "RETURN\n\n");
    fprintf(f, "# END OF DEFINITION OF BUILTIN FUNCTION CHR()\n\n");
}

/**
 * Function generates ONE instruction only.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the single instruction from inside code.
 */
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
        case PUSHS:
            generate_pushs(f,instruction);
            break;
        case POPS:
            generate_pops(f, instruction);
            break;
        case PRINT_CALL:
            generate_print(f,instruction,true);
            break;
        case NOPRINT_CALL:
            generate_print(f,instruction,false);
            break;
        case EXPRESSION_CALL:
            generate_expression(f, instruction);
            break;
        case ADDS:
            generate_adds(f, instruction);
            break;
        case SUBS:
            generate_subs(f, instruction);
            break;
        case MULS:
            generate_muls(f, instruction);
            break;
        case DIVS:
            generate_divs(f, instruction);
            break;
        case LTS:
            generate_lts(f, instruction);
            break;
        case GTS:
            generate_gts(f, instruction);
            break;
        case EQS:
            generate_eqs(f, instruction);
            break;
        case CONCAT_CALL:
            generate_concatenation(f, instruction);
            break;
        case NOTS:
            generate_nots(f, instruction);
            break;
        case NOP:
            break;
    }
}

/**
 * Function generates ALL defvars inside main.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the list of instructions.
 */
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
        }
        if(instruction->instr == DEFVAR)
        {
            fprintf(f, "DEFVAR TF@%s\n", instruction->params->param.attr.str.str);
            fprintf(f, "MOVE TF@%s nil@nil\n", instruction->params->param.attr.str.str);
            // replace defvar with nop to skip this instruction next time
            instruction->instr = NOP;
        }
        instruction = instruction->next;
    }
}

/**
 * Function generates ALL defvars inside function.
 * @param f Pointer to the IFJcode2018 source code.
 * @param instruction Pointer to the list of instructions.
 */
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
            fprintf(f, "MOVE TF@%s nil@nil\n", instruction->params->param.attr.str.str);
            // replace defvar with nop to skip this instruction next time
            instruction->instr = NOP;
        }
        instruction = instruction->next;
    }
}

/**
 * Function generates instruction one by one but ignores function definitions and everything inside it until end macro.
 * @param f Pointer to the IFJcode2018 source code.
 */
void generate_main(FILE *f)
{
    fprintf(f, "########## MAIN ##########\n");
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
 */
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
        case POPS:
        case FUN_CALL:
        case INPUTF_CALL:
        case INPUTI_CALL:
        case INPUTS_CALL:
        case LENGTH_CALL:
        case SUBSTR_CALL:
        case ORD_CALL:
        case CHR_CALL: 
        case CONCAT_CALL:
        case CONCAT_END:
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
 */
void generate_fundef(FILE *f)
{
    fprintf(f, "########## USER'S FUNCTIONS ##########\n");
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
        begin = begin->next;
    }
    if(ret.type == NORETVAL) // If we stored string in ret token, we need to free it
    {
        str_free(&ret.attr.str);
    }
}

/**
 * Function generates builtin functions based on request bools.
 * @param f Pointer to the IFJcode18 source code.
 */
void generate_builtin(FILE *f)
{
    fprintf(f, "########## BUILTIN FUNCTIONS ##########\n");
    // If built in function was called, must check if it was generated already, if it was not generate it, else do nothing
    while(chrRequest != false || lengthRequest != false || substrRequest != false || ordRequest != false)
    {
        if(lengthRequest == true)
        {
            generate_length(f);
            // function was generated, set bool to false so function will not be generated again
            lengthRequest = false;
        }
        else if(substrRequest == true)
        {
            generate_substr(f);
            substrRequest = false;
        }
        else if(ordRequest == true)
        {
            generate_ord(f);
            ordRequest = false;
        }
        else if(chrRequest == true)
        {
            generate_chr(f);
            chrRequest = false;
        }
    }
}


/**
 * Function which generates parsed inside code to final IFJcode2018 code.
 */
void generate_code()
{
    if(ilist == NULL)
    {
        return;
    }
    FILE *f = generate_head();
    generate_main(f);
    generate_fundef(f);
    generate_builtin(f);
    fclose(f);
    free_ilist(ilist);
}
