/************************************************************************
 * 
 * Compiler implementation for imperative programming language IFJ18
 * 
 * Autors:
 * Sasák Tomáš - xsasak01
 * Venkrbec Tomáš - xvenkr01
 * Krajči Martin - xkrajc21
 * Dižová Natália - xdizov00 
 * 
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "expression.h"

tPData pData; //< This global variable, are data of parser, symbol tables, instruction list etc.
bool endoffile = false; //< This global bool, signifies if endofline was reached, to prevent double free.
tToken noretval; //< THis global token, is used for inside compiler variable $noretval


/**
 * Function initializes structure tPData, which are data of parser.
 */
void parser_init()
{
    pData.global = (tSymTable *)malloc(sizeof(struct symTable));
    pData.local = (tSymTable *)malloc(sizeof(struct symTable));
    pData.instrs = (tIList *)malloc(sizeof(struct instructionList));
    pData.stack = (tStack *)malloc(sizeof(struct tokenStack));
    pData.currentLine = 0;
    pData.scopes = 0;
    pData.inDefinition = false;

    CREATE_NORETVAL_TOKEN();

    init_stack(pData.stack);
    init_ilist(pData.instrs);
    init_table(pData.global);
    init_table(pData.local);
}

/**
 * Function free's out all parser resources (pData) and call's error print function.
 * Function is only called when semantic or synthathic analysis fails.
 * @param id Type of error.
 * @param line Number of line where is error.
 */
void error(int id, int line)
{
    free_symtable(pData.global);
    free_symtable(pData.local);
    free_ilist(pData.instrs);
    //str_free(&noretval.attr.str);
    free_stack(pData.stack);
    if(pData.token.type == ID || pData.token.type == STRING || pData.token.type == IDF)
    {
        str_free(&pData.token.attr.str);
    }

    print_error_exit(id, line);
}

/**
 * Function searches if string is keyword or built-in function.
 * @param name Name of the function/variable.
 * @return Macro number of keyword or builtin function from code_generator.h.
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
    else if (str_cmp_const_str(&(name),"length") == 0)
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

/**
 * Function validates number of every function call.
 * @param root Pointer to the global symbol table, which has all functions.
 */
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
            // Its user defined function, must check number of parameters from global symbol table
            // If the function was not found (dead code) or the number of calling parameters is not equal to function parameters number semantic error
            function = search_table(root,name.attr.str);
            if(function == NULL || function->paramsNum != callingNum)
            {
                error(PARAM_NUM, pData.currentLine);
            }
        }
        // Built in function call with not saved result
        else if(head->instr == NOINPUTF_CALL || head->instr == NOINPUTI_CALL || head->instr == NOINPUTS_CALL || head->instr == NOCHR_CALL || head->instr == NOLENGTH_CALL || head->instr == NOORD_CALL || head->instr == NOPRINT_CALL || head->instr == NOSUBSTR_CALL)
        {
            int result = head->instr;
            params = head->params;
            while(params != NULL)
            {
                callingNum++;
                params = params->next;
            }
                            
            if(result == NOINPUTF_CALL || result == NOINPUTI_CALL || result == NOINPUTS_CALL)
            {
                if(callingNum != 0)
                {
                    error(PARAM_NUM, pData.currentLine);
                }
            }
            else if(result == NOLENGTH_CALL || result == NOCHR_CALL)
            {
                if(callingNum != 1)
                {
                    error(PARAM_NUM, pData.currentLine);
                }
            }
            else if(result == NOORD_CALL)
            {
                if(callingNum != 2)
                {
                    error(PARAM_NUM, pData.currentLine);
                }
            }
            else if(result == NOSUBSTR_CALL)
            {
                if(callingNum != 3)
                {
                    error(PARAM_NUM, pData.currentLine);
                }
            }
            else if(result == NOPRINT_CALL)
            {
                if(callingNum == 0)
                {
                    error(PARAM_NUM, pData.currentLine);
                }
            }

        }
        // Built in function call with saved result
        else if(head->instr == INPUTF_CALL || head->instr == INPUTI_CALL || head->instr == INPUTS_CALL || head->instr == LENGTH_CALL || head->instr == CHR_CALL || head->instr == ORD_CALL || head->instr == SUBSTR_CALL || head->instr == PRINT_CALL)
        {
            int result = head->instr;
            params = head->params;
            while(params != NULL)
            {
                callingNum++;
                params = params->next;
            }
            // These functions cant have any calling parameter
            if(result == INPUTF_CALL || result == INPUTI_CALL || result == INPUTS_CALL)
            {
                if(callingNum != 1)
                {
                    error(PARAM_NUM, pData.currentLine);
                }
            }
            // These functions must have only one parameter
            else if(result == LENGTH_CALL || result == CHR_CALL)
            {
                if(callingNum != 2)
                {
                    error(PARAM_NUM, pData.currentLine);
                }
            }
            // These functions must have only two parameters
            else if(result == ORD_CALL)
            {
                if(callingNum != 3)
                {
                    error(PARAM_NUM, pData.currentLine);
                }
            }
            // These functions must have only three parameters
            else if(result == SUBSTR_CALL)
            {
                if(callingNum != 4)
                {
                    error(PARAM_NUM, pData.currentLine);
                }
            }
            // These functions must have more than zero parameters
            else if(result == PRINT_CALL)
            {
                if(callingNum == 1)
                {
                    error(PARAM_NUM, pData.currentLine);
                }
            }
        }
        callingNum = 0;
        head = head->next;
    }
}

/**
 * Function validates, if every function call is calling existing function.
 * @param root Pointer to the global symbol table, which has all functions.
 */
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
        error(UNDEF_F, pData.currentLine);
    }
}

/**
 * Function validates name of variable, checks if its name is builtin function or keyword, and searches inside local table.
 * If found, function calls error.
 * @param name Name of the variable.
 */
void validate_variable(string name)
{
    // Check if the variable name is not built-in function name or keyword
    int result = validate_symbol(name);
    if(result != OK)
    {
        error(IDF_REDEF, pData.currentLine);
    }
    // Search the if the variable is inside global
    tNode *temp = search_table(pData.global->root, name);
    // Expecting NULL, if not its redefinition
    if(temp != NULL)
    {
        error(IDF_REDEF, pData.currentLine);
    }
}

// <f_def> <start> <f_decl>
/**
 * Function parses function definition, by calling the start() of the function recursively.
 */
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
    if(pData.token.type != END)
    {
        free_tree(pData.local->root);
        pData.local->root = temp;
        error(EXPECTED_END, pData.currentLine);
    }
    GET_TOKEN();
    if(pData.token.type != END_OF_FILE && pData.token.type != END_OF_LINE)
    {
        free_tree(pData.local->root);
        pData.local->root = temp;
        error(UNEXPECTED_END, pData.currentLine);
    }
    // Function is parsed without error, saving end keyword
    insert_instr(pData.instrs,FUN_END);
    // Restoring back local table, restoring bool and decrementing scope
    free_tree(pData.local->root);
    pData.local->root = temp;
    pData.inDefinition = false;
    pData.scopes--;
}

// <params> ID  <params> ) <f_decl>
/**
 * Function parses current function formal parameters.
 * @param function Pointer straight to the function inside global symbol table.
 */
void params(tNode *function)
{
    clear_stack(pData.stack);
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
        error(UNEXPECTED_F, pData.currentLine);
    }
    // Now parser will take every parameter until token is not ID
    // Clear out the so parser can save params one by one and compare them 
    while(pData.token.type == ID)
    {
        // Search the parameter inside global table, if exists, redefiniton
        tNode *result = search_table(pData.global->root, pData.token.attr.str);
        if(result != NULL)
        {
            error(IDF_REDEF, pData.currentLine);
        }
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
                pData.token.type = NIL; // Change token type of second parameter, to prevent double free
                error(SAME_PARAM, pData.currentLine);
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
            error(UNEXPECTED_F, pData.currentLine);
        }
        GET_TOKEN();
        if(pData.token.type != ID)
        {
            error(UNEXPECTED_F, pData.currentLine);
        }
    }
}

/**
 * Function parses function declaration, name, parameters and body.
 */
void function_declaration()
{
    // Function definition implementation
    // Cannot define function inside other function or inside while, if
    if(pData.inDefinition == true || pData.scopes > 0)
    {
        error(UNEXPECTED_F, pData.currentLine);
    }
    // Expecting function identificator
    GET_TOKEN();
    if(pData.token.type != IDF && pData.token.type != ID)
    {
        error(EXPECTED_IDF, pData.currentLine);
    }
    // Expecting valid function ID, that means: no keyword, no redefinition of built-in functions
    int result = validate_symbol(pData.token.attr.str);
    if(result != OK)
    {
        error(IDF_REDEF, pData.currentLine);
    }
    // Now needs to check if the function name is in the local table (variables), if it is, its error, there cannot be same name for function and variable
    tNode *temp = search_table(pData.local->root,pData.token.attr.str);
    if(temp != NULL)
    {
        error(ID_REDEF, pData.currentLine);
    }
    // Funtion name is correct and is not a keyword, nor variable name, now needs to check if the function was already called or not (if it was called, there is global table node created already)
    temp = search_table(pData.global->root,pData.token.attr.str);
    // Function was already called somewhere inside code, parser must mark that the definintion of the function was found or if was defined already, thats redefinition error
    if(temp != NULL)
    {
        if(temp->wasDefined == true)
        {
            error(IDF_REDEF, pData.currentLine);
        }
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
        error(INT_ERR, pData.currentLine);
    }
    // Function definition is correct, parser needs to generate instruction for code generation and save function name
    insert_instr(pData.instrs, FUN_DEF);
    insert_param(pData.instrs, pData.token);
    // Parse function parameters
    GET_TOKEN();
    if(pData.token.type != OPEN_PARENTH)
    {
        error(UNEXPECTED_F, pData.currentLine);
    }
    params(function);
    // Parameters are parsed, expecting EOL
    GET_TOKEN();
    if(pData.token.type != END_OF_LINE)
    {
        error(UNEXPECTED_F, pData.currentLine);
    }
    // Expecting function definition
    parse_function_definition();
    // Function was parsed, get token and start parsing
    GET_TOKEN();
    start();
}

/**
 * Function parses, function call and inserts instructions inside instruction list.
 * @param moved Bool which signifies if function result is going to be saved.
 * @param pushed Bool which signifies if parser "ate" one more token to analyse correct derivation tree.
 */ 
void function_call(bool moved, bool pushed)
{
    // Check if the called function is in the global table
    tNode *result = search_table(pData.global->root,pData.token.attr.str);
    int builtin = OK;
    // Bool if leftbracket to check for rightbracket
    bool leftbracket = false;
    bool rightbracket = false;
    // If parser did not found function inside global symtable and parser is inside main, semantic error
    if(result == NULL && pData.inDefinition == false)
    {
        // Check if the called function is not built in function
        builtin = validate_symbol(pData.token.attr.str);
        if(builtin == OK)
        {
            error(UNDEF_F, pData.currentLine);
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
            // The first argument was eaten up to get information which function is it
            if(pushed == true)
            {
                pData.token = head_stack(pData.stack);
                pop_stack(pData.stack);
            }
        }
        // If its built int just NOBUILTINCALL (BUILTINCALL + DIFFERENCE)
        else
        {
            insert_instr(pData.instrs,(builtin + NOCALL_CALL_DIFFERENCE));
            str_free(&pData.token.attr.str); // We dont need to save the name of builtin function
            if(pushed == true)
            {
                pData.token = head_stack(pData.stack);
                pop_stack(pData.stack);
            }
        }
    }
    // If will be moved
    else
    {
        tToken param; 
        if(pushed == true)
        {
            param = head_stack(pData.stack);
            pop_stack(pData.stack);
        }
        tToken name = head_stack(pData.stack);
        pop_stack(pData.stack);
        // Need to search if the variable exists already, if not need to define it
        tNode *result = search_table(pData.local->root,name.attr.str);
        if(result == NULL)
        {
            // Need to check if variable name is not builtin function or keyword
            validate_variable(name.attr.str);
            if(pData.local->root == NULL)
            {
                pData.local->root = insert_var(pData.local->root, name);
            }
            else
            {
                insert_var(pData.local->root, name);
            }
            insert_instr(pData.instrs,DEFVAR);
            insert_param(pData.instrs,name);
        }
        if(builtin == OK)
        {
            insert_instr(pData.instrs,FUN_CALL);
            insert_param(pData.instrs,name);
            insert_param(pData.instrs,pData.token);
        }
        // If function is builtin, need to save the macro of builtin fuinction
        else
        {
            insert_instr(pData.instrs,builtin);
            insert_param(pData.instrs,name);
            str_free(&pData.token.attr.str); // We dont need to save the name of builtin function
        }
        if(pushed == true)
        {
            // If parser, took open parenth before function definition was called, need to correct it
            if(param.type == OPEN_PARENTH)
            {
                leftbracket = true;
                GET_TOKEN();
            }
            // If parser, took function parameter before function definition was called, need to correct it
            else if(param.type == ID || param.type == FLOAT || param.type == INTEGER || param.type == STRING)
            {
                insert_param(pData.instrs,param);
                GET_TOKEN();
            }
            else if(param.type == END_OF_LINE || param.type == END_OF_FILE)
            {
                pData.token.type = END_OF_LINE;
            }
            clear_stack(pData.stack);
        }
    }
    if(pushed != true)
    {
        GET_TOKEN();
    }
    // Expecting leftbracket or ID or EOF
    // If leftbracket, activate bool and get next token
    if(pData.token.type == OPEN_PARENTH)
    {
        leftbracket = true;
        GET_TOKEN();
    }
    // Int for checking comma; 1 => first parameter, 2 => we have comma and have to give next param, 3 => we need to supply comma 
    int comma = 1;
    // Get parameters
    while(true)
    {
        // If close parenth, must check if there was open parenth and return
        if(pData.token.type == CLOSE_PARENTH)
        {
            if(leftbracket == false || comma == 2)
            {
                error(UNEXPECTED_F, pData.currentLine);
            }
            // Jump from the while
            rightbracket = true;
            break;
        }
        // If it is variable search it in local symtable, if int,float,string, just add as paramter
        else if(comma != 3 && (pData.token.type == ID || pData.token.type == INTEGER || pData.token.type == FLOAT || pData.token.type == STRING || pData.token.type == NIL))
        {
            comma = 3;
            // Search if the variable is defined, if not, error
            if(pData.token.type == ID)
            {
                result = search_table(pData.local->root,pData.token.attr.str);
                if(result == NULL)
                {
                    error(UNDEF_V, pData.currentLine);
                }
            }
            // If is, generate parameter
            insert_param(pData.instrs,pData.token);
        }
        // Unexpected token
        else if(pData.token.type == END_OF_LINE || pData.token.type == END_OF_FILE)
        {
            if(comma == 2)
            {
                error(WRONG_PARAM, pData.currentLine);
            }
            break;
        }
        else if(comma != 2 && pData.token.type == COMMA)
        {
            comma = 2;
        }
        else
        {
            error(WRONG_PARAM, pData.currentLine);
        }
        // Get next parameter and parse
        GET_TOKEN();
    }
    // Here is EOL, need to check parenths are allright
    if(leftbracket == true && rightbracket != true)
    {
        error(WRONG_PARAM, pData.currentLine);
    }
    // Expection EOL or EOF
    else if(rightbracket == true)
    {
        GET_TOKEN();
        if(pData.token.type != END_OF_FILE && pData.token.type != END_OF_LINE)
        {
            error(UNEXPECTED_TOKEN, pData.currentLine);
        }
    }
    // Function call is parsed, get next token and continue parsing.
    GET_TOKEN();
    start();
}

/**
 * Function parses, while loop and generates instructions inside instruction list.
 */
void while_loop()
{
    // Got into while scope
    pData.scopes++;
    insert_instr(pData.instrs, WHILE_CALL);
    GET_TOKEN();
    // If is in condition something else then variable, float, int or string constant
    if(pData.token.type != ID && pData.token.type != INTEGER && pData.token.type != FLOAT && pData.token.type != STRING && pData.token.type != NIL && pData.token.type != OPEN_PARENTH)
    {
        error(EXPECTED_EXPR, pData.currentLine);
    } 

    pars_expression();

    // Expecting DO after condition
    if(pData.token.type != DO)
    {
        error(EXPECTED_DO, pData.currentLine);
    }

    insert_instr(pData.instrs, WHILE_COND_END);
    
    GET_TOKEN();
    // Expecting EOL after DO
    if(pData.token.type != END_OF_LINE)
    {
        error(EXPECTED_EOL, pData.currentLine);
    }
    // Parsing body of while
    GET_TOKEN();
    start();
    if(pData.token.type != END)
    {
        error(EXPECTED_END, pData.currentLine);
    }
    GET_TOKEN();
    if(pData.token.type != END_OF_FILE && pData.token.type != END_OF_LINE)
    {
        error(UNEXPECTED_END, pData.currentLine);
    }
    // While was successfully parsed
    insert_instr(pData.instrs, WHILE_END);
    pData.scopes--;
    GET_TOKEN();
    start();
}

/**
 * Function parses if condition and generates instructions inside instruction list.
 */
void if_condition()
{
    pData.scopes++;
    insert_instr(pData.instrs, IF_CALL);
    GET_TOKEN(); // Expression starts with one of the following tokens
    if(pData.token.type != ID && pData.token.type != INTEGER && pData.token.type != FLOAT && pData.token.type != STRING && pData.token.type != OPEN_PARENTH && pData.token.type != NIL)
    {
        error(EXPECTED_EXPR, pData.currentLine);
    }

    pars_expression();

    // Expression returns token which should contain THEN keyword
    if(pData.token.type != THEN)
    {
        error(UNEXPECTED_IF, pData.currentLine);
    }
    insert_instr(pData.instrs, IF_COND_END);

    GET_TOKEN();
    if(pData.token.type != END_OF_LINE)
    {
        error(EXPECTED_EOL, pData.currentLine);
    }

    GET_TOKEN();
    start(); // Parsing first part of if

    if(pData.token.type != ELSE)
    {
        error(EXPECTED_ELSE, pData.currentLine);
    }
    GET_TOKEN();
    if(pData.token.type != END_OF_LINE)
    {
        error(EXPECTED_EOL, pData.currentLine);
    }
    insert_instr(pData.instrs, ELSE_CALL);

    GET_TOKEN(); 
    start(); // Parsing 'else' part of if condition

    if(pData.token.type != END)
    {
        error(EXPECTED_END, pData.currentLine);
    }
    GET_TOKEN();
    if(pData.token.type != END_OF_FILE && pData.token.type != END_OF_LINE)
    {
        error(UNEXPECTED_END, pData.currentLine);
    }
    insert_instr(pData.instrs, IF_END);
    pData.scopes--;

    GET_TOKEN();
    start(); // Continue parsing
}

/**
 * Function parses end of line.
 */
void end_of_line()
{
    GET_TOKEN();
    start();
}

/**
 * Function parses end of file, checks if all conditions, loops and definitions were ended "end".
 * Free's out all parser resources and submit's intruction list, to code generator.
 */
void end_of_file()
{
    if(endoffile == false)
    {
        if(pData.inDefinition == true) // Function wasn't ended
        {
            return; // We will handle error in function
        }
        // Check for unexpected end of file (in while or if)
        if(pData.scopes > 0)
        {
            error(UNEXPECTED_EOF, pData.currentLine);
        }
        // Check if there is some function that was called but not defined
        validate_calls(pData.global->root);
        validate_params(pData.global->root);
        // Free out the symtables and stack
        free_symtable(pData.global);
        free_symtable(pData.local);
        free_stack(pData.stack);
        // Make sure noretval is freed, insert it into list as empty instruciton
        insert_instr(pData.instrs, NOP);
        insert_param(pData.instrs, noretval);
        // Give the final instruction list to the code generator
        ilist = pData.instrs;
        endoffile = true;
    }
}

/**
 * Function parses assignmnet to variable.
 */
void assignment()
{
    // Left assignment side, is still on stack
    GET_TOKEN();
    // Need to check the if the token after assignment is IDF
    if(pData.token.type == IDF)
    {
        // its function call, need to call it true, because result is going to be save
        function_call(true, false);
        return;
    }
    // Right side of assignment cannot be missing
    else if(pData.token.type == END_OF_FILE || pData.token.type == END_OF_LINE)
    {
        error(EXPECTED_EXPR, pData.currentLine);
    }
    // If its ID, need to check if its function
    tNode *result = NULL;
    if(pData.token.type == ID)
    {
        // If it is function, its function call
        result = search_table(pData.global->root, pData.token.attr.str);
        int builtin = validate_symbol(pData.token.attr.str);
        if(result != NULL || builtin != OK)
        {
            // User is calling function which was already called recursive, but was still not defined and is inside main
            if(result != NULL)
            {
                if(result->wasDefined == false && pData.inDefinition == false)
                {
                    error(UNDEF_F, pData.currentLine);
                }
            }
            function_call(true, false);
            return;
        }
        // Here left side is still not inside local table, need to insert it
        tToken where = head_stack(pData.stack);
        result = search_table(pData.local->root, where.attr.str);
        if(result == NULL)
        {
            validate_variable(where.attr.str);
            if(pData.local->root == NULL)
            {
                pData.local->root = insert_var(pData.local->root, where);
            }
            else
            {
                insert_var(pData.local->root, where);
            }
            insert_instr(pData.instrs, DEFVAR);
            insert_param(pData.instrs, where);
        }
        // Search it inside local table, if it is variable, it only can be expression
        result = search_table(pData.local->root, pData.token.attr.str);
        if(result != NULL)
        {
            tToken where = head_stack(pData.stack);
            pop_stack(pData.stack);
            // Search if the left side is defined, if not define it
            result = search_table(pData.local->root, where.attr.str);
            if(result == NULL)
            {
                validate_variable(where.attr.str);
                insert_instr(pData.instrs, DEFVAR);
                insert_param(pData.instrs, where);
                if(pData.local->root == NULL)
                {
                    pData.local->root = insert_var(pData.local->root, where);
                }
                else
                {
                    insert_var(pData.local->root, where);
                }
            }
            // Call expression parsing
            pars_expression();
            // Save the result from stack
            insert_instr(pData.instrs, POPS);
            insert_param(pData.instrs, where);
            return;
        }
        // If parser is inside function, it still can be a function call, but user is calling undefined function or expression
        else if(result == NULL && pData.inDefinition == true)
        {
            function_call(true, false);
            return;
        }        
        // If parser is in main, and result is NULL, it cannot be function call, it can be only expression or error
        else 
        {
            error(UNDEF_F_OR_V, pData.currentLine);
        }
    }

    if(pData.token.type == INTEGER || pData.token.type == FLOAT || pData.token.type == STRING || pData.token.type == OPEN_PARENTH || pData.token.type == NIL)
    {
        tToken where = head_stack(pData.stack);
        pop_stack(pData.stack);
        // Search if the variable is inside global table, if is, its semnatic error, asigning to the function
        tNode *result = search_table(pData.global->root, where.attr.str);
        if(result != NULL)
        {
            error(IDF_REDEF, pData.currentLine);
        }
        //Search the variable inside local symtable, if it is not there, need to define it, if is, do nothing
        result = search_table(pData.local->root, where.attr.str);
        if(result == NULL)
        {
            validate_variable(where.attr.str);
            insert_instr(pData.instrs, DEFVAR);
            insert_param(pData.instrs, where);
            // and insert it inside local table
            if(pData.local->root == NULL)
            {
                pData.local->root = insert_var(pData.local->root, where);
            }
            insert_var(pData.local->root, where);
        }
        if(pData.token.type == INTEGER || pData.token.type == FLOAT || pData.token.type == OPEN_PARENTH || pData.token.type == NIL || pData.token.type == STRING)
        {
            // Expression is parsed, 
            pars_expression();
            // need to pop it out from stack and save POPS to the ilist
            insert_instr(pData.instrs, POPS);
            insert_param(pData.instrs, where);
        }
        return;
    }
    else
    {
        error(UNEXPECTED_TOKEN, pData.currentLine);
    }
}

/**
 * Function analyses ID and chooses which derivation tree should create.
 */
void analyse_id()
{
    // push the ID on the stack
    push_stack(pData.stack,pData.token);
    // Search local table, if it is variable 
    tNode *result = search_table(pData.local->root, pData.token.attr.str);
    if(result != NULL)
    {
        tToken where = pData.token;
        pars_expression();
        // If the precedence parsing got assignment and let it be, it must be function call
        if(pData.token.type == ASSIGNMENT)
        {
            push_stack(pData.stack, where);
            assignment();
            if(pData.token.type == END || pData.token.type == ELSE)
            {
                return;
            }
            GET_TOKEN();
            start();
            return;
        }
        // Precedence was calculated
        else
        {
            insert_instr(pData.instrs, POPS);
            insert_param(pData.instrs, noretval);
            if(pData.token.type == END)
            {
                return;
            }
            else if(pData.token.type != END_OF_FILE && pData.token.type != END_OF_LINE)
            {
                error(UNEXPECTED_TOKEN, pData.currentLine);
            }
            GET_TOKEN();
            start();
            return;
        }
    }
    // Search global table, if found, its function_call with no result saved
    result = search_table(pData.global->root,pData.token.attr.str);
    int builtin = validate_symbol(pData.token.attr.str);
    if(result != NULL || builtin != OK)
    {
        // User is calling function which was already called recursive, but was still not defined and is inside main
        if(result != NULL)
        {
            if(result->wasDefined == false && pData.inDefinition == false)
            {
                error(UNDEF_F, pData.currentLine);
            }
        }
        // Now parser needs to check if user is not assigning to the function
        push_stack(pData.stack, pData.token);
        GET_TOKEN();
        if(pData.token.type == ASSIGNMENT)
        {
            error(IDF_REDEF, pData.currentLine);
        }
        tToken temp = head_stack(pData.stack);
        pop_stack(pData.stack);
        push_stack(pData.stack, pData.token);
        pData.token = temp;
        // Parser is again one token off and parameter is on stack
        function_call(false, true);
        return;        
    }
    GET_TOKEN();
    // If EOL or EOF, function, check if parser is inside function
    if(pData.token.type == END_OF_LINE || pData.token.type == END_OF_FILE)
    {
        // Need to get function name back
        tToken temp = head_stack(pData.stack);
        pop_stack(pData.stack);
        // Now need to push the first argument
        push_stack(pData.stack, pData.token);
        // Return function ID
        pData.token = temp;
        function_call(false, true);
        return;
    }
    // If "=" its assignment
    else if(pData.token.type == ASSIGNMENT)
    {
        assignment();
    }
    // If is in definition it still can be function call with no return value saved
    else if(result == NULL && pData.inDefinition == true)
    {
        // Need to get function name back
        tToken temp = head_stack(pData.stack);
        pop_stack(pData.stack);
        // Now need to push the first argument
        push_stack(pData.stack, pData.token);
        // Return function ID
        pData.token = temp;
        function_call(false, true);
        return;
    }
    else
    {
        error(WRONG_PARAM, pData.currentLine);
    }

    if(pData.token.type == END || pData.token.type == ELSE)
    {
        return;
    }
    // Parse next
    GET_TOKEN();
    start();
}

/**
 * Main parse function which chooses the start of the derivation tree.
 * This function is called everytime when new line.
 */
void start()
{
    pData.currentLine++;
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
            analyse_id();
            break;
        case END_OF_LINE:
            end_of_line();
            break;
        // <start> IDF -> <f_call>
        case IDF:
            function_call(false, false);
            break;
        // <start> WHILE <expr> DO EOL
        case WHILE:
            while_loop();
            break;
        // <start> IF <expr> THEN EOL
        case IF:
            if_condition();
            break;
        // <start> ELSE EOL
        case ELSE:
            break;
        // <end> -> <f_def>
        // <end> -> <else>
        // <end> -> <while>
        case END:
            // If there no scopes, and END is called = syntax error
            if(pData.scopes == 0)
            {
                error(UNEXPECTED_TOKEN, pData.currentLine);
            }
            // If there are scopes, end just returns back to the parsing function (parser_function_definition, while_function, if_function)
            break;
            // <EOF> -> end
        case END_OF_FILE:
            // Need to check if we are not in function, check if all scopes are 0 and if all called functions got their own definition
            end_of_file();
            break;
        case OPEN_PARENTH:
        case NIL:
        case INTEGER:
        case FLOAT:
        case STRING:
             //Just calculate the expression 
            pars_expression();
            // Insert empty POPS to pop to RETVAL
            insert_instr(pData.instrs, POPS);
            insert_param(pData.instrs, noretval);
            GET_TOKEN();
            start();
            break;
        default:
            error(UNEXPECTED_TOKEN, pData.currentLine);
    }
}

/**
 * Function initializes parser and start creating derivation tree.
 */
void parse()
{
    parser_init();
    // get first token
    GET_TOKEN();
    start();
}
