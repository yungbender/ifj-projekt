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
bool validate_symbol(string name)
{
    if(str_cmp_const_str(&(name),"def") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"do") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"else") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"end") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"if") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"not") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"nil") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"then") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"while") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"inputs") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"inputi") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"inputf") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"print") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"lengths") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"substr") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"ord") == 0)
    {
        return false;
    }
    else if (str_cmp_const_str(&(name),"chr") == 0)
    {
        return false;
    }
    return true;
}

bool validate_variable(string name)
{
    // Check if the variable name is not built-in function name or keyword
    bool result = validate_symbol(name);
    if(result != false)
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
    bool result = validate_symbol(pData.token.attr.str);
    if(result == false)
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

void start()
{
    switch(pData.token.type)
    {
        // <start> DEF <f_decl> IDF ( <params> EOL <f_def> <start>
        case DEF:
            // <f_decl> IDF ( <param> EOL <f_def>
            // If parser found def keyword inside function
            function_declaration();
            // end of function parsing
            pData.token = get_token();
            start();
            break;
        // <start> ID = 
        case ID:
            
            break;
        // <start> ID_F -> <f_call>
        case ID_F:
            function_call();
            // Function was called and exectuted, parse next
            pData.token = get_token();
            start();
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
            // If there are scopes, end just returns back to the parsing function (parser_function_definition, while_function, if_function)
            return;
            // This break instruction is dead code, there is no possibility that parser reaches this break!
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

int main()
{
    return 0;
}