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
#include "error.h"
#include "scanner.h"

/**
 * Function which based on error id, will generate defined error and close the compiler.
 * @param id Number of error.
 */
void print_error_exit(int id, int line)
{
    switch(id)
    {
        case EXPECTED_IDF:
            fprintf(stderr,"Syntax error, expected function identificator! Line: %d\n", line);
            exit(SY_ERR);
        case IDF_REDEF:
            fprintf(stderr,"Semantic error, redefinition of function! Line: %d\n", line);
            exit(DEF_ERR);
        case SAME_PARAM:
            fprintf(stderr,"Semantic error, parameters cannot have same name! Line: %d\n", line);
            exit(DEF_ERR);
        case UNDEF_F:
            fprintf(stderr,"Semantic error, calling undefined function! Line: %d\n", line);
            exit(DEF_ERR);
        case PARAM_NUM:
            fprintf(stderr,"Semantic error, wrong number of parameters inside function call! Line: %d\n", line);
            exit(ARG_ERR);
        case UNDEF_V:
            fprintf(stderr,"Semantic error, using undefined variable! Line: %d\n", line);
            exit(DEF_ERR);
        case UNEXPECTED_F:
            fprintf(stderr,"Syntax error, unexpected token inside function! Line: %d\n", line);
            exit(SY_ERR);
        case INT_ERR:
            fprintf(stderr,"Compiler internal error occured! Line: %d\n", line);
            exit(INT_ERR);
        case UNEXPECTED_EOF:
            fprintf(stderr,"Syntax error, unexpected end of file! Line: %d\n", line);
            exit(SY_ERR);
        case UNEXPECTED_END:
            fprintf(stderr,"Syntax error, unexpected token after keyword END! Line: %d\n", line);
            exit(SY_ERR);
        case WRONG_PARAM:
            fprintf(stderr,"Syntax error, unexpected token inside function call! Line: %d\n", line);
            exit(SY_ERR);
        case L_ERR:
            exit(L_ER);
        case COND_ERR:
            fprintf(stderr,"Semantic error, wrong data type in condition! Line: %d\n", line);
            exit(DATA_ERR);
        case EXPECTED_DO:
            fprintf(stderr, "Syntax error, expecting do after condition in while Line: %d\n", line);
            exit(SY_ERR);
        case UNEXPECTED_W:
            fprintf(stderr, "Syntax error, unexpected token after keyword do Line: %d\n", line);
            exit(SY_ERR);
        case EXPECTED_EOL:
            fprintf(stderr, "Syntax error, expected EOL after keyword Line: %d\n", line);
            exit(SY_ERR);
        case UNEXPECTED_IF:
            fprintf(stderr,"Syntax error, unexpected token inside if condition! Line: %d\n", line);
            exit(SY_ERR);
        case EXPECTED_ELSE:
            fprintf(stderr,"Syntax error, expected else keyword! Line: %d\n", line);
            exit(SY_ERR);
        case EXPECTED_END:
            fprintf(stderr,"Syntax error, expected end keyword! Line: %d\n", line);
            exit(SY_ERR);
        case UNDEFINED_VAR:
            fprintf(stderr,"Semantic error, undefined variable inside expression or function call! Line: %d\n", line);
            exit(DATA_ERR);
        case UNEXPECTED_FUN:
            fprintf(stderr,"Semantic error, unexpected function call inside expression! Line: %d\n", line);
            exit(SE_ERR);
        case UNEXPECTED_EXPR:
            fprintf(stderr,"Syntax error, unexpected token in expression! Line: %d\n", line);
            exit(SY_ERR);
        case UNEXPECTED_TOKEN:
            fprintf(stderr,"Syntax error, unexpected token! Line: %d\n", line);
            exit(SY_ERR);
        case INCOMPATIBLE_EXPR:
            fprintf(stderr,"Semantic error, wrong data type in expression! Line: %d\n", line);
            exit(DATA_ERR);
        case WRONG_OP_CONCAT:
            fprintf(stderr, "Semantic error, wrong operations inside concatenation! Line: %d\n", line);
            exit(DATA_ERR);
        case EXPECTED_EXPR:
            fprintf(stderr,"Syntax error, missing expression! Line: %d\n", line);
            exit(SY_ERR);
        case ID_REDEF:
            fprintf(stderr, "Semantic error, redefinition of variable! Line: %d\n", line);
            exit(DEF_ERR);
        case UNDEF_F_OR_V:
            fprintf(stderr, "Semantic error, calling undefined function or using undefined variable! Line: %d\n", line);
            exit(DEF_ERR);
    }
}
