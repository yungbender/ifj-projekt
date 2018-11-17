#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"

tPData pData;

#define GET_TOKEN() \
    pData.token = get_token(); \
    if(pData.token.type == L_ERR) \
        error(L_ERR);

void parser_init()
{
    pData.global = (tSymTable *)malloc(sizeof(struct symTable));
    pData.local = (tSymTable *)malloc(sizeof(struct symTable));
    pData.instrs = (tIList *)malloc(sizeof(struct instructionList));
    pData.stack = (tStack *)malloc(sizeof(struct tokenStack));
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
            tToken name = params->param;
            if(params != NULL)
            {
                params = params->next;
            }
            while(params != NULL)
            {
                callingNum++;
                params = params->next;
            }
            // Parser must check first if user called built in function or user defined function
            result = validate_symbol(name.attr.str);
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
                function = search_table(root,name.attr.str);
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
    tNode *temp = pData.local->root;
    pData.local->root = NULL;
    // Parameters of this function are still on stack and needs to get in the new local symtable
    tStack *stack = pData.stack;
    while(stack->head.type != EMPTY)
    {
        tToken param = head_stack(stack);
        pop_stack(stack);
        if(pData.local->root == NULL)
        {
            pData.local->root = insert_var(pData.local->root,param);
        }
        else
        {
            insert_var(pData.local->root,param);
        }
    }
    // Recursively calling parser
    GET_TOKEN();
    start();
    // Function is parsed without error, saving end keyword
    insert_instr(pData.instrs,FUN_END);
    // Restoring back local table, restoring bool and decrementing scope
    free_tree(pData.local->root);
    pData.local->root = temp;
    pData.inDefinition = false;
    pData.scopes--;
}

// <params> ID  <params> ) <f_decl>
void params(tNode *function)
{
    GET_TOKEN();
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
    // Clear out the so parser can save params one by one and compare them 
    clear_stack(pData.stack);
    while(pData.token.type == ID)
    {
        // Need to save the parameter inside instruction list
        insert_param(pData.instrs, pData.token);
        function->paramsNum++;
        // Now needs to compare, if parameters doesnt have same name
        tStack *stackHead = pData.stack;
        // Compare every parameter
        while(stackHead->head.type != EMPTY)
        {
            if((str_cmp_string(&(pData.token.attr.str),&(stackHead->head.attr.str)) == 0))
            {
                error(SAME_PARAM);
            }
            stackHead = stackHead->next;
        }
        // If the parameter is correct push the parameter to get compared with next parameter
        push_stack(pData.stack,pData.token);
        // Parameters will stay pushed on the stack so parser, can later add them to the new local table
        // Expecting comma or close parenth
        GET_TOKEN();
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
        GET_TOKEN();
        if(pData.token.type != ID)
        {
            error(UNEXPECTED_F);
        }
    }
}

void function_declaration()
{
    // Function definition implementation
    // Cannot define function inside other function or inside while, if
    if(pData.inDefinition == true || pData.scopes > 0)
    {
        error(UNEXPECTED_F);
    }
    // Expecting function identificator
    GET_TOKEN();
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
    GET_TOKEN();
    if(pData.token.type != OPEN_PARENTH)
    {
        error(UNEXPECTED_F);
    }
    params(function);
    // Parameters are parsed, expecting EOL
    GET_TOKEN();
    if(pData.token.type != END_OF_LINE)
    {
        error(UNEXPECTED_F);
    }
    // Expecting function definition
    parse_function_definition();
    // Function was parsed, get token and start parsing
    GET_TOKEN();
    start();
}

void function_call(bool moved)
{
    // Check if the called function is in the global table
    tNode *result = search_table(pData.global->root,pData.token.attr.str);
    int builtin = OK;
    // If parser did not found function inside global symtable and parser is inside main, semantic error
    if(result == NULL && pData.inDefinition == false)
    {
        // Check if the called function is not built in function
        builtin = validate_symbol(pData.token.attr.str);
        if(result == OK)
        {
            error(UNDEF_F);
        }
    }
    // If parser did not found function, but is inside function definition, parser just creates new function inside global symtable that it was called but not still not defined
    else if(result == NULL && pData.inDefinition == true)
    {
        // Check if the called function is not built in function
        builtin = validate_symbol(pData.token.attr.str);
        if(builtin == OK)
        {
            insert_fun(pData.global->root,pData.token,0,false);
        }
    }
    // Add function calling as instruction
    // If the function result value will not be moved
    if(moved == false)
    {
        // If its not builtin function
        if(builtin == OK)
        {
            insert_instr(pData.instrs,NOFUN_CALL);
            insert_param(pData.instrs,pData.token);
        }
        // If its built int just NOBUILTINCALL (BUILTINCALL + DIFFERENCE)
        else
        {
            insert_instr(pData.instrs,(builtin + NOCALL_CALL_DIFFERENCE));
        }
    }
    // If will be moved
    else
    {
        // insert instruction and pop where is it going to bo safed
        if(builtin == OK)
        {
            insert_instr(pData.instrs,FUN_CALL);
            tToken name = head_stack(pData.stack);
            pop_stack(pData.stack);
            insert_param(pData.instrs,name);
        }
        // If its builtin function
        else
        {
            insert_instr(pData.instrs,builtin);
            tToken name = head_stack(pData.stack);
            pop_stack(pData.stack);
            insert_param(pData.instrs,name);
        }
    }

    // Bool if leftbracket to check for rightbracket
    bool leftbracket = false;
    bool rightbracket = false;
    GET_TOKEN();
    // Expecting leftbracket or ID or EOF
    // If leftbracket, activate bool and get next token
    if(pData.token.type == OPEN_PARENTH)
    {
        leftbracket = true;
        GET_TOKEN();
    }
    // Bool for checking comma
    bool comma = false;
    // Get parameters
    while(true)
    {
        // If close parenth, must check if there was open parenth and return
        if(pData.token.type == CLOSE_PARENTH)
        {
            if(leftbracket == false || comma == true)
            {
                error(UNEXPECTED_F);
            }
            // Jump from the while
            rightbracket = true;
            break;
        }
        // If it is variable search it in local symtable, if int,float,string, just add as paramter
        else if(pData.token.type == ID || pData.token.type == INTEGER || pData.token.type == FLOAT || pData.token.type == STRING)
        {
            comma = false;
            // Search if the variable is defined, if not, error
            if(pData.token.type == ID)
            {
                result = search_table(pData.local->root,pData.token.attr.str);
                if(result == NULL)
                {
                    error(UNDEF_V);
                }
            }
            // If is, generate parameter
            insert_param(pData.instrs,pData.token);
        }
        // Unexpected token
        else if(pData.token.type == END_OF_LINE)
        {
            if(comma == true)
            {
                error(WRONG_PARAM);
            }
            break;
        }
        else if(pData.token.type == COMMA)
        {
            comma = true;
        }
        else
        {
            error(WRONG_PARAM);
        }
        // Get next parameter and parse
        GET_TOKEN();
        // Expecting comma or EOL

    }
    // Here is EOL, need to check parenths are allright
    if(leftbracket == true && rightbracket != true)
    {
        error(WRONG_PARAM);
    }
    // Function call is parsed, get next token and continue parsing.
    GET_TOKEN();
    start();
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
    // Free out the symtables and stack
    free_symtable(pData.global);
    free_symtable(pData.local);
    free_stack(pData.stack);
    // Give the final instruction list to the code generator
    ilist = pData.instrs;
}

void while_loop()
{
    int tokenType = pData.token.type;
    // Got into while scope
    pData.scopes++;
    insert_instr(pData.instrs, WHILE_CALL);
    GET_TOKEN();
    // If is in condition something else then variable, float, int or string constant
    if(tokenType != ID && tokenType != INTEGER && tokenType != FLOAT && tokenType != STRING)
    {
        error(DATA_ERR);
    } 

    //TODO: spracovanie výrazu;

    GET_TOKEN();
    // Expecting DO after condition
    if(tokenType != DO)
    {
        error(EXPECTED_DO);
    }

    GET_TOKEN();
    // Expecting EOL after DO
    if(pData.token.type != END_OF_LINE)
    {
        error(UNEXPECTED_W);
    }
    // Parsing body of while
    GET_TOKEN();
    start();
    // While was successfully parsed
    insert_instr(pData.instrs, WHILE_END);
    pData.scopes--;
    GET_TOKEN();
    start();
}

void end_of_line()
{
    GET_TOKEN();
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
            GET_TOKEN();
            start();
            break;
        case END_OF_LINE:
            end_of_line();
            break;
        // <start> IDF -> <f_call>
        case IDF:
            function_call(false);
            break;
        // <end> -> <f_def>
        // <end> -> <if>
        // <end> -> <else>
        // <end> -> <while>
        case WHILE_CALL:
            while_loop();
            break;
        case END:
            // If there no scopes, and END is called = syntax error
            if(pData.scopes == 0)
            {
                error(SY_ERR);
            }
            // Expecting EOL or EOF
            GET_TOKEN();
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
    GET_TOKEN();
    start();
}
