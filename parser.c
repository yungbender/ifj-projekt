#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"

tPData pData;

void parser_init()
{
    pData.global = (tSymTable *)malloc(sizeof(struct symTable));
    pData.local = (tSymTable *)malloc(sizeof(struct symTable));
    pData.instrs = (tIList *)malloc(sizeof(struct instructionList));
    pData.stack = (tStack *)malloc(sizeof(struct nodeStack));
    pData.scopes = 0;
    pData.inDefinition = false;
    init_stack(pData.stack);
    init_ilist(pData.instrs);
    init_table(pData.global);
    init_table(pData.local);
}

void error(int id)
{
    free_symtable(pData.global);
    free_symtable(pData.local);
    free_ilist(pData.instrs);
    free_stack(pData.stack);
    print_error_exit(id);
}

/**
 * Function searches if string is keyword or built-in function.
 * @params name Name of the function/variable.
 * @returns True if it is validate name, false if not.
 **/ 
int validate_symbol(string name)
{
    if(str_cmp_const_str(&(name),"def") == 0)
    {
        return DEF;
    }
    else if (str_cmp_const_str(&(name),"do") == 0)
    {
        return DO;
    }
    else if (str_cmp_const_str(&(name),"else") == 0)
    {
        return ELSE;
    }
    else if (str_cmp_const_str(&(name),"end") == 0)
    {
        return END;
    }
    else if (str_cmp_const_str(&(name),"if") == 0)
    {
        return IF;
    }
    else if (str_cmp_const_str(&(name),"not") == 0)
    {
        return NOT;
    }
    else if (str_cmp_const_str(&(name),"nil") == 0)
    {
        return NIL;
    }
    else if (str_cmp_const_str(&(name),"then") == 0)
    {
        return THEN;
    }
    else if (str_cmp_const_str(&(name),"while") == 0)
    {
        return WHILE;
    }
    else if (str_cmp_const_str(&(name),"inputs") == 0)
    {
        return INPUTS_CALL;
    }
    else if (str_cmp_const_str(&(name),"inputi") == 0)
    {
        return INPUTI_CALL;
    }
    else if (str_cmp_const_str(&(name),"inputf") == 0)
    {
        return INPUTF_CALL;
    }
    else if (str_cmp_const_str(&(name),"print") == 0)
    {
        return PRINT_CALL;
    }
    else if (str_cmp_const_str(&(name),"lengths") == 0)
    {
        return LENGTH_CALL;
    }
    else if (str_cmp_const_str(&(name),"substr") == 0)
    {
        return SUBSTR_CALL;
    }
    else if (str_cmp_const_str(&(name),"ord") == 0)
    {
        return ORD_CALL;
    }
    else if (str_cmp_const_str(&(name),"chr") == 0)
    {
        return CHR_CALL;
    }
    return OK;
}

void validate_params(tNode *root)
{
    tInstr *head = pData.instrs->head;
    tTList *params = NULL;
    tNode *function = NULL;
    int callingNum = 0;

    while(head != NULL)
    {
        // FUN_CALL has first parameter where function return value is saved, second parameter name of the function, other parameters are function parameters
        // NOFUN_CALL has first parameter name of function, other parameters are fucntion parameters
        if(head->instr == FUN_CALL || head->instr == NOFUN_CALL)
        {
            int result = OK;
            params = head->params;
            // If FUN_CALL parser must skip first parameter beacause its where return value is stored
            if(head->instr == FUN_CALL)
            {
                params = params->next;
            }
            // Search the function inside global table to get paramsNum
            // For every paramter token increase counter of function call parameters
            params = params->next;
            while(params != NULL)
            {
                callingNum++;
                params = params->next;
            }
            // Parser must check first if user called built in function or user defined function
            result = validate_symbol(params->param.attr.str);
            // It is built in function, must check its parameters
            if(result != OK)
            {
                // These functions cant have any calling parameter
                if(result == INPUTF_CALL || result == INPUTI_CALL || result == INPUTS_CALL)
                {
                    if(callingNum != 0)
                    {
                        error(PARAM_NUM);
                    }
                }
                // These functions must have only one parameter
                else if(result == LENGTH_CALL || result == CHR_CALL)
                {
                    if(callingNum != 1)
                    {
                        error(PARAM_NUM);
                    }
                }
                // These functions must have only two parameters
                else if(result == ORD_CALL)
                {
                    if(callingNum != 2)
                    {
                        error(PARAM_NUM);
                    }
                }
                // These functions must have only three parameters
                else if(result == SUBSTR_CALL)
                {
                    if(callingNum != 3)
                    {
                        error(PARAM_NUM);
                    }
                }
                // These functions must have more than zero parameters
                else if(result == PRINT_CALL)
                {
                    if(callingNum == 0)
                    {
                        error(PARAM_NUM);
                    }
                }
            }
            // Its user defined function, must check number of parameters from global symbol table
            else
            {
                // If the function was not found (dead code) or the number of calling parameters is not equal to function parameters number semantic error
                function = search_table(root,params->param.attr.str);
                if(function == NULL || function->paramsNum != callingNum)
                {
                    error(PARAM_NUM);
                }
            }

        }
        callingNum = 0;
        head = head->next;
    }
}

void validate_calls(tNode *root)
{
    if(root == NULL)
    {
        return;
    }
    if(root->rptr != NULL)
    {
        validate_calls(root->rptr);
    }
    if(root->lptr != NULL)
    {
        validate_calls(root->lptr);
    }
    if(root->wasDefined == false)
    {
        error(UNDEF_F);
    }
}

bool validate_variable(string name)
{
    // Check if the variable name is not built-in function name or keyword
    int result = validate_symbol(name);
    if(result != OK)
    {
        error(IDF_REDEF);
    }
    // Search the if the variable is inside local table
    tNode *temp = search_table(pData.local->root, name);
    // Expecting NULL, if not its redefinition
    if(temp != NULL)
    {
        error(IDF_REDEF);
    }

    return true;
}

// <f_def> <start> <f_decl>
void parse_function_definition()
{
    // Setting if the parser is currently in definition to true, and also incrementing scope number
    pData.inDefinition = true;
    pData.scopes++;
    // Parser needs to save now the local table, to work with a new one inside function definition
    push_stack(pData.stack,pData.local->root);
    pData.local->root = NULL;
    // Recursively calling parser
    pData.token = get_token();
    start();
    // Function is parsed without error, saving end keyword
    insert_instr(pData.instrs,FUN_END);
    // Restoring back local table, restoring bool and decrementing scope
    pData.local->root = pop_stack(pData.stack);
    pData.inDefinition = false;
    pData.scopes--;
}

// <params> ID  <params> ) <f_decl>
void params(tNode *function)
{
    pData.token = get_token();
    // Expecting ID or parenth
    // If we get parenth there are no parameters for function
    if(pData.token.type == CLOSE_PARENTH)
    {
        function->paramsNum = 0;
        return;
    }

    if(pData.token.type != ID)
    {
        error(UNEXPECTED_F);
    }
    // Now parser will take every parameter until token is not ID
    while(pData.token.type == ID)
    {
        // Need to save the parameter inside instruction list
        insert_param(pData.instrs, pData.token);
        function->paramsNum++;
        // Expecting comma or close parenth
        pData.token = get_token();
        // If there is no other argument, expect close parenth and return
        if(pData.token.type == CLOSE_PARENTH)
        {
            return;
        }
        // If there was not close parenth, there must be comma and next parameter, else syntax error
        if(pData.token.type != COMMA)
        {
            error(UNEXPECTED_F);
        }
        pData.token = get_token();
        if(pData.token.type != ID)
        {
            error(UNEXPECTED_F);
        }
    }
}

void function_declaration()
{
    // Function definition implementation
    // Cannot define function inside other function
    if(pData.inDefinition == true)
    {
        error(UNEXPECTED_F);
    }
    // Expecting function identificator
    pData.token = get_token();
    if(pData.token.type != IDF && pData.token.type != ID)
    {
        error(IDF);
    }
    // Expecting valid function ID, that means: no keyword, no redefinition of built-in functions
    int result = validate_symbol(pData.token.attr.str);
    if(result != OK)
    {
        error(IDF_REDEF);
    }
    // Now needs to check if the function name is in the local table (variables), if it is, its error, there cannot be same name for function and variable
    tNode *temp = search_table(pData.local->root,pData.token.attr.str);
    if(temp != NULL)
    {
        error(IDF_REDEF);
    }
    // Funtion name is correct and is not a keyword, nor variable name, now needs to check if the function was already called or not (if it was called, there is global table node created already)
    temp = search_table(pData.global->root,pData.token.attr.str);
    // Function was already called somewhere inside code, parser must mark that the definintion of the function was found.
    if(temp != NULL)
    {
        temp->wasDefined = true;
    }
    else if(temp == NULL) // Function was not called, parser must insert new symbol inside symbol table (into Global)
    {
        // If its first node we must initalize the whole table root
        if(pData.global->root == NULL)
        {
            pData.global->root = insert_fun(pData.global->root,pData.token,0, true);
        }
        // If its not first node, just insert
        else
        {
            insert_fun(pData.global->root,pData.token,0, true);
        }
    }
    // Need to get function node again, to save function parameters
    tNode *function = search_table(pData.global->root,pData.token.attr.str);
    if(function == NULL)
    {
        error(INT_ERR);
    }
    // Function definition is correct, parser needs to generate instruction for code generation and save function name
    insert_instr(pData.instrs, FUN_DEF);
    insert_param(pData.instrs, pData.token);
    // Parse function parameters
    pData.token = get_token();
    if(pData.token.type != OPEN_PARENTH)
    {
        error(UNEXPECTED_F);
    }
    params(function);
    // Parameters are parsed, expecting EOL
    pData.token = get_token();
    if(pData.token.type != END_OF_LINE)
    {
        error(UNEXPECTED_F);
    }
    // Expecting function definition
    parse_function_definition();
    // Function was parsed, get token and start parsing
    pData.token = get_token();
    start();
}

void function_call()
{
    
}

void end_of_file()
{
    // Check for unexpected end of file (in definition or in while)
    if(pData.inDefinition == true || pData.scopes > 0)
    {
        error(UNEXPECTED_EOF);
    }
    // Check if there is some function that was called but not defined
    validate_calls(pData.global->root);
    validate_params(pData.global->root);
    free_symtable(pData.global);
    free_symtable(pData.local);
    free_stack(pData.stack);
    // Give the final instruction list to the code generator
    ilist = pData.instrs;
}

void end_of_line()
{
    pData.token = get_token();
    start();
}

void start()
{
    switch(pData.token.type)
    {
        // <start> DEF <f_decl> IDF ( <params> EOL <f_def> <start>
        case DEF:
            // <f_decl> IDF ( <param> EOL <f_def>
            // If parser found def keyword inside function
            function_declaration();
            break;
        // <start> ID = 
        case ID:
            
            break;
        case END_OF_LINE:
            end_of_line();
            break;
        // <start> ID_F -> <f_call>
        case ID_F:
            function_call();
            // Function was called and exectuted, parse next
            break;
        // <end> -> <f_def>
        // <end> -> <if>
        // <end> -> <else>
        // <end> -> <while>
        case END:
            // If there no scopes, and END is called = syntax error
            if(pData.scopes == 0)
            {
                error(SY_ERR);
            }
            // Expecting EOL or EOF
            pData.token = get_token();
            if(pData.token.type != END_OF_FILE && pData.token.type != END_OF_LINE)
            {
                error(UNEXPECTED_END);
            }
            // If there are scopes, end just returns back to the parsing function (parser_function_definition, while_function, if_function)
            return;
            // This break instruction is dead code, there is no possibility that parser reaches this break!
            break;
            // <EOF> -> end
        case END_OF_FILE:
            // Need to check if we are not in function, check if all scopes are 0 and if all called functions got their own definition
            end_of_file();
            break;
    }
}

void parse()
{
    parser_init();
    // get first token
    pData.token = get_token();
    start();
}
