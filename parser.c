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
    pData.inDefinition = false;
    ilist_init(pData.instrs);
    stack_init(pData.global);
    stack_init(pData.local);
}

void error(int id)
{
    free_symtable(pData.global);
    free_symtable(pData.local);
    free_ilist(pData.instrs);
    print_error_exit(id);
}

bool validate_variable(string name)
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
    // Search the if the variable/function is inside global table
    tNode *temp = search_global_table(pData.global, pData.token.attr.str);
    // Expecting NULL, if not its redefinition
    if(temp != NULL)
    {
        return false;
    }
    return true;
}

/**
 * Function checks if the function name is viable. Function name cannot be same as keywords and also cannot be same as default IFJ18 functions.
 * @param name String which contains requested function name.
 * @return Function returns boolean true if the name is viable and correct, false if it is not.
 **/
bool validate_function(string name)
{
    bool result = validate_variable(name);
    if(result == false)
    {
        return false;
    }
    if (str_cmp_const_str(&(name),"inputs") == 0)
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
// <f_def> <start> <f_decl>
void parse_function_definition(tNode *function)
{
    // Function definition is going to be parsed, need to swap the global table with local so parsing can be simulated like it was in "main"
    tSymTable *temp = pData.global;
    tIList *listBackup = pData.instrs;
    pData.global = pData.local;
    pData.local = temp;
    // Setting if the parser is currently in definition to true so parser can search functions in local symtable
    pData.inDefinition = true;
    pData.instrs = NULL;
    // Recursively calling parser
    start();
    // Function is parsed restoring symtables and saving the instruction list to the BST
    function->instrs = pData.instrs;
    pData.global = pData.local;
    free_symtable(pData.local);
    pData.local = NULL;
    pData.instrs = listBackup;
}

// <params> ID  <params> ) <f_decl>
void params(tNode *function)
{
    pData.token = get_token();
    // Expecting ID or parenth
    if(pData.token.type == CLOSE_PARENTH)
    {
        return;
    }

    if(pData.token.type != ID)
    {
        error(UNEXPECTED_F);
    }
    // Now parser will take every parameter until token is not ID
    while(pData.token.type == ID)
    {
        // If the parameter is first, initialize the whole list
        if(function->params == NULL)
        {
            function->params = insert_param(function->params,pData.token.attr.str);
            function->paramsNum++;
        }
        // If the parameters is not first, just add the parameter to the list
        else
        {
            insert_param(function->params,pData.token.attr.str);
            function->paramsNum++;
        }
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

void start()
{
    switch(pData.token.type)
    {
        // <start> DEF <f_decl> IDF ( <params> EOL <f_def> <start>
        case DEF:
            // <f_decl> IDF ( <param> EOL <f_def>
            // If parser found def keyword inside function
            if(pData.inDefinition == true)
            {
                error(UNEXPECTED_F);
            }
            // Function definition implementation
            pData.token = get_token();
            // Expecting function identificator
            if(pData.token.type != IDF)
            {
                error(IDF);
            }
            // Expecting valid function ID, that means: no keyword, no redefinition of existing function
            bool result = validate_function(pData.token.attr.str);
            if(result == false)
            {
                error(IDF_REDEF);
            }

            // Funtion name is correct and is not defined already, need to save the function name for including in table
            if(pData.global->root == NULL) // If the table is empty, the root must be saved as first node
            {
                pData.global->root = insert_fun(pData.global->root,pData.token,0);
            }
            else
            {
                insert_fun(pData.global->root,pData.token,0);
            }
            tNode *function = search_global_table(pData.global,pData.token.attr.str);
            if(function == NULL)
            {
                error(INT_ERR);
            }
            // Parse parameters function parameters
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
            parse_function_definition(function);
            // end of function parsing
            pData.token = get_token();
            start();
            break;
        // <start> ID = 
        case ID:

            
            break;
        // <start> ID_F -> <f_call>
        case ID_F:
            
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